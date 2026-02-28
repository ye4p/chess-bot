#include "board.h"
#include "iostream"
#include <vector>
#include "move.h"
#include "iostream"
void TestUnderAttackMethod(Board &board)
{
    Piece wKing = Piece(PieceType::King, Color::White);
    Piece bKnight = Piece(PieceType::Knight, Color::Black);
    Piece bBishop = Piece(PieceType::Bishop, Color::Black);
    Piece bQueen = Piece(PieceType::Queen, Color::Black);
    Piece bRook = Piece(PieceType::Rook, Color::Black);
    Piece bPawn = Piece(PieceType::Pawn, Color::Black);
    Piece wPawn = Piece(PieceType::Pawn, Color::White);
    // Piece bRook = Piece(PieceType::Rook, Color::Black);
    // board.FakeMove(bRook, 18);
    board.FakeMove(wKing, 17);
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
    board.FakeMove(wPawn, 16);
    board.FakeMove(bPawn, 25);
    // Double move:
    Move m=Move(16, 24, wPawn, board.getPiece(24), Piece());
    board.makeMove(m);
    std::cout << board.getEnPassantSquare() <<"\n";
    std::cout << "Is under attack: " << board.isSquareAttacked(24);
}
int main()
{
    std::vector<Move> moves;
    Color sideToMove = Color::White;
    Board board;
    TestUnderAttackMethod(board);
    // For the actual move generation: minimax algorithm
    //  Alpha beta prunning
};