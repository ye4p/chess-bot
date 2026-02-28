#include <board.h>
#include <movehistory.h>
MoveHistory::MoveHistory()
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
}
MoveHistory::MoveHistory(int from, int to, Piece moved, Piece captured, Piece promoted,
                         MoveFlag flag, int prevEnPassantSquare, bool prevWhiteCastleKingSide,
                         bool prevWhiteCastleQueenSide, bool prevBlackCastleKingSide,
                         bool prevBlackCastleQueenSide, int prevHalfMoveCount)
    : from(from), to(to), moved(moved), captured(captured), promoted(promoted), flag(flag),
      prevEnPassantSquare(prevEnPassantSquare), prevWhiteCastleKingSide(prevWhiteCastleKingSide),
      prevWhiteCastleQueenSide(prevWhiteCastleQueenSide), prevBlackCastleKingSide(prevBlackCastleKingSide),
      prevBlackCastleQueenSide(prevBlackCastleQueenSide), prevHalfMoveCount(prevHalfMoveCount)
{
}