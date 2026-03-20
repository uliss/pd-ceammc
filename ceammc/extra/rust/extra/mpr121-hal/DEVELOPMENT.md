## Running your Linter Locally

This project uses [MegaLinter](https://github.com/oxsecurity/megalinter) which provides linters for various different file formats and languages. When a Pull request to main is done, the linters will run and ensure the codebase is in good standing. It is recommended that you run the linter locally beforehand as it can sometimes autofix common mistakes.

```bash
npx mega-linter-runner
```

You will need to have docker and Node installed to use this, more information can be found on their [repo](https://github.com/oxsecurity/megalinter)

### Issues with Running Mega Linter on Forks

Sadly due to the way GitHub handles PAT Tokens we are unable to run all of MegaLinters functionality when processing Pull Requests from forks. It is recommended that you run the linter locally as stated above, otherwise you can [setup your own PAT token](https://github.com/marketplace/actions/megalinter#apply-fixes-issues) for your fork repository and make a PR there in which you will receive the full capability of MegaLinter.

#### PAT Token for Mega Linter running on Forks

There are Two PAT Tokens requred for full GitHub MegaLinter functionality. One is already for you which is the `secrets.GITHUB_TOKEN` however to get the full functionality you will be required to create a new PAT Token with the following settings:

- Commit Statuses: Read and Write
- Contents: Read and Write
- Pull Requests: Read and Write
- Work Flows: Read and Write

If this is your first time creating a Mega Linter PAT Token it may be recommended to create it for all Repositories so you can use it for multiple projects.

This will enable MegaLinter to create PRs and modify files.

More on this can be found here:

<https://megalinter.io/latest/install-github/>

<https://github.com/oxsecurity/megalinter/issues/4317>

#### PAT Token for Dependabot (Main Repo Only)

This repository also contains a workflow for auto merging Dependabot updates. ([Workflow](./.github/workflows/dependabot.yaml)) ([Dependabot Configuration](./.github/dependabot.yaml)).

As such it requires another PAT Token named `DEPENDABOT` with the following credentials

- Commit Statuses: Read and Write
- Contents: Read and Write
- Pull Requests: Read and Write
- Work Flows: Read and Write

More on this GitHub action can be found [here](https://github.com/marketplace/actions/dependabot-auto-merge).

### Issues with rust fmt

Currently at the time of this commit `rust fmt` is not supported as part of MegaLinter, thus to ensure it is correctly formatted we have added an extra build stage which can be seen [here](./.github/workflows/mega-linter.yaml). You can run this locally using

```bash
cargo fmt --all
```

## Running Integration Tests

At present there is limited integration tests for the driver. Generic tests that can be applied across numerous architectures/platforms are stored in a [sub crate](./tests-common/) within the repository. These are platform agnostic and are focused on the driver itself. Within the [tests](./tests/) directory are platform dependent tests which can run on the developer OS or MCU platforms.

When running the tests on the developer platform with the FT232H Breakout Board, you will need to run them single threaded and not in parallel. This is due to there only being one I2C hardware block and the locking around that. To do that, run the following command:

```bash
cargo test -- --test-threads=1
```
