# CI Failure Triage

Always diagnose the first failing layer before rerunning. A green rerun without
an identified cause is not evidence that a defect was fixed.

## Decision path

1. Did checkout/setup fail? Classify as infrastructure or dependency setup.
2. Did `g++` return nonzero? Read the first compiler diagnostic and reproduce
   the exact command locally.
3. Did the executable return nonzero or time out? Inspect the uploaded `.run`,
   `.stdout`, and `.stderr` files.
4. Did Pytest report an assertion diff? Compare the oracle to its requirement;
   do not copy current output into expected data.
5. Did a test pass locally only? Compare compiler, Python version, paths,
   environment variables, and platform-specific sanitizer support.

## RCA 1 — compile failure exercise

- Symptom: the build step is red before Pytest starts.
- Evidence: compiler command, nonzero exit code, and first relevant diagnostic.
- Root cause class: C++ syntax/include/signature error, not an assertion error.
- Fix: correct the isolated branch change and rebuild with the same command.
- Verification: build job turns green and downstream test jobs execute.

## RCA 2 — assertion failure exercise

- Symptom: compile and process execution succeed but Pytest shows expected !=
  actual.
- Evidence: named test, assertion diff, stdout/stderr artifact, spec reference.
- Root cause class: product behavior, wrong oracle, or incorrect test setup.
- Fix: change only the component proven wrong; never update expected merely to
  copy observed output.
- Verification: reproduce the original red result, apply fix, then obtain a
  green run for the same test.

## RCA 3 — missing fixture/artifact exercise

- Symptom: Pytest setup error or artifact upload warning; no test call occurs.
- Evidence: fixture lookup error or `if-no-files-found` warning and job paths.
- Root cause class: test infrastructure/configuration.
- Fix: restore the fixture or correct the producer/upload path. Artifact upload
  remains `if: always()` so failure evidence is retained.
- Verification: test reaches call phase and the downloadable artifact contains
  the expected JUnit and diagnostics files.

## Status vocabulary

- Pytest failed: at least one collected test failed or errored.
- Job failed: any required step in that runner job failed.
- Workflow failed: at least one non-ignored job failed; the quality gate also
  fails when a required upstream job fails or is cancelled.
- Job skipped: its condition evaluated false; skipped is not equivalent to
  success for the required quality gate.
