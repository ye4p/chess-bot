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

    //  Kiwipete, max up to 5
    board->setFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");

    //  board->setFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPB1PPP/R2BK2R b KQkq - 0 1");
    //  board->setFEN("r3k2r/p1ppqpb1/1n2pnp1/3PN3/1p2P3/2N2Q1p/PPPB1PPP/R2BKb1R w KQkq - 0 1");

    // board->setFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2NB1Q1p/PPPB1PPP/R3K2R b KQkq - 0 1");
    // board->setFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2NB1Q2/PPPB1PpP/R3K2R w KQkq - 0 1");
    // board->setFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/P1NB1Q2/1PPB1PpP/R3K2R b KQkq - 0 1");

    // board->setFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/P1N2Q1p/1PPBBPPP/R3K2R b KQkq - 0 1");
    // board->setFEN("r3k2r/p1p1qpb1/bn1ppnp1/3PN3/1p2P3/P1N2Q1p/1PPBBPPP/R3K2R w KQkq - 0 1");
    // board->setFEN("r3k2r/p1p1qpb1/bn1ppnp1/1B1PN3/1p2P3/P1N2Q1p/1PPB1PPP/R3K2R b KQkq - 0 1");

    // board->setFEN("r3k2r/p1ppqpb1/bn2pnN1/3P4/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1");
    // board->setFEN("r3k2r/p1pp1pb1/bn1qpnN1/3P4/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    // board->setFEN("r3k2N/p1pp1pb1/bn1qpn2/3P4/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQq - 0 1");

    //board->setFEN("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 ");

    board->displayBoard();

    // std::cout << board->isKingAttacked(1);

    auto start = std::chrono::steady_clock::now();
    int nodes = board->perftDivide(6);
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "It took " << (double)duration.count() / 1000 << std::endl;
    std::cout << "NPS: " << (double)nodes / ((double)duration.count() / 1000) << std::endl;

    return 0;
}