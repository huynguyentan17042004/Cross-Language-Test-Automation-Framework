#include "dragon.h"

#include <iostream>
#include <string>

namespace {
int as_int(const char *value) { return std::stoi(value); }

bool needs(int argc, int expected) {
    if (argc == expected) {
        return true;
    }
    std::cerr << "invalid argument count" << std::endl;
    return false;
}
} // namespace

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "missing scenario" << std::endl;
        return 64;
    }

    const std::string scenario = argv[1];

    if (scenario == "construct-default") {
        if (!needs(argc, 2)) return 64;
        const Position position;
        std::cout << position.str();
        return 0;
    }

    if (scenario == "construct-values") {
        if (!needs(argc, 4)) return 64;
        const Position position(as_int(argv[2]), as_int(argv[3]));
        std::cout << position.str();
        return 0;
    }

    if (scenario == "construct-string") {
        if (!needs(argc, 3)) return 64;
        const Position position(argv[2]);
        std::cout << position.str();
        return 0;
    }

    if (scenario == "set-row") {
        if (!needs(argc, 5)) return 64;
        Position position(as_int(argv[2]), as_int(argv[3]));
        position.setRow(as_int(argv[4]));
        std::cout << position.str();
        return 0;
    }

    if (scenario == "set-col") {
        if (!needs(argc, 5)) return 64;
        Position position(as_int(argv[2]), as_int(argv[3]));
        position.setCol(as_int(argv[4]));
        std::cout << position.str();
        return 0;
    }

    if (scenario == "is-equal") {
        if (!needs(argc, 6)) return 64;
        const Position position(as_int(argv[2]), as_int(argv[3]));
        std::cout << std::boolalpha
                  << position.isEqual(as_int(argv[4]), as_int(argv[5]));
        return 0;
    }

    std::cerr << "unknown scenario: " << scenario << std::endl;
    return 64;
}

