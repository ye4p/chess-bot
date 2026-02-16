#pragma once // If "piece.h" included in multiple files, can cause error, and this line prevents that
#include <iostream>
enum class PieceType // Enum makes it enumarable, so basically just integers are stored
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
    PieceType type;
    Color color;
    Piece(PieceType t = PieceType::None, Color c = Color::None) // If nothing passed there, the default values is taken from the class(none)
        : type(t),
          color(c)
    {
    }
};

std::ostream &operator<<(std::ostream &os, const Piece &obj);
