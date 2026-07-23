import subprocess
import os

# ==== DANH SÁCH TEST ====
test_position = [
    ("Position", "r_", "attribute", "private", "d->r_;"),
    ("Position", "c_", "attribute", "private", "d->c_;"),
    ("Position", "npos", "attribute", "public", "Position::npos;"),
    ("Position", "getRow()", "method", "public", "d->getRow();"),
    ("Position", "getCol()", "method", "public", "d->getCol();"),
    ("Position", "setRow(r)", "method", "public", "d->setRow(1);"),
    ("Position", "setCol(c)", "method", "public", "d->setCol(2);"),
    ("Position", "str()", "method", "public", "d->str();"),
    ("Position", "isEqual(r,c)", "method", "public", "d->isEqual(0,0);"),
    ("Position", "isEqual(pos)", "method", "public", "d->isEqual(Position(0,0));"),
]

test_mapele = [
    ("MapElement", "type", "attribute", "private", "d->type;"),
    ("MapElement", "req_dmg", "attribute", "private", "d->req_dmg;"),
    ("MapElement", "getType()", "method", "public", "d->getType();"),
    ("MapElement", "getReqDmg()", "method", "public", "d->getReqDmg();"),
]

test_map = [
    ("Map", "num_rows", "attribute", "private", "d->num_rows;"),
    ("Map", "num_cols", "attribute", "private", "d->num_cols;"),
    ("Map", "map", "attribute", "private", "d->map;"),
    ("Map", "isValid(pos, ptr)", "method", "public", "d->isValid(Position::npos,nullptr);"),
]

test_fly = [
    # moving obj
    ("FlyTeam", "index", "attribute", "protected", "d->index;"),
    ("FlyTeam", "pos", "attribute", "protected", "d->pos;"),
    ("FlyTeam", "map", "attribute", "protected", "d->map;"),
    ("FlyTeam", "name", "attribute", "protected", "d->name;"),
    ("FlyTeam", "getNextPosition()", "method", "public", "d->getNextPosition();"),
    ("FlyTeam", "getCurrentPosition()", "method", "public", "d->getCurrentPosition();"),
    ("FlyTeam", "move()", "method", "public", "d->getCurrentPosition();"),
    ("FlyTeam", "str()", "method", "public", "d->getCurrentPosition();"),
    # Warrior
    ("FlyTeam", "hp", "attribute", "protected", "d->hp;"),
    ("FlyTeam", "damage", "attribute", "protected", "d->damage;"),
    ("FlyTeam", "bag", "attribute", "protected", "d->bag;"),
    ("FlyTeam", "getHp()", "method", "public", "d->getHp();"),
    ("FlyTeam", "setHp(1)", "method", "public", "d->setHp(1);"),
    ("FlyTeam", "getDamage()", "method", "public", "d->getDamage();"),
    ("FlyTeam", "setDamage(1)", "method", "public", "d->setDamage(1);"),
    ("FlyTeam", "setBag()", "method", "public", "d->getBag();"),
    # flyteam
    ("FlyTeam", "moving_rule", "attribute", "private", "d->moving_rule;"),
    ("FlyTeam", "moving_index", "attribute", "private", "d->moving_index;"),
    ("FlyTeam", "attack(lord)", "method", "public", "d->attack(nullptr);"),
]

test_ground = [
    # moving obj
    ("GroundTeam", "index", "attribute", "protected", "d->index;"),
    ("GroundTeam", "pos", "attribute", "protected", "d->pos;"),
    ("GroundTeam", "map", "attribute", "protected", "d->map;"),
    ("GroundTeam", "name", "attribute", "protected", "d->name;"),
    ("GroundTeam", "getNextPosition()", "method", "public", "d->getNextPosition();"),
    ("GroundTeam", "getCurrentPosition()", "method", "public", "d->getCurrentPosition();"),
    ("GroundTeam", "move()", "method", "public", "d->getCurrentPosition();"),
    ("GroundTeam", "str()", "method", "public", "d->getCurrentPosition();"),
    # Warrior
    ("GroundTeam", "hp", "attribute", "protected", "d->hp;"),
    ("GroundTeam", "damage", "attribute", "protected", "d->damage;"),
    ("GroundTeam", "bag", "attribute", "protected", "d->bag;"),
    ("GroundTeam", "getHp()", "method", "public", "d->getHp();"),
    ("GroundTeam", "setHp(1)", "method", "public", "d->setHp(1);"),
    ("GroundTeam", "getDamage()", "method", "public", "d->getDamage();"),
    ("GroundTeam", "setDamage(1)", "method", "public", "d->setDamage(1);"),
    ("GroundTeam", "setBag()", "method", "public", "d->getBag();"),
    # groundteam
    ("GroundTeam", "moving_rule", "attribute", "private", "d->moving_rule;"),
    ("GroundTeam", "moving_index", "attribute", "private", "d->moving_index;"),
    ("GroundTeam", "trap_turns", "attribute", "private", "d->trap_turns;"),
    ("GroundTeam", "trap()", "method", "public", "d->trap(nullptr);"),
    ("GroundTeam", "getTrapTurns()", "method", "public", "d->getTrapTurns();"),
    ("GroundTeam", "setTrapTurns()", "method", "public", "d->getTrapTurns();"),
]


test_war = [
    # moving obj
    ("Warrior", "index", "attribute", "protected", "d->index;"),
    ("Warrior", "pos", "attribute", "protected", "d->pos;"),
    ("Warrior", "map", "attribute", "protected", "d->map;"),
    ("Warrior", "name", "attribute", "protected", "d->name;"),
    ("Warrior", "getNextPosition()", "method", "public", "d->getNextPosition();"),
    ("Warrior", "getCurrentPosition()", "method", "public", "d->getCurrentPosition();"),
    ("Warrior", "move()", "method", "public", "d->getCurrentPosition();"),
    ("Warrior", "str()", "method", "public", "d->getCurrentPosition();"),
    # Warrior
    ("Warrior", "hp", "attribute", "protected", "d->hp;"),
    ("Warrior", "damage", "attribute", "protected", "d->damage;"),
    ("Warrior", "bag", "attribute", "protected", "d->bag;"),
    ("Warrior", "getHp()", "method", "public", "d->getHp();"),
    ("Warrior", "setHp(1)", "method", "public", "d->setHp(1);"),
    ("Warrior", "getDamage()", "method", "public", "d->getDamage();"),
    ("Warrior", "setDamage(1)", "method", "public", "d->setDamage(1);"),
    ("Warrior", "setBag()", "method", "public", "d->getBag();"),
]



test_lord = [
    # moving obj
    ("DragonLord", "index", "attribute", "protected", "d->index;"),
    ("DragonLord", "pos", "attribute", "protected", "d->pos;"),
    ("DragonLord", "map", "attribute", "protected", "d->map;"),
    ("DragonLord", "name", "attribute", "protected", "d->name;"),
    ("DragonLord", "getNextPosition()", "method", "public", "d->getNextPosition();"),
    ("DragonLord", "getCurrentPosition()", "method", "public", "d->getCurrentPosition();"),
    ("DragonLord", "move()", "method", "public", "d->getCurrentPosition();"),
    ("DragonLord", "str()", "method", "public", "d->getCurrentPosition();"),
]


test_movobj = [
    # moving obj
    ("MovingObject", "index", "attribute", "protected", "d->index;"),
    ("MovingObject", "pos", "attribute", "protected", "d->pos;"),
    ("MovingObject", "map", "attribute", "protected", "d->map;"),
    ("MovingObject", "name", "attribute", "protected", "d->name;"),
    ("MovingObject", "getNextPosition()", "method", "public", "d->getNextPosition();"),
    ("MovingObject", "getCurrentPosition()", "method", "public", "d->getCurrentPosition();"),
    ("MovingObject", "move()", "method", "public", "d->getCurrentPosition();"),
    ("MovingObject", "str()", "method", "public", "d->getCurrentPosition();"),
]


test_config = [
    # Configuration attributes
  ("Configuration", "map_num_rows", "attribute", "private", "d->map_num_rows;"),
    ("Configuration", "map_num_cols", "attribute", "private", "d->map_num_cols;"),
    ("Configuration", "max_num_moving_objects", "attribute", "private", "d->max_num_moving_objects;"),

    ("Configuration", "num_obstacles", "attribute", "private", "d->num_obstacles;"),
    ("Configuration", "arr_obstacles", "attribute", "private", "d->arr_obstacles;"),

    ("Configuration", "num_ground_obstacles", "attribute", "private", "d->num_ground_obstacles;"),
    ("Configuration", "arr_ground_obstacles", "attribute", "private", "d->arr_ground_obstacles;"),

    ("Configuration", "flyteam1_moving_rule", "attribute", "private", "d->flyteam1_moving_rule;"),
    ("Configuration", "flyteam1_init_pos", "attribute", "private", "d->flyteam1_init_pos;"),
    ("Configuration", "flyteam1_init_hp", "attribute", "private", "d->flyteam1_init_hp;"),
    ("Configuration", "flyteam1_init_damage", "attribute", "private", "d->flyteam1_init_damage;"),

    ("Configuration", "flyteam2_moving_rule", "attribute", "private", "d->flyteam2_moving_rule;"),
    ("Configuration", "flyteam2_init_pos", "attribute", "private", "d->flyteam2_init_pos;"),
    ("Configuration", "flyteam2_init_hp", "attribute", "private", "d->flyteam2_init_hp;"),
    ("Configuration", "flyteam2_init_damage", "attribute", "private", "d->flyteam2_init_damage;"),

    ("Configuration", "groundteam_moving_rule", "attribute", "private", "d->groundteam_moving_rule;"),
    ("Configuration", "groundteam_init_pos", "attribute", "private", "d->groundteam_init_pos;"),
    ("Configuration", "groundteam_init_hp", "attribute", "private", "d->groundteam_init_hp;"),
    ("Configuration", "groundteam_init_damage", "attribute", "private", "d->groundteam_init_damage;"),

    ("Configuration", "dragonlord_init_pos", "attribute", "private", "d->dragonlord_init_pos;"),
    ("Configuration", "num_steps", "attribute", "private", "d->num_steps;"),

    # Configuration methods
    ("Configuration", "getMapNumRows()", "method", "public", "d->getMapNumRows();"),
    ("Configuration", "getMapNumCols()", "method", "public", "d->getMapNumCols();"),
    ("Configuration", "getMaxNumMovingObjects()", "method", "public", "d->getMaxNumMovingObjects();"),

    ("Configuration", "getNumObstacles()", "method", "public", "d->getNumObstacles();"),
    ("Configuration", "getArrObstacles()", "method", "public", "d->getArrObstacles();"),

    ("Configuration", "getNumGroundObstacles()", "method", "public", "d->getNumGroundObstacles();"),
    ("Configuration", "getArrGroundObstacles()", "method", "public", "d->getArrGroundObstacles();"),

    ("Configuration", "getFlyTeam1MovingRule()", "method", "public", "d->getFlyTeam1MovingRule();"),
    ("Configuration", "getFlyTeam1InitPos()", "method", "public", "d->getFlyTeam1InitPos();"),
    ("Configuration", "getFlyTeam1InitHP()", "method", "public", "d->getFlyTeam1InitHP();"),
    ("Configuration", "getFlyTeam1InitDamage()", "method", "public", "d->getFlyTeam1InitDamage();"),

    ("Configuration", "getFlyTeam2MovingRule()", "method", "public", "d->getFlyTeam2MovingRule();"),
    ("Configuration", "getFlyTeam2InitPos()", "method", "public", "d->getFlyTeam2InitPos();"),
    ("Configuration", "getFlyTeam2InitHP()", "method", "public", "d->getFlyTeam2InitHP();"),
    ("Configuration", "getFlyTeam2InitDamage()", "method", "public", "d->getFlyTeam2InitDamage();"),

    ("Configuration", "getGroundTeamMovingRule()", "method", "public", "d->getGroundTeamMovingRule();"),
    ("Configuration", "getGroundTeamInitPos()", "method", "public", "d->getGroundTeamInitPos();"),
    ("Configuration", "getGroundTeamInitHP()", "method", "public", "d->getGroundTeamInitHP();"),
    ("Configuration", "getGroundTeamInitDamage()", "method", "public", "d->getGroundTeamInitDamage();"),

    ("Configuration", "getDragonLordInitPos()", "method", "public", "d->getDragonLordInitPos();"),
    ("Configuration", "getNumSteps()", "method", "public", "d->getNumSteps();"),


      ("Configuration", "setMapNumRows(val)", "method", "public", "d->setMapNumRows(10);"),
    ("Configuration", "setMapNumCols(val)", "method", "public", "d->setMapNumCols(10);"),
    ("Configuration", "setMaxNumMovingObjects(val)", "method", "public", "d->setMaxNumMovingObjects(5);"),

    ("Configuration", "setNumObstacles(val)", "method", "public", "d->setNumObstacles(3);"),
    ("Configuration", "setArrObstacles(arr)", "method", "public", "d->setArrObstacles(nullptr);"),

    ("Configuration", "setNumGroundObstacles(val)", "method", "public", "d->setNumGroundObstacles(2);"),
    ("Configuration", "setArrGroundObstacles(arr)", "method", "public", "d->setArrGroundObstacles(nullptr);"),

    ("Configuration", "setFlyTeam1MovingRule(val)", "method", "public", "d->setFlyTeam1MovingRule(\"RUU\");"),
    ("Configuration", "setFlyTeam1InitPos(pos)", "method", "public", "d->setFlyTeam1InitPos(Position(1,3));"),
    ("Configuration", "setFlyTeam1InitHP(hp)", "method", "public", "d->setFlyTeam1InitHP(250);"),
    ("Configuration", "setFlyTeam1InitDamage(dmg)", "method", "public", "d->setFlyTeam1InitDamage(500);"),

    ("Configuration", "setFlyTeam2MovingRule(val)", "method", "public", "d->setFlyTeam2MovingRule(\"LLD\");"),
    ("Configuration", "setFlyTeam2InitPos(pos)", "method", "public", "d->setFlyTeam2InitPos(Position(4,8));"),
    ("Configuration", "setFlyTeam2InitHP(hp)", "method", "public", "d->setFlyTeam2InitHP(350);"),
    ("Configuration", "setFlyTeam2InitDamage(dmg)", "method", "public", "d->setFlyTeam2InitDamage(400);"),

    ("Configuration", "setGroundTeamMovingRule(val)", "method", "public", "d->setGroundTeamMovingRule(\"LU\");"),
    ("Configuration", "setGroundTeamInitPos(pos)", "method", "public", "d->setGroundTeamInitPos(Position(2,1));"),
    ("Configuration", "setGroundTeamInitHP(hp)", "method", "public", "d->setGroundTeamInitHP(300);"),
    ("Configuration", "setGroundTeamInitDamage(dmg)", "method", "public", "d->setGroundTeamInitDamage(350);"),

    ("Configuration", "setDragonLordInitPos(pos)", "method", "public", "d->setDragonLordInitPos(Position(7,9));"),
    ("Configuration", "setNumSteps(val)", "method", "public", "d->setNumSteps(100);"),
]


# ==== CẤU TRÚC: (init_code, list_of_tests)
obj_list = [
    ("Position* d = new Position(0,0);", test_position),
    ("MapElement* d = new MapElement(ElementType::PATH);", test_mapele),
    ("Map * d = new Map(10, 10, 0, nullptr, 0, nullptr);", test_map),
    ("""
     Map map = Map(10, 10, 0, nullptr, 0, nullptr);
     FlyTeam * d = new FlyTeam(0,\"LRUD\",Position(\"(0,0)\"), &map, 20, 20);""", test_fly),  
    ("""
     Map map = Map(10, 10, 0, nullptr, 0, nullptr);
     GroundTeam * d = new GroundTeam(0,\"LRUD\",Position(\"(0,0)\"), &map, 20, 20);""", test_ground), 
    ("""
     Map map = Map(10, 10, 0, nullptr, 0, nullptr);
     Warrior * d = new GroundTeam(0,\"LRUD\",Position(\"(0,0)\"), &map, 20, 20);""", test_war), 
     ("""
     DragonLord* d = new DragonLord(0,Position(9,9),nullptr,nullptr,nullptr,nullptr);""", test_lord),
     ("""
     MovingObject* d = new DragonLord(0,Position(9,9),nullptr,nullptr,nullptr,nullptr);""", test_movobj),
     ("""
     Configuration* d = new Configuration("sa_tc_02_config");""", test_config),
]

# ==== GHÉP THÀNH DANH SÁCH TEST ĐẦY ĐỦ ====
tests = []
for init_code, test_group in obj_list:
    for cls, member, mem_type, expected_access, test_line in test_group:
        full_init = init_code + "\n" + test_line
        tests.append((cls, member, mem_type, expected_access, full_init))

# ==== TEMPLATE CODE ====
template = '''
#include "dragon.h"
#include <iostream>
using namespace std;

int main() {{
    {init}
    return 0;
}}
'''

# ==== LOG HEADER ====
log_lines = []
header = (
    f"{'Class':<20} {'Member':<30} {'Type':<10} "
    f"{'Expected':<10} {'Output':<10} {'Result'}"
)
log_lines.append(header)
log_lines.append("-" * len(header))

# ==== MAIN LOOP ====
for cls, member, mem_type, expected_access, init_code in tests:
    code = template.format(init=init_code)

    with open("probe.cpp", "w") as f:
        f.write(code)

    result = subprocess.run(
        ["g++", "-std=c++11", "probe.cpp", "dragon.cpp", "-o", "probe_bin"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )

    compile_failed = result.returncode != 0
    stderr_text = result.stderr.lower()

    if not compile_failed:
        detected_access = "public"
    elif "private" in stderr_text:
        detected_access = "private"
    elif "protected" in stderr_text:
        detected_access = "protected"
    else:
        detected_access = "unknown"

    # So sánh kết quả
    if detected_access == "unknown":
        result_str = "FAIL"
    else:
        result_str = "PASS" if detected_access == expected_access else "FAIL"

    # In kết quả
    line = (
        f"{cls:<20} {member:<30} {mem_type:<10} "
        f"{expected_access:<10} {detected_access:<10} {result_str}"
    )
    log_lines.append(line)
    print(line)

# ==== GHI LOG ====
with open("probe_log.txt", "w") as f:
    f.write("\n".join(log_lines))

# ==== CLEANUP ====
if os.path.exists("probe_bin"):
    os.remove("probe_bin")

print("\nProbe finished. See 'probe_log.txt'")
