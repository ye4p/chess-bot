#include "gamestate.h"
#include "piece.h"
GameState::GameState()
{
    enPassantSquare = -1; // If pawn moved double step, then the enPassanSquare should be the final square from the pawn's double step
    whiteCastleKingSide = true;
    whiteCastleQueenSide = true;
    blackCastleKingSide = true;
    blackCastleQueenSide = true;
    halfMoveCount = 0;
    fullMoveCount = 0;
    sideToMove = Color::White;
}
GameState::GameState(bool b)
{
    enPassantSquare = -1; // If pawn moved double step, then the enPassanSquare should be the final square from the pawn's double step
    whiteCastleKingSide = b;
    whiteCastleQueenSide = b;
    blackCastleKingSide = b;
    blackCastleQueenSide = b;
    halfMoveCount = 0;
    fullMoveCount = 0;
    sideToMove = Color::White;
}
GameState::GameState(int enPassantSquare, bool whiteCastleKingSide, bool whiteCastleQueenSide, bool blackCastleKingSide, bool blackCastleQueenSide, int halfMoveCount, int fullMoveCount, Color sideToMove)
{
    this->enPassantSquare = enPassantSquare;
    this->whiteCastleKingSide = whiteCastleKingSide;
    this->whiteCastleQueenSide = whiteCastleQueenSide;
    this->blackCastleKingSide = blackCastleKingSide;
    this->blackCastleQueenSide = blackCastleQueenSide;
    this->halfMoveCount = halfMoveCount;
    this->fullMoveCount = fullMoveCount;
    this->sideToMove = sideToMove;
}
