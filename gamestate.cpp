#include "gamestate.h"
GameState::GameState()
{
    int enPassantSquare = -1; // If pawn moved double step, then the enPassanSquare should be the final square from the pawn's double step
    bool whiteCastleKingSide = true;
    bool whiteCastleQueenSide = true;
    bool blackCastleKingSide = true;
    bool blackCastleQueenSide = true;
}
GameState::GameState(int enPassantSquare, bool whiteCastleKingSide, bool whiteCastleQueenSide, bool blackCastleKingSide, bool blackCastleQueenSide)
{
    this->enPassantSquare = enPassantSquare;
    this->whiteCastleKingSide = whiteCastleKingSide;
    this->whiteCastleQueenSide = whiteCastleQueenSide;
    this->blackCastleKingSide = blackCastleKingSide;
    this->blackCastleQueenSide = blackCastleQueenSide;
}