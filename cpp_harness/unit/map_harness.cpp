#include "dragon.h"

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

class TestDragonWar
{
public:
    static const MapElement &elementAt(const Map &map, int row, int col)
    {
        return *map.map[row][col];
    }
};

static const char *elementTypeName(ElementType type)
{
    switch (type)
    {
    case PATH:
        return "PATH";
    case OBSTACLE:
        return "OBSTACLE";
    case GROUND_OBSTACLE:
        return "GROUND_OBSTACLE";
    }
    return "UNKNOWN";
}

static ElementType parseElementType(const string &value)
{
    if (value == "path")
        return PATH;
    if (value == "obstacle")
        return OBSTACLE;
    if (value == "ground-obstacle")
        return GROUND_OBSTACLE;
    throw invalid_argument("unknown element type");
}

static int inspectMapElement(int argc, char **argv)
{
    if (argc != 3 && argc != 4)
        return 64;

    ElementType type = parseElementType(argv[2]);
    int required_damage = argc == 4 ? stoi(argv[3]) : 0;
    MapElement element(type, required_damage);

    cout << elementTypeName(element.getType()) << "," << element.getReqDmg();
    return 0;
}

static int inspectGroundObstacle(int argc, char **argv)
{
    if (argc != 4)
        return 64;

    int row = stoi(argv[2]);
    int col = stoi(argv[3]);
    if (row < 0 || col < 0)
        return 64;

    Position ground_obstacle[] = {Position(row, col)};
    Map map(row + 1, col + 1, 0, nullptr, 1, ground_obstacle);
    const MapElement &element = TestDragonWar::elementAt(map, row, col);

    cout << elementTypeName(element.getType()) << "," << element.getReqDmg();
    return 0;
}

static int inspectMapValidity(int argc, char **argv)
{
    if (argc != 6)
        return 64;

    string actor = argv[2];
    int damage = stoi(argv[3]);
    Position target(stoi(argv[4]), stoi(argv[5]));
    Position obstacles[] = {Position(1, 1)};
    Position ground_obstacles[] = {Position(1, 2)};
    Map map(3, 3, 1, obstacles, 1, ground_obstacles);

    bool valid = false;
    if (actor == "fly")
    {
        FlyTeam team(1, "", Position(0, 0), &map, 500, damage);
        valid = map.isValid(target, &team);
    }
    else if (actor == "ground")
    {
        GroundTeam team(1, "", Position(0, 0), &map, 500, damage);
        valid = map.isValid(target, &team);
    }
    else
    {
        return 64;
    }

    cout << (valid ? "true" : "false");
    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 2)
        return 64;

    try
    {
        string command = argv[1];
        if (command == "element")
            return inspectMapElement(argc, argv);
        if (command == "ground-obstacle")
            return inspectGroundObstacle(argc, argv);
        if (command == "is-valid")
            return inspectMapValidity(argc, argv);
    }
    catch (const exception &error)
    {
        cerr << error.what();
        return 64;
    }

    cerr << "unknown scenario";
    return 64;
}
