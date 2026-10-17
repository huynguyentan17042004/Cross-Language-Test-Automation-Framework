# Specification Decisions

## Status vocabulary

- `DECIDED`: backed by version 1.1 or an explicit project decision.
- `OPEN`: insufficient evidence; do not create a mandatory assertion.

## Decisions

| ID | Status | Decision / question | Evidence |
|---|---|---|---|
| SPEC-QUESTION-01 | OPEN | DragonLord constructor parameters remain unresolved because section 3.8 and the v1.1 change log conflict. | Specification section 3.8 and page 25/25 |
| SPEC-QUESTION-02 | DECIDED | Stopping and exhaustion conditions use `HP <= 1`; stop immediately when a stop condition is met. | Version 1.1 change log, page 25/25 |
| SPEC-QUESTION-03 | DECIDED | This framework preserves repository filenames `dragon.cpp` and `dragon.h`; the original submission naming rule is not silently applied to this snapshot. | Repository baseline versus specification section 4 |
| SPEC-QUESTION-04 | OPEN | Integer rounding/capping after 20% and 25% item effects requires an explicit interpretation. | Specification section 3.12 |
| SPEC-QUESTION-05 | OPEN | Malformed configuration and unspecified edge cases are exploratory until requirements are supplied. | No normative behavior found |

