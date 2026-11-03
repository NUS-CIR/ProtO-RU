// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

namespace ocudu {

// Versions of the interfaces consumed by external applications, as MAJOR.MINOR.
// The MAJOR version is increased for breaking changes (a field removed, renamed or with a changed meaning), the MINOR
// version for backward compatible ones (a field added). The CI checks that a version is increased whenever a merge
// request changes the files of its interface.
// Note: keep each version on a single line together with its name, the CI extracts it with grep.

/// Version of the JSON metrics, as reported to the remote control metrics subscribers.
constexpr const char* metrics_interface_version = "1.1";

} // namespace ocudu
