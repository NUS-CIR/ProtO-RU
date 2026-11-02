# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI


# Gitlab OpenTofu automatically injects some ci_* variables like ci_project_id
# https://gitlab.com/components/opentofu#auto-forwarded-predefined-ci-variables
variable "ci_project_id" {
  description = "GitLab Project ID"
  type        = string
  default     = ""
}

variable "ci_project_title" {
  # Project API uses the title in name field
  description = "GitLab Project Title (display name)"
  type        = string
  default     = ""
}

# Project Settings
variable "description" {
  description = "Project description"
  type        = string
  default     = ""
}

variable "default_branch" {
  description = "Default branch for the project"
  type        = string
  default     = "main"
}

variable "visibility_level" {
  description = "Project visibility level"
  type        = string
  default     = "private"
  validation {
    condition     = contains(["private", "internal", "public"], var.visibility_level)
    error_message = "Visibility level must be private, internal, or public."
  }
}

# Merge Configuration
variable "merge_method" {
  description = "Merge method for merge requests"
  type        = string
  default     = "ff"
  validation {
    condition     = contains(["merge", "rebase_merge", "ff"], var.merge_method)
    error_message = "Merge method must be merge, rebase_merge, or ff."
  }
}

variable "only_allow_merge_if_pipeline_succeeds" {
  description = "Only allow merge if pipeline succeeds"
  type        = bool
  default     = true
}

variable "only_allow_merge_if_all_discussions_are_resolved" {
  description = "Only allow merge if all discussions are resolved"
  type        = bool
  default     = false
}

variable "remove_source_branch_after_merge" {
  description = "Remove source branch after merge"
  type        = bool
  default     = true
}

variable "resolve_outdated_diff_discussions" {
  description = "Resolve outdated diff discussions"
  type        = bool
  default     = false
}

variable "squash_option" {
  description = "Squash option for merge requests"
  type        = string
  default     = "default_off"
  validation {
    condition     = contains(["never", "always", "default_on", "default_off"], var.squash_option)
    error_message = "Squash option must be never, always, default_on, or default_off."
  }
}

variable "allow_merge_on_skipped_pipeline" {
  description = "Allow merge on skipped pipeline"
  type        = bool
  default     = false
}

variable "reviewer_assignment_strategy" {
  description = "Automatic reviewer assignment for merge requests (Premium/Ultimate), null leaves it unmanaged"
  type        = string
  default     = null
  validation {
    condition     = var.reviewer_assignment_strategy == null || contains(["disabled", "code_owners"], coalesce(var.reviewer_assignment_strategy, "disabled"))
    error_message = "Reviewer assignment strategy must be null, disabled or code_owners."
  }
}

# CI/CD Configuration
variable "auto_cancel_pending_pipelines" {
  description = "Auto cancel pending pipelines"
  type        = string
  default     = "enabled"
  validation {
    condition     = contains(["enabled", "disabled"], var.auto_cancel_pending_pipelines)
    error_message = "Auto cancel pending pipelines must be enabled or disabled."
  }
}

variable "auto_devops_enabled" {
  description = "Enable Auto DevOps"
  type        = bool
  default     = false
}

variable "build_git_strategy" {
  description = "Git strategy for builds"
  type        = string
  default     = "fetch"
  validation {
    condition     = contains(["clone", "fetch"], var.build_git_strategy)
    error_message = "Build git strategy must be clone or fetch."
  }
}

variable "build_timeout" {
  description = "Build timeout in seconds"
  type        = number
  default     = 3600
}

variable "ci_forward_deployment_enabled" {
  description = "Enable CI forward deployment"
  type        = bool
  default     = false
}

variable "ci_default_git_depth" {
  description = "Default git depth for CI"
  type        = number
  default     = 1
}

variable "ci_separated_caches" {
  description = "Use separated caches for CI"
  type        = bool
  default     = false
}

variable "keep_latest_artifact" {
  description = "Keep latest artifact"
  type        = bool
  default     = true
}

variable "merge_pipelines_enabled" {
  description = "Enable merge pipelines"
  type        = bool
  default     = true
}

variable "merge_trains_enabled" {
  description = "Enable merge trains"
  type        = bool
  default     = false
}

# Repository Configuration
variable "autoclose_referenced_issues" {
  description = "Autoclose referenced issues"
  type        = bool
  default     = true
}

variable "lfs_enabled" {
  description = "Enable Git LFS"
  type        = bool
  default     = false
}

# Feature Access Levels
variable "builds_access_level" {
  description = "Builds access level"
  type        = string
  default     = "enabled"
}

variable "container_registry_access_level" {
  description = "Container registry access level"
  type        = string
  default     = "enabled"
}

variable "forking_access_level" {
  description = "Forking access level"
  type        = string
  default     = "enabled"
}

variable "merge_requests_access_level" {
  description = "Merge requests access level"
  type        = string
  default     = "enabled"
}

variable "packages_enabled" {
  description = "Enable packages"
  type        = bool
  default     = true
}

variable "pages_access_level" {
  description = "Pages access level"
  type        = string
  default     = "public"
}

variable "repository_access_level" {
  description = "Repository access level"
  type        = string
  default     = "enabled"
}

variable "requirements_access_level" {
  description = "Requirements access level"
  type        = string
  default     = "enabled"
}

variable "security_and_compliance_access_level" {
  description = "Security and compliance access level"
  type        = string
  default     = "private"
}

variable "snippets_access_level" {
  description = "Snippets access level"
  type        = string
  default     = "enabled"
}

variable "wiki_access_level" {
  description = "Wiki access level"
  type        = string
  default     = "enabled"
}

variable "request_access_enabled" {
  description = "Enable request access"
  type        = bool
  default     = true
}

# Container Registry Expiration Policy
variable "container_expiration_policy" {
  description = "Container registry expiration policy"
  type = object({
    cadence           = string
    enabled           = bool
    keep_n            = number
    older_than        = string
    name_regex_delete = string
    name_regex_keep   = optional(string)
  })
  default = {
    cadence           = "1d"
    enabled           = true
    keep_n            = 25
    older_than        = "7d"
    name_regex_delete = ".*"
    name_regex_keep   = null
  }
}

# Push Rules Configuration
variable "push_rules" {
  description = "Push rules configuration"
  type = object({
    prevent_secrets         = bool
    commit_message_regex    = string
    reject_unsigned_commits = bool
    reject_non_dco_commits  = bool
    deny_delete_tag         = bool
  })
  default = {
    prevent_secrets         = true
    commit_message_regex    = ".*"
    reject_unsigned_commits = false
    reject_non_dco_commits  = false
    deny_delete_tag         = true
  }
}

# MR Approval Configuration
variable "mr_approvals" {
  description = "Merge request approval configuration"
  type = object({
    disable_overriding_approvers_per_merge_request = bool
    merge_requests_author_approval                 = bool
    merge_requests_disable_committers_approval     = bool
    require_reauthentication_to_approve            = bool
    reset_approvals_on_push                        = bool
    selective_code_owner_removals                  = bool
  })
  default = {
    disable_overriding_approvers_per_merge_request = true
    merge_requests_author_approval                 = true
    merge_requests_disable_committers_approval     = true
    require_reauthentication_to_approve            = false
    reset_approvals_on_push                        = false
    selective_code_owner_removals                  = false
  }
}

variable "approval_rules" {
  description = "Approval rules configuration"
  type = map(object({
    approvals_required                = number
    rule_type                         = string
    applies_to_all_protected_branches = bool
  }))
  default = {
    all_members = {
      approvals_required                = 1
      rule_type                         = "any_approver"
      applies_to_all_protected_branches = true
    }
  }
}

# Protected Branches Configuration
variable "protected_branches" {
  description = "Protected branches configuration"
  type = map(object({
    allow_force_push             = bool
    code_owner_approval_required = bool
    allowed_to_merge = optional(set(object({
      access_level = optional(string)
      user_id      = optional(number)
      group_id     = optional(number)
    })), [])
    allowed_to_push = optional(set(object({
      access_level  = optional(string)
      user_id       = optional(number)
      group_id      = optional(number)
      deploy_key_id = optional(number)
    })), [])
  }))
}

# Protected Tags Configuration
variable "protected_tags" {
  description = "Protected tags configuration"
  type = map(object({
    create_access_level = string
  }))
}

# Pipeline Schedules Configuration
variable "schedules" {
  description = "Definition of Scheduled Pipelines in the project"
  type = map(object({
    description = string
    cron        = string # "0 2 * * *"
    timezone    = string # "Europe/Madrid"
    ref         = string # "refs/heads/main"
    active      = optional(bool, true)
    variables   = optional(map(string), {})
    inputs      = optional(map(string), {})
  }))
}
