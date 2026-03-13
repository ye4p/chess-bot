#include "board.h"
#include "iostream"
#include <vector>
#include "move.h"
#include "iostream"
#include "chrono"
#include <thread>

void testUnderAttackMethod(Board &board)
{
    // Piece wKing = Piece(PieceType::King, Color::White);
    // Piece bKnight = Piece(PieceType::Knight, Color::Black);
    // Piece bBishop = Piece(PieceType::Bishop, Color::Black);
    // Piece bQueen = Piece(PieceType::Queen, Color::Black);
    // Piece bRook = Piece(PieceType::Rook, Color::Black);
    // Piece bPawn = Piece(PieceType::Pawn, Color::Black);
    // Piece wPawn = Piece(PieceType::Pawn, Color::White);
    // Piece bRook = Piece(PieceType::Rook, Color::Black);
    // board.FakeMove(bRook, 18);
    // board.FakeMove(wKing, 17);
    //
    // Check for knight attacks
    //
    // board.FakeMove(bKnight, 35); //false
    // board.FakeMove(bKnight, 7); //false
    // board.FakeMove(bKnight, 23); //false
    // board.FakeMove(bKnight, 34); //true
    //

    //
    // DIAGONAL MOVES
    //
    // board.FakeMove(bQueen, 25);
    // board.FakeMove(bQueen, 27);
    // board.FakeMove(bQueen, 31);
    // board.FakeMove(bBishop, 27);
    // board.FakeMove(bBishop, 31);
    // board.FakeMove(bBishop, 25);
    //

    //
    // HORIZONTAL MOVES
    //
    // board.FakeMove(bRook, 14);
    // board.FakeMove(bRook, 24);
    // board.FakeMove(bRook, 35);

    //
    // PAWN MOVES
    //
    // board.FakeMove(bPawn, 9);
    // board.FakeMove(bPawn, 25);
    // board.FakeMove(bPawn, 16);
    // board.FakeMove(bPawn, 18);
    //

    //
    // En Passant
    //
    // board.FakeMove(wPawn, 16);
    // board.FakeMove(bPawn, 25);
    // Double move:
    // Move m = Move(16, 24, wPawn, board.getPiece(24), Piece());
    // board.makeMove(m);
    // std::cout << board.getEnPassantSquare() << "\n";
    // std::cout << "Is under attack: " << board.isSquareAttacked(24);
}
void testMoveFilter(Board &board)
{
    std::vector<Move> pseudo;
    std::vector<Move> legal;

    //
    // board.startingPosition(); WORKED with starting position correctly
    //

    Piece wKing = Piece(PieceType::King, Color::White);
    Piece bKnight = Piece(PieceType::Knight, Color::Black);
    Piece bBishop = Piece(PieceType::Bishop, Color::Black);
    Piece bQueen = Piece(PieceType::Queen, Color::Black);
    Piece bRook = Piece(PieceType::Rook, Color::Black);
    Piece bPawn = Piece(PieceType::Pawn, Color::Black);
    Piece wPawn = Piece(PieceType::Pawn, Color::White);

    board.setSideToMove(Color::White);
    board.FakeMove(wPawn, 32);
    board.FakeMove(bPawn, 49);
    board.FakeMove(wPawn, 7);

    board.generateMoves(pseudo);
    board.filterLegalMoves(pseudo, legal);
    for (Move &m : legal)
    {
        std::cout << m << "\n";
    }
}

int main()
{
    std::vector<Move> pseudo;
    std::vector<Move> legal;
    Board board;
    // For the actual move generation: minimax algorithm
    //  Alpha beta prunning

    // board.startingPosition();

    // board.setFEN("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    // board.setFEN("rnbqkbnr/p1pppppp/8/1p6/4P3/8/PPPP1PPP/RNBQKBNR w KQkq b6 0 1");
    //  Piece wPawn=Piece(PieceType::Pawn, Color::White);
    //  board.FakeMove(wPawn, 13);
    // board.setFEN("rnbqkbnr/pppppppp/8/8/8/2N5/PPPPPPPP/R1BQKBNR b KQkq - 0 1");
    // board.setFEN("rnbqkbnr/ppp1pppp/8/3p4/8/2N5/PPPPPPPP/R1BQKBNR w KQkq d6 0 1");
    // board.setFEN("rnbqkbnr/ppp1pppp/8/3N4/8/8/PPPPPPPP/R1BQKBNR b KQkq - 0 1");
    // board.setFEN("rnbqkbnr/pppppppp/8/8/8/6P1/PPPPPP1P/RNBQKBNR b KQkq - 0 1");
    // board.setFEN("rnbqkbnr/ppp1pppp/3p4/8/8/6P1/PPPPPP1P/RNBQKBNR w KQkq - 0 1");
    // board.setFEN("rnbqkbnr/ppp1pppp/3p4/8/8/6PB/PPPPPP1P/RNBQK1NR b KQkq - 0 1");

    // board.setFEN("rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq d3 0 1");
    // board.setFEN("rnbqkbnr/1ppppppp/p7/8/3P4/8/PPP1PPPP/RNBQKBNR w KQkq - 0 1");
    // board.setFEN("rnbqkbnr/1ppppppp/p6B/8/3P4/8/PPP1PPPP/RN1QKBNR b KQkq - 0 1");

    // board.setFEN("rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
    // board.setFEN("rnbqkbnr/pppp1ppp/4p3/8/8/4P3/PPPP1PPP/RNBQKBNR w KQkq - 0 1");
    // board.setFEN("rnbqkbnr/pppp1ppp/4p3/8/8/4P3/PPPPKPPP/RNBQ1BNR b - - 0 1");
    // board.setFEN("rnb1kbnr/pppp1ppp/4pq2/8/8/4P3/PPPPKPPP/RNBQ1BNR w KQkq - 0 1");

    // board.setFEN("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    // board.setFEN("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 1");
    // board.setFEN("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPPKPPP/RNBQ1BNR b KQkq - 0 1");
    // board.setFEN("rnbqkbnr/pp1ppppp/8/8/2p1P3/8/PPPPKPPP/RNBQ1BNR w KQkq - 0 1");

    // board.setFEN("rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
    // board.setFEN("rnbqkbnr/pppp1ppp/8/4p3/8/4P3/PPPP1PPP/RNBQKBNR w KQkq e6 0 1");
    // board.setFEN("rnbqkbnr/pppp1ppp/8/4p3/8/4P3/PPPPKPPP/RNBQ1BNR b KQkq - 0 1");
    // board.setFEN("rnbqkbnr/pppp1ppp/8/8/4p3/4P3/PPPPKPPP/RNBQ1BNR w KQkq - 0 1");
    // board.setFEN("rnbqkbnr/pppp1ppp/8/8/4p3/3KP3/PPPP1PPP/RNBQ1BNR b - - 0 1");

    // board.setFEN("rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
    // board.setFEN("rnbqkbnr/ppp1pppp/8/3p4/8/4P3/PPPP1PPP/RNBQKBNR w KQkq d6 0 1");
    // board.setFEN("rnbqkbnr/ppp1pppp/8/3p4/2B5/4P3/PPPP1PPP/RNBQK1NR b KQkq - 0 1");
    // board.setFEN("rnbqkbnr/ppp1pppp/8/8/2p5/4P3/PPPP1PPP/RNBQK1NR w KQkq - 0 1");
    // board.setFEN("rnbqkbnr/ppp1pppp/8/8/2pP4/4P3/PPP2PPP/RNBQK1NR b KQkq d3 0 1");

    // Move m = Move(60, 28, Piece(PieceType::Queen, Color::Black), board.getPiece(28), Piece(), MoveFlag::Capture);
    // board.makeMove(m);
    // board.displayBoard();
    // board.undoMove(m);
    // board.displayBoard();

    // // board.displayBoard();
    // // std::cout << "The d4: " << board.getPiece(board.codeToIndex("d4")) << "\n";
    // std::cout << "Starting generating moves...\n";
    // board.generateMoves(pseudo);
    // std::cout << "Generates psuedo legal moves:\n";
    // board.displayBoard();
    // board.filterLegalMoves(pseudo, legal);

    // board.setFEN("rnbqkbnr/ppp1pppp/8/8/2pP4/4P3/PPP2PPP/RNBQK1NR b KQkq d3 0 1");
    // for (Move m : legal)
    // {
    //     m.displayMove();
    // }
    // Move m=Move(board.codeToIndex("e8"),board.codeToIndex("d7"), )
    // board.generateGeometryMoves(board.codeToIndex("e8"), moves, {-9, -8, -7, -1, 1, 7, 8, 9});
    // for (Move m : moves)
    // {
    //     m.displayMove();
    //     std::cout << m << std::endl;
    // }
    // board.makeMove(moves[0]);
    // std::cout << "Is king in check: " << board.isKingInCheck(Color::Black);
    // uint64_t result = board.perft(6);
    // std::cout << result << std::endl;
    // std::cout << board.crossesBorderPawn(board.codeToIndex("g7"), board.codeToIndex("h6"));
    // std::cout << "is under attack: " << board.isSquareAttacked(board.codeToIndex("d3"));
    // board.generateGeometryMoves(board.codeToIndex("d8"), moves, {-9, -8, -7, -1, 1, 7, 8, 9});
    // for (Move m : moves)
    // {
    //     m.displayMove();
    // }

    //
    //      POSITION #2
    //
    board.setFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
    //
    // board.setFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N4Q/PPPBBPPP/R3K2R b KQkq - 0 1");
    //  a1b1
    //   board.setFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K1R1 b Qkq - ");
    //   board.displayCastlingRights();
    // board.setFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/1R2K2R b Kkq - 1 1");
    // board.setFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q2/PPPBBPpP/1R2K2R w Kkq - 0 2");
    //
    // board.setFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3KR2 b Qkq - ");
    //
    // board.setFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/1R2K2R b Kkq - ");
    // board.setFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q2/PPPBBPpP/1R2K2R w Kkq - ");
    // board.setFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P2P/2N2Q2/PPPBBPp1/1R2K2R b Kkq - ");
    // board.setFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R2K3R b kq - 1 1"); // 74416
    // board.setFEN("r3k3/p1ppqpb1/bn2pnp1/3PN3/1p2P2r/2N2Q1p/PPPBBPPP/R2K3R w q - 2 2"); // 1815

    board.displayBoard();
    // board.displayCastlingRights();

    // board.generateKingMoves(board.codeToIndex("e1"), pseudo);
    // std::cout <<"Outputing moves: \n";
    // for (Move m :pseudo) {
    //     m.displayMove();
    // }
    auto start = std::chrono::steady_clock::now();
    int nodes = board.perftDivide(4);
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "It took " << (double)duration.count() / 1000 << std::endl;
    std::cout << "NPS: " << (double)nodes / (duration.count() / 1000) << std::endl;
    // std::cout << "The d4: " << board.getPiece(board.codeToIndex("d4")) << "\n";
    // board.displayBoard();

    // testMoveFilter(board);
    return 0;
};