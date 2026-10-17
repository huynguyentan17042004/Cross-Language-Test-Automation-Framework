# Initial Test Matrix

| Test ID | Requirement | Level | Scenario | Oracle | Status |
|---|---|---|---|---|---|
| POS-001 | REQ-POS | unit/smoke | default constructor | `(0,0)` from section 3.3 | automated |
| POS-002 | REQ-POS | unit/smoke | explicit row and column | `str()` format from section 3.3 | automated |
| POS-003 | REQ-POS | unit/smoke | setter mutation | getter/setter contract from section 3.3 | automated |
| POS-004 | REQ-POS | unit/smoke | equality true/false partitions | `isEqual` contract from section 3.3 | automated |
| OOP-POS-001 | REQ-OOP | structural | `getRow()` is public | section 3.3 | automated |
| OOP-POS-002 | REQ-OOP | structural | `r_` is private | section 3.3 | automated |
| GAME-BASELINE-001 | REQ-GAME | integration | legacy cases 19-22 execute | process evidence only; not full correctness | automated |
| MEM-POS-001 | REQ-MEM | memory | Position harness under ASan | no sanitizer error/process failure | automated |

