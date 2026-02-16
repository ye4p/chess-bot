#include "move.h"
#include "board.h"
Move::Move(int f, int t, Piece piece, Piece captured, Piece promoted, MoveFlag flag)
    : from(f), to(t), piece(piece), captured(captured), promoted(promoted), flag(flag)
{
}
std::ostream &operator<<(std::ostream &os, const MoveFlag flag)
{
    switch (flag)
    {
    case MoveFlag::Normal:
        return os << "Normal";
    case MoveFlag::Capture:
        return os << "Capture";
    case MoveFlag::DoublePawn:
        return os << "DoublePawn";
    case MoveFlag::KingCastle:
        return os << "KingCastle";
    case MoveFlag::QueenCastle:
        return os << "QueenCastle";
    case MoveFlag::EnPassant:
        return os << "EnPassant";
    case MoveFlag::Promotion:
        return os << "Promotion";
    }
    return os << "Unknown";
}
std::ostream &operator<<(std::ostream &os, const Move &m)
{
    os << "Move from square " << m.from << " To square " << m.to << " 1)  Piece moved: " << m.piece << " 2) Piece captured: " << m.captured << " 3) Piece Promoted: " << m.promoted << ", Move's flag: " << m.flag;
    return os;
}