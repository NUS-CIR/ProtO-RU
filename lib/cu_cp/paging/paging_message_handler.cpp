// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "paging_message_handler.h"
#include "../du_processor/du_processor_repository.h"
#include "ocudu/adt/format.h"
#include "ocudu/ran/cu_cp_types.h"
#include "ocudu/ran/tac.h"

using namespace ocudu;
using namespace ocucp;

paging_message_handler::paging_message_handler(const paging_message_handler_dependencies& dependencies) :
  dus(dependencies.dus), logger(dependencies.logger)
{
}

bool paging_message_handler::handle_paging_message(const cu_cp_paging_message& msg) const
{
  // Forward paging message to all DU processors
  bool paging_sent = false;
  for (const auto& du_idx : dus.get_du_processor_indexes()) {
    paging_sent |= handle_du_paging_message(du_idx, msg);
  }

  if (!paging_sent) {
    logger.warning("No DU processor was able to handle the paging message");
  }
  return paging_sent;
}

static bool is_tac_in_list(span<const cu_cp_tai_list_for_paging_item> tai_list, tac_t tac)
{
  return std::any_of(tai_list.begin(), tai_list.end(), [&tac](const auto& tai) { return tai.tai.tac == tac; });
}

/// \brief True if any TAC the cell broadcasts appears in the paging TAI list.
///
/// A cell belongs to every tracking area it broadcasts; matching only the first drops paging for the others.
static bool is_cell_in_tai_list(span<const cu_cp_tai_list_for_paging_item> tai_list, const du_cell_configuration& cell)
{
  if (cell.tac_list.empty()) {
    return is_tac_in_list(tai_list, cell.tac);
  }

  return std::any_of(
      cell.tac_list.begin(), cell.tac_list.end(), [&tai_list](tac_t tac) { return is_tac_in_list(tai_list, tac); });
}

/// True for a RAN paging. A RAN paging identifies the UE by its I-RNTI.
static bool is_ran_paging(const cu_cp_paging_message& msg)
{
  return std::holds_alternative<full_i_rnti_t>(msg.ue_paging_id);
}

/// \brief Replace each recommended cell with the served cells of this DU that are paged under it.
///
/// Drops the ones this DU does not serve. A CN paging also drops the ones outside every tracking area being paged.
static void resolve_recommended_cells(cu_cp_paging_message& msg, const du_configuration_context& du_cfg)
{
  auto& recommended_cells = msg.assist_data_for_paging.value()
                                .assist_data_for_recommended_cells.value()
                                .recommended_cells_for_paging.recommended_cell_list;

  // The core names a recommended cell by the identity this gNB reported for it: its Uu Cell ID outside NTN, and in an
  // NTN cell the Mapped Cell ID of the area, TS 38.300 sec. 16.14.5. The gNB-DU pages the cells it knows by their Uu
  // Cell ID, TS 38.473 sec. 8.7.1.2, so each identity is resolved to the served cells carrying it and stands down for
  // their own. A Mapped Cell ID names a geographical area and may resolve to several, and the core may name one cell
  // by both its identities, which must not page it twice.
  std::vector<cu_cp_recommended_cell_item> paged_cells;
  for (const cu_cp_recommended_cell_item& recommended_cell : recommended_cells) {
    for (const du_cell_configuration* cell : du_cfg.find_cells_by_reported_cgi(recommended_cell.ngran_cgi)) {
      if (!is_ran_paging(msg) && !is_cell_in_tai_list(msg.tai_list_for_paging, *cell)) {
        continue;
      }
      if (std::any_of(paged_cells.begin(), paged_cells.end(), [cell](const cu_cp_recommended_cell_item& paged) {
            return paged.ngran_cgi == cell->cgi;
          })) {
        continue;
      }
      cu_cp_recommended_cell_item paged_cell = recommended_cell;
      paged_cell.ngran_cgi                   = cell->cgi;
      paged_cells.push_back(paged_cell);
    }
  }

  recommended_cells = std::move(paged_cells);
}

bool paging_message_handler::handle_du_paging_message(cu_cp_du_index_t            du_index,
                                                      const cu_cp_paging_message& msg_before) const
{
  du_processor&                   du     = dus.get_du_processor(du_index);
  const du_configuration_context* du_cfg = du.get_context();
  if (du_cfg == nullptr) {
    // DU has not completed F1 Setup.
    return false;
  }

  // Recommended cells will be added to the original paging message.
  cu_cp_paging_message msg_filtered{msg_before};
  if (!msg_filtered.assist_data_for_paging.has_value()) {
    msg_filtered.assist_data_for_paging.emplace();
  }
  if (!msg_filtered.assist_data_for_paging.value().assist_data_for_recommended_cells.has_value()) {
    msg_filtered.assist_data_for_paging.value().assist_data_for_recommended_cells.emplace();
  }
  auto& recommended_cells = msg_filtered.assist_data_for_paging.value()
                                .assist_data_for_recommended_cells.value()
                                .recommended_cells_for_paging.recommended_cell_list;

  // Resolve the recommended cells to the cells of this DU that are paged under them, dropping the rest.
  resolve_recommended_cells(msg_filtered, *du_cfg);

  // A RAN paging pages only the recommended cells, which are the cells of the RNA (TS 38.300 section 9.2.2.3).
  // TODO: Support RANAC based paging.
  if (!is_ran_paging(msg_filtered)) {
    // A CN paging also pages the served cells in the paged tracking areas that are not recommended.
    for (const du_cell_configuration& cell : du_cfg->served_cells) {
      // Skip a cell already recommended: resolve_recommended_cells() left the list holding Uu Cell IDs alone.
      if (std::any_of(recommended_cells.begin(), recommended_cells.end(), [&cell](const auto& c) {
            return c.ngran_cgi == cell.cgi;
          })) {
        continue;
      }

      if (!is_cell_in_tai_list(msg_filtered.tai_list_for_paging, cell)) {
        continue;
      }

      // Setup recommended cell item to add in case it doesn't exist
      cu_cp_recommended_cell_item cell_item;
      cell_item.ngran_cgi = cell.cgi;
      recommended_cells.push_back(cell_item);
    }
  }

  if (recommended_cells.empty()) {
    logger.info("du={}: No cells to page at this DU", du_index);
    return false;
  }

  // Forward message to F1AP of the respective DU.
  du.get_f1ap_handler().get_f1ap_paging_manager().handle_paging(msg_filtered);

  return true;
}
