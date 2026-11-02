# OCUDU Merge Request Template

<!--

## Pre-submission Checklist
- Read the OCUDU contribution guidelines.
- Browse the open merge requests to ensure this is not already worked on.
- Perform a self-review of the code and remove any debug prints or temporary hacks.
- Rebase your changes on the latest version of the dev branch.
- Confirm that all existing unit tests pass and new tests have been added for the changes.

-->

## Description

<!--
Provide a general summary of your changes in the title above.
Why is this change required? What problem does it solve?
-->

## Type of Change

- [ ] Bug Fix
- [ ] New Feature
- [ ] Refactor / Technical Debt
- [ ] Documentation

### Bug Fixes

- Provide a detailed summary of the issue being resolved and the root cause.
- **Diagnostic Information:** If no related issue exists for this fix, you should include the following in this
  description:
  - Relevant logs and PCAP files demonstrating the failure.
  - Configuration files used when the issue occurred.
  - Steps to reproduce the original problem.
- **Verification:** Detail how existing unit tests were extended or which new unit tests were created to ensure this
  specific issue does not recur.

### New Features

- Provide a detailed summary of the new functionality.
- **Specifications:** List the relevant 3GPP or ORAN specifications (including version and section) that this feature
  implements.
- **Unit Testing:** Describe the new unit tests created to demonstrate that the feature functions as intended.
- **E2E Validation:** Provide details on the end-to-end testing performed.
- **Attachments:** If possible, attach logs, PCAPs and configuration files of a successful run to the merge request as
  evidence of functional verification.

### Refactors

- Provide a summary of the architectural or structural changes made.
- **Justification:** Explain why this refactor is necessary (e.g., improving maintainability, performance, or reducing
  technical debt).
- **Regression Testing:** Confirm that no functional changes were introduced. Detail how existing unit tests were used
  or updated to verify that the system's behavior remains consistent.

### Documentation

- Provide a summary of the documentation changes or additions.
- **Verification:** Confirm that the documentation has been rendered/built locally to ensure there are no formatting or
  syntax errors.

## Related Issues

- Link the relevant issue(s) here (e.g., Closes #0).

---

## Reviewer Notes

- Provide any specific focus areas for the reviewer, such as architectural decisions, performance concerns, or potential
  impact on other modules.

---

## Checklist

- [ ] I have read the [OCUDU contribution guidelines](https://docs.ocudu.org/dev_guide/contributing_guide/)
- [ ] My code follows the code style of this project. See the
      [code guidelines](https://docs.ocudu.org/dev_guide/code_guide/).
- [ ] I have updated the documentation accordingly.
- [ ] I have added tests to cover my changes, and all previous tests pass.
- [ ] I have enabled GitLab Shared Runners in my private OCUDU fork. See the
      [CI setup instructions](https://docs.ocudu.org/dev_guide/contributing_guide/#gitlab-ci)
