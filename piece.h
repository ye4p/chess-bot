#pragma once // If "piece.h" included in multiple files, can cause error, and this line prevents that
#include <iostream>
enum class PieceType
{
    None,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

std::ostream &operator<<(std::ostream &os, PieceType type);

enum class Color
{
    None,
    White,
    Black
};

std::ostream &operator<<(std::ostream &os, Color color);

struct Piece
{
    Color color;
    PieceType type;
    Piece(PieceType t = PieceType::None, Color c = Color::None) // If nothing passed there, the default values is taken from the class(none)
        : type(t),
          color(c)
    {
    }
};

std::ostream &operator<<(std::ostream &os, const Piece &obj);
