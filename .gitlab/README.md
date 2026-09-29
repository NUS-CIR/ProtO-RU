# Setting Up Your OCUDU Fork

This guide explains how to fork and configure the OCUDU repository to replicate the CI/CD infrastructure in your own
GitLab instance.

## 1. Fork the Repository

Create your own copy of this repository to customize it for your development needs:

```bash
# Clone your forked repository
git clone https://gitlab.com/your_user_or_group/ocudu.git
cd ocudu
```

## 2. Choose Your CI/CD Configuration

OCUDU supports three CI/CD modes depending on your requirements:

- **Option A**: Disable CI/CD entirely in GitLab settings (for users not needing automated builds)
- **Option B**: Use basic CI with public images for builders and tools (no customization required)
- **Option C**: Enable complete CI/CD, building your custom images (full control)

### 2.1. Configure CI/CD Variables

OCUDU CI/CD uses two variables to resolve the registry and images it will use:

| Variable                | Default Value         | Description               |
| ----------------------- | --------------------- | ------------------------- |
| `OCUDU_REGISTRY_SERVER` | `registry.gitlab.com` | Container registry server |
| `OCUDU_PROJECT_PATH`    | `ocudu/ocudu`         | Project path for images   |

These variables point to the public OCUDU repository by default. You can override them by creating variables in your
project or group.

**Option B - Use Public Images:**

In your GitLab project, go to **Settings → CI/CD → Variables** and add:

| Variable                            | Value  |
| ----------------------------------- | ------ |
| `SKIP_OCUDU_BUILDING_CONTAINERS_CI` | `true` |

This uses default public images without building custom ones.

**Option C - Build Custom Images:**

Override the registry variables with your values:

| Variable                | Value                | Example                                               |
| ----------------------- | -------------------- | ----------------------------------------------------- |
| `OCUDU_REGISTRY_SERVER` | Your registry server | `registry.gitlab.com` or `$CI_TEMPLATE_REGISTRY_HOST` |
| `OCUDU_PROJECT_PATH`    | Your project path    | `your_group/ocudu` or `$CI_PROJECT_PATH`              |

You can use [predefined GitLab CI/CD variables](https://docs.gitlab.com/ee/ci/variables/predefined_variables.html) for
dynamic configuration.

### 2.2. Configure the GitLab Project

#### GitOps

OCUDU uses a Terraform/OpenTofu solution to configure the GitLab project itself, including project settings, pipeline
schedules, protected branches, approval rules, and more.

**Setup:**

1. Create a CI/CD variable called `GITLAB_TOKEN`:
   - Type: Project Access Token or Personal Access Token
   - Role: `Maintainer`
   - Scopes: `api`, `read_user`, `read_repository`, `write_repository`

2. Modify [.gitlab/main.tf](./main.tf) according to your needs

3. Commit to main branch (via MR or directly)

For more details, see the [terraform module documentation](./ci-shared/gitlab_settings/README.md).

#### Manual Configuration

If you don't create the `GITLAB_TOKEN` variable, GitOps won't be used and you can manage configuration manually.

In that case, check the scheduled pipelines defined at the end of [.gitlab/main.tf](./main.tf) to replicate them
manually in your project's **Settings → CI/CD → Schedules**.

#### Code Owners

[GitLab Code Owners File](./CODEOWNERS) lists upstream OCUDU users for each repo. In your fork they are not project
members, so GitLab ignores them and auto-approves their entries: nothing is blocked, but code owners have no effect.

To use code owners in your fork, replace those usernames with members of your project (Developer role or higher).
Otherwise, delete the file or set `code_owner_approval_required = false` in [.gitlab/main.tf](./main.tf).

Code owner approvals require GitLab Premium / Ultimate. See the
[GitLab Code Owners documentation](https://docs.gitlab.com/user/project/codeowners/).

### 2.3. GitLab Runners

OCUDU CI/CD is designed to work primarily with GitLab Shared Runners, with options for custom runners when needed.

#### Runner Tags and Usage

Different pipeline types use different runner tags:

| Tag                       | When Used                          | Runner Type                            | Requirements                                                                                     |
| ------------------------- | ---------------------------------- | -------------------------------------- | ------------------------------------------------------------------------------------------------ |
| (no tag)                  | All pipelines                      | Free GitLab Shared Runners             | Small tasks, minimal resources                                                                   |
| `saas-linux-medium-amd64` | MR pipelines & Scheduled pipelines | Free GitLab Shared Runners             | Available to all GitLab users                                                                    |
| `saas-linux-medium-arm64` | Scheduled pipelines                | GitLab Shared Runners / Custom Runners | Available to Premium / Ultimate GitLab users                                                     |
| `avx512`                  | Scheduled pipelines                | Custom Runner                          | AVX512 instruction set support (combined with `saas-linux-medium-amd64`)                         |
| `sctp`                    | Scheduled pipelines                | Custom Runner                          | SCTP kernel module active (combined with `saas-linux-medium-amd64` or `saas-linux-medium-arm64`) |

> The CI is using `OCUDU_RUNNER_TAG` variable with value `saas-linux-medium` to define the default runner type for OCUDU
> builds. You can change that variable at group / project level and select other runners in your forked repo. For
> example, setting `OCUDU_RUNNER_TAG` to `saas-linux-large` will use `saas-linux-large-amd64` / `saas-linux-large-arm64`
> in all OCUDU build jobs. `saas-linux-large` are available in Premium / Ultimate GitLab tiers.

#### Default Behavior

- **MR Pipelines**: Work out-of-the-box for all GitLab users using free shared runners
- **Scheduled Pipelines**: Require custom runners for specialized features (AVX512, SCTP)

#### Custom Runner Options

You have several options for configuring runners:

##### Option 1 - GitLab Shared Runners Only (Default)

- Enable shared runners in **Settings → CI/CD → Runners**
- Scheduled pipeline: Some features (AVX512, SCTP) won't be tested because they require custom runners

##### Option 2 - Custom Runners Only

- Disable GitLab shared runners
- Set up your own on-premise or cloud runners with matching tags:
  - `saas-linux-medium-amd64` and/or `saas-linux-medium-arm64` for build jobs. These tags should match
    `${OCUDU_RUNNER_TAG}-amd64` / `${OCUDU_RUNNER_TAG}-arm64` in case you want to modify that variable.
  - `avx512` for jobs requiring AVX512 instruction set
  - `sctp` for jobs requiring SCTP protocol support
- See [GitLab Runner installation guide](https://docs.gitlab.com/runner/install/)

##### Option 3 - Hybrid (Shared + Custom)

- Enable both GitLab shared runners and custom runners
- Shared runners handle standard jobs
- Custom runners handle specialized requirements (AVX512, SCTP)
- Best of both worlds: leverage GitLab infrastructure while supporting custom features
