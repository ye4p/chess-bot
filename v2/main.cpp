#include "board.h"

int main()
{

    uint64_t notAfile = 9259542123273814144;
    Board board;
    board.generatePawnMoves();
    board.generateKnightMoves();
    board.generateKingMoves();
    board.generateBishopMoves();
    board.generateRookMoves();
    

    return 0;
}