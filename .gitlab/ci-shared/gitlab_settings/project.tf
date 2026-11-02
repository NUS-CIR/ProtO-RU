# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI


resource "gitlab_project" "this" {
  name        = var.ci_project_title
  description = var.description

  # Basic Configuration
  default_branch   = var.default_branch
  visibility_level = var.visibility_level

  # Merge Configuration
  merge_method                                     = var.merge_method
  only_allow_merge_if_pipeline_succeeds            = var.only_allow_merge_if_pipeline_succeeds
  only_allow_merge_if_all_discussions_are_resolved = var.only_allow_merge_if_all_discussions_are_resolved
  remove_source_branch_after_merge                 = var.remove_source_branch_after_merge
  resolve_outdated_diff_discussions                = var.resolve_outdated_diff_discussions
  squash_option                                    = var.squash_option
  allow_merge_on_skipped_pipeline                  = var.allow_merge_on_skipped_pipeline
  reviewer_assignment_strategy                     = var.reviewer_assignment_strategy

  # CI/CD Configuration
  auto_cancel_pending_pipelines = var.auto_cancel_pending_pipelines
  auto_devops_enabled           = var.auto_devops_enabled
  build_git_strategy            = var.build_git_strategy
  build_timeout                 = var.build_timeout
  ci_forward_deployment_enabled = var.ci_forward_deployment_enabled
  ci_default_git_depth          = var.ci_default_git_depth
  ci_separated_caches           = var.ci_separated_caches
  keep_latest_artifact          = var.keep_latest_artifact
  merge_pipelines_enabled       = var.merge_pipelines_enabled
  merge_trains_enabled          = var.merge_trains_enabled

  # Repository Configuration
  autoclose_referenced_issues = var.autoclose_referenced_issues
  lfs_enabled                 = var.lfs_enabled

  # Feature Access Levels
  builds_access_level                  = var.builds_access_level
  container_registry_access_level      = var.container_registry_access_level
  forking_access_level                 = var.forking_access_level
  merge_requests_access_level          = var.merge_requests_access_level
  packages_enabled                     = var.packages_enabled
  pages_access_level                   = var.pages_access_level
  repository_access_level              = var.repository_access_level
  requirements_access_level            = var.requirements_access_level
  security_and_compliance_access_level = var.security_and_compliance_access_level
  snippets_access_level                = var.snippets_access_level
  wiki_access_level                    = var.wiki_access_level

  # Additional Features
  request_access_enabled = var.request_access_enabled

  # Container Registry Expiration Policy
  container_expiration_policy {
    cadence           = var.container_expiration_policy.cadence
    enabled           = var.container_expiration_policy.enabled
    keep_n            = var.container_expiration_policy.keep_n
    older_than        = var.container_expiration_policy.older_than
    name_regex_delete = var.container_expiration_policy.name_regex_delete
    name_regex_keep   = var.container_expiration_policy.name_regex_keep
  }
}
