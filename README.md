# Dragon Warriors — Automated Testing Framework with GitHub Actions CI

A Python/Pytest-based automated testing framework for validating the **Dragon Warriors C++ object-oriented application**. The framework compiles and tests the C++ code through focused test layers, while **GitHub Actions CI** validates pull requests and retains test reports and diagnostics.

> **Scope:** The project implements CI plus optional console release packaging.
> It does not deploy a web service or publish a GitHub Release automatically.

## Tech Stack

- **Application under test:** C++11
- **Test automation:** Python 3.11+, Pytest
- **Build/compiler:** `g++` (or set `CXX`)
- **CI:** GitHub Actions, Ubuntu runners, Python 3.13 in CI
- **Reports:** JUnit XML, GitHub Actions artifacts
- **Version control:** Git, feature/lab branches, pull requests

## Continuous Integration (CI)

The workflow is defined in [`.github/workflows/ci.yml`](.github/workflows/ci.yml). It runs when a pull request targets the `python-driven-version` branch, and it can also be started manually with `workflow_dispatch`.

### Pipeline flow

```text
Developer creates a feature/lab branch
                 |
                 v
Pull request -> python-driven-version
                 |
                 v
          GitHub Actions
                 |
       +---------+----------+------------+
       |         |          |            |
       v         v          v            v
    Unit      Structural  Integration  Regression
    tests     tests       tests        GCC + Clang
       |         |          |            |
       +---------+----------+------------+
                 |
                 v
         Required quality gate
                 |
                 v
         Review results / merge
```

### Independent CI jobs

| Job | What it validates | Report/artifacts |
| --- | --- | --- |
| `unit-tests` | Compiles the legacy C++ suite, then runs Pytest's unit layer | `reports/unit.xml` |
| `structural-tests` | Checks compile-time OOP structural contracts | `reports/structural.xml` and structural diagnostics |
| `integration-tests` | Checks configuration and integrated game scenarios | `reports/integration.xml` and integration diagnostics |
| `regression-tests` | Runs domain regression on GCC and Clang | compiler-specific `reports/regression.xml` |
| `quality-gate` | Requires every PR test job to succeed | stable branch-protection check |

Jobs use Ubuntu runners, pip caching, least-privilege permissions, concurrency
control, and explicit timeouts. Artifact-upload steps use `if: always()` so
reports/diagnostics can be retained after test failures when files exist.

The separate nightly workflow runs AddressSanitizer lifecycle tests. The
manual/tag release workflow packages a platform-specific executable, configs,
JUnit evidence, manifest, and SHA-256 checksum.

**CI techniques demonstrated:** pull-request validation, automated C++ build verification, test-layer separation, automated regression checks, machine-readable test reporting, failure diagnostics, and artifact retention.

[View CI runs](https://github.com/huynguyentan17042004/Dragon-Warriors-OOP-Test-Suite/actions)

## Requirements

- Python **3.11 or newer**
- A C++ compiler available as `g++`, or set `CXX`
- macOS or Linux for the current local workflow

## Local setup

```bash
cd dragon_warriors_pytest_framework
python3 -m venv .venv
.venv/bin/python -m pip install -r requirements-dev.txt
```

Use the same interpreter for every Python command:

```bash
.venv/bin/python -m pytest
```

The default CLI shows every test name, its tags, the five slowest durations, and a per-tag summary. Use `-q` only when you intentionally want compact output.

## Run separate test layers

```bash
# Python framework behavior only
.venv/bin/python -m pytest --layer framework

# First C++ vertical slice
.venv/bin/python -m pytest --layer smoke

# All focused C++ unit behavior
.venv/bin/python -m pytest --layer unit

# Compile-time OOP checks
.venv/bin/python -m pytest --layer structural

# Configuration, game scenarios, and selectable legacy cases
.venv/bin/python -m pytest --layer integration

# Warrior, movement, dragon, item, bag, and array regression
.venv/bin/python -m pytest --layer regression

# AddressSanitizer lifecycle suite
.venv/bin/python -m pytest --layer memory

# Tests tagged as intentionally slower
.venv/bin/python -m pytest --layer slow

# Full suite with detailed output
.venv/bin/python -m pytest --layer all

# JUnit evidence for local/CI review
.venv/bin/python -m pytest --layer all --junitxml=reports/pytest.xml
```

Standard Pytest marker expressions remain available for combinations:

```bash
.venv/bin/python -m pytest -m "smoke or structural"
.venv/bin/python -m pytest -m "unit and not slow"
```

## Current automated test coverage

- Compiler success and diagnostic preservation.
- Process exit, stdout/stderr capture, missing command, and timeout behavior.
- Position functional scenarios through a C++ harness.
- MapElement constructor/getter scenarios and five independently calculated ground-obstacle damage requirements.
- Map validity decision table covering paths, obstacles, ground obstacles, FlyTeam, GroundTeam, threshold boundaries, and out-of-bounds positions.
- Ten isolated structural probes for public/private/protected access, inheritance, abstract base-class status, and virtual destruction.
- Configuration parsing and exact `Configuration::str()` formatting.
- Specification-traceable movement, encounter, and stop-condition scenarios using temporary configuration files.
- Independently executed legacy integration cases 19–22 using a case argument.
- Forty-eight regression scenarios across Warrior, movement, DragonLord,
  SmartDragon, items, bags, and ArrayMovingObject.
- Position, Map, Configuration, Program, Bag, SmartDragon, and Array lifecycle
  checks under AddressSanitizer.
- Versioned release packaging with sample configs, manifest, test evidence, and
  checksum.

## Test result boundaries

- Successful compilation does **not** prove functional correctness.
- Process exit code `0` does **not** prove stdout matches the specification.
- A compiler failure is **not** automatically proof that a field is private.
- Unknown specification behavior is recorded in [`docs/spec-decisions.md`](docs/spec-decisions.md), not guessed into mandatory assertions.

## Development workflow

1. Create a focused feature/lab branch from `python-driven-version`.
2. Implement changes and run the relevant Pytest layer locally.
3. Push the branch and open a pull request targeting `python-driven-version`.
4. Review unit, structural, integration, GCC/Clang regression, and quality-gate results.
5. Inspect uploaded JUnit XML reports and diagnostics when needed.
6. Merge after review and required checks pass.

Branch-protection setup is documented in
[`docs/branch-protection.md`](docs/branch-protection.md). Reliability decisions,
failure triage, coverage gaps, and release usage are documented under `docs/`.
