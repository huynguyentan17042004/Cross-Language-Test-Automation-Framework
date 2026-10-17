#include "dragon.h"

#include <iostream>
#include <string>

namespace
{
void printUsage()
{
    std::cerr << "Usage: scenario_harness <config-str|movement|encounter|stop> <config-path>\n";
}
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printUsage();
        return 64;
    }

    const std::string scenario = argv[1];
    const std::string config_path = argv[2];

    if (scenario == "config-str")
    {
        Configuration config(config_path);
        std::cout << config.str() << '\n';
        return 0;
    }

    DragonWarriorsProgram program(config_path);

    if (scenario == "movement")
    {
        std::cout << "before=" << program.flyteam1->getCurrentPosition().str() << '\n';
        program.flyteam1->move();
        std::cout << "after=" << program.flyteam1->getCurrentPosition().str() << '\n';
        return 0;
    }

    if (scenario == "encounter")
    {
        program.flyteam1->fight(program.dragonlord);
        std::cout << "dragonlord_defeated="
                  << (program.dragonlord->defeated ? "true" : "false") << '\n';
        return 0;
    }

    if (scenario == "stop")
    {
        std::cout << "stop=" << (program.isStop() ? "true" : "false") << '\n';
        return 0;
    }

    printUsage();
    return 64;
}
