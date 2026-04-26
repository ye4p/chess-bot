#include "board.h"
#include <iostream>
#include <memory>
#include "chrono"
#include <thread>

int main()
{
    // std::cout << "Running\n";
    auto board = std::make_unique<Board>();

    // Default position
    board->startpos();

    // board->setFEN("rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1");
    //  board->setFEN("rnbqkbnr/1ppppppp/p7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 0 1");

    // board->setFEN("rnbqkbnr/pppppppp/8/8/8/N7/PPPPPPPP/R1BQKBNR b KQkq - 0 1");
    // board->setFEN("rnbqkbnr/1ppppppp/p7/8/8/N7/PPPPPPPP/R1BQKBNR w KQkq - 0 1");

    // board->setFEN("rnbqkbnr/pppppppp/8/8/8/1P6/P1PPPPPP/RNBQKBNR b KQkq - 0 1");
    // board->setFEN("rnbqkbnr/pppp1ppp/8/4p3/8/1P6/P1PPPPPP/RNBQKBNR w KQkq e6 0 1");
    // board->setFEN("rnbqkbnr/pppp1ppp/8/4p3/8/BP6/P1PPPPPP/RN1QKBNR b KQkq - 0 1");

    // board->setFEN("rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq a3 0 1");
    // board->setFEN("rnbqkbnr/p1pppppp/8/1p6/P7/8/1PPPPPPP/RNBQKBNR w KQkq b6 0 1");
    // board->setFEN("rnbqkbnr/p1pppppp/8/1P6/8/8/1PPPPPPP/RNBQKBNR b KQkq - 0 1");
    // board->setFEN("rnbqkbnr/2pppppp/8/pP6/8/8/1PPPPPPP/RNBQKBNR w KQkq a6 0 1");

    // board->setFEN("rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq d3 0 1");
    //  board->setFEN("rnbqkbnr/p1pppppp/8/1p6/3P4/8/PPP1PPPP/RNBQKBNR w KQkq b6 0 1");
    //  board->setFEN("rnbqkbnr/p1pppppp/8/1p6/3P4/8/PPPKPPPP/RNBQ1BNR b kq - 0 1");
    //  board->setFEN("rnbqkbnr/p1pppppp/8/8/1p1P4/8/PPPKPPPP/RNBQ1BNR w kq - 0 1");
    //  board->setFEN("rnbqkbnr/p1pppppp/8/8/1p1P4/2K5/PPP1PPPP/RNBQ1BNR b kq - 0 1");

    board->setFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");

    board->displayBoard();

    // std::cout << board->isKingAttacked(1);

    auto start = std::chrono::steady_clock::now();
    int nodes = board->perftDivide(1);
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "It took " << (double)duration.count() / 1000 << std::endl;
    std::cout << "NPS: " << (double)nodes / ((double)duration.count() / 1000) << std::endl;

    return 0;
}