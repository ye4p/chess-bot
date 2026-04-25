#include "board.h"
#include <iostream>
#include <memory>

int main()
{
    // std::cout << "Running\n";
    auto board = std::make_unique<Board>();

    // Default position
    // board->startpos();

    // board->setFEN("rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1");
    //  board->setFEN("rnbqkbnr/1ppppppp/p7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 0 1");

    // board->setFEN("rnbqkbnr/pppppppp/8/8/8/N7/PPPPPPPP/R1BQKBNR b KQkq - 0 1");
    // board->setFEN("rnbqkbnr/1ppppppp/p7/8/8/N7/PPPPPPPP/R1BQKBNR w KQkq - 0 1");

    // board->setFEN("rnbqkbnr/pppppppp/8/8/8/1P6/P1PPPPPP/RNBQKBNR b KQkq - 0 1");
    // board->setFEN("rnbqkbnr/pppp1ppp/8/4p3/8/1P6/P1PPPPPP/RNBQKBNR w KQkq e6 0 1");
    board->setFEN("rnbqkbnr/pppp1ppp/8/4p3/8/BP6/P1PPPPPP/RN1QKBNR b KQkq - 0 1");

    board->displayBoard();
    // board->displayBB(board->bbs[6]);

    int res = board->perftDivide(1);
    std::cout << res;

    return 0;
}