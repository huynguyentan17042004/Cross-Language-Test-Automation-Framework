## Summary

This PR completes the remaining Dragon Warriors Pytest and CI/CD framework
milestones in one integrated update, covering LAB 05 through LAB 10.

The C++ implementation under `sut/legacy/` is preserved. New behavior is tested
through external C++ harnesses orchestrated by Pytest, with independently
defined expected results and traceable diagnostics.

## What changed

### LAB 05 — Configuration and integration testing

- Added a complete 21-key temporary configuration builder.
- Added exact `Configuration::str()` formatting verification, including
  intentional whitespace handling.
- Added movement, encounter, and stop-condition integration scenarios.
- Made legacy sample cases 19–22 independently selectable by argument without
  manually editing the original `main()`.
- Added JUnit and per-process stdout/stderr/run metadata artifacts.

### LAB 06 — Domain regression coverage

- Added a reusable C++ domain harness.
- Added 48 regression scenarios across seven requirement groups:
  - Warrior HP and damage boundaries.
  - FlyTeam and GroundTeam movement decisions.
  - DragonLord derived-position and Manhattan-distance boundaries.
  - SmartDragon direction priority, stationary behavior, and spawn interval.
  - DragonScale, HealingHerb, and TrapEnhancer boundaries.
  - TeamBag capacity, insertion order, and retrieval behavior.
  - ArrayMovingObject capacity and invalid lookup behavior.
- Added a dedicated `regression` Pytest marker and CLI layer.
- Documented covered, partially covered, and specification-blocked behavior.

### LAB 07 — Failure triage and quality gate

- Added CI troubleshooting guidance for compile, assertion, fixture, process,
  and artifact failures.
- Added a stable `Required CI quality gate` job that requires unit, structural,
  integration, and GCC/Clang regression jobs to succeed.
- Added a branch-protection setup and verification checklist.
- Kept scheduled memory testing outside the required PR gate.

### LAB 08 — Memory safety and scheduled CI

- Expanded AddressSanitizer coverage from Position only to six lifecycle paths:
  Position, Map, Configuration/Program, Bag, SmartDragon, and
  ArrayMovingObject.
- Added process diagnostics for sanitizer runs.
- Added platform-aware leak detection: enabled on Linux and omitted on macOS,
  where the Apple ASan runtime reports it as unsupported.
- Added a weekday scheduled workflow plus manual `workflow_dispatch` support.

### LAB 09 — CI reliability, security, and optimization

- Added GCC and Clang regression matrix execution with `fail-fast: false`.
- Added pip dependency caching keyed by `requirements-dev.txt`.
- Added workflow concurrency controls and explicit job timeouts.
- Retained least-privilege `contents: read` permissions.
- Added weekly Dependabot checks for GitHub Actions and Python dependencies.
- Documented cache, matrix, action-version, and flaky-test trade-offs.

### LAB 10 — Release artifact packaging

- Added a manual/tag-triggered release workflow.
- Added a version-validated Python release builder.
- The generated archive contains:
  - Platform-specific C++ executable.
  - Four sample configuration files.
  - JUnit test evidence.
  - Build manifest with version, commit, platform, and compile command.
  - Usage README.
- Added a SHA-256 checksum beside every archive.
- The workflow uploads a versioned artifact but does not automatically publish
  a GitHub Release or deploy a service.

## CI flow

```text
Pull request
    |
    +-- Unit tests
    +-- Structural contract tests
    +-- Configuration/integration tests
    +-- Regression tests (GCC)
    +-- Regression tests (Clang)
              |
              v
       Required CI quality gate
```

AddressSanitizer runs separately in the nightly/manual memory workflow. Release
packaging runs only through manual dispatch or a `v*` tag.

## Local verification

Environment:

- macOS
- Python 3.13.13
- Pytest 8.4.2
- Apple Clang 21.0.0 through `g++`/`clang++`

Results:

| Verification | Result |
|---|---:|
| Full Pytest suite | 116 passed in 14.65s |
| Framework layer | 18 passed |
| Unit layer | 26 passed |
| Structural layer | 10 passed |
| Integration layer | 8 passed |
| Regression layer | 48 passed |
| Memory/ASan layer | 6 passed |
| Clang regression run | 48 passed |
| Python compilation check | passed |
| Workflow and Dependabot YAML parse | passed |
| `git diff --check` | passed |
| Release archive creation | passed |
| SHA-256 verification | passed |
| Packaged executable run | passed |

Main local command:

```bash
python -m pytest --layer all --junitxml=reports/release-tests.xml
```

## Review guide

Recommended review order:

1. `framework/configuration.py`, `framework/diagnostics.py`, and
   `framework/sanitizers.py`.
2. `cpp_harness/unit/domain_harness.cpp` and the regression tests.
3. Memory tests and `nightly.yml`.
4. `ci.yml`, especially the compiler matrix and quality gate.
5. `scripts/build_release.py` and `release.yml`.
6. Requirement matrix, coverage gaps, and operational documentation.

## Boundaries and follow-up verification

- Production C++ behavior was not changed to make tests pass.
- Item fractional rounding outside exact specification boundaries remains a
  documented specification question rather than a guessed assertion.
- Legacy cases 19–22 prove independent execution and traceability; they do not
  claim every printed value is specification-correct.
- Linux leak detection, the GCC/Clang runner matrix, cache behavior, artifact
  upload, nightly scheduling, and the quality gate require GitHub Actions
  evidence after this branch is pushed.
- Branch protection must be enabled in repository settings after the quality
  gate check appears on the pull request.

## Checklist

- [x] Local full suite passes.
- [x] Regression suite passes with both local compiler commands.
- [x] ASan lifecycle suite passes locally.
- [x] Workflow YAML parses successfully.
- [x] Release package, checksum, and executable were validated locally.
- [ ] GitHub Actions PR workflow passes on Ubuntu.
- [ ] CI artifacts are downloadable and contain the expected evidence.
- [ ] Nightly memory workflow is manually dispatched and verified.
- [ ] Release artifact workflow is manually dispatched and verified.
- [ ] `Required CI quality gate` is configured as a protected-branch check.
