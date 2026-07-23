/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Summer 2025
 * Date: 28.07.2025
 */

// The library here is concretely set, students are not allowed to include any other libraries.

#include "dragon.h"

#define TEST

// using namespace std;
// int current_test_no = 0;
// string current_test_content = "default";

// void print_result(const string &expected, const string &output)
// {
//     static bool header_printed = false;

//     bool passed = (expected == output);

//     if (!header_printed)
//     {
//         cout << left
//              << setw(5) << "No"
//              << setw(70) << "Content"
//              << setw(10) << "Status" << "\n";
//         cout << string(5 + 70 + 10, '-') << "\n";
//         header_printed = true;
//     }

//     if (!passed)
//         cout << "\033[31m"; // đỏ

//     cout << left
//          << setw(5) << current_test_no
//          << setw(70) << current_test_content
//          << setw(10) << (passed ? "PASS" : "FAIL") << "\n";
//     cout << "\033[0m"; // reset màu

// #ifdef TEST
//     if (!passed)
//     {

//         cout << "Expect : " << expected << "\n";
//         cout << "Output : " << output << "\n";
//     }
// #endif
// }

// #include <string>
// #include <vector>
// #include <iostream>
// #include <iomanip>
// #include <functional>

// #include <type_traits>

// void exe_test_pack(vector<pair<string, function<void()>>> a)
// {
//     vector<pair<string, function<void()>>> test_pack = a;
//     for (size_t i = 0; i < test_pack.size(); ++i)
//     {
//         current_test_content = test_pack[i].first;
//         test_pack[i].second();
//         current_test_no++;
//     }
// }

// class TestDragonWar
// {
// public:
//     static MapElement *getMapElementAt(const Map &map, int r, int c)
//     {
//         int rows = map.getNumRows();
//         int cols = map.getNumCols();

//         if (r < 0 || r >= rows || c < 0 || c >= cols)
//         {
//             std::cerr << "Out of bounds: (" << r << ", " << c << ")\n";
//             return nullptr;
//         }

//         if (!map.grid)
//         {
//             std::cerr << "map.grid is nullptr — grid not allocated with new[]\n";
//             return nullptr;
//         }

//         if (!map.grid[r])
//         {
//             std::cerr << "map.grid[" << r << "] is nullptr — row not allocated\n";
//             return nullptr;
//         }

//         // Capture the cell
//         auto cell = map.grid[r][c];

//         // Runtime type check
//         typedef decltype(cell) CellType;
//         if (!std::is_pointer<CellType>::value)
//         {
//             std::cerr << "grid[" << r << "][" << c << "] is not a pointer — likely returning MapElement by value\n";
//             return nullptr;
//         }

//         if (cell == nullptr)
//         {
//             std::cerr << "grid[" << r << "][" << c << "] is nullptr — element not initialized\n";
//             return nullptr;
//         }

//         return cell;
//     }
// };

// void sample_pos()
// {
//     ostringstream oss;
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

//     Position a = Position("(1,4)");
//     cout << a.str() << endl;

//     Position b = Position(1, 4);
//     cout << b.str() << endl;

//     cout.rdbuf(oldBuf); // Khôi phục cout

//     string expect = "(1,4)\n(1,4)\n";
//     print_result(expect, oss.str());
// }

// void testPos_1()
// {
//     ostringstream oss;
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

//     Position a = Position("(1,4)");
//     cout << a.str() << endl;

//     Position b = Position(1, 4);
//     cout << b.str() << endl;

//     Position c = Position();
//     cout << c.str() << endl;

//     Position *ptr = new Position();
//     cout << ptr->str() << endl;
//     delete ptr;

//     cout.rdbuf(oldBuf); // Khôi phục cout

//     string expect = "(1,4)\n(1,4)\n(0,0)\n(0,0)\n";
//     print_result(expect, oss.str());
// }

// void testPos_2()
// {
//     ostringstream oss;
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

//     Position a = Position(" (1,4) ");
//     cout << a.str() << endl;

//     Position b = Position("(1, 4)");
//     cout << b.str() << endl;

//     Position c = Position("(11, 4)");
//     cout << c.str() << endl;

//     Position d = Position("(113, 12)");
//     cout << d.str() << endl;

//     cout.rdbuf(oldBuf); // Khôi phục cout

//     string expect = "(1,4)\n(1,4)\n(11,4)\n(113,12)\n";
//     print_result(expect, oss.str());
// }

// void testPos_3()
// {
//     ostringstream oss;
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

//     Position a = Position("(1,4)");
//     cout << a.str() << endl;

//     cout << a.isEqual(1, 4) << a.isEqual(2, 4) << endl;

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     string expect = "(1,4)\n10\n";
//     print_result(expect, oss.str());
// }

// void testPos_4()
// {
//     ostringstream oss;
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
//     Position a = Position("(1,4)");
//     cout << a.str() << endl;

//     cout << a.getRow() << endl;
//     a.setRow(6);
//     cout << a.getRow() << endl;
//     cout << a.str() << endl;
//     cout.rdbuf(oldBuf); // Khôi phục cout

//     string expect = "(1,4)\n1\n6\n(6,4)\n";
//     print_result(expect, oss.str());
// }

// void testPos_5()
// {
//     ostringstream oss;
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
//     Position a = Position("(1,4)");
//     cout << a.str() << endl;

//     cout << a.getCol() << endl;
//     a.setCol(6);
//     cout << a.getCol() << endl;
//     cout << a.str() << endl;
//     cout.rdbuf(oldBuf); // Khôi phục cout

//     string expect = "(1,4)\n4\n6\n(1,6)\n";
//     print_result(expect, oss.str());
// }

// vector<pair<string, function<void()>>> TestPosition = {
//     {"Position _ str()", sample_pos},
//     {"Position _ constructor", testPos_1},
//     {"Position _ constructor by str_pos", testPos_2},
//     {"Position _ isEqual", testPos_3},
//     {"Position _ Get & Set Row", testPos_4},
//     {"Position _ Get & Set Col", testPos_5},
// };

// void sample_mapele()
// {
//     ostringstream oss;
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

//     MapElement a = MapElement(PATH, 10);
//     MapElement b = MapElement(OBSTACLE, 145);
//     MapElement c = MapElement(GROUND_OBSTACLE);

//     cout << a.getType() << " - " << a.getReqDmg() << endl;
//     cout << b.getType() << " - " << b.getReqDmg() << endl;
//     cout << c.getType() << " - " << c.getReqDmg() << endl;

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     string expect = "0 - 10\n1 - 145\n2 - 0\n";
//     print_result(expect, oss.str());
// }

// void testMapele_1()
// {
//     ostringstream oss;
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

//     MapElement a = MapElement(ElementType::PATH);
//     MapElement b = MapElement(ElementType::OBSTACLE);
//     MapElement c = MapElement(ElementType::GROUND_OBSTACLE);

//     cout << a.getType() << b.getType() << c.getType() << endl;

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     string expect = "012\n";
//     print_result(expect, oss.str());
// }

// void testMapele_2()
// {
//     ostringstream oss;
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

//     MapElement a = MapElement(ElementType::PATH);
//     MapElement b = MapElement(ElementType::OBSTACLE, 2);
//     MapElement c = MapElement(ElementType::GROUND_OBSTACLE, 100);

//     cout << a.getReqDmg() << b.getReqDmg() << c.getReqDmg() << endl;

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     string expect = "02100\n";
//     print_result(expect, oss.str());
// }

// vector<pair<string, function<void()>>> TestMapElement = {
//     {"MapElement _ constructor", sample_mapele},
//     {"MapElement _ getType", testMapele_1},
//     {"MapElement _ getRegDmg", testMapele_2},
// };

// void sampleMap()
// {
//     ostringstream oss;
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

//     Position arr[10];

//     Map map = Map(10, 10, 0, arr, 0, arr);
//     cout << map.getNumRows() << " " << map.getNumCols() << endl;

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     string expect = "10 10\n";
//     print_result(expect, oss.str());
// }

// void testMap_1()
// {
//     string expect = "";
//     ostringstream oss;
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

//     Position arr[10];

//     Map map = Map(10, 10, 0, arr, 0, arr);
//     cout << map.getNumRows() << " " << map.getNumCols() << endl;

//     for (int i = 0; i < 10; i++)
//     {
//         for (int j = 0; j < 10; j++)
//         {
//             MapElement *tmp = TestDragonWar::getMapElementAt(map, i, j);
//             if (tmp == nullptr)
//             {
//                 expect = "Grid should be *** or **?";
//                 cout.rdbuf(oldBuf); // Khôi phục cout
//                 print_result(expect, oss.str());
//                 return;
//             }
//             cout << tmp->getType();
//         }
//         cout << endl;
//     }

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     expect = "10 10\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n";
//     print_result(expect, oss.str());
// }

// void testMap_2()
// {
//     string expect = "";
//     ostringstream oss;
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

//     Position arr_obst[10];
//     Position arr_grd_obst[10];
//     arr_obst[0] = Position("(1,1)");
//     arr_grd_obst[0] = Position("(0,1)");

//     Map map = Map(10, 10, 1, arr_obst, 1, arr_grd_obst);
//     cout << map.getNumRows() << " " << map.getNumCols() << endl;

//     for (int i = 0; i < 10; i++)
//     {
//         for (int j = 0; j < 10; j++)
//         {
//             MapElement *tmp = TestDragonWar::getMapElementAt(map, i, j);
//             if (tmp == nullptr)
//             {
//                 expect = "Grid should be *** or **?";
//                 cout.rdbuf(oldBuf); // Khôi phục cout
//                 print_result(expect, oss.str());
//                 return;
//             }
//             cout << tmp->getType();
//         }
//         cout << endl;
//     }

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     expect = "10 10\n0200000000\n0100000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n";
//     print_result(expect, oss.str());
// }

// void testMap_3()
// {
//     string expect = "";
//     ostringstream oss;
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss

//     Position arr_obst[10];
//     Position arr_grd_obst[10];
//     int count_obst = 0;
//     int count_grd_obst = 0;

//     for (int r = 0; r < 10; ++r)
//     {
//         for (int c = 0; c < 10; ++c)
//         {
//             if ((r + c) % 5 == 4 && count_obst < 10)
//                 arr_obst[count_obst++] = Position(r, c);
//             if ((r + c) % 5 == 2 && count_grd_obst < 10)
//                 arr_grd_obst[count_grd_obst++] = Position(r, c);
//         }
//     }

//     Map map = Map(10, 10, count_obst, arr_obst, count_grd_obst, arr_grd_obst);
//     cout << map.getNumRows() << " " << map.getNumCols() << endl;

//     for (int i = 0; i < 10; i++)
//     {
//         for (int j = 0; j < 10; j++)
//         {
//             MapElement *tmp = TestDragonWar::getMapElementAt(map, i, j);
//             if (tmp == nullptr)
//             {
//                 expect = "Grid should be *** or **?";
//                 cout.rdbuf(oldBuf); // Khôi phục cout
//                 print_result(expect, oss.str());
//                 return;
//             }
//             cout << tmp->getType();
//         }
//         cout << endl;
//     }

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     expect = "10 10\n0020100201\n0201002010\n2010020100\n0100201002\n1002010020\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n";
//     print_result(expect, oss.str());
// }

// void testMap_4()
// {
//     string expect = "";
//     ostringstream oss;
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
//     cout << endl;
//     Position arr_obst[10];
//     Position arr_grd_obst[100];
//     int count_obst = 0;
//     int count_grd_obst = 0;
//     for (int i = 0; i < 10; i++)
//         for (int j = 0; j < 10; j++)
//             arr_grd_obst[count_grd_obst++] = Position(i, j);

//     Map map = Map(10, 10, count_obst, arr_obst, count_grd_obst, arr_grd_obst);

//     for (int i = 0; i < 10; i++)
//     {
//         for (int j = 0; j < 10; j++)
//         {
//             MapElement *tmp = TestDragonWar::getMapElementAt(map, i, j);

//             cout << left << setw(5) << tmp->getReqDmg();
//         }
//         cout << "|" << endl;
//     }

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     expect += "\n90   229  368  507  646  785  24   163  302  441  |\n347  486  625  764  3    142  281  420  559  698  |\n604  743  882  121  260  399  538  677  816  55   |\n861  100  239  378  517  656  795  34   173  312  |\n218  357  496  635  774  13   152  291  430  569  |\n475  614  753  892  131  270  409  548  687  826  |\n732  871  110  249  388  527  666  805  44   183  |\n89   228  367  506  645  784  23   162  301  440  |\n346  485  624  763  2    141  280  419  558  697  |\n603  742  881  120  259  398  537  676  815  54   |\n";
//     print_result(expect, oss.str());
// }

// vector<pair<string, function<void()>>> TestMap = {
//     {"MapElement _ sample", sampleMap},
//     {"MapElement _ check ***", testMap_1},
//     {"MapElement _ check handle construct right type of map ele 1", testMap_2},
//     {"MapElement _ check handle construct right type of map ele 2", testMap_3},
//     {"MapElement _ check handle construct right req_dmg ", testMap_4},
// };

// void sampleWarrior()
// {
//     ostringstream oss;
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
//     cout << endl;

//     Position arr_obst[10];
//     Position arr_grd_obst[10];
//     int count_obst = 0;
//     int count_grd_obst = 0;

//     Map map = Map(10, 10, count_obst, arr_obst, count_grd_obst, arr_grd_obst);

//     Warrior *a;
//     a = new FlyTeam(1, "LRUD", Position("(1,5)"), &map, 10, 200);
//     cout << a->str() << endl;
//     cout << a->getHP() << " " << a->getDamage() << endl;
//     cout << a->getCurrentPosition().str() << " " << a->getNextPosition().str() << endl;
//     a->move();
//     cout << a->getCurrentPosition().str() << " " << a->getNextPosition().str() << endl;
//     a->move();
//     cout << a->getCurrentPosition().str() << " " << a->getNextPosition().str() << endl;
//     a->move();
//     cout << a->getCurrentPosition().str() << " " << a->getNextPosition().str() << endl;
//     a->move();
//     cout << a->getCurrentPosition().str() << " " << a->getNextPosition().str() << endl;
//     a->move();
//     cout << a->getCurrentPosition().str() << " " << a->getNextPosition().str() << endl;
//     delete a;

//     a = new GroundTeam(1, "LRUD", Position("(1,5)"), &map, 10, 200, 10);
//     cout << a->str() << endl;
//     cout << a->getHP() << " " << a->getDamage() << endl;
//     cout << a->getCurrentPosition().str() << " " << a->getNextPosition().str() << endl;
//     a->move();
//     cout << a->getCurrentPosition().str() << " " << a->getNextPosition().str() << endl;
//     a->move();
//     cout << a->getCurrentPosition().str() << " " << a->getNextPosition().str() << endl;
//     a->move();
//     cout << a->getCurrentPosition().str() << " " << a->getNextPosition().str() << endl;
//     a->move();
//     cout << a->getCurrentPosition().str() << " " << a->getNextPosition().str() << endl;
//     a->move();
//     cout << a->getCurrentPosition().str() << " " << a->getNextPosition().str() << endl;
//     delete a;

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     string expect = "\nFlyTeam[index=1;pos=(1,5);moving_rule=LRUD]\n10 200\n(1,5) (1,4)\n(1,4) (1,5)\n(1,5) (0,5)\n(0,5) (1,5)\n(1,5) (-1,-1)\n(1,5) (-1,-1)\n";
//     expect += "GroundTeam[index=1;pos=(1,5);moving_rule=LRUD]\n10 200\n(1,5) (1,4)\n(1,4) (1,5)\n(1,5) (0,5)\n(0,5) (1,5)\n(1,5) (-1,-1)\n(1,5) (-1,-1)\n";

//     print_result(expect, oss.str());
// }

// void TestWarrior_1()
// {
//     ostringstream oss;
//     string expect = "\n";
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
//     cout << endl;

//     Position arr_obst[10];
//     Position arr_grd_obst[10];
//     int count_obst = 0;
//     int count_grd_obst = 0;

//     Map map = Map(10, 10, count_obst, arr_obst, count_grd_obst, arr_grd_obst);

//     Warrior *a;
//     a = new FlyTeam(1, "LRUD", Position("(1,5)"), &map, 10, 200);
//     cout << a->str() << " " << a->getHP() << " " << a->getDamage() << endl;
//     a->setDamage(123);
//     a->setHP(123);
//     cout << a->str() << " " << a->getHP() << " " << a->getDamage() << endl;
//     delete a;

//     a = new GroundTeam(1, "LRUD", Position("(1,5)"), &map, 10, 200, 4);
//     cout << a->str() << " " << a->getHP() << " " << a->getDamage() << endl;
//     a->setDamage(123);
//     a->setHP(123);
//     cout << a->str() << " " << a->getHP() << " " << a->getDamage() << endl;
//     delete a;

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     expect += "FlyTeam[index=1;pos=(1,5);moving_rule=LRUD] 10 200\n";
//     expect += "FlyTeam[index=1;pos=(1,5);moving_rule=LRUD] 123 123\n";
//     expect += "GroundTeam[index=1;pos=(1,5);moving_rule=LRUD] 10 200\n";
//     expect += "GroundTeam[index=1;pos=(1,5);moving_rule=LRUD] 123 123\n";
//     print_result(expect, oss.str());
// }

// void TestWarrior_2()
// {
//     ostringstream oss;
//     string expect = "\n";
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
//     cout << endl;

//     Position arr_obst[10];
//     Position arr_grd_obst[10];
//     int count_obst = 0;
//     int count_grd_obst = 0;

//     Map map = Map(10, 10, count_obst, arr_obst, count_grd_obst, arr_grd_obst);

//     Warrior *a;
//     // check di chuyển 4 hướng + hết chuổi rule
//     a = new FlyTeam(1, "LRUD", Position("(1,5)"), &map, 10, 200);
//     cout << a->str() << endl;
//     a->move();
//     cout << a->str() << endl;
//     a->move();
//     cout << a->str() << endl;
//     a->move();
//     cout << a->str() << endl;
//     a->move();
//     cout << a->str() << endl;
//     a->move(); // move lần này không thành công, dù chạy đc nhưng thực tế đã hết lượt di chuyển
//     cout << a->str() << endl;
//     delete a;

//     // check di chuyển chạm tường
//     a = new FlyTeam(1, "L", Position("(0,0)"), &map, 10, 200);
//     cout << a->str() << endl;
//     a->move();
//     cout << a->str() << endl;
//     delete a;

//     a = new FlyTeam(1, "U", Position("(0,0)"), &map, 10, 200);
//     cout << a->str() << endl;
//     a->move();
//     cout << a->str() << endl;
//     delete a;

//     a = new FlyTeam(1, "R", Position("(9,9)"), &map, 10, 200);
//     cout << a->str() << endl;
//     a->move();
//     cout << a->str() << endl;
//     delete a;

//     a = new FlyTeam(1, "D", Position("(9,9)"), &map, 10, 200);
//     cout << a->str() << endl;
//     a->move();
//     cout << a->str() << endl;
//     delete a;

//     // check di chuyen ngoài map -> không thể duy chuyển
//     a = new FlyTeam(1, "LRUD", Position("(10,10)"), &map, 10, 200);
//     cout << a->str() << endl;
//     a->move();
//     cout << a->str() << endl;
//     delete a;

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     expect += "FlyTeam[index=1;pos=(1,5);moving_rule=LRUD]\n";
//     expect += "FlyTeam[index=1;pos=(1,4);moving_rule=LRUD]\n";
//     expect += "FlyTeam[index=1;pos=(1,5);moving_rule=LRUD]\n";
//     expect += "FlyTeam[index=1;pos=(0,5);moving_rule=LRUD]\n";
//     expect += "FlyTeam[index=1;pos=(1,5);moving_rule=LRUD]\n";
//     expect += "FlyTeam[index=1;pos=(1,5);moving_rule=LRUD]\n";

//     expect += "FlyTeam[index=1;pos=(0,0);moving_rule=L]\n";
//     expect += "FlyTeam[index=1;pos=(0,0);moving_rule=L]\n";
//     expect += "FlyTeam[index=1;pos=(0,0);moving_rule=U]\n";
//     expect += "FlyTeam[index=1;pos=(0,0);moving_rule=U]\n";

//     expect += "FlyTeam[index=1;pos=(9,9);moving_rule=R]\n";
//     expect += "FlyTeam[index=1;pos=(9,9);moving_rule=R]\n";
//     expect += "FlyTeam[index=1;pos=(9,9);moving_rule=D]\n";
//     expect += "FlyTeam[index=1;pos=(9,9);moving_rule=D]\n";

//     expect += "FlyTeam[index=1;pos=(10,10);moving_rule=LRUD]\n";
//     expect += "FlyTeam[index=1;pos=(10,10);moving_rule=LRUD]\n";
//     print_result(expect, oss.str());
// }

// void TestWarrior_3()
// {
//     ostringstream oss;
//     string expect = "\n";
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
//     cout << endl;

//     Position arr_obst[10];
//     Position arr_grd_obst[10];
//     int count_obst = 0;
//     int count_grd_obst = 0;

//     Map map = Map(10, 10, count_obst, arr_obst, count_grd_obst, arr_grd_obst);

//     Warrior *a;
//     // check di chuyển 4 hướng + hết chuổi rule
//     a = new GroundTeam(1, "LRUD", Position("(1,5)"), &map, 10, 200, 0);
//     cout << a->str() << endl;
//     a->move();
//     cout << a->str() << endl;
//     a->move();
//     cout << a->str() << endl;
//     a->move();
//     cout << a->str() << endl;
//     a->move();
//     cout << a->str() << endl;
//     a->move(); // move lần này không thành công, dù chạy đc nhưng thực tế đã hết lượt di chuyển
//     cout << a->str() << endl;
//     delete a;

//     // check di chuyển chạm tường
//     a = new GroundTeam(1, "L", Position("(0,0)"), &map, 10, 200, 0);
//     cout << a->str() << endl;
//     a->move();
//     cout << a->str() << endl;
//     delete a;

//     a = new GroundTeam(1, "U", Position("(0,0)"), &map, 10, 200, 0);
//     cout << a->str() << endl;
//     a->move();
//     cout << a->str() << endl;
//     delete a;

//     a = new GroundTeam(1, "R", Position("(9,9)"), &map, 10, 200, 0);
//     cout << a->str() << endl;
//     a->move();
//     cout << a->str() << endl;
//     delete a;

//     a = new GroundTeam(1, "D", Position("(9,9)"), &map, 10, 200, 0);
//     cout << a->str() << endl;
//     a->move();
//     cout << a->str() << endl;
//     delete a;

//     // check di chuyen ngoài map -> không thể duy chuyển
//     a = new GroundTeam(1, "LRUD", Position("(10,10)"), &map, 10, 200, 0);
//     cout << a->str() << endl;
//     a->move();
//     cout << a->str() << endl;
//     delete a;

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     expect += "GroundTeam[index=1;pos=(1,5);moving_rule=LRUD]\n";
//     expect += "GroundTeam[index=1;pos=(1,4);moving_rule=LRUD]\n";
//     expect += "GroundTeam[index=1;pos=(1,5);moving_rule=LRUD]\n";
//     expect += "GroundTeam[index=1;pos=(0,5);moving_rule=LRUD]\n";
//     expect += "GroundTeam[index=1;pos=(1,5);moving_rule=LRUD]\n";
//     expect += "GroundTeam[index=1;pos=(1,5);moving_rule=LRUD]\n";

//     expect += "GroundTeam[index=1;pos=(0,0);moving_rule=L]\n";
//     expect += "GroundTeam[index=1;pos=(0,0);moving_rule=L]\n";
//     expect += "GroundTeam[index=1;pos=(0,0);moving_rule=U]\n";
//     expect += "GroundTeam[index=1;pos=(0,0);moving_rule=U]\n";

//     expect += "GroundTeam[index=1;pos=(9,9);moving_rule=R]\n";
//     expect += "GroundTeam[index=1;pos=(9,9);moving_rule=R]\n";
//     expect += "GroundTeam[index=1;pos=(9,9);moving_rule=D]\n";
//     expect += "GroundTeam[index=1;pos=(9,9);moving_rule=D]\n";

//     expect += "GroundTeam[index=1;pos=(10,10);moving_rule=LRUD]\n";
//     expect += "GroundTeam[index=1;pos=(10,10);moving_rule=LRUD]\n";
//     print_result(expect, oss.str());
// }

// void TestWarrior_4()
// {
//     ostringstream oss;
//     string expect = "\n";
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
//     cout << endl;

//     Position arr_obst[20];
//     Position arr_grd_obst[20];
//     int count_obst = 0;
//     int count_grd_obst = 0;

//     for (int i = 0; i < 10; i++)
//     {
//         if ((i == 4) || (i == 5))
//             continue;
//         arr_obst[count_obst++] = Position(4, i);
//         arr_obst[count_obst++] = Position(5, i);
//         arr_grd_obst[count_grd_obst++] = Position(i, 4);
//         arr_grd_obst[count_grd_obst++] = Position(i, 5);
//     }

//     Map map = Map(10, 10, count_obst, arr_obst, count_grd_obst, arr_grd_obst);
//     // print map
//     for (int i = 0; i < 10; i++)
//     {
//         for (int j = 0; j < 10; j++)
//         {
//             MapElement *tmp = TestDragonWar::getMapElementAt(map, i, j);
//             cout << tmp->getType();
//         }
//         cout << endl;
//     }

//     Warrior *a;
//     // check di chuyển 4 hướng + hết chuổi rule
//     a = new FlyTeam(1, "RULLDDRRRUUULLLL", Position("(2,1)"), &map, 10, 200);
//     cout << a->str() << endl;

//     for (int i = 0; i < 20; i++)
//         a->move();
//     cout << a->str() << endl;
//     delete a;

//     a = new FlyTeam(1, "RULLDDRRRUUULLLL", Position("(2,7)"), &map, 10, 200);
//     cout << a->str() << endl;

//     for (int i = 0; i < 20; i++)
//         a->move();
//     cout << a->str() << endl;
//     delete a;

//     a = new FlyTeam(1, "RULLDDRRRUUULLLL", Position("(8,1)"), &map, 10, 200);
//     cout << a->str() << endl;

//     for (int i = 0; i < 20; i++)
//         a->move();
//     cout << a->str() << endl;
//     delete a;

//     a = new FlyTeam(1, "RULLDDRRRUUULLLL", Position("(8,7)"), &map, 10, 200);
//     cout << a->str() << endl;

//     for (int i = 0; i < 20; i++)
//         a->move();
//     cout << a->str() << endl;
//     delete a;

//     count_obst = 0;
//     count_grd_obst = 0;
//     for (int i = 0; i < 10; i++)
//     {
//         if ((i == 4) || (i == 5))
//             continue;
//         arr_obst[count_obst++] = Position(i, 4);
//         arr_obst[count_obst++] = Position(i, 5);
//         arr_grd_obst[count_grd_obst++] = Position(4, i);
//         arr_grd_obst[count_grd_obst++] = Position(5, i);
//     }

//     map = Map(10, 10, count_obst, arr_obst, count_grd_obst, arr_grd_obst);
//     for (int i = 0; i < 10; i++)
//     {
//         for (int j = 0; j < 10; j++)
//         {
//             MapElement *tmp = TestDragonWar::getMapElementAt(map, i, j);
//             cout << tmp->getType();
//         }
//         cout << endl;
//     }

//     a = new FlyTeam(1, "RULLDDRRRUUULLLL", Position("(2,1)"), &map, 10, 200);
//     cout << a->str() << endl;

//     for (int i = 0; i < 20; i++)
//         a->move();
//     cout << a->str() << endl;
//     delete a;

//     a = new FlyTeam(1, "RULLDDRRRUUULLLL", Position("(2,7)"), &map, 10, 200);
//     cout << a->str() << endl;

//     for (int i = 0; i < 20; i++)
//         a->move();
//     cout << a->str() << endl;
//     delete a;

//     a = new FlyTeam(1, "RULLDDRRRUUULLLL", Position("(8,1)"), &map, 10, 200);
//     cout << a->str() << endl;

//     for (int i = 0; i < 20; i++)
//         a->move();
//     cout << a->str() << endl;
//     delete a;

//     a = new FlyTeam(1, "RULLDDRRRUUULLLL", Position("(8,7)"), &map, 10, 200);
//     cout << a->str() << endl;

//     for (int i = 0; i < 20; i++)
//         a->move();
//     cout << a->str() << endl;
//     delete a;

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     expect += "0000220000\n0000220000\n0000220000\n0000220000\n1111001111\n1111001111\n0000220000\n0000220000\n0000220000\n0000220000\n";
//     expect += "FlyTeam[index=1;pos=(2,1);moving_rule=RULLDDRRRUUULLLL]\n";
//     expect += "FlyTeam[index=1;pos=(0,0);moving_rule=RULLDDRRRUUULLLL]\n";

//     expect += "FlyTeam[index=1;pos=(2,7);moving_rule=RULLDDRRRUUULLLL]\n";
//     expect += "FlyTeam[index=1;pos=(0,5);moving_rule=RULLDDRRRUUULLLL]\n";

//     expect += "FlyTeam[index=1;pos=(8,1);moving_rule=RULLDDRRRUUULLLL]\n";
//     expect += "FlyTeam[index=1;pos=(6,0);moving_rule=RULLDDRRRUUULLLL]\n";

//     expect += "FlyTeam[index=1;pos=(8,7);moving_rule=RULLDDRRRUUULLLL]\n";
//     expect += "FlyTeam[index=1;pos=(6,5);moving_rule=RULLDDRRRUUULLLL]\n";

//     expect += "0000110000\n0000110000\n0000110000\n0000110000\n2222002222\n2222002222\n0000110000\n0000110000\n0000110000\n0000110000\n";

//     expect += "FlyTeam[index=1;pos=(2,1);moving_rule=RULLDDRRRUUULLLL]\n";
//     expect += "FlyTeam[index=1;pos=(0,0);moving_rule=RULLDDRRRUUULLLL]\n";

//     expect += "FlyTeam[index=1;pos=(2,7);moving_rule=RULLDDRRRUUULLLL]\n";
//     expect += "FlyTeam[index=1;pos=(0,6);moving_rule=RULLDDRRRUUULLLL]\n";

//     expect += "FlyTeam[index=1;pos=(8,1);moving_rule=RULLDDRRRUUULLLL]\n";
//     expect += "FlyTeam[index=1;pos=(6,0);moving_rule=RULLDDRRRUUULLLL]\n";

//     expect += "FlyTeam[index=1;pos=(8,7);moving_rule=RULLDDRRRUUULLLL]\n";
//     expect += "FlyTeam[index=1;pos=(6,6);moving_rule=RULLDDRRRUUULLLL]\n";
//     print_result(expect, oss.str());
// }

// void TestWarrior_5()
// {
//     ostringstream oss;
//     string expect = "\n";
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
//     cout << endl;

//     Position arr_obst[20];
//     Position arr_grd_obst[20];
//     int count_obst = 0;
//     int count_grd_obst = 0;

//     for (int i = 0; i < 10; i++)
//     {
//         if ((i == 4) || (i == 5))
//             continue;
//         arr_obst[count_obst++] = Position(4, i);
//         arr_obst[count_obst++] = Position(5, i);
//         arr_grd_obst[count_grd_obst++] = Position(i, 4);
//         arr_grd_obst[count_grd_obst++] = Position(i, 5);
//     }

//     Map map = Map(10, 10, count_obst, arr_obst, count_grd_obst, arr_grd_obst);
//     // print map
//     for (int i = 0; i < 10; i++)
//     {
//         for (int j = 0; j < 10; j++)
//         {
//             MapElement *tmp = TestDragonWar::getMapElementAt(map, i, j);
//             cout << tmp->getType();
//         }
//         cout << endl;
//     }

//     Warrior *a;

//     a = new GroundTeam(1, "RRRRRRRRRRRR", Position("(0,0)"), &map, 10, 900, 10);
//     cout << a->str() << endl;
//     for (int i = 0; i < 10; i++)
//         a->move();
//     cout << a->str() << endl;
//     delete a;

//     a = new GroundTeam(1, "RRRRRRRRRRRR", Position("(0,0)"), &map, 10, 10, 10);
//     cout << a->str() << endl;
//     for (int i = 0; i < 10; i++)
//         a->move();
//     cout << a->str() << endl;
//     delete a;

//     a = new GroundTeam(1, "DDDDDDDDDDDD", Position("(0,0)"), &map, 10, 10, 10);
//     cout << a->str() << endl;
//     for (int i = 0; i < 10; i++)
//         a->move();
//     cout << a->str() << endl;
//     delete a;

//     a = new GroundTeam(1, "LLLLLLLLLLLL", Position("(0,0)"), &map, 10, 10, 10);
//     cout << a->str() << endl;
//     for (int i = 0; i < 10; i++)
//         a->move();
//     cout << a->str() << endl;
//     delete a;

//     a = new GroundTeam(1, "RRRRRRRRRRRR", Position("(0,0)"), &map, 10, 700, 10);
//     cout << a->str() << endl;
//     for (int i = 0; i < 10; i++)
//         a->move();
//     cout << a->str() << endl;
//     delete a;

//     count_obst = 0;
//     count_grd_obst = 0;
//     for (int i = 0; i < 10; i++)
//     {
//         if ((i == 4) || (i == 5))
//             continue;
//         arr_obst[count_obst++] = Position(i, 4);
//         arr_obst[count_obst++] = Position(i, 5);
//         arr_grd_obst[count_grd_obst++] = Position(4, i);
//         arr_grd_obst[count_grd_obst++] = Position(5, i);
//     }

//     map = Map(10, 10, count_obst, arr_obst, count_grd_obst, arr_grd_obst);

//     // print map
//     for (int i = 0; i < 10; i++)
//     {
//         for (int j = 0; j < 10; j++)
//         {
//             MapElement *tmp = TestDragonWar::getMapElementAt(map, i, j);
//             cout << tmp->getType();
//         }
//         cout << endl;
//     }
//     // DDDDDLLLUUUUURRDDDDLUUUU
//     a = new GroundTeam(1, "DDDDDLLLUUUUURRDDDDLUUUUUUU", Position("(0,3)"), &map, 10, 900, 10);
//     cout << a->str() << endl;
//     for (int i = 0; i < 30; i++)
//         a->move();
//     cout << a->str() << endl;
//     delete a;

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     expect += "0000220000\n0000220000\n0000220000\n0000220000\n1111001111\n1111001111\n0000220000\n0000220000\n0000220000\n0000220000\n";
//     expect += "GroundTeam[index=1;pos=(0,0);moving_rule=RRRRRRRRRRRR]\n";
//     expect += "GroundTeam[index=1;pos=(0,9);moving_rule=RRRRRRRRRRRR]\n";

//     expect += "GroundTeam[index=1;pos=(0,0);moving_rule=RRRRRRRRRRRR]\n";
//     expect += "GroundTeam[index=1;pos=(0,3);moving_rule=RRRRRRRRRRRR]\n";

//     expect += "GroundTeam[index=1;pos=(0,0);moving_rule=DDDDDDDDDDDD]\n";
//     expect += "GroundTeam[index=1;pos=(3,0);moving_rule=DDDDDDDDDDDD]\n";

//     expect += "GroundTeam[index=1;pos=(0,0);moving_rule=LLLLLLLLLLLL]\n";
//     expect += "GroundTeam[index=1;pos=(0,0);moving_rule=LLLLLLLLLLLL]\n";

//     expect += "GroundTeam[index=1;pos=(0,0);moving_rule=RRRRRRRRRRRR]\n";
//     expect += "GroundTeam[index=1;pos=(0,4);moving_rule=RRRRRRRRRRRR]\n";

//     expect += "0000110000\n0000110000\n0000110000\n0000110000\n2222002222\n2222002222\n0000110000\n0000110000\n0000110000\n0000110000\n";
//     expect += "GroundTeam[index=1;pos=(0,3);moving_rule=DDDDDLLLUUUUURRDDDDLUUUUUUU]\n";
//     expect += "GroundTeam[index=1;pos=(0,1);moving_rule=DDDDDLLLUUUUURRDDDDLUUUUUUU]\n";

//     print_result(expect, oss.str());
// }

// vector<pair<string, function<void()>>> TestWarrior = {
//     {"Warrior _ general constructor", sampleWarrior},
//     {"Warrior _ general getter & setter", TestWarrior_1},
//     {"Warrior _ FlyTeam move in empty map", TestWarrior_2},
//     {"Warrior _ GroundTeam move in empty map", TestWarrior_3},
//     {"Warrior _ FlyTeam move in specified map", TestWarrior_4},
//     {"Warrior _ GroundTeam move in specified map", TestWarrior_5},
// };

// void sampleDragonLord()
// {
//     ostringstream oss;
//     string expect = "\n";
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
//     cout << endl;

//     Position arr_obst[10];
//     Position arr_grd_obst[10];
//     int count_obst = 0;
//     int count_grd_obst = 0;
//     Map map = Map(10, 10, count_obst, arr_obst, count_grd_obst, arr_grd_obst);
//     FlyTeam a = FlyTeam(1, "LLLL", Position("(1,4)"), &map, 10, 10);
//     FlyTeam b = FlyTeam(1, "LLLL", Position("(1,4)"), &map, 10, 10);

//     DragonLord lord = DragonLord(1, Position("(0,0)"), &map, &a, &b);
//     cout << lord.str() << endl;

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     expect += "DragonLord[index=1;pos=(0,0)]\n";
//     print_result(expect, oss.str());
// }

// void TestLord_1()
// {
//     ostringstream oss;
//     string expect = "\n";
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
//     cout << endl;

//     Position arr_obst[10];
//     Position arr_grd_obst[10];
//     int count_obst = 0;
//     int count_grd_obst = 0;
//     Map map = Map(10, 10, count_obst, arr_obst, count_grd_obst, arr_grd_obst);

//     FlyTeam a = FlyTeam(1, "LLLL", Position("(1,4)"), &map, 10, 10);
//     FlyTeam b = FlyTeam(1, "LLLL", Position("(1,4)"), &map, 10, 10);

//     DragonLord lord = DragonLord(1, Position("(0,0)"), &map, &a, &b);
//     cout << lord.str() << endl;

//     FlyTeam *c = new FlyTeam(1, "LLLL", Position("(1,4)"), &map, 10, 10);
//     FlyTeam *d = new FlyTeam(1, "LLLL", Position("(1,4)"), &map, 10, 10);

//     lord = DragonLord(1, Position("(0,0)"), &map, c, d);
//     cout << lord.str() << endl;

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     expect += "DragonLord[index=1;pos=(0,0)]\n";
//     expect += "DragonLord[index=1;pos=(0,0)]\n";

//     print_result(expect, oss.str());
// }

// void TestLord_2()
// {
//     ostringstream oss;
//     string expect = "\n";
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
//     cout << endl;

//     Position arr_obst[10];
//     Position arr_grd_obst[10];
//     int count_obst = 0;
//     int count_grd_obst = 0;
//     Map map = Map(10, 10, count_obst, arr_obst, count_grd_obst, arr_grd_obst);

//     FlyTeam a = FlyTeam(1, "LLLL", Position("(1,4)"), &map, 10, 10);
//     FlyTeam b = FlyTeam(1, "LLLL", Position("(1,4)"), &map, 10, 10);

//     DragonLord lord = DragonLord(1, Position("(1,0)"), &map, &a, &b);
//     cout << lord.str() << endl;

//     FlyTeam *c = new FlyTeam(1, "LLLL", Position("(1,4)"), &map, 10, 10);
//     FlyTeam *d = new FlyTeam(1, "LLLL", Position("(1,4)"), &map, 10, 10);

//     lord = DragonLord(1, Position("(0,1)"), &map, c, d);
//     cout << lord.str() << endl;

//     lord = DragonLord(1, Position::npos, &map, c, d);
//     cout << lord.str() << endl;

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     expect += "DragonLord[index=1;pos=(1,0)]\n";
//     expect += "DragonLord[index=1;pos=(0,1)]\n";
//     expect += "DragonLord[index=1;pos=(-1,-1)]\n";

//     print_result(expect, oss.str());
// }

// void TestLord_3()
// {
//     ostringstream oss;
//     string expect = "\n";
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
//     cout << endl;

//     Position arr_obst[10];
//     Position arr_grd_obst[10];
//     int count_obst = 0;
//     int count_grd_obst = 0;
//     Map map = Map(10, 10, count_obst, arr_obst, count_grd_obst, arr_grd_obst);

//     FlyTeam a = FlyTeam(1, "DDDDDDDDDDDDDDDDDDDD", Position("(0,9)"), &map, 10, 10);
//     FlyTeam b = FlyTeam(1, "RRRRRRRRRRRRRRRRRRRR", Position("(9,0)"), &map, 10, 10);
//     cout << a.str() << endl;
//     cout << b.str() << endl;

//     DragonLord lord = DragonLord(1, Position("(0,0)"), &map, &a, &b);
//     cout << lord.str() << endl;

//     for (int i = 0; i < 12; i++)
//     {
//         cout << "Move " << i << endl;
//         lord.move();
//         a.move();
//         b.move();

//         cout << a.str() << endl;
//         cout << b.str() << endl;
//         cout << lord.str() << endl;
//     }

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     expect += "FlyTeam[index=1;pos=(0,9);moving_rule=DDDDDDDDDDDDDDDDDDDD]\n";
//     expect += "FlyTeam[index=1;pos=(9,0);moving_rule=RRRRRRRRRRRRRRRRRRRR]\n";
//     expect += "DragonLord[index=1;pos=(0,0)]\n";

//     expect += "Move 0\nFlyTeam[index=1;pos=(1,9);moving_rule=DDDDDDDDDDDDDDDDDDDD]\nFlyTeam[index=1;pos=(9,1);moving_rule=RRRRRRRRRRRRRRRRRRRR]\nDragonLord[index=1;pos=(9,9)]\nMove 1\nFlyTeam[index=1;pos=(2,9);moving_rule=DDDDDDDDDDDDDDDDDDDD]\nFlyTeam[index=1;pos=(9,2);moving_rule=RRRRRRRRRRRRRRRRRRRR]\nDragonLord[index=1;pos=(8,8)]\nMove 2\nFlyTeam[index=1;pos=(3,9);moving_rule=DDDDDDDDDDDDDDDDDDDD]\nFlyTeam[index=1;pos=(9,3);moving_rule=RRRRRRRRRRRRRRRRRRRR]\nDragonLord[index=1;pos=(7,7)]\nMove 3\nFlyTeam[index=1;pos=(4,9);moving_rule=DDDDDDDDDDDDDDDDDDDD]\nFlyTeam[index=1;pos=(9,4);moving_rule=RRRRRRRRRRRRRRRRRRRR]\nDragonLord[index=1;pos=(6,6)]\nMove 4\nFlyTeam[index=1;pos=(5,9);moving_rule=DDDDDDDDDDDDDDDDDDDD]\nFlyTeam[index=1;pos=(9,5);moving_rule=RRRRRRRRRRRRRRRRRRRR]\nDragonLord[index=1;pos=(5,5)]\nMove 5\nFlyTeam[index=1;pos=(6,9);moving_rule=DDDDDDDDDDDDDDDDDDDD]\nFlyTeam[index=1;pos=(9,6);moving_rule=RRRRRRRRRRRRRRRRRRRR]\nDragonLord[index=1;pos=(4,4)]\nMove 6\nFlyTeam[index=1;pos=(7,9);moving_rule=DDDDDDDDDDDDDDDDDDDD]\nFlyTeam[index=1;pos=(9,7);moving_rule=RRRRRRRRRRRRRRRRRRRR]\nDragonLord[index=1;pos=(3,3)]\nMove 7\nFlyTeam[index=1;pos=(8,9);moving_rule=DDDDDDDDDDDDDDDDDDDD]\nFlyTeam[index=1;pos=(9,8);moving_rule=RRRRRRRRRRRRRRRRRRRR]\nDragonLord[index=1;pos=(2,2)]\nMove 8\nFlyTeam[index=1;pos=(9,9);moving_rule=DDDDDDDDDDDDDDDDDDDD]\nFlyTeam[index=1;pos=(9,9);moving_rule=RRRRRRRRRRRRRRRRRRRR]\nDragonLord[index=1;pos=(1,1)]\nMove 9\nFlyTeam[index=1;pos=(9,9);moving_rule=DDDDDDDDDDDDDDDDDDDD]\nFlyTeam[index=1;pos=(9,9);moving_rule=RRRRRRRRRRRRRRRRRRRR]\nDragonLord[index=1;pos=(0,0)]\nMove 10\nFlyTeam[index=1;pos=(9,9);moving_rule=DDDDDDDDDDDDDDDDDDDD]\nFlyTeam[index=1;pos=(9,9);moving_rule=RRRRRRRRRRRRRRRRRRRR]\nDragonLord[index=1;pos=(0,0)]\nMove 11\nFlyTeam[index=1;pos=(9,9);moving_rule=DDDDDDDDDDDDDDDDDDDD]\nFlyTeam[index=1;pos=(9,9);moving_rule=RRRRRRRRRRRRRRRRRRRR]\nDragonLord[index=1;pos=(0,0)]\n";
//     print_result(expect, oss.str());
// }

// void TestLord_4()
// {
//     ostringstream oss;
//     string expect = "\n";
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
//     cout << endl;

//     Position arr_obst[10];
//     Position arr_grd_obst[10];
//     int count_obst = 0;
//     int count_grd_obst = 0;
//     Map map = Map(10, 10, count_obst, arr_obst, count_grd_obst, arr_grd_obst);

//     FlyTeam a = FlyTeam(1, "DDDDDDDDDDDDDDDDDDDD", Position("(0,9)"), &map, 10, 10);
//     FlyTeam b = FlyTeam(1, "RRRRRRRRRRRRRRRRRRRR", Position("(8,0)"), &map, 10, 10);
//     DragonLord lord = DragonLord(1, Position("(0,0)"), &map, &a, &b);

//     cout << a.str() << endl;
//     cout << b.str() << endl;
//     cout << lord.str() << endl;
//     lord.move();
//     cout << lord.str() << endl;

//     a = FlyTeam(1, "DDDDDDDDDDDDDDDDDDDD", Position("(5,6)"), &map, 10, 10);
//     b = FlyTeam(1, "RRRRRRRRRRRRRRRRRRRR", Position("(6,8)"), &map, 10, 10);
//     lord = DragonLord(1, Position("(0,0)"), &map, &a, &b);

//     cout << a.str() << endl;
//     cout << b.str() << endl;
//     cout << lord.str() << endl;
//     lord.move();
//     cout << lord.str() << endl;

//     a = FlyTeam(1, "DDDDDDDDDDDDDDDDDDDD", Position("(10,10)"), &map, 10, 10);
//     b = FlyTeam(1, "RRRRRRRRRRRRRRRRRRRR", Position("(2,3)"), &map, 10, 10);
//     lord = DragonLord(1, Position("(0,0)"), &map, &a, &b);

//     cout << a.str() << endl;
//     cout << b.str() << endl;
//     cout << lord.str() << endl;
//     lord.move();
//     cout << lord.str() << endl;

//     a = FlyTeam(1, "DDDDDDDDDDDDDDDDDDDD", Position("(10,4)"), &map, 10, 10);
//     b = FlyTeam(1, "RRRRRRRRRRRRRRRRRRRR", Position("(2,3)"), &map, 10, 10);
//     lord = DragonLord(1, Position("(0,0)"), &map, &a, &b);

//     cout << a.str() << endl;
//     cout << b.str() << endl;
//     cout << lord.str() << endl;
//     lord.move();
//     cout << lord.str() << endl;

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     expect += "FlyTeam[index=1;pos=(0,9);moving_rule=DDDDDDDDDDDDDDDDDDDD]\n";
//     expect += "FlyTeam[index=1;pos=(8,0);moving_rule=RRRRRRRRRRRRRRRRRRRR]\n";
//     expect += "DragonLord[index=1;pos=(0,0)]\n";
//     expect += "DragonLord[index=1;pos=(8,9)]\n";

//     expect += "FlyTeam[index=1;pos=(5,6);moving_rule=DDDDDDDDDDDDDDDDDDDD]\n";
//     expect += "FlyTeam[index=1;pos=(6,8);moving_rule=RRRRRRRRRRRRRRRRRRRR]\n";
//     expect += "DragonLord[index=1;pos=(0,0)]\n";
//     expect += "DragonLord[index=1;pos=(1,2)]\n";

//     expect += "FlyTeam[index=1;pos=(10,10);moving_rule=DDDDDDDDDDDDDDDDDDDD]\n";
//     expect += "FlyTeam[index=1;pos=(2,3);moving_rule=RRRRRRRRRRRRRRRRRRRR]\n";
//     expect += "DragonLord[index=1;pos=(0,0)]\n";
//     expect += "DragonLord[index=1;pos=(7,8)]\n";

//     expect += "FlyTeam[index=1;pos=(10,4);moving_rule=DDDDDDDDDDDDDDDDDDDD]\n";
//     expect += "FlyTeam[index=1;pos=(2,3);moving_rule=RRRRRRRRRRRRRRRRRRRR]\n";
//     expect += "DragonLord[index=1;pos=(0,0)]\n";
//     expect += "DragonLord[index=1;pos=(1,8)]\n";

//     print_result(expect, oss.str());
// }

// void TestLord_5()
// {
//     ostringstream oss;
//     string expect = "\n";
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
//     cout << endl;

//     Position arr_obst[10];
//     Position arr_grd_obst[10];
//     int count_obst = 0;
//     int count_grd_obst = 0;
//     Map map = Map(10, 10, count_obst, arr_obst, count_grd_obst, arr_grd_obst);

//     FlyTeam a = FlyTeam(1, "DDDDDDDDDDDDDDDDDDDD", Position("(0,12)"), &map, 10, 10);
//     FlyTeam b = FlyTeam(1, "RRRRRRRRRRRRRRRRRRRR", Position("(12,0)"), &map, 10, 10);
//     DragonLord lord = DragonLord(1, Position("(0,0)"), &map, &a, &b);

//     lord.move();

//     cout << lord.getCurrentPosition().str() << endl;

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     expect += "(0,0)\n";
//     print_result(expect, oss.str());
// }

// vector<pair<string, function<void()>>> TestDragonLord = {
//     {"DragonLord _ general constructor", sampleDragonLord},
//     {"DragonLord _ constructor in multi way", TestLord_1},
//     {"DragonLord _ current position", TestLord_2},
//     {"DragonLord _ next position position (check abs delta)", TestLord_3},
//     {"DragonLord _ next position position (x,y) or (y,x)", TestLord_4},
//     {"DragonLord _ next position position with no valid position", TestLord_5},
// };

// void sampleArrMovObj()
// {
//     ostringstream oss;
//     string expect = "\n";
//     streambuf *oldBuf = cout.rdbuf(); // Lưu lại buffer gốc
//     cout.rdbuf(oss.rdbuf());          // Chuyển cout sang oss
//     cout << endl;

//     Position arr_obst[10];
//     Position arr_grd_obst[10];
//     int count_obst = 0;
//     int count_grd_obst = 0;
//     Map map = Map(10, 10, count_obst, arr_obst, count_grd_obst, arr_grd_obst);
//     FlyTeam a = FlyTeam(1, "LLLL", Position("(1,4)"), &map, 10, 10);
//     FlyTeam b = FlyTeam(1, "LLLL", Position("(1,4)"), &map, 10, 10);
//     GroundTeam c = GroundTeam(1, "UUUU", Position(1, 1), &map, 10, 10, 10);
//     DragonLord lord = DragonLord(1, Position("(0,0)"), &map, &a, &b);

//     ArrayMovingObject arr = ArrayMovingObject(10);

//     arr.add(&a);
//     arr.add(&b);
//     arr.add(&c);
//     arr.add(&lord);

//     cout << arr.str() << endl;
//     cout << arr.isFull() << endl;

//     cout.rdbuf(oldBuf); // Khôi phục cout
//     expect += "ArrayMovingObject[count=4;capacity=10;FlyTeam[index=1;pos=(1,4);moving_rule=LLLL];FlyTeam[index=1;pos=(1,4);moving_rule=LLLL];GroundTeam[index=1;pos=(1,1);moving_rule=UUUU];DragonLord[index=1;pos=(0,0)]]\n0\n";
//     print_result(expect, oss.str());
// }

// // need more test
// vector<pair<string, function<void()>>> TestArrMovObj = {
//     {"ArrMovObj _ general constructor", sampleArrMovObj},
// };

#include <string>
#include <iostream>
using namespace std;

int main()
{
    // exe_test_pack(TestPosition);
    // exe_test_pack(TestMapElement);
    // exe_test_pack(TestMap);
    // exe_test_pack(TestWarrior);
    // exe_test_pack(TestDragonLord);
    // exe_test_pack(TestArrMovObj);

    // cout << "----- Sample Testcase 11 -----" << endl;
    // Configuration *config = new Configuration("sa_tc_01_config");
    // cout << config->getFlyTeam1InitHP() << endl;
    // cout << config->str() << endl;
    // delete config;

    cout << "----- Sample Testcase 12 -----" << endl;
    Configuration *config = new Configuration("sa_tc_02_config");
    cout << config->str() << endl;
    delete config;
    return 0;
}