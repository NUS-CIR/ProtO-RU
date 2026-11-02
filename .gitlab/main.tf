# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI

terraform {
  backend "http" {}
}

provider "gitlab" {
  # https://search.opentofu.org/provider/opentofu/gitlab/latest
  # It uses GITLAB_TOKEN environment variable to authenticate
  # It uses GITLAB_BASE_URL environment variable to set the GitLab instance URL. If not defined, it defaults to gitlab.com
}

variable "ocudu_path" {
  type    = string
  default = "" # gitlab.com/ocudu/ocudu                
}
variable "ocudu_ref" {
  type    = string
  default = "" # dev
}
variable "ci_project_id" {
  type        = string
  description = "GitLab Project ID" # Auto-injected by GitLab OpenTofu component
}
variable "ci_project_title" {
  type        = string
  description = "GitLab Project Title (display name)"
}

# Imports existing GitLab project
import {
  to = module.settings.gitlab_project.this
  id = var.ci_project_id
}
import {
  to = module.settings.gitlab_project_level_mr_approvals.this
  id = var.ci_project_id
}
import {
  to = module.settings.gitlab_project_push_rules.this
  id = var.ci_project_id
}
import {
  to = module.settings.gitlab_branch_protection.protected_branches["main"]
  id = "${var.ci_project_id}:main"
}
import {
  to = module.settings.gitlab_branch_protection.protected_branches["dev"]
  id = "${var.ci_project_id}:dev"
}

module "settings" {
  source           = "git::https://${var.ocudu_path}.git//.gitlab/ci-shared/gitlab_settings?ref=${var.ocudu_ref}"
  ci_project_id    = var.ci_project_id
  ci_project_title = var.ci_project_title

  # =============================================================================
  # Basic Project Settings
  # =============================================================================
  default_branch   = "dev"
  visibility_level = "public" # private, internal, public

  # =============================================================================
  # Merge Request Configuration
  # =============================================================================
  merge_method                                     = "ff" # merge, rebase_merge, ff
  only_allow_merge_if_pipeline_succeeds            = true
  only_allow_merge_if_all_discussions_are_resolved = false
  remove_source_branch_after_merge                 = true
  resolve_outdated_diff_discussions                = false
  squash_option                                    = "never" # never, always, default_on, default_off
  allow_merge_on_skipped_pipeline                  = false
  reviewer_assignment_strategy                     = "code_owners" # disabled, code_owners, null (Free tier)

  # =============================================================================
  # CI/CD Configuration
  # =============================================================================
  auto_cancel_pending_pipelines = "enabled" # enabled, disabled
  auto_devops_enabled           = false
  build_git_strategy            = "fetch" # clone, fetch
  build_timeout                 = 3600
  ci_forward_deployment_enabled = true
  ci_default_git_depth          = 1
  ci_separated_caches           = true
  keep_latest_artifact          = true
  merge_pipelines_enabled       = true
  merge_trains_enabled          = true

  # =============================================================================
  # Repository Settings
  # =============================================================================
  autoclose_referenced_issues = true
  lfs_enabled                 = false

  # =============================================================================
  # Feature Access Levels
  # =============================================================================
  builds_access_level                  = "enabled"
  container_registry_access_level      = "enabled"
  forking_access_level                 = "enabled"
  merge_requests_access_level          = "enabled"
  packages_enabled                     = true
  pages_access_level                   = "enabled"
  repository_access_level              = "enabled"
  requirements_access_level            = "enabled"
  security_and_compliance_access_level = "private"
  snippets_access_level                = "enabled"
  wiki_access_level                    = "disabled"
  request_access_enabled               = false

  # =============================================================================
  # Container Registry Configuration
  # =============================================================================
  container_expiration_policy = {
    cadence           = "1d"
    enabled           = true
    keep_n            = 100
    older_than        = "7d"
    name_regex_delete = ".*"
    name_regex_keep   = ".*-stable$|.*-latest$|^latest$|^sha256-.*\\.(sig|att)$"
  }

  # =============================================================================
  # Push Rules Configuration
  # =============================================================================
  push_rules = {
    prevent_secrets         = true
    commit_message_regex    = ".*"
    reject_unsigned_commits = false
    reject_non_dco_commits  = true
    deny_delete_tag         = true
  }

  # =============================================================================
  # Merge Request Approvals Configuration
  # =============================================================================
  mr_approvals = {
    disable_overriding_approvers_per_merge_request = true
    merge_requests_author_approval                 = true
    merge_requests_disable_committers_approval     = true
    require_reauthentication_to_approve            = false
    reset_approvals_on_push                        = false
    selective_code_owner_removals                  = false
  }

  approval_rules = {
    all_members = {
      approvals_required                = 1
      rule_type                         = "any_approver"
      applies_to_all_protected_branches = true
    }
  }

  # =============================================================================
  # Protected Branches Configuration
  # =============================================================================
  protected_branches = {
    dev = {
      allow_force_push             = false
      code_owner_approval_required = true

      allowed_to_merge = [{ access_level = "developer" }]
      allowed_to_push  = [{ access_level = "no one" }]
    }
    main = {
      allow_force_push             = false
      code_owner_approval_required = false

      allowed_to_merge = [{ access_level = "no one" }]
      allowed_to_push  = [{ access_level = "maintainer" }]
    }
    srs_functional = {
      allow_force_push             = false
      code_owner_approval_required = false

      allowed_to_merge = [{ access_level = "no one" }]
      allowed_to_push  = [{ access_level = "maintainer" }]
    }
    srs_performance = {
      allow_force_push             = false
      code_owner_approval_required = false

      allowed_to_merge = [{ access_level = "no one" }]
      allowed_to_push  = [{ access_level = "maintainer" }]
    }
    srs_rf = {
      allow_force_push             = false
      code_owner_approval_required = false

      allowed_to_merge = [{ access_level = "no one" }]
      allowed_to_push  = [{ access_level = "maintainer" }]
    }
    srs_interop = {
      allow_force_push             = false
      code_owner_approval_required = false

      allowed_to_merge = [{ access_level = "no one" }]
      allowed_to_push  = [{ access_level = "maintainer" }]
    }
  }

  # =============================================================================
  # Protected Tags Configuration
  # =============================================================================
  protected_tags = {
    "*" = {
      create_access_level = "maintainer"
    }
  }

  # =============================================================================
  # Pipeline Schedules Configuration
  # =============================================================================
  # OCUDU_RUNNER_TAG overrides the CI default (saas-linux-medium, set in
  # .gitlab/ci-shared/workflow.yml) so scheduled builds run on the upstream
  # on-premise runner fleet instead of GitLab SaaS runners.
  #
  # Forks: this is upstream-specific. Remove the "variables" entries below, or
  # replace the value with a tag your own runners use — otherwise the scheduled
  # pipelines queue forever waiting for runners that do not exist. See section 2.3
  # of .gitlab/README.md for the available runner options.
  schedules = {
    nightly = {
      description = "Nightly"
      cron        = "00 23 * * 1-5"
      timezone    = "Europe/Madrid"
      ref         = "refs/heads/dev"
      active      = true
      variables   = { OCUDU_RUNNER_TAG = "on-prem-linux" }
    }
    weekly = {
      description = "Weekly"
      cron        = "00 10 * * 6"
      timezone    = "Europe/Madrid"
      ref         = "refs/heads/dev"
      active      = true
      variables   = { OCUDU_RUNNER_TAG = "on-prem-linux" }
    }
    alternative_oss = {
      description = "Alternative OSs"
      cron        = "00 13 * * 0"
      timezone    = "Europe/Madrid"
      ref         = "refs/heads/dev"
      active      = true
      variables   = { OCUDU_RUNNER_TAG = "on-prem-linux" }
    }
  }

}
