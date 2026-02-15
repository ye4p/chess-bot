#include "piece.h"
#include "array"
#include "iostream"
class Board
{
    std::array<Piece, 64> board;

public:
    Board()
    {
        Piece empty;
        for (int i = 0; i < board.size(); i++)
        {
            board[i] = empty;
        }
    }
    void assignDefaultRow(int row, Color color)
    {
        int start = (row - 1) * 8;
        board[start] = Piece(PieceType::Rook, color);
        board[start + 1] = Piece(PieceType::Knight, color);
        board[start + 2] = Piece(PieceType::Bishop, color);
        board[start + 3] = Piece(PieceType::King, color);
        board[start + 4] = Piece(PieceType::Queen, color);
        board[start + 5] = Piece(PieceType::Bishop, color);
        board[start + 6] = Piece(PieceType::Knight, color);
        board[start + 7] = Piece(PieceType::Rook, color);
    }
    void assignPawns(int row, Color color)
    {
        int start = (row - 1) * 8;
        for (int i = start; i < start + 8; i++)
        {
            board[i] = Piece(PieceType::Pawn, color);
        }
    }
    void startingPosition()
    {
        assignDefaultRow(1, Color::White);
        assignPawns(2, Color::White);
        assignPawns(7, Color::Black);
        assignDefaultRow(8, Color::Black);
    }
};