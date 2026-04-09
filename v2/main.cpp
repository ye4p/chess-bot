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

    board->generateMoves();
    board->displayMoves();
    int res = board->perft(1);
    std::cout << res;

    return 0;
}