#pragma once
#include "piece.h"

enum class MoveFlag
{
    Normal,
    Capture,
    DoublePawn,
    KingCastle,
    QueenCastle,
    EnPassant,
    Promotion
};

struct Move
{
    int from;
    int to;
    Piece piece;
    Piece captured;
    Piece promoted;
    MoveFlag flag;
    Move(int f, int t, Piece piece, Piece captured = Piece(), Piece promoted = Piece(), MoveFlag flag = MoveFlag::Normal);
};
std::ostream &operator<<(std::ostream &os, const Move &obj);