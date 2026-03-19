#pragma once
#include <array>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <cstdio>

static const std::unordered_map<char, int> pieceMap =
    {
        {'p', 0},
        {'n', 1},
        {'b', 2},
        {'r', 3},
        {'q', 4},
        {'k', 5}};

struct Undo
{
    uint8_t castlingRights;
    int8_t enPassantSquare;
    uint8_t halfMoveClock;
    uint64_t fullMoveClock;
    Undo(uint8_t castlingRights, int8_t enPassantSquare, uint8_t halfMoveClock, uint64_t fullMoveClock)
    {
    }
};
// clang-format off
enum squares : int{
    a8, b8, c8, d8, e8, f8, g8, h8, 
    a7, b7, c7, d7, e7, f7, g7, h7, 
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3, 
    a2, b2, c2, d2, e2, f2, g2, h2, 
    a1, b1, c1, d1, e1, f1, g1, h1,
};
// clang-format on
class Board
{

    // a1 = 0, h8 - 63;
    std::array<uint64_t, 12> bbs;
    // 0-5 - white pieces. 0 - pawns, 1 - knights, 2 - bishops, 3 - rooks, 4 - queen, 5 - king
    // 6-11 - black pieces. 6 - pawns, 7 - knights, 8 - bishops, 9 - rooks, 10 - queen, 11 - king
    std::array<uint64_t, 3> occupancies;
    // 0 - all white pieces, 1 - all black pieces, 2 - all pieces

    uint8_t castlingRights = 0b0000; // 1st - white king, 2nd - white queen, 3rd - black king, 4th - black queen

    int enPassantSquare = -1;

    int halfMoveClock = 0;
    int fullMoveClock = 1;

    bool whiteToMove = true;

    // BBitboards with attacks:
    uint64_t pawn_attacks[2][64];

public:
    Board();
    void inline setBit(uint64_t &bb, int square);
    void clearBit(uint64_t &bb, int square);
    bool isBitSet(uint64_t bb, int square);
    uint64_t get_lsb(uint64_t bb);
    uint64_t pop_lsb(uint64_t &bb);
    inline int popcount(uint64_t bb);
    void displayBoard(uint64_t bb);
    std::vector<std::string> splitString(std::string str, char delimiter);
    int codeToIndex(std::string code);
    std::string indexToCode(int index);
    std::string getFEN();
    void setFEN(std::string s);
    void updateOccupancies();
    uint64_t mask_pawn_attacks(int side, int square);
    void generateMoves();
    void generateKnightMoves();
    void generateKingMoves();
    void generateSlidingMoves();
};