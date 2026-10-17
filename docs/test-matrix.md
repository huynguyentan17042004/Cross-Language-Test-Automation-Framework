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
| OOP-POS-001 | REQ-OOP | structural | `getRow()` and `getCol()` are public | compile succeeds | automated |
| OOP-POS-002 | REQ-OOP | structural | `r_` and `c_` are private | compiler diagnostic is specifically private access | automated |
| OOP-MAP-001 | REQ-OOP | structural | `MapElement::req_dmg` is private | compiler diagnostic is specifically private access | automated |
| OOP-MOVE-001 | REQ-OOP | structural | `MovingObject::index` is protected | compiler diagnostic is specifically protected access | automated |
| OOP-INHERIT-001 | REQ-OOP | structural | FlyTeam/GroundTeam inherit Warrior | `std::is_base_of` static assertions compile | automated |
| OOP-ABSTRACT-001 | REQ-OOP | structural | MovingObject is abstract | `std::is_abstract` static assertion compiles | automated |
| OOP-DTOR-001 | REQ-OOP | structural | MovingObject has virtual destructor | `std::has_virtual_destructor` static assertion compiles | automated |
| CFG-001 | REQ-CONFIG §3.10 | integration | parse all 21 keys with whitespace and format using `str()` | exact independently written output | automated |
| GAME-MOVE-001 | REQ-MOVE | integration | FlyTeam1 moves right from `(0,0)` on an empty map | final position `(0,1)` and movement message | automated |
| GAME-ENCOUNTER-001 | REQ-ENCOUNTER | integration | FlyTeam1 and DragonLord share `(0,0)` | encounter message and defeated state | automated |
| GAME-STOP-001 | REQ-STOP | integration | both FlyTeams have HP `1` | `isStop()` is true | automated |
| GAME-BASELINE-019–022 | REQ-GAME | integration/slow | select and execute legacy cases 19-22 independently | case-specific begin/end process evidence; not full correctness | automated |
| MEM-POS-001 | REQ-MEM | memory | Position harness under ASan | no sanitizer error/process failure | automated |
