#include "board.h"
#include <iostream>
#include <memory>
#include "chrono"
#include <thread>

int main()
{
    // std::cout << "Running\n";
    auto board = std::make_unique<Board>();

    // board->run_perft(3, 7);

    board->pos(2);

    std::cout << "Position evaluation is: " << board->evaluate() << "\n";

    return 0;
}