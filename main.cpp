#include "board.h"
#include "iostream"
#include <vector>
#include "move.h"
#include "iostream"
void TestUnderAttackMethod(Board &board)
{
    Piece wKing = Piece(PieceType::King, Color::White);
    Piece bKnight = Piece(PieceType::Knight, Color::Black);
    // Piece bRook = Piece(PieceType::Rook, Color::Black);
    // board.FakeMove(bRook, 18);
    board.FakeMove(wKing, 17);
    board.FakeMove(bKnight, 35);
    board.FakeMove(bKnight, 7);
    // board.FakeMove(bKnight, 23);
    std::cout << "Is under attack: " << board.isSquareAttacked(17);
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