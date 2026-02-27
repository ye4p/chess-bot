#pragma once
struct GameState
{
    int enPassantSquare;
    bool whiteCastleKingSide;
    bool whiteCastleQueenSide;
    bool blackCastleKingSide;
    bool blackCastleQueenSide;
    GameState();
    GameState(int enPassantSquare, bool whiteCastleKingSide, bool whiteCastleQueenSide, bool blackCastleKingSide, bool blackCastleQueenSide);
};