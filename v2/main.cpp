#include "board.h"
#include <iostream>
#include <memory>

int main()
{
    // std::cout << "Running\n";
    auto board = std::make_unique<Board>();

    // Default position
    board->startpos();

    board->displayBoard();

    int res = board->perft(3);
    std::cout << res;

    return 0;
}