# Dragon Warriors Pytest Framework - Design v1

## Goal

Treat the C++ program as the System Under Test and use Pytest to compile C++
harnesses, execute binaries, preserve diagnostics, and report assertions.

## Architecture

```text
Pytest data row
    -> C++ harness compiler
    -> executable runner with timeout
    -> structured stdout/stderr/exit result
    -> Pytest assertion and report
```

The framework contains:

- `framework/models.py`: immutable compile, run, and probe evidence.
- `framework/compiler.py`: deterministic C++ argument-list compilation.
- `framework/executor.py`: safe execution without `shell=True`.
- `framework/probes.py`: public/private/protected/contract/toolchain diagnosis.
- `framework/assertions.py`: readable compile and execution failures.
- `conftest.py`: shared compiler, build, harness, and runner fixtures.
- `cpp_harness/`: small C++ adapters grouped by domain.
- `tests/`: framework, smoke, unit, structural, integration, and memory layers.

## Important decisions

- The original repository is not modified.
- `sut/legacy/` is a pinned source snapshot with recorded provenance.
- One stateless harness is compiled per domain and reused across parameter rows.
- Business expectations live in Pytest, not in the harness.
- Compiler failure, process failure, timeout, assertion failure, and unresolved
  specification are different outcomes.
- Default implementation remains local-first. Git, GitHub, and GitHub Actions
  are introduced only after the framework passes locally.

## Implementation gates

1. Framework self-tests.
2. Position vertical slice.
3. Structural probe example.
4. Legacy integration baseline.
5. AddressSanitizer example.
6. Local all-tests verification and JUnit report.
7. Git/GitHub learning.
8. Incremental GitHub Actions jobs.

