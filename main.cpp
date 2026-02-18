#include "board.h"
#include "iostream"
#include <vector>
#include "move.h"
#include "iostream"
int main()
{
    std::vector<Move> moves;
    Board board;
    board.startingPosition();
    // board.displayBoard();
    Piece wRook = Piece(PieceType::Rook, Color::White);
    Piece bRook = Piece(PieceType::Rook, Color::Black);
    // board.FakeMove(wRook, 16);
    //  board.FakeMove(bRook, 18);
    board.generateBishopMoves(2, moves);
    for (Move i : moves)
    {
        std::cout << i << "\n";
    }
    return 0;
};