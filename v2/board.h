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

static const uint64_t BISHOP_MAGICS[64];

static const uint64_t ROOK_MAGICS[64];

struct Undo
{
    uint8_t castlingRights;
    uint8_t capturedPiece;
    int8_t enPassantSquare;
    uint8_t halfMoveClock;
    uint16_t fullMoveClock;
    Undo();
    Undo(uint8_t castlingRights, uint8_t capturedPiece, int8_t enPassantSquare, uint8_t halfMoveClock, uint16_t fullMoveClock);
};
struct Move
{

    //  first 6 bits - from, next 6 - to,
    //  2 for type(0-normal, 1-promotion, 2- en passant, 3 -castling),
    //  and 2 for promoted piece type(0 - knight, 1-bishop, 2-rook, 3-queen)

    // NEW WAY - 4 bits for variations:
    // 0000 - quiet
    // 0001 - pawn double push
    // 0010 - king castle
    // 0011 - queen castle
    // 0100 - capture
    // 0101 - en passant
    // 1000 - knight promotion
    // 1001 - bishop promotion
    // 1010 - rook promotion
    // 1011 - queen promotion
    // 1100 - knight promotion + capture
    // 1101 - bishop promotion + capture
    // 1110 - rook promotion + capture
    // 1111 - queen promotion + capture

    uint16_t data;
    Move();
    Move(int from, int to, int status);
    int from() const;
    int to() const;
    int status() const;

    bool isPromotion() const;
    bool isEnPassant() const;
    bool isCastling() const;
    bool isCapture() const;
};

std::ostream &operator<<(std::ostream &os, const Move &obj);

// clang-format off
enum squares : int{
    a1, b1, c1, d1, e1, f1 ,g1, h1,
    a2, b2, c2, d2, e2, f2 ,g2, h2,
    a3, b3, c3, d3, e3, f3 ,g3, h3,
    a4, b4, c4, d4, e4, f4 ,g4, h4,
    a5, b5, c5, d5, e5, f5 ,g5, h5,
    a6, b6, c6, d6, e6, f6 ,g6, h6,
    a7, b7, c7, d7, e7, f7 ,g7, h7,
    a8, b8, c8, d8, e8, f8 ,g8, h8
    // a8, b8, c8, d8, e8, f8, g8, h8, 
    // a7, b7, c7, d7, e7, f7, g7, h7, 
    // a6, b6, c6, d6, e6, f6, g6, h6,
    // a5, b5, c5, d5, e5, f5, g5, h5,
    // a4, b4, c4, d4, e4, f4, g4, h4,
    // a3, b3, c3, d3, e3, f3, g3, h3, 
    // a2, b2, c2, d2, e2, f2, g2, h2, 
    // a1, b1, c1, d1, e1, f1, g1, h1,
};
// clang-format on

namespace masks
{
    constexpr uint64_t notAfile = 9259542123273814144;
}

class Board
{
public:
    // a1 = 0, h8 - 63;
    std::array<uint64_t, 12> bbs;
    // 0-5 - white pieces. 0 - pawns, 1 - knights, 2 - bishops, 3 - rooks, 4 - queen, 5 - king
    // 6-11 - black pieces. 6 - pawns, 7 - knights, 8 - bishops, 9 - rooks, 10 - queen, 11 - king
    std::array<uint64_t, 3> occupancies;
    // 0 - all white pieces, 1 - all black pieces, 2 - all pieces

    std::array<int, 64> mailbox;

    std::array<Move, 256> moveList;
    std::array<Move, 256> legalList;

    uint8_t castlingRights = 0b0000; // 1st - white king, 2nd - white queen, 3rd - black king, 4th - black queen

    int enPassantSquare = -1;

    int halfMoveClock = 0;
    int fullMoveClock = 1;

    int sideToMove = 0;
    // bool whiteToMove = true;

    Undo undo;

    // Bitboard masks with attacks:
    static uint64_t pawn_masks[2][64];
    static uint64_t pawn_push[2][64];
    static uint64_t pawn_double_push[2][64];
    static uint64_t knight_masks[64];
    static uint64_t king_masks[64];
    static uint64_t bishop_masks[64];
    static uint64_t rook_masks[64];

    static uint64_t bishop_relevant_bits[64];
    static uint64_t rook_relevant_bits[64];

    //  Magic bbs
    static uint64_t bishop_attacks[64][512];
    static uint64_t rook_attacks[64][4096];

    Board();
    void inline setBit(uint64_t &bb, int square);
    void clearBit(uint64_t &bb, int square);
    bool isBitSet(uint64_t bb, int square);
    int pieceOn(int square);

    uint64_t get_lsb_bb(uint64_t bb);
    uint64_t pop_lsb_bb(uint64_t &bb);
    int get_lsb_index(uint64_t bb);
    inline int popcount(uint64_t bb);

    void displayBoard();
    void displayBB(uint64_t bb);
    void displayMoves();

    std::vector<std::string> splitString(std::string str, char delimiter);
    int codeToIndex(std::string code);
    std::string indexToCode(int index);
    std::string getFEN();
    void setFEN(std::string s);

    void updateOccupancies();

    // Magic generation
    uint64_t rand64();
    uint64_t sparse_rand();
    uint64_t find_magic(int sq, bool bishop);

    //  Initialization
    uint64_t mask_pawn_attacks(int side, int square);
    uint64_t mask_pawn_push(int side, int square, int push);
    uint64_t mask_knight_attacks(int square);
    uint64_t mask_king_attacks(int square);
    uint64_t mask_bishop_attacks(int square);
    uint64_t mask_rook_attacks(int square);

    uint64_t set_occupancy(int index, int bits, uint64_t mask);
    uint64_t bishop_attacks_from_occupancy(int square, uint64_t blockers);
    uint64_t rook_attacks_from_occupancy(int square, uint64_t blockers);

    uint64_t get_bishop_attacks(int square, uint64_t board_occupancy);
    uint64_t get_rook_attacks(int square, uint64_t board_occupancy);

    void generateMoves();
    void generateKnightMoves();
    void generateKingMoves();
    void generatePawnMoves();
    void generateBishopMoves();
    void generateRookMoves();

    void makeMove(Move m);
    void undoMove(Move m);

    bool isSquareAttacked(int square, int by);
    bool isKingAttacked(int by);

    //  Initializing
    void startpos();

    //  Testing
    int perft(int depth);
    int perftDivide(int depth);
};