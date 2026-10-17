# Domain Regression Coverage

The `regression` layer uses independently calculated inputs and expected values.
It does not generate oracles from the current C++ output.

| Requirement group | Automated coverage | Status |
|---|---|---|
| REQ-WAR | constructor HP/DMG lower and upper boundaries; setter exhaustion at HP `<=1` | covered |
| REQ-FLY / REQ-GROUND | normal move, opposite fallback, blocked on both sides | covered |
| REQ-LORD | derived coordinates; Manhattan boundary 5 versus 6; coordinate swap | covered |
| REQ-SMART | U/R/D/L movement, U tie priority, SD3 stationary, spawn interval 5 | covered |
| REQ-ITEM | DragonScale 400/401, HealingHerb 100/101, TrapEnhancer actor type | covered |
| REQ-BAG | capacities 5/7, overflow rejection, head insertion, usable/type retrieval | covered |
| REQ-ARRAY | below/at/over capacity and invalid negative lookup | covered |
| REQ-GAME | movement, encounter, stop, legacy 19–22 | covered by integration layer |

## Deliberate gaps

- Item fractional rounding outside exact boundary examples remains blocked by
  `SPEC-QUESTION-04`; no guessed mandatory assertion is added.
- The spawn interval helper is verified for move counts 0, 4, 5, 6 and 10.
  A separate assertion that only a *successful* DragonLord move increments the
  count remains partially covered by the integration traces.
- Full combat/trap simulations retain legacy process-evidence status until
  every expected message and state transition is mapped to an unambiguous spec
  statement.
- No `xfail` markers are used to hide a known product failure.
