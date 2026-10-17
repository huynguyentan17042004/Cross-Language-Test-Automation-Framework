# LAB 05 Integration Scenarios

These scenario cards define the assertions before execution. Expected values
come from the assignment contract, not from copying the SUT output.

## INT-CONFIG-001 — Parse and format a complete configuration

- Given: a temporary configuration containing all 21 keys from section 3.10,
  with spaces around `=`.
- When: `Configuration` parses the file and `str()` formats it.
- Then: whitespace around values is removed and every field appears in the
  specified order and format.
- Isolation: Pytest creates the input below its own temporary directory.

## INT-MOVE-001 — FlyTeam1 moves right

- Given: a 5x5 empty map, FlyTeam1 at `(0,0)`, movement rule `R`, and positive
  HP.
- When: FlyTeam1 performs one movement action.
- Then: the position changes from `(0,0)` to `(0,1)` and the movement message
  is emitted.
- Contract source: FlyTeam movement rule and Position format.

## INT-ENCOUNTER-001 — FlyTeam1 encounters DragonLord

- Given: FlyTeam1 and DragonLord both start at `(0,0)`.
- When: FlyTeam1 fights DragonLord.
- Then: the encounter message is emitted and DragonLord is marked defeated.
- Contract source: FlyTeam–DragonLord encounter behavior.

## INT-STOP-001 — Both FlyTeams are out

- Given: both FlyTeams start with HP equal to `1`.
- When: the program evaluates its stop condition.
- Then: `isStop()` returns true.
- Contract source: program stop condition in the current assignment snapshot.

## LEGACY-019–022 — Existing full-program samples remain selectable

- Given: one case ID from 19 through 22.
- When: the argument-driven legacy harness runs that case.
- Then: the process succeeds and prints that case's begin/end trace.
- Boundary: this proves independent execution and traceability, not that every
  value printed by the legacy sample is specification-correct.
