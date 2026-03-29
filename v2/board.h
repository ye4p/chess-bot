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

static const uint64_t BISHOP_MAGICS[64] = {
    0x0002020202020200ULL, 0x0002020202020000ULL, 0x0004010202000000ULL,
    0x0004040080000000ULL, 0x0001104000000000ULL, 0x0000821040000000ULL,
    0x0000410410400000ULL, 0x0000104104104000ULL, 0x0000040404040400ULL,
    0x0000020202020200ULL, 0x0000040102020000ULL, 0x0000040400800000ULL,
    0x0000011040000000ULL, 0x0000008210400000ULL, 0x0000004104104000ULL,
    0x0000002082082000ULL, 0x0004000808080800ULL, 0x0002000404040400ULL,
    0x0001000202020200ULL, 0x0000800802004000ULL, 0x0000800400A00000ULL,
    0x0000200100884000ULL, 0x0000400082082000ULL, 0x0000200041041000ULL,
    0x0002080010101000ULL, 0x0001040008080800ULL, 0x0000208004010400ULL,
    0x0000404004010200ULL, 0x0000840000802000ULL, 0x0000404002011000ULL,
    0x0000808001041000ULL, 0x0000404000820800ULL, 0x0001041000202000ULL,
    0x0000820800101000ULL, 0x0000104400080800ULL, 0x0000020080080080ULL,
    0x0000404040040100ULL, 0x0000808100020100ULL, 0x0001010100020800ULL,
    0x0000808080010400ULL, 0x0000820820004000ULL, 0x0000410410002000ULL,
    0x0000082088001000ULL, 0x0000002011000800ULL, 0x0000080100400400ULL,
    0x0001010101000200ULL, 0x0002020202000400ULL, 0x0001010101000200ULL,
    0x0000410410400000ULL, 0x0000208208200000ULL, 0x0000002084100000ULL,
    0x0000000020880000ULL, 0x0000001002020000ULL, 0x0000040408020000ULL,
    0x0004040404040000ULL, 0x0002020202020000ULL, 0x0000104104104000ULL,
    0x0000002082082000ULL, 0x0000000020841000ULL, 0x0000000000208800ULL,
    0x0000000010020200ULL, 0x0000000404080200ULL, 0x0000040404040400ULL,
    0x0002020202020200ULL};
static const uint64_t ROOK_MAGICS[64] = {
    0x80102080004000ULL, 0x280200082400014ULL, 0x82002040800a0010ULL, 0x880080080100184ULL,
    0x200040820100200ULL, 0x480020080440001ULL, 0x4680008002000100ULL, 0x2080104080002100ULL,
    0x28080032c814000ULL, 0x2413002100814000ULL, 0x8090802004801000ULL, 0xc801000830800ULL,
    0x8008808008008400ULL, 0x602800400020080ULL, 0x840008020110d4ULL, 0x1c0a002281064412ULL,
    0x120208000904000ULL, 0x8110004040002004ULL, 0x2100808020001000ULL, 0x808008001000ULL,
    0x829010008001004ULL, 0x1104808002000400ULL, 0x2011040002100108ULL, 0x400002000850811cULL,
    0x40044480008020ULL, 0x340014140201000ULL, 0x401100200108ULL, 0xc0104202000a2010ULL,
    0x416000a00042090ULL, 0xd840100801400420ULL, 0x80400021001ULL, 0x11a00004084ULL,
    0x80004000402004ULL, 0x20004000403000ULL, 0x400a401202002080ULL, 0x100100080800804ULL,
    0x80080800402ULL, 0x4006001004040020ULL, 0x40280244002110ULL, 0x400800040800100ULL,
    0x220800040008028ULL, 0x8040500020024000ULL, 0x410020090012ULL, 0xc400081200220040ULL,
    0x3040050008010010ULL, 0xe92000804020010ULL, 0x800020001008080ULL, 0x2000010040a20004ULL,
    0x8000802900420a00ULL, 0x403040100c200040ULL, 0x20080040100040ULL, 0x9008008010010880ULL,
    0x8018040080080080ULL, 0xc02003144082200ULL, 0x40800100020080ULL, 0x2208064010200ULL,
    0x1800120900204082ULL, 0xc8631400081002dULL, 0x82000104101ULL, 0xab000810000621ULL,
    0x2012004100802ULL, 0x80010024000e4809ULL, 0x5030100241080084ULL, 0x1000041208e01ULL};

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

    // first 6 bits - from, next 6 - to, 2 for type(0-normal, 1-promotion, 2- en passant, 3 -castling), and 2 for promoted piece type(0 - knight, 1-bishop, 2-rook, 3-queen)
    uint16_t data;
    Move();
    Move(int from, int to, int type = 0, int promotion = 0);
    int from() const;
    int to() const;
    int type() const;
    int promotion() const;

    bool isPromotion() const;
    bool isEnPassant() const;
    bool isCastling() const;
};
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

    uint8_t castlingRights = 0b0000; // 1st - white king, 2nd - white queen, 3rd - black king, 4th - black queen

    int enPassantSquare = -1;

    int halfMoveClock = 0;
    int fullMoveClock = 1;

    bool whiteToMove = true;

    Undo undo;

    // Bitboard masks with attacks:
    uint64_t pawn_masks[2][64];
    uint64_t knight_masks[64];
    uint64_t king_masks[64];
    uint64_t bishop_masks[64];
    uint64_t rook_masks[64];

    uint64_t bishop_relevant_bits[64];
    uint64_t rook_relevant_bits[64];

    //  Magic bbs
    uint64_t bishop_attacks[64][512];
    uint64_t rook_attacks[64][4096];

    Board();
    void inline setBit(uint64_t &bb, int square);
    void clearBit(uint64_t &bb, int square);
    bool isBitSet(uint64_t bb, int square);
    int pieceOn(int square);

    uint64_t get_lsb_bb(uint64_t bb);
    uint64_t pop_lsb_bb(uint64_t &bb);
    int get_lsb_index(uint64_t bb);
    inline int popcount(uint64_t bb);

    void displayBoard(uint64_t bb);

    std::vector<std::string> splitString(std::string str, char delimiter);
    int codeToIndex(std::string code);
    std::string indexToCode(int index);
    std::string getFEN();
    void setFEN(std::string s);

    void updateOccupancies();

    uint64_t mask_pawn_attacks(int side, int square);
    uint64_t mask_knight_attacks(int square);
    uint64_t mask_king_attacks(int square);
    uint64_t mask_bishop_attacks(int square);
    uint64_t mask_rook_attacks(int square);

    uint64_t get_bishop_attacks(int square, uint64_t board_occupancy);
    uint64_t set_occupancy(int index, int bits, uint64_t mask);
    uint64_t bishop_attacks_from_occupancy(int square, uint64_t blockers);
    uint64_t rook_attacks_from_occupancy(int square, uint64_t blockers);

    void generateMoves();
    void generateKnightMoves();
    void generateKingMoves();
    void generatePawnMoves();
    void generateBishopMoves();
    void generateRookMoves();
    void generateSlidingMoves();

    void makeMove(Move m, Undo &undo);
    void unmakeMove(Move m, Undo &undo);
};