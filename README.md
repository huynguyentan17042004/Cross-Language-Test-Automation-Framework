# Dragon Warriors Pytest Framework

An isolated Pytest-driven framework for compiling and testing the Dragon
Warriors C++ assignment. The original repository remains unchanged.

## Requirements

- Python 3.11 or newer
- A C++ compiler available as `g++`, or set `CXX`
- macOS or Linux for the current local workflow

## Local setup

```bash
cd dragon_warriors_pytest_framework
/opt/anaconda3/bin/python3 -m venv .venv
.venv/bin/python -m pip install -r requirements-dev.txt
```

Use the same interpreter for every Python command:

```bash
.venv/bin/python -m pytest
```

The default CLI shows every test name, its tags, the five slowest durations,
and a per-tag summary. Use `-q` only when you intentionally want compact output.

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

# Existing full-program baseline
.venv/bin/python -m pytest --layer integration

# AddressSanitizer example
.venv/bin/python -m pytest --layer memory

# Tests tagged as intentionally slower
.venv/bin/python -m pytest --layer slow

# Full suite with detailed output
.venv/bin/python -m pytest --layer all

# JUnit evidence for later CI use
.venv/bin/python -m pytest --layer all --junitxml=reports/pytest.xml
```

Standard Pytest marker expressions remain available for combinations:

```bash
.venv/bin/python -m pytest -m "smoke or structural"
.venv/bin/python -m pytest -m "unit and not slow"
```

## Result boundaries

- A successful compile does not prove functional correctness.
- Exit code 0 does not prove stdout matches the specification.
- A compiler failure is not automatically proof that a field is private.
- Unknown specification behavior is recorded in `docs/spec-decisions.md`, not
  guessed into a mandatory assertion.

## Current automated evidence

- Compiler success and diagnostic preservation.
- Process exit, stdout/stderr capture, missing command, and timeout behavior.
- Position functional scenarios through a C++ harness.
- MapElement constructor/getter scenarios and five independently calculated
  ground-obstacle damage requirements.
- Map validity decision table for path, obstacle, ground obstacle, FlyTeam,
  GroundTeam, threshold boundaries, and out-of-bounds positions.
- Position public/private structural probes.
- Legacy integration cases 19-22 execute from the isolated snapshot.
- Position harness runs under AddressSanitizer.

Pull requests build the C++ suite and run the unit layer on an Ubuntu runner.
CI publishes `reports/unit.xml` as the `unit-test-report` artifact even when
the Pytest step fails. The next CI milestone is a separate structural-test job.
