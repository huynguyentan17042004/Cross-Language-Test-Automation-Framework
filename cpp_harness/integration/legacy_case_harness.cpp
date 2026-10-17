#define main bundled_legacy_main
#include "main.cpp"
#undef main

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: legacy_case_harness <19|20|21|22>\n";
        return 64;
    }

    const std::string case_id = argv[1];
    if (case_id == "19")
        TestDragonWar::satc_19();
    else if (case_id == "20")
        TestDragonWar::satc_20();
    else if (case_id == "21")
        TestDragonWar::satc_21();
    else if (case_id == "22")
        TestDragonWar::satc_22();
    else
    {
        std::cerr << "Unknown legacy case: " << case_id << '\n';
        return 64;
    }

    return 0;
}
