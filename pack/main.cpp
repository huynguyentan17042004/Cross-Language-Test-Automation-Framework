/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Summer 2025
 * Date: 28.07.2025
 */

// The library here is concretely set, students are not allowed to include any other libraries.

#include "dragon.h"

// #define TEST

using namespace std;
int current_test_no = 0;
string current_test_content = "default";

void print_result(const string &expected, const string &output)
{
    static bool header_printed = false;

    bool passed = (expected == output);

    if (!header_printed)
    {
        cout << left
             << setw(5) << "No"
             << setw(70) << "Content"
             << setw(10) << "Status" << "\n";
        cout << string(5 + 70 + 10, '-') << "\n";
        header_printed = true;
    }

    if (!passed)
        cout << "\033[31m"; // đỏ

    cout << left
         << setw(5) << current_test_no
         << setw(70) << current_test_content
         << setw(10) << (passed ? "PASS" : "FAIL") << "\n";
    cout << "\033[0m"; // reset màu

#ifdef TEST
    if (!passed)
    {

        cout << "Expect : " << expected << "\n";
        cout << "Output : " << output << "\n";
    }
#endif
}

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <functional>

#include <type_traits>

void exe_test_pack(vector<pair<string, function<void()>>> a)
{
    vector<pair<string, function<void()>>> test_pack = a;
    for (size_t i = 0; i < test_pack.size(); ++i)
    {
        current_test_content = test_pack[i].first;
        test_pack[i].second();
        current_test_no++;
    }
}

void check_access(const string &name)
{
    ifstream file("probe_log.txt");
    if (!file.is_open())
    {
        cerr << "Cannot open probe_log.txt\n";
        return;
    }

    string line;
    vector<string> failedLines;
    bool allPass = true;

    // Bỏ qua dòng tiêu đề và gạch ngang
    getline(file, line); // Header
    getline(file, line); // Divider

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        istringstream iss(line);
        string className, member, type, expected, detected, result;
        iss >> className >> member >> type >> expected >> detected >> result;

        if (className != name)
            continue;

        if (result != "PASS")
        {
            failedLines.push_back(line);
            allPass = false;
        }
    }

    file.close();

    if (allPass)
    {
        cout << "All access modifier pass\n";
    }
    else
    {
        cout << "Found failures in access modifier for class: " << name << "\n";
        for (const string &l : failedLines)
        {
            cout << l << "\n";
        }
    }
}

class TestDragonWar
{
public:
    static MapElement *getMapElementAt(const Map &map, int r, int c)
    {
        int rows = map.getNumRows();
        int cols = map.getNumCols();

        if (r < 0 || r >= rows || c < 0 || c >= cols)
        {
            std::cerr << "Out of bounds: (" << r << ", " << c << ")\n";
            return nullptr;
        }

        if (!map.map)
        {
            std::cerr << "map.map is nullptr — map not allocated with new[]\n";
            return nullptr;
        }

        if (!map.map[r])
        {
            std::cerr << "map.map[" << r << "] is nullptr — row not allocated\n";
            return nullptr;
        }

        // Capture the cell
        auto cell = map.map[r][c];

        // Runtime type check
        typedef decltype(cell) CellType;
        if (!std::is_pointer<CellType>::value)
        {
            std::cerr << "grid[" << r << "][" << c << "] is not a pointer — likely returning MapElement by value\n";
            return nullptr;
        }

        if (cell == nullptr)
        {
            std::cerr << "map[" << r << "][" << c << "] is nullptr — element not initialized\n";
            return nullptr;
        }

        return cell;
    }
};

void sample_pos()
{
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

    Position a = Position("(1,4)");
    cout << a.str() << endl;

    Position b = Position(1, 4);
    cout << b.str() << endl;

    cout.rdbuf(oldBuf); // Khôi phục cout

    string expect = "(1,4)\n(1,4)\n";
    print_result(expect, oss.str());
}

void pos_1()
{
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

    Position a = Position("(    1   ,      4     )");
    a = Position("(    1   ,      4     )");
    a = Position("(    1   ,      4     )");
    a = Position("(    1   ,      4     )");
    a = Position("(    1   ,      4     )");
    a = Position("(    1   ,      4     )");
    a = Position("(    1   ,      4     )");
    a = Position("(    1   ,      4     )");
    a = Position("(    1   ,      4     )");
    a = Position("(    1   ,      4     )");
    cout << a.str() << endl;

    a = Position(1, 4);
    cout << a.str() << endl;

    cout.rdbuf(oldBuf); // Khôi phục cout

    string expect = "(1,4)\n(1,4)\n";
    print_result(expect, oss.str());
}

void pos_2()
{
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    check_access("Position");
    cout.rdbuf(oldBuf); // Khôi phục cout
    string expect = "All access modifier pass\n";
    print_result(expect, oss.str());
}

void pos_3()
{
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

    Position a = Position("(0,0)");
    cout << a.str() << endl;

    a.setRow(1);
    cout << a.str() << endl;
    a.setCol(1);
    cout << a.str() << endl;
    a.setCol(10);
    a.setRow(9);
    cout << a.getRow() << " " << a.getCol() << endl;

    cout.rdbuf(oldBuf); // Khôi phục cout

    string expect = "(0,0)\n(1,0)\n(1,1)\n9 10\n";
    print_result(expect, oss.str());
}

void pos_4()
{
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

    Position a = Position("(0,0)");
    cout << a.str() << endl;

    cout << a.isEqual(0, 0);
    cout << a.isEqual(-1, -1);
    cout << a.isEqual(Position(0, 0));
    cout << a.isEqual(Position(1, 1));
    cout.rdbuf(oldBuf); // Khôi phục cout

    string expect = "(0,0)\n1010";
    print_result(expect, oss.str());
}

void pos_5()
{
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

    Position a = Position::npos;

    cout << a.str() << endl;
    a.setCol(10);

    cout << Position::npos.str() << endl;
    // Position::npos.setCol(10); check manually do const so cannot be set

    cout.rdbuf(oldBuf); // Khôi phục cout

    string expect = "(-1,-1)\n(-1,-1)\n";
    print_result(expect, oss.str());
}

vector<pair<string, function<void()>>> TestPosition = {
    {"Position _ sample", sample_pos},
    {"Position _ construct + copy assign", pos_1},
    {"Position _ member access", pos_2},
    {"Position _ get & set", pos_3},
    {"Position _ isEqual", pos_4},
    {"Position _ npos", pos_5},
};

void sample_mapele()
{
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

    MapElement a = MapElement(PATH, 10);
    MapElement b = MapElement(OBSTACLE, 145);
    MapElement c = MapElement(GROUND_OBSTACLE);

    cout << a.getType() << " - " << a.getReqDmg() << endl;
    cout << b.getType() << " - " << b.getReqDmg() << endl;
    cout << c.getType() << " - " << c.getReqDmg() << endl;

    cout.rdbuf(oldBuf); // Khôi phục cout
    string expect = "0 - 10\n1 - 145\n2 - 0\n";
    print_result(expect, oss.str());
}

void mapele_1()
{
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    check_access("MapElement");
    cout.rdbuf(oldBuf); // Khôi phục cout
    string expect = "All access modifier pass\n";
    print_result(expect, oss.str());
}

void mapele_2()
{
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

    MapElement a = MapElement(ElementType::PATH);
    MapElement b = MapElement(ElementType::OBSTACLE);
    MapElement c = b;

    cout << a.getType() << b.getType() << c.getType() << endl;

    a = b;
    b = c;
    cout << a.getType() << b.getType() << c.getType() << endl;
    cout.rdbuf(oldBuf); // Khôi phục cout
    string expect = "011\n111\n";
    print_result(expect, oss.str());
}

void mapele_3()
{
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

    MapElement a = MapElement(ElementType::PATH);
    MapElement b = MapElement(ElementType::OBSTACLE, 2);
    MapElement c = MapElement(ElementType::GROUND_OBSTACLE, 100);

    cout << a.getReqDmg() << b.getReqDmg() << c.getReqDmg() << endl;
    cout << a.getType() << b.getType() << c.getType() << endl;

    cout.rdbuf(oldBuf); // Khôi phục cout
    string expect = "02100\n012\n";
    print_result(expect, oss.str());
}

vector<pair<string, function<void()>>> TestMapElement = {
    {"MapElement _ sample", sample_mapele},
    {"MapElement _ member access", mapele_1},
    {"MapElement _ copy assign", mapele_2},
    {"MapElement _ getRegDmg vs getType", mapele_3},
};

void sample_map()
{
    string expect = "";
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

    Position arr_obst[10];
    Position arr_grd_obst[10];
    arr_obst[0] = Position("(1,1)");
    arr_grd_obst[0] = Position("(0,1)");

    Map map = Map(10, 10, 1, arr_obst, 1, arr_grd_obst);
    cout << map.getNumRows() << " " << map.getNumCols() << endl;

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            MapElement *tmp = TestDragonWar::getMapElementAt(map, i, j);
            if (tmp == nullptr)
            {
                expect = "map should be *** or **?";
                cout.rdbuf(oldBuf); // Khôi phục cout
                print_result(expect, oss.str());
                return;
            }
            cout << tmp->getType();
        }
        cout << endl;
    }

    cout.rdbuf(oldBuf); // Khôi phục cout
    expect = "10 10\n0200000000\n0100000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n";
    print_result(expect, oss.str());
}

void map_1()
{
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    check_access("Map");
    cout.rdbuf(oldBuf); // Khôi phục cout
    string expect = "All access modifier pass\n";
    print_result(expect, oss.str());
}

void map_2()
{
    string expect = "";
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

    Position arr_obst[10];
    Position arr_grd_obst[10];
    arr_obst[0] = Position("(1,1)");
    arr_grd_obst[0] = Position("(0,1)");

    Map map = Map(10, 10, 1, arr_obst, 1, arr_grd_obst);
    cout << map.getNumRows() << " " << map.getNumCols() << endl;

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            MapElement *tmp = TestDragonWar::getMapElementAt(map, i, j);
            if (tmp == nullptr)
            {
                expect = "Grid should be *** or **?";
                cout.rdbuf(oldBuf); // Khôi phục cout
                print_result(expect, oss.str());
                return;
            }
            cout << tmp->getType();
        }
        cout << endl;
    }
    map = Map(10, 10, 1, arr_obst, 1, arr_grd_obst);
    map = Map(10, 10, 1, arr_obst, 1, arr_grd_obst);
    map = Map(10, 10, 1, arr_obst, 1, arr_grd_obst);
    map = Map(10, 10, 1, arr_obst, 1, arr_grd_obst);
    map = Map(10, 10, 1, arr_obst, 1, arr_grd_obst);
    map = Map(10, 10, 1, arr_obst, 1, arr_grd_obst);
    map = Map(10, 10, 1, arr_obst, 1, arr_grd_obst);
    map = Map(10, 10, 1, arr_obst, 1, arr_grd_obst);

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            MapElement *tmp = TestDragonWar::getMapElementAt(map, i, j);
            if (tmp == nullptr)
            {
                expect = "Grid should be *** or **?";
                cout.rdbuf(oldBuf); // Khôi phục cout
                print_result(expect, oss.str());
                return;
            }
            cout << tmp->getType();
        }
        cout << endl;
    }
    cout.rdbuf(oldBuf); // Khôi phục cout
    expect = "10 10\n0200000000\n0100000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n";
    expect += "0200000000\n0100000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n";

    print_result(expect, oss.str());
}

void map_3()
{
    string expect = "";
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

    Position arr_obst[10];
    Position arr_grd_obst[10];
    int count_obst = 0;
    int count_grd_obst = 0;

    for (int r = 0; r < 10; ++r)
    {
        for (int c = 0; c < 10; ++c)
        {
            if ((r + c) % 5 == 4 && count_obst < 10)
                arr_obst[count_obst++] = Position(r, c);
            if ((r + c) % 5 == 2 && count_grd_obst < 10)
                arr_grd_obst[count_grd_obst++] = Position(r, c);
        }
    }

    Map map = Map(10, 10, count_obst, arr_obst, count_grd_obst, arr_grd_obst);
    cout << map.getNumRows() << " " << map.getNumCols() << endl;

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            MapElement *tmp = TestDragonWar::getMapElementAt(map, i, j);
            if (tmp == nullptr)
            {
                expect = "Grid should be *** or **?";
                cout.rdbuf(oldBuf); // Khôi phục cout
                print_result(expect, oss.str());
                return;
            }
            cout << tmp->getType();
        }
        cout << endl;
    }

    cout.rdbuf(oldBuf); // Khôi phục cout
    expect = "10 10\n0020100201\n0201002010\n2010020100\n0100201002\n1002010020\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n";
    print_result(expect, oss.str());
}

void map_4()
{
    string expect = "";
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;
    Position arr_obst[10];
    Position arr_grd_obst[100];
    int count_obst = 0;
    int count_grd_obst = 0;
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            arr_grd_obst[count_grd_obst++] = Position(i, j);

    Map map = Map(10, 10, count_obst, arr_obst, count_grd_obst, arr_grd_obst);

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            MapElement *tmp = TestDragonWar::getMapElementAt(map, i, j);

            cout << left << setw(5) << tmp->getReqDmg();
        }
        cout << "|" << endl;
    }

    cout.rdbuf(oldBuf); // Khôi phục cout
    expect += "\n90   229  368  507  646  785  24   163  302  441  |\n347  486  625  764  3    142  281  420  559  698  |\n604  743  882  121  260  399  538  677  816  55   |\n861  100  239  378  517  656  795  34   173  312  |\n218  357  496  635  774  13   152  291  430  569  |\n475  614  753  892  131  270  409  548  687  826  |\n732  871  110  249  388  527  666  805  44   183  |\n89   228  367  506  645  784  23   162  301  440  |\n346  485  624  763  2    141  280  419  558  697  |\n603  742  881  120  259  398  537  676  815  54   |\n";
    print_result(expect, oss.str());
}

vector<pair<string, function<void()>>> TestMap = {
    {"Map _ sample", sample_map},
    {"Map _ member access", map_1},
    {"Map _ constructor + UB test (mem check)", map_2},
    {"Map _ constructor right way", map_3},
    {"Map _ check req damage fomula", map_4},
};

void sample_flyteam()
{
    ostringstream oss;
    string expect = "";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

    Map map = Map(10, 10, 0, nullptr, 0, nullptr);
    FlyTeam a = FlyTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);
    cout << a.str() << endl;
    cout.rdbuf(oldBuf); // Khôi phục cout
    expect += "FlyTeam1[index=0;pos=(0,0);moving_rule=LRUD]\n";
    print_result(expect, oss.str());
}

void flyteam_1()
{
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    check_access("FlyTeam");
    cout.rdbuf(oldBuf); // Khôi phục cout
    string expect = "All access modifier pass\n";
    print_result(expect, oss.str());
}

void flyteam_2()
{
    ostringstream oss;
    string expect = "";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

    Map map = Map(10, 10, 0, nullptr, 0, nullptr);
    FlyTeam a = FlyTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);
    a = FlyTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);
    a = FlyTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);
    a = FlyTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);
    a = FlyTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);
    a = FlyTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);

    cout << a.str() << endl;
    cout.rdbuf(oldBuf); // Khôi phục cout
    expect += "FlyTeam2[index=0;pos=(0,0);moving_rule=LRUD]\n";
    print_result(expect, oss.str());
}

void flyteam_3()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;
    Map map = Map(10, 10, 0, nullptr, 0, nullptr);

    char arr[4] = {'R', 'L', 'D', 'U'};
    string pos_arr[4] = {"(0,0)", "(0,9)", "(0,0)", "(9,0)"};
    FlyTeam a = FlyTeam(0, "", Position("(0,0)"), &map, 20, 20);
    cout << a.str() << endl;
    a.move();
    cout << a.str() << endl;
    for (int i = 0; i < 4; i++)
    {
        string rule = string(10, arr[i]);
        a = FlyTeam(0, rule, Position(pos_arr[i]), &map, 20, 20);
        cout << a.str() << endl;
        a.move();
        cout << a.str() << endl;
        a.move();
        cout << a.str() << endl;
        // nếu ko thể đi được -> quay đầu -> ko thể tiếp -> khỏi move
    }

    cout.rdbuf(oldBuf); // Khôi phục cout
    expect += "FlyTeam1[index=0;pos=(0,0);moving_rule=]\n";
    expect += "FlyTeam1[index=0;pos=(0,0);moving_rule=]\n";

    expect += "FlyTeam2[index=0;pos=(0,0);moving_rule=RRRRRRRRRR]\n";
    expect += "FlyTeam2[index=0;pos=(0,8);moving_rule=RRRRRRRRRR]\n";
    expect += "FlyTeam2[index=0;pos=(0,8);moving_rule=RRRRRRRRRR]\n";

    expect += "FlyTeam2[index=0;pos=(0,9);moving_rule=LLLLLLLLLL]\n";
    expect += "FlyTeam2[index=0;pos=(0,1);moving_rule=LLLLLLLLLL]\n";
    expect += "FlyTeam2[index=0;pos=(0,1);moving_rule=LLLLLLLLLL]\n";

    expect += "FlyTeam2[index=0;pos=(0,0);moving_rule=DDDDDDDDDD]\n";
    expect += "FlyTeam2[index=0;pos=(8,0);moving_rule=DDDDDDDDDD]\n";
    expect += "FlyTeam2[index=0;pos=(8,0);moving_rule=DDDDDDDDDD]\n";

    expect += "FlyTeam2[index=0;pos=(9,0);moving_rule=UUUUUUUUUU]\n";
    expect += "FlyTeam2[index=0;pos=(1,0);moving_rule=UUUUUUUUUU]\n";
    expect += "FlyTeam2[index=0;pos=(1,0);moving_rule=UUUUUUUUUU]\n";

    print_result(expect, oss.str());
}

void flyteam_4()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;

    Position arr_obst[10];
    Position arr_grd_obst[10];
    arr_obst[0] = Position("(0,9)");
    arr_obst[1] = Position("(2,9)");

    Map map = Map(10, 10, 2, arr_obst, 0, arr_grd_obst);

    char arr[4] = {'R', 'L', 'D', 'U'};
    string pos_arr[4] = {"(0,0)", "(0,9)", "(0,0)", "(9,0)"};
    FlyTeam a = FlyTeam(0, "", Position("(0,0)"), &map, 20, 20);
    cout << a.str() << endl;
    a.move();
    cout << a.str() << endl;

    a = FlyTeam(0, "RRRRRRRRRUUDD", Position("(1,0)"), &map, 20, 20);
    cout << a.str() << endl;
    a.move();
    cout << a.str() << endl;

    cout.rdbuf(oldBuf); // Khôi phục cout
    expect += "FlyTeam1[index=0;pos=(0,0);moving_rule=]\n";
    expect += "FlyTeam1[index=0;pos=(0,0);moving_rule=]\n";

    expect += "FlyTeam2[index=0;pos=(1,0);moving_rule=RRRRRRRRRUUDD]\n";
    expect += "FlyTeam2[index=0;pos=(1,9);moving_rule=RRRRRRRRRUUDD]\n";

    print_result(expect, oss.str());
}

void flyteam_5()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;

    Position arr_obst[10];
    Position arr_grd_obst[10];
    arr_obst[0] = Position("(0,9)");
    arr_obst[1] = Position("(2,9)");
    for (int i = 0; i < 10; i++)
        arr_grd_obst[i] = Position(1, i);

    Map map = Map(10, 10, 2, arr_obst, 10, arr_grd_obst);

    char arr[4] = {'R', 'L', 'D', 'U'};
    string pos_arr[4] = {"(0,0)", "(0,9)", "(0,0)", "(9,0)"};
    FlyTeam a = FlyTeam(0, "", Position("(0,0)"), &map, 20, 20);
    cout << a.str() << endl;
    a.move();
    cout << a.str() << endl;

    a = FlyTeam(0, "RRRRRRRRRUUDD", Position("(1,0)"), &map, 20, 20);
    cout << a.str() << endl;
    a.move();
    cout << a.str() << endl;

    cout.rdbuf(oldBuf); // Khôi phục cout
    expect += "FlyTeam1[index=0;pos=(0,0);moving_rule=]\n";
    expect += "FlyTeam1[index=0;pos=(0,0);moving_rule=]\n";

    expect += "FlyTeam2[index=0;pos=(1,0);moving_rule=RRRRRRRRRUUDD]\n";
    expect += "FlyTeam2[index=0;pos=(1,9);moving_rule=RRRRRRRRRUUDD]\n";

    print_result(expect, oss.str());
}

void flyteam_6()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;

    Position arr_obst[10];
    Position arr_grd_obst[10];
    arr_obst[0] = Position("(0,9)");
    arr_obst[1] = Position("(2,9)");
    for (int i = 0; i < 10; i++)
        arr_grd_obst[i] = Position(1, i);

    Map map = Map(10, 10, 0, arr_obst, 0, arr_grd_obst);

    FlyTeam a = FlyTeam(0, "RRRRRRRRR", Position("(0,0)"), &map, 1, 20);
    cout << a.str() << endl;
    a.move();
    cout << a.str() << endl;

    a = FlyTeam(0, "RRRRRRRRR", Position("(0,0)"), &map, -1, 20);
    cout << a.str() << endl;
    a.move();
    cout << a.str() << endl;
    cout.rdbuf(oldBuf); // Khôi phục cout
    expect += "FlyTeam1[index=0;pos=(0,0);moving_rule=RRRRRRRRR]\n";
    expect += "FlyTeam1[index=0;pos=(0,0);moving_rule=RRRRRRRRR]\n";

    expect += "FlyTeam2[index=0;pos=(0,0);moving_rule=RRRRRRRRR]\n";
    expect += "FlyTeam2[index=0;pos=(0,0);moving_rule=RRRRRRRRR]\n";

    print_result(expect, oss.str());
}

void flyteam_7()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;

    Position arr_obst[10];
    Position arr_grd_obst[10];
    arr_obst[0] = Position("(0,9)");
    arr_obst[1] = Position("(2,9)");
    for (int i = 0; i < 10; i++)
        arr_grd_obst[i] = Position(1, i);

    Map map = Map(10, 10, 0, arr_obst, 0, arr_grd_obst);

    FlyTeam a = FlyTeam(0, "RRRRRRRRR", Position("(0,0)"), &map, 20, 20);
    cout << a.str() << endl;
    DragonLord lord = DragonLord(0, Position("(0,0)"), nullptr, nullptr, nullptr, nullptr);

    cout << a.attack(&lord) << endl;
    a.move();
    cout << a.str() << endl;
    cout << a.attack(&lord) << endl;

    cout.rdbuf(oldBuf); // Khôi phục cout
    expect += "FlyTeam1[index=0;pos=(0,0);moving_rule=RRRRRRRRR]\n1\n";
    expect += "FlyTeam1[index=0;pos=(0,9);moving_rule=RRRRRRRRR]\n0\n";

    print_result(expect, oss.str());
}

vector<pair<string, function<void()>>> TestFlyTeam = {
    {"FlyTeam _ sample", sample_flyteam},
    {"FlyTeam _ member access ", flyteam_1},
    {"FlyTeam _ construct + copy (UB check)", flyteam_2},
    {"FlyTeam _ move LRUP + cannot move 1st", flyteam_3},
    {"FlyTeam _ move LRUP + cannot move 2st", flyteam_4},
    {"FlyTeam _ move LRUP + obst vs grd obst", flyteam_5},
    {"FlyTeam _ move + hp check", flyteam_6},
    {"FlyTeam _ attack T/F", flyteam_7},
};

void sample_groundteam()
{
    ostringstream oss;
    string expect = "";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

    Map map = Map(10, 10, 0, nullptr, 0, nullptr);
    GroundTeam a = GroundTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);
    cout << a.str() << endl;
    cout.rdbuf(oldBuf); // Khôi phục cout
    expect += "GroundTeam[index=0;pos=(0,0);moving_rule=LRUD]\n";
    print_result(expect, oss.str());
}

void groundteam_1()
{
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    check_access("GroundTeam");
    cout.rdbuf(oldBuf); // Khôi phục cout
    string expect = "All access modifier pass\n";
    print_result(expect, oss.str());
}

void groundteam_2()
{
    ostringstream oss;
    string expect = "";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

    Map map = Map(10, 10, 0, nullptr, 0, nullptr);
    GroundTeam a = GroundTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);
    a = GroundTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);
    a = GroundTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);
    a = GroundTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);
    a = GroundTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);
    a = GroundTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);

    cout << a.str() << endl;
    cout.rdbuf(oldBuf); // Khôi phục cout
    expect += "GroundTeam[index=0;pos=(0,0);moving_rule=LRUD]\n";
    print_result(expect, oss.str());
}

void groundteam_3()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;
    Map map = Map(10, 10, 0, nullptr, 0, nullptr);

    char arr[4] = {'R', 'L', 'D', 'U'};
    string pos_arr[4] = {"(0,0)", "(0,9)", "(0,0)", "(9,0)"};
    GroundTeam a = GroundTeam(0, "", Position("(0,0)"), &map, 20, 20);
    cout << a.str() << endl;
    a.move();
    cout << a.str() << endl;
    for (int i = 0; i < 4; i++)
    {
        string rule = string(10, arr[i]);
        a = GroundTeam(0, rule, Position(pos_arr[i]), &map, 20, 20);
        cout << a.str() << endl;
        a.move();
        cout << a.str() << endl;
        a.move();
        cout << a.str() << endl;
        // nếu ko thể đi được -> quay đầu -> ko thể tiếp -> khỏi move
    }

    cout.rdbuf(oldBuf); // Khôi phục cout
    expect += "GroundTeam[index=0;pos=(0,0);moving_rule=]\n";
    expect += "GroundTeam[index=0;pos=(0,0);moving_rule=]\n";

    expect += "GroundTeam[index=0;pos=(0,0);moving_rule=RRRRRRRRRR]\n";
    expect += "GroundTeam[index=0;pos=(0,8);moving_rule=RRRRRRRRRR]\n";
    expect += "GroundTeam[index=0;pos=(0,8);moving_rule=RRRRRRRRRR]\n";

    expect += "GroundTeam[index=0;pos=(0,9);moving_rule=LLLLLLLLLL]\n";
    expect += "GroundTeam[index=0;pos=(0,1);moving_rule=LLLLLLLLLL]\n";
    expect += "GroundTeam[index=0;pos=(0,1);moving_rule=LLLLLLLLLL]\n";

    expect += "GroundTeam[index=0;pos=(0,0);moving_rule=DDDDDDDDDD]\n";
    expect += "GroundTeam[index=0;pos=(8,0);moving_rule=DDDDDDDDDD]\n";
    expect += "GroundTeam[index=0;pos=(8,0);moving_rule=DDDDDDDDDD]\n";

    expect += "GroundTeam[index=0;pos=(9,0);moving_rule=UUUUUUUUUU]\n";
    expect += "GroundTeam[index=0;pos=(1,0);moving_rule=UUUUUUUUUU]\n";
    expect += "GroundTeam[index=0;pos=(1,0);moving_rule=UUUUUUUUUU]\n";

    print_result(expect, oss.str());
}

void groundteam_4()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;

    Position arr_obst[10];
    Position arr_grd_obst[10];
    arr_obst[0] = Position("(0,9)");
    arr_obst[1] = Position("(2,9)");

    Map map = Map(10, 10, 2, arr_obst, 0, arr_grd_obst);

    char arr[4] = {'R', 'L', 'D', 'U'};
    string pos_arr[4] = {"(0,0)", "(0,9)", "(0,0)", "(9,0)"};
    GroundTeam a = GroundTeam(0, "", Position("(0,0)"), &map, 20, 20);
    cout << a.str() << endl;
    a.move();
    cout << a.str() << endl;

    a = GroundTeam(0, "RRRRRRRRRUUDD", Position("(1,0)"), &map, 20, 20);
    cout << a.str() << endl;
    a.move();
    cout << a.str() << endl;

    cout.rdbuf(oldBuf); // Khôi phục cout
    expect += "GroundTeam[index=0;pos=(0,0);moving_rule=]\n";
    expect += "GroundTeam[index=0;pos=(0,0);moving_rule=]\n";

    expect += "GroundTeam[index=0;pos=(1,0);moving_rule=RRRRRRRRRUUDD]\n";
    expect += "GroundTeam[index=0;pos=(1,9);moving_rule=RRRRRRRRRUUDD]\n";

    print_result(expect, oss.str());
}

void groundteam_5()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;

    Position arr_obst[10];
    Position arr_grd_obst[10];
    arr_obst[0] = Position("(0,9)");
    arr_obst[1] = Position("(2,9)");
    for (int i = 0; i < 10; i++)
        arr_grd_obst[i] = Position(1, i);

    Map map = Map(10, 10, 2, arr_obst, 10, arr_grd_obst);

    GroundTeam a = GroundTeam(0, "RRRRRRRRRUU", Position("(1,0)"), &map, 20, 700);
    cout << a.str() << endl;
    a.move();
    cout << a.str() << endl;

    cout.rdbuf(oldBuf); // Khôi phục cout

    expect += "GroundTeam[index=0;pos=(1,0);moving_rule=RRRRRRRRRUU]\n";
    expect += "GroundTeam[index=0;pos=(1,1);moving_rule=RRRRRRRRRUU]\n";

    print_result(expect, oss.str());
}

void groundteam_6()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;

    Position arr_obst[10];
    Position arr_grd_obst[10];
    arr_obst[0] = Position("(0,9)");
    arr_obst[1] = Position("(2,9)");
    for (int i = 0; i < 10; i++)
        arr_grd_obst[i] = Position(1, i);

    Map map = Map(10, 10, 2, arr_obst, 10, arr_grd_obst);

    GroundTeam a = GroundTeam(0, "RRRRRRRRRUU", Position("(9,0)"), &map, 1, 700);
    cout << a.str() << endl;
    a.move();
    cout << a.str() << endl;

    cout.rdbuf(oldBuf); // Khôi phục cout

    expect += "GroundTeam[index=0;pos=(9,0);moving_rule=RRRRRRRRRUU]\n";
    expect += "GroundTeam[index=0;pos=(9,0);moving_rule=RRRRRRRRRUU]\n";

    print_result(expect, oss.str());
}

void groundteam_7()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;

    Position arr_obst[10];
    Position arr_grd_obst[10];
    arr_obst[0] = Position("(0,9)");
    arr_obst[1] = Position("(2,9)");
    for (int i = 0; i < 10; i++)
        arr_grd_obst[i] = Position(1, i);

    Map map = Map(10, 10, 2, arr_obst, 10, arr_grd_obst);

    GroundTeam a = GroundTeam(0, "RRRRRRRRRUU", Position("(0,0)"), &map, 400, 900);
    cout << a.str() << endl;
    cout << a.getTrapTurns() << endl;
    a.setTrapTurns(0);
    cout << a.getTrapTurns() << endl;

    cout.rdbuf(oldBuf); // Khôi phục cout

    expect += "GroundTeam[index=0;pos=(0,0);moving_rule=RRRRRRRRRUU]\n3\n0\n";

    print_result(expect, oss.str());
}

vector<pair<string, function<void()>>> TestGroundTeam = {
    {"GroundTeam _ sample", sample_groundteam},
    {"GroundTeam _ member access ", groundteam_1},
    {"GroundTeam _ construct + copy (UB check)", groundteam_2},
    {"GroundTeam _ move LRUP + cannot move 1st", groundteam_3},
    {"GroundTeam _ move LRUP + cannot move 2st", groundteam_4},
    {"GroundTeam _ move LRUP + obst vs grd obst", groundteam_5},
    {"GroundTeam _ move with hp == 1", groundteam_6},
    {"GroundTeam _ trap + set & get trap", groundteam_7},
};

void sample_warrior()
{
    ostringstream oss;
    string expect = "";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

    Map map = Map(10, 10, 0, nullptr, 0, nullptr);
    Warrior *a = new GroundTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);
    cout << a->str() << endl;
    delete a;
    a = new FlyTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);
    cout << a->str() << endl;
    delete a;

    cout.rdbuf(oldBuf); // Khôi phục cout
    expect += "GroundTeam[index=0;pos=(0,0);moving_rule=LRUD]\n";
    expect += "FlyTeam1[index=0;pos=(0,0);moving_rule=LRUD]\n";
    print_result(expect, oss.str());
}

void warrior_1()
{
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    check_access("Warrior");
    cout.rdbuf(oldBuf); // Khôi phục cout
    string expect = "All access modifier pass\n";
    print_result(expect, oss.str());
}

void warrior_2()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;

    Position arr_obst[10];
    Position arr_grd_obst[10];
    arr_obst[0] = Position("(0,9)");
    arr_obst[1] = Position("(2,9)");
    for (int i = 0; i < 10; i++)
        arr_grd_obst[i] = Position(1, i);

    Map map = Map(10, 10, 2, arr_obst, 10, arr_grd_obst);

    Warrior *a = new GroundTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);
    cout << a->str() << endl;
    cout << a->getHp() << endl;
    a->setHp(2);
    cout << a->getHp() << endl;

    cout << a->str() << endl;
    cout << a->getDamage() << endl;
    a->setDamage(2);
    cout << a->getDamage() << endl;

    cout << a->getBag()->str() << endl;

    delete a;

    cout.rdbuf(oldBuf); // Khôi phục cout

    expect += "GroundTeam[index=0;pos=(0,0);moving_rule=LRUD]\n20\n2\n";
    expect += "GroundTeam[index=0;pos=(0,0);moving_rule=LRUD]\n20\n2\n";
    expect += "Bag[count=0;]\n";

    print_result(expect, oss.str());
}

void warrior_3()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;

    Position arr_obst[10];
    Position arr_grd_obst[10];
    arr_obst[0] = Position("(0,9)");
    arr_obst[1] = Position("(2,9)");
    for (int i = 0; i < 10; i++)
        arr_grd_obst[i] = Position(1, i);

    Map map = Map(10, 10, 2, arr_obst, 10, arr_grd_obst);

    Warrior *a = new GroundTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);
    cout << a->str() << endl;
    // dynamic_cast<Warrior *>(objam)
    if (dynamic_cast<GroundTeam *>(a))
        cout << "GroundTeam" << endl;
    if (!dynamic_cast<FlyTeam *>(a))
        cout << "not Flyteam" << endl;
    delete a;

    a = new FlyTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);
    cout << a->str() << endl;
    if (dynamic_cast<FlyTeam *>(a))
        cout << "Flyteam" << endl;
    if (!dynamic_cast<GroundTeam *>(a))
        cout << "not GroundTeam" << endl;
    delete a;

    GroundTeam *b = new GroundTeam(0, "LRUD", Position("(0,0)"), &map, 20, 20);
    if (dynamic_cast<Warrior *>(b))
        cout << "GroundTeam's base = Warrior" << endl;

    cout.rdbuf(oldBuf); // Khôi phục cout

    expect += "GroundTeam[index=0;pos=(0,0);moving_rule=LRUD]\nGroundTeam\nnot Flyteam\n";
    expect += "FlyTeam1[index=0;pos=(0,0);moving_rule=LRUD]\nFlyteam\nnot GroundTeam\n";
    expect += "GroundTeam's base = Warrior\n";

    print_result(expect, oss.str());
}

// all is ptr
vector<pair<string, function<void()>>> TestWarrior = {
    {"Warrior _ sample", sample_warrior},
    {"Warrior _ member access ", warrior_1},
    {"Warrior _ get & set hp/dmg/bag", warrior_2},
    {"Warrior _ inhe base vs devr", warrior_3},
};

void sample_lord()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;
    Map *map = new Map(10, 10, 0, nullptr, 0, nullptr);

    FlyTeam *fly1 = new FlyTeam(0, "RRRRRRRRRR", Position("(0,0)"), map, 10, 10);
    FlyTeam *fly2 = new FlyTeam(0, "DDDDDDDDDD", Position("(0,0)"), map, 10, 10);

    GroundTeam *ground = new GroundTeam(0, "RDRD", Position("(0,0)"), map, 10, 500);

    // hiện thực việc bị attack/trap trên dragon lord luôn
    DragonLord *lord = new DragonLord(0, Position(9, 9), map, fly1, fly2, ground);
    cout << lord->str() << endl;

    delete map;
    delete fly1;
    delete fly2;
    delete ground;
    delete lord;
    cout.rdbuf(oldBuf); // Khôi phục cout
    expect += "DragonLord[index=0;pos=(9,9)]\n";
    print_result(expect, oss.str());
}

void lord_1()
{
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    check_access("DragonLord");
    cout.rdbuf(oldBuf); // Khôi phục cout
    string expect = "All access modifier pass\n";
    print_result(expect, oss.str());
}

void lord_2()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;
    Map *map = new Map(10, 10, 0, nullptr, 0, nullptr);

    FlyTeam *fly1 = new FlyTeam(0, "RRRRRRRRRR", Position("(1,0)"), map, 10, 10);
    FlyTeam *fly2 = new FlyTeam(0, "DDDDDDDDDD", Position("(0,1)"), map, 10, 10);

    GroundTeam *ground = new GroundTeam(0, "RDRD", Position("(8,8)"), map, 10, 500);

    // hiện thực việc bị attack/trap trên dragon lord luôn
    DragonLord *lord = new DragonLord(0, Position(9, 9), map, fly1, fly2, ground);
    cout << lord->str() << endl;
    cout << lord->getNextPosition().str() << endl;
    fly1->move();
    cout << fly1->str() << endl;
    fly2->move();
    cout << fly2->str() << endl;
    cout << lord->getNextPosition().str() << endl;

    delete map;
    delete fly1;
    delete fly2;
    delete ground;
    delete lord;
    cout.rdbuf(oldBuf); // Khôi phục cout
    expect += "DragonLord[index=0;pos=(9,9)]\n(1,1)\n";
    expect += "FlyTeam1[index=0;pos=(1,8);moving_rule=RRRRRRRRRR]\nFlyTeam2[index=0;pos=(8,1);moving_rule=DDDDDDDDDD]\n";
    expect += "(7,7)\n";
    print_result(expect, oss.str());
}

void lord_3()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;
    Map *map = new Map(10, 10, 0, nullptr, 0, nullptr);

    FlyTeam *fly1 = new FlyTeam(0, "RRRRRRRRRR", Position("(5,0)"), map, 10, 10);
    FlyTeam *fly2 = new FlyTeam(0, "DDDDDDDDDD", Position("(0,4)"), map, 10, 10);

    GroundTeam *ground = new GroundTeam(0, "RDRD", Position("(8,8)"), map, 10, 500);

    // hiện thực việc bị attack/trap trên dragon lord luôn
    DragonLord *lord = new DragonLord(0, Position(9, 9), map, fly1, fly2, ground);
    cout << lord->str() << endl;
    cout << lord->getNextPosition().str() << endl;

    delete fly1;
    delete fly2;
    delete lord;
    fly1 = new FlyTeam(0, "RRRRRRRRRR", Position("(7,0)"), map, 10, 10);
    fly2 = new FlyTeam(0, "DDDDDDDDDD", Position("(0,1)"), map, 10, 10);
    lord = new DragonLord(0, Position(9, 9), map, fly1, fly2, ground);
    lord->move();
    cout << lord->str() << endl;

    delete map;
    delete fly1;
    delete fly2;
    delete ground;
    delete lord;
    cout.rdbuf(oldBuf); // Khôi phục cout
    expect += "DragonLord[index=0;pos=(9,9)]\n(4,5)\n";
    expect += "DragonLord[index=0;pos=(1,7)]\n";
    print_result(expect, oss.str());
}

void lord_4()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;
    Map *map = new Map(10, 10, 0, nullptr, 0, nullptr);

    FlyTeam *fly1 = new FlyTeam(0, "RRRRRRRRRR", Position("(8,8)"), map, 10, 10);
    FlyTeam *fly2 = new FlyTeam(0, "DDDDDDDDDD", Position("(0,9)"), map, 10, 10);

    GroundTeam *ground = new GroundTeam(0, "RDRD", Position("(8,8)"), map, 10, 500);

    // hiện thực việc bị attack/trap trên dragon lord luôn
    DragonLord *lord = new DragonLord(0, Position(8, 8), map, fly1, fly2, ground);
    cout << lord->str() << endl;
    cout << fly1->attack(lord) << endl;
    cout << fly2->attack(lord) << endl;

    delete map;
    delete fly1;
    delete fly2;
    delete ground;
    delete lord;
    cout.rdbuf(oldBuf); // Khôi phục cout
    expect += "DragonLord[index=0;pos=(8,8)]\n1\n0\n";
    print_result(expect, oss.str());
}

void lord_5()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;
    Map *map = new Map(10, 10, 0, nullptr, 0, nullptr);

    FlyTeam *fly1 = new FlyTeam(0, "RRRRRRRRRR", Position("(9,9)"), map, 10, 10);
    FlyTeam *fly2 = new FlyTeam(0, "DDDDDDDDDD", Position("(0,9)"), map, 10, 10);

    GroundTeam *ground = new GroundTeam(0, "LLLLLLLLD", Position("(8,8)"), map, 10, 500);

    // hiện thực việc bị attack/trap trên dragon lord luôn
    DragonLord *lord = new DragonLord(0, Position(8, 8), map, fly1, fly2, ground);
    cout << lord->str() << endl;
    cout << ground->trap(lord) << endl;
    lord->move();
    cout << ground->trap(lord) << endl;
    lord->move();
    lord->move();
    cout << lord->str() << endl;
    lord->move();
    cout << lord->str() << endl;
    ground->move();
    // cout << ground->str() << endl;
    cout << ground->trap(lord) << endl;

    delete map;
    delete fly1;
    delete fly2;
    delete ground;
    delete lord;
    cout.rdbuf(oldBuf); // Khôi phục cout
    // trap không cộng dồn, hết trap r mới đc lượt trap mới đc trap tiếp
    expect += "DragonLord[index=0;pos=(8,8)]\n1\n0\n";
    expect += "DragonLord[index=0;pos=(8,8)]\n";
    expect += "DragonLord[index=0;pos=(9,0)]\n1\n";
    print_result(expect, oss.str());
}

void lord_6()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;
    Map *map = new Map(10, 10, 0, nullptr, 0, nullptr);

    FlyTeam *fly1 = new FlyTeam(0, "RRRRRRRRRR", Position("(9,9)"), map, 10, 10);
    FlyTeam *fly2 = new FlyTeam(0, "DDDDDDDDDD", Position("(0,9)"), map, 10, 10);

    GroundTeam *ground = new GroundTeam(0, "LLLLLLLLD", Position("(8,8)"), map, 10, 500);

    // hiện thực việc bị attack/trap trên dragon lord luôn
    DragonLord *lord = new DragonLord(0, Position(8, 8), map, fly1, fly2, ground);
    cout << lord->str() << endl;
    cout << ground->trap(lord) << endl;
    lord->move();
    cout << ground->trap(lord) << endl;
    lord->move();
    lord->move();
    cout << lord->str() << endl;
    lord->move();
    cout << lord->str() << endl;
    ground->move();
    // cout << ground->str() << endl;
    cout << ground->trap(lord) << endl;

    delete map;
    delete fly1;
    delete fly2;
    delete ground;
    delete lord;
    cout.rdbuf(oldBuf); // Khôi phục cout
    // trap không cộng dồn, hết trap r mới đc lượt trap mới đc trap tiếp
    expect += "DragonLord[index=0;pos=(8,8)]\n1\n0\n";
    expect += "DragonLord[index=0;pos=(8,8)]\n";
    expect += "DragonLord[index=0;pos=(9,0)]\n1\n";
    print_result(expect, oss.str());
}

vector<pair<string, function<void()>>> TestLord = {
    {"DragonLord _ sample", sample_lord},
    {"DragonLord _ member access", lord_1},
    {"DragonLord _ move manhan dis with fly > 5", lord_2},
    {"DragonLord _ move manhan dis <= 5", lord_3},
    {"DragonLord _ attack by flyteam", lord_4},
    {"DragonLord _ trap by groundteam", lord_5},
    // {"DragonLord _ move to invalid", lord_6},
};

void sample_movobj()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;
    Map *map = new Map(10, 10, 0, nullptr, 0, nullptr);

    FlyTeam *fly1 = new FlyTeam(0, "RRRRRRRRRR", Position("(0,0)"), map, 10, 10);
    FlyTeam *fly2 = new FlyTeam(0, "DDDDDDDDDD", Position("(0,0)"), map, 10, 10);

    GroundTeam *ground = new GroundTeam(0, "RDRD", Position("(0,0)"), map, 10, 500);

    // hiện thực việc bị attack/trap trên dragon lord luôn
    DragonLord *lord = new DragonLord(0, Position(9, 9), map, fly1, fly2, ground);
    cout << lord->str() << endl;

    delete map;
    delete fly1;
    delete fly2;
    delete ground;
    delete lord;
    cout.rdbuf(oldBuf); // Khôi phục cout
    expect += "DragonLord[index=0;pos=(9,9)]\n";
    print_result(expect, oss.str());
}

void movobj_1()
{
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    check_access("MovingObject");
    cout.rdbuf(oldBuf); // Khôi phục cout
    string expect = "All access modifier pass\n";
    print_result(expect, oss.str());
}

void movobj_2()
{
    ostringstream oss;
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;
    MovingObject *a = new GroundTeam(0, "", Position::npos, nullptr, 10, 10);
    cout << a->str() << endl;
    delete a;

    a = new FlyTeam(0, "", Position::npos, nullptr, 10, 10);
    cout << a->str() << endl;
    delete a;

    a = new DragonLord(0, Position(9, 9), nullptr, nullptr, nullptr, nullptr);
    cout << a->str() << endl;
    delete a;

    cout.rdbuf(oldBuf); // Khôi phục cout
    string expect = "\n";
    expect += "GroundTeam[index=0;pos=(-1,-1);moving_rule=]\n";
    expect += "FlyTeam1[index=0;pos=(-1,-1);moving_rule=]\n";
    expect += "DragonLord[index=0;pos=(9,9)]\n";
    print_result(expect, oss.str());
}

vector<pair<string, function<void()>>> TestMovingObject = {
    {"MovingObject _ sample", sample_movobj},
    {"MovingObject _ member access", movobj_1},
    {"MovingObject _ inher base vs devr", movobj_2},
};

void sample_config()
{
    ostringstream oss;
    string expect = "\n";
    streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
    cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
    cout << endl;

    

    cout.rdbuf(oldBuf); // Khôi phục cout
    expect += "DragonLord[index=0;pos=(9,9)]\n";
    print_result(expect, oss.str());
}

vector<pair<string, function<void()>>> TestConfiguration = {
    {"Configuration _ sample", sample_config},
    {"Configuration _ member access", movobj_1},
    {"Configuration _ inher base vs devr", movobj_2},
};

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

int main()
{
    exe_test_pack(TestPosition);
    exe_test_pack(TestMapElement);
    exe_test_pack(TestMap);
    exe_test_pack(TestFlyTeam);
    exe_test_pack(TestGroundTeam);
    exe_test_pack(TestWarrior);
    exe_test_pack(TestLord);
    exe_test_pack(TestMovingObject);
    return 0;
}