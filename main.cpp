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
    Piece wBishop = Piece(PieceType::Bishop, Color::White);
    board.FakeMove(wRook, 17);
    //  board.FakeMove(bRook, 18);
    board.generateRookMoves(17, moves);
    for (Move i : moves)
    {
        std::cout << i << "\n";
    }
    return 0;
    //For the actual move generation: minimax algorithm
    // Alpha beta prunning
};