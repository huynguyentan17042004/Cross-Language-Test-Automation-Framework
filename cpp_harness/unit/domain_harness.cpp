#include "dragon.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

namespace
{
const char *boolText(bool value)
{
    return value ? "true" : "false";
}

const char *itemName(BaseItem *item)
{
    if (dynamic_cast<DragonScale *>(item))
        return "DragonScale";
    if (dynamic_cast<HealingHerb *>(item))
        return "HealingHerb";
    if (dynamic_cast<TrapEnhancer *>(item))
        return "TrapEnhancer";
    return item ? "Unknown" : "null";
}

DragonType parseDragonType(const string &value)
{
    if (value == "SD1")
        return SD1;
    if (value == "SD2")
        return SD2;
    if (value == "SD3")
        return SD3;
    throw invalid_argument("unknown SmartDragon type");
}

int warriorScenario(int argc, char *argv[])
{
    if (argc != 5)
        return 64;

    const string actor = argv[2];
    const int hp = stoi(argv[3]);
    const int damage = stoi(argv[4]);
    Map map(3, 3, 0, nullptr, 0, nullptr);

    if (actor == "fly")
    {
        FlyTeam team(1, "", Position(1, 1), &map, hp, damage);
        cout << "hp=" << team.getHp() << ";damage=" << team.getDamage()
             << ";position=" << team.getCurrentPosition().str() << '\n';
        return 0;
    }
    if (actor == "ground")
    {
        GroundTeam team(1, "", Position(1, 1), &map, hp, damage);
        cout << "hp=" << team.getHp() << ";damage=" << team.getDamage()
             << ";position=" << team.getCurrentPosition().str() << '\n';
        return 0;
    }
    return 64;
}

int setHpScenario(int argc, char *argv[])
{
    if (argc != 3)
        return 64;

    Map map(3, 3, 0, nullptr, 0, nullptr);
    FlyTeam team(1, "", Position(1, 1), &map, 300, 100);
    team.setHp(stoi(argv[2]));
    cout << "hp=" << team.getHp()
         << ";position=" << team.getCurrentPosition().str() << '\n';
    return 0;
}

int movementScenario(int argc, char *argv[])
{
    if (argc != 9)
        return 64;

    const string actor = argv[2];
    const string rule = argv[3];
    const int rows = stoi(argv[4]);
    const int cols = stoi(argv[5]);
    const Position start(stoi(argv[6]), stoi(argv[7]));
    const int damage = stoi(argv[8]);
    Map map(rows, cols, 0, nullptr, 0, nullptr);

    if (actor == "fly")
    {
        FlyTeam team(1, rule, start, &map, 300, damage);
        team.move();
        cout << "position=" << team.getCurrentPosition().str() << '\n';
        return 0;
    }
    if (actor == "ground")
    {
        GroundTeam team(1, rule, start, &map, 300, damage);
        team.move();
        cout << "position=" << team.getCurrentPosition().str() << '\n';
        return 0;
    }
    return 64;
}

int lordNextScenario(int argc, char *argv[])
{
    if (argc != 6)
        return 64;

    Map map(20, 20, 0, nullptr, 0, nullptr);
    FlyTeam fly1(1, "", Position(stoi(argv[2]), stoi(argv[3])), &map, 300, 100);
    FlyTeam fly2(2, "", Position(stoi(argv[4]), stoi(argv[5])), &map, 300, 100);
    GroundTeam ground(3, "", Position(10, 10), &map, 300, 100);
    DragonLord lord(0, Position(19, 19), &map, &fly1, &fly2, &ground);

    cout << "next=" << lord.getNextPosition().str() << '\n';
    return 0;
}

int smartNextScenario(int argc, char *argv[])
{
    if (argc != 8)
        return 64;

    Map map(7, 7, 0, nullptr, 0, nullptr);
    FlyTeam target(1, "", Position(stoi(argv[5]), stoi(argv[6])), &map, 300, 100);
    SmartDragon dragon(
        4,
        Position(stoi(argv[3]), stoi(argv[4])),
        &map,
        parseDragonType(argv[2]),
        &target,
        stoi(argv[7]));

    cout << "next=" << dragon.getNextPosition().str() << '\n';
    return 0;
}

int spawnScenario(int argc, char *argv[])
{
    if (argc != 3)
        return 64;

    Map map(10, 10, 0, nullptr, 0, nullptr);
    FlyTeam fly1(1, "", Position(0, 0), &map, 300, 100);
    FlyTeam fly2(2, "", Position(9, 9), &map, 300, 100);
    GroundTeam ground(3, "", Position(5, 5), &map, 300, 100);
    DragonLord lord(0, Position(4, 4), &map, &fly1, &fly2, &ground);
    lord.moved_step = stoi(argv[2]);
    int sd1 = 0;
    int sd2 = 0;
    int sd3 = 0;
    unique_ptr<SmartDragon> created(
        lord.creat_smart_dragon(Position(4, 4), sd1, sd2, sd3));

    cout << "created=" << boolText(created != nullptr);
    if (created)
        cout << ";type=" << created->str();
    cout << '\n';
    return 0;
}

int itemScenario(int argc, char *argv[])
{
    if (argc != 6)
        return 64;

    const string kind = argv[2];
    const string actor = argv[3];
    const int hp = stoi(argv[4]);
    const int damage = stoi(argv[5]);
    Map map(3, 3, 0, nullptr, 0, nullptr);

    unique_ptr<Warrior> warrior;
    if (actor == "fly")
        warrior.reset(new FlyTeam(1, "", Position(1, 1), &map, hp, damage));
    else if (actor == "ground")
        warrior.reset(new GroundTeam(1, "", Position(1, 1), &map, hp, damage));
    else
        return 64;

    unique_ptr<BaseItem> item;
    if (kind == "scale")
        item.reset(new DragonScale());
    else if (kind == "herb")
        item.reset(new HealingHerb());
    else if (kind == "trap")
        item.reset(new TrapEnhancer());
    else
        return 64;

    const bool usable = item->canUse(warrior.get());
    if (usable)
        item->use(warrior.get());
    GroundTeam *ground = dynamic_cast<GroundTeam *>(warrior.get());
    cout << "usable=" << boolText(usable)
         << ";hp=" << warrior->getHp()
         << ";damage=" << warrior->getDamage();
    if (ground)
        cout << ";trap_turns=" << ground->getTrapTurns();
    cout << '\n';
    return 0;
}

int bagCapacityScenario(int argc, char *argv[])
{
    if (argc != 4)
        return 64;

    const string actor = argv[2];
    const int attempts = stoi(argv[3]);
    Map map(3, 3, 0, nullptr, 0, nullptr);
    unique_ptr<Warrior> warrior;
    if (actor == "fly")
        warrior.reset(new FlyTeam(1, "", Position(1, 1), &map, 300, 500));
    else if (actor == "ground")
        warrior.reset(new GroundTeam(1, "", Position(1, 1), &map, 300, 500));
    else
        return 64;

    int accepted = 0;
    for (int index = 0; index < attempts; ++index)
    {
        BaseItem *item = new DragonScale();
        if (warrior->getBag()->insert(item))
            ++accepted;
        else
            delete item;
    }
    cout << "accepted=" << accepted << ';' << warrior->getBag()->str() << '\n';
    return 0;
}

int bagOrderScenario()
{
    Map map(3, 3, 0, nullptr, 0, nullptr);
    GroundTeam ground(1, "", Position(1, 1), &map, 300, 500);
    ground.getBag()->insert(new DragonScale());
    ground.getBag()->insert(new HealingHerb());
    ground.getBag()->insert(new TrapEnhancer());
    cout << ground.getBag()->str() << '\n';
    return 0;
}

int bagGetTypeScenario()
{
    Map map(3, 3, 0, nullptr, 0, nullptr);
    GroundTeam ground(1, "", Position(1, 1), &map, 300, 500);
    ground.getBag()->insert(new DragonScale());
    ground.getBag()->insert(new HealingHerb());
    ground.getBag()->insert(new TrapEnhancer());
    BaseItem *selected = ground.getBag()->get(HEALINGHERB);
    cout << "selected=" << itemName(selected) << ';' << ground.getBag()->str() << '\n';
    delete selected;
    return 0;
}

int bagGetUsableScenario()
{
    Map map(3, 3, 0, nullptr, 0, nullptr);
    FlyTeam fly(1, "", Position(1, 1), &map, 100, 500);
    fly.getBag()->insert(new DragonScale());
    fly.getBag()->insert(new HealingHerb());
    BaseItem *selected = fly.getBag()->get();
    cout << "selected=" << itemName(selected) << ';' << fly.getBag()->str() << '\n';
    delete selected;
    return 0;
}

int arrayScenario(int argc, char *argv[])
{
    if (argc != 4)
        return 64;

    const int capacity = stoi(argv[2]);
    const int attempts = stoi(argv[3]);
    Map map(3, 3, 0, nullptr, 0, nullptr);
    vector<unique_ptr<FlyTeam>> teams;
    ArrayMovingObject objects(capacity);
    string accepted;
    for (int index = 0; index < attempts; ++index)
    {
        teams.emplace_back(new FlyTeam(index, "", Position(0, index % 3), &map, 300, 100));
        accepted += objects.add(teams.back().get()) ? '1' : '0';
    }

    cout << "accepted=" << accepted
         << ";size=" << objects.size()
         << ";full=" << boolText(objects.isFull())
         << ";negative_is_null=" << boolText(objects.get(-1) == nullptr)
         << '\n';
    return 0;
}
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        return 64;

    try
    {
        const string command = argv[1];
        if (command == "warrior")
            return warriorScenario(argc, argv);
        if (command == "set-hp")
            return setHpScenario(argc, argv);
        if (command == "movement")
            return movementScenario(argc, argv);
        if (command == "lord-next")
            return lordNextScenario(argc, argv);
        if (command == "smart-next")
            return smartNextScenario(argc, argv);
        if (command == "spawn")
            return spawnScenario(argc, argv);
        if (command == "item")
            return itemScenario(argc, argv);
        if (command == "bag-capacity")
            return bagCapacityScenario(argc, argv);
        if (command == "bag-order")
            return bagOrderScenario();
        if (command == "bag-get-type")
            return bagGetTypeScenario();
        if (command == "bag-get-usable")
            return bagGetUsableScenario();
        if (command == "array")
            return arrayScenario(argc, argv);
    }
    catch (const exception &error)
    {
        cerr << error.what() << '\n';
        return 64;
    }

    cerr << "unknown scenario\n";
    return 64;
}
