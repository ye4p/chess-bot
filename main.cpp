#include "board.h"
#include "iostream"
#include <vector>
#include "move.h"
#include "iostream"
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
    std::vector<Move> moves;
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
    board.setFEN("rnbqkbnr/ppp1pppp/8/8/2pP4/4P3/PPP2PPP/RNBQK1NR b KQkq d3 0 1");

    board.displayBoard();
    // Move m=Move(board.codeToIndex("e8"),board.codeToIndex("d7"), )
    // board.generateGeometryMoves(board.codeToIndex("e8"), moves, {-9, -8, -7, -1, 1, 7, 8, 9});
    // for (Move m : moves)
    // {
    //     m.displayMove();
    //     std::cout << m << std::endl;
    // }
    // board.makeMove(moves[0]);
    // std::cout << "Is king in check: " << board.isKingInCheck(Color::Black);
    // uint64_t result = board.perft(2);
    // std::cout << result << std::endl;
    // std::cout << board.crossesBorderPawn(board.codeToIndex("g7"), board.codeToIndex("h6"));
    // std::cout << "is under attack: " << board.isSquareAttacked(board.codeToIndex("d3"));
    // board.generateGeometryMoves(board.codeToIndex("d8"), moves, {-9, -8, -7, -1, 1, 7, 8, 9});
    // for (Move m : moves)
    // {
    //     m.displayMove();
    // }
    board.perftDivide(2);
    //     std::cout << "Over";
    //     testMoveFilter(board);
    return 0;
};