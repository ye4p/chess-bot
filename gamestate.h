#pragma once
#include "piece.h"
struct GameState
{
    int enPassantSquare;
    bool whiteCastleKingSide;
    bool whiteCastleQueenSide;
    bool blackCastleKingSide;
    bool blackCastleQueenSide;
    int halfMoveCount;
    int fullMoveCount;
    Color sideToMove;
    GameState();
    GameState(bool b);
    GameState(int enPassantSquare, bool whiteCastleKingSide, bool whiteCastleQueenSide, bool blackCastleKingSide, bool blackCastleQueenSide, int halfMoveCount, int fullMoveCount, Color sideToMove);
};