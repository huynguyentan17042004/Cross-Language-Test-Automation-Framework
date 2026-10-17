# Initial Test Matrix

| Test ID | Requirement | Level | Scenario | Oracle | Status |
|---|---|---|---|---|---|
| POS-001 | REQ-POS | unit/smoke | default constructor | `(0,0)` from section 3.3 | automated |
| POS-002 | REQ-POS | unit/smoke | explicit row and column | `str()` format from section 3.3 | automated |
| POS-003 | REQ-POS | unit/smoke | setter mutation | getter/setter contract from section 3.3 | automated |
| POS-004 | REQ-POS | unit/smoke | equality true/false partitions | `isEqual` contract from section 3.3 | automated |
| MAPEL-001 | REQ-MAP | unit | PATH/OBSTACLE default damage and explicit ground damage | `MapElement` constructor/getters from section 3.2 | automated |
| DMG-001 | REQ-DMG | unit | five ground-obstacle coordinates, including modulo behavior | independently calculated `(r*257+c*139+89)%900+1` | automated |
| MAP-001 | REQ-MAP | unit | PATH for FlyTeam and GroundTeam | PATH is traversable | automated |
| MAP-002 | REQ-MAP | unit | OBSTACLE for FlyTeam and GroundTeam | OBSTACLE is blocked | automated |
| MAP-003 | REQ-MAP | unit | GROUND_OBSTACLE for FlyTeam | FlyTeam ignores ground damage threshold | automated |
| MAP-004 | REQ-MAP | unit/boundary | GroundTeam damage equals/just exceeds required damage | valid only when damage is strictly greater | automated |
| MAP-005 | REQ-MAP | unit/negative | negative and upper out-of-bounds positions | invalid before element lookup | automated |
| OOP-POS-001 | REQ-OOP | structural | `getRow()` is public | section 3.3 | automated |
| OOP-POS-002 | REQ-OOP | structural | `r_` is private | section 3.3 | automated |
| GAME-BASELINE-001 | REQ-GAME | integration | legacy cases 19-22 execute | process evidence only; not full correctness | automated |
| MEM-POS-001 | REQ-MEM | memory | Position harness under ASan | no sanitizer error/process failure | automated |
