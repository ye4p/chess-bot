#include "board.h"
#include <iostream>
#include <memory>

int main()
{
    // std::cout << "Running\n";
    auto board = std::make_unique<Board>();

    // Default position
    std::cout << "Setting fen\n";
    board->startpos();
    std::cout << "Set fen\n";
    board->displayBoard();

    board->generateMoves();
    board->displayMoves();
    // int res = board->perft(2);
    // std::cout << res;

    return 0;
}