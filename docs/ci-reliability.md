# CI Reliability and Optimization

## Implemented controls

- Least privilege: workflows default to `contents: read`.
- Time bounds: every job has `timeout-minutes`.
- Concurrency: newer runs cancel superseded CI/nightly work for the same ref.
- Dependency cache: pip cache keys include `requirements-dev.txt`.
- Compiler matrix: regression runs independently on GCC and Clang with
  `fail-fast: false` so both results remain visible.
- Dependency maintenance: Dependabot checks GitHub Actions and pip weekly.
- Evidence retention: JUnit and diagnostics upload with `if: always()`.
- Gate: one stable `Required CI quality gate` check aggregates required PR jobs.

## Trade-offs

- The compiler matrix roughly doubles regression runner usage but catches
  compiler-specific behavior.
- Cache can become stale when inputs are incomplete; the dependency file is an
  explicit invalidation input, and deleting the cache remains a recovery step.
- Official actions currently use reviewed major-version tags and Dependabot.
  Commit-SHA pinning would reduce tag-movement risk but requires an intentional
  recurring SHA update process.

## Flaky-test policy

- No automatic blanket retries.
- Preserve the first-run failure and diagnostics.
- Classify product, test, environment, or infrastructure cause.
- Quarantine only with an owner, issue, evidence, and removal deadline.
- A rerun is diagnostic evidence, never the fix itself.

GitHub job duration before/after must be filled from actual Actions runs after
push; local wall-clock measurements are recorded in `docs/local-verification.md`
and must not be presented as GitHub runner timings.
