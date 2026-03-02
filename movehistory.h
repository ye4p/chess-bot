#pragma once
#include "piece.h"
#include "move.h"
struct MoveHistory
{
    int from;
    int to;
    Piece moved;
    Piece captured;
    Piece promoted;
    MoveFlag flag;
    int prevEnPassantSquare;
    bool prevWhiteCastleKingSide;
    bool prevWhiteCastleQueenSide;
    bool prevBlackCastleKingSide;
    bool prevBlackCastleQueenSide;
    int prevHalfMoveCount;
    MoveHistory();
    MoveHistory(int from, int to, Piece moved, Piece captured, Piece promoted,
                MoveFlag flag, int prevEnPassantSquare, bool prevWhiteCastleKingSide,
                bool prevWhiteCastleQueenSide, bool prevBlackCastleKingSide,
                bool prevBlackCastleQueenSide, int prevHalfMoveCount);
};