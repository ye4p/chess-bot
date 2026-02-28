#pragma once
struct GameState
{
    int enPassantSquare;
    bool whiteCastleKingSide;
    bool whiteCastleQueenSide;
    bool blackCastleKingSide;
    bool blackCastleQueenSide;
    int halfMoveCount = 0;
    int fullMoveCount = 0;
    Color sideToMove = Color::White;
    GameState();
    GameState(int enPassantSquare, bool whiteCastleKingSide, bool whiteCastleQueenSide, bool blackCastleKingSide, bool blackCastleQueenSide);
};