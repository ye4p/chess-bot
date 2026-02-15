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

enum class Color
{
    None,
    White,
    Black
};
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