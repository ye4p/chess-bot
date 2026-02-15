#include "piece.h"
#include <array>
class Board
{
    std::array<Piece, 64> board;

public:
    Board();
    void assignDefaultRow(int row, Color color);
    void assignPawns(int row, Color color);
    void startingPosition();
    void displayBoard();
};