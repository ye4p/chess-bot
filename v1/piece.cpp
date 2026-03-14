#include "piece.h"
#include "iostream"

std::ostream &operator<<(std::ostream &os, const PieceType type)
{
    switch (type)
    {
    case PieceType::None:
        return os << "None";
    case PieceType::Pawn:
        return os << "Pawn";
    case PieceType::Knight:
        return os << "Knight";
    case PieceType::Bishop:
        return os << "Bishop";
    case PieceType::Rook:
        return os << "Rook";
    case PieceType::Queen:
        return os << "Queen";
    case PieceType::King:
        return os << "King";
    }
    return os << "Unknown";
}

std::ostream &operator<<(std::ostream &os, const Color color)
{
    switch (color)
    {
    case Color::None:
        return os << "None";
    case Color::White:
        return os << "White";
    case Color::Black:
        return os << "Black";
    }
    return os << "Unknown";
}

std::ostream &operator<<(std::ostream &os, const Piece &obj)
{
    os << "Piece of type=" << obj.type << ", color=" << obj.color << ".";
    return os;
}