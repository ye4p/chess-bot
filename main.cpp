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
    //board.startingPosition();
    board.setFEN("rnbqkbnr/ppppp1pp/8/5p2/4P3/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 1");
    // Piece wPawn=Piece(PieceType::Pawn, Color::White);
    // board.FakeMove(wPawn, 13);
    board.displayBoard();
    uint64_t result=board.perft(2);    
    std::cout << result;

    board.perftDivide(2);
   // std::cout << "Over";
    //testMoveFilter(board);
    return 0;
}; 