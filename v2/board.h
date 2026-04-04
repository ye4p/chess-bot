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
    0x8a80104000800020ULL, 0x140002000100040ULL, 0x2801880a0017001ULL, 0x100081001000420ULL,
    0x200020010080420ULL, 0x3001c0002010008ULL, 0x8480008002000100ULL, 0x2080088004402900ULL,
    0x800098204000ULL, 0x2024401000200040ULL, 0x100802000801000ULL, 0x120800800801000ULL,
    0x208808088000400ULL, 0x2802200800400ULL, 0x2200800100020080ULL, 0x801000060821100ULL,
    0x80044006422000ULL, 0x100808020004000ULL, 0x12108a0010204200ULL, 0x140848010000802ULL,
    0x481828014002800ULL, 0x8094004002004100ULL, 0x4010040010010802ULL, 0x20008806104ULL,
    0x100400080208000ULL, 0x2040002120081000ULL, 0x21200680100081ULL, 0x20100080080080ULL,
    0x2000a00200410ULL, 0x20080800400ULL, 0x80088400100102ULL, 0x80004600042881ULL,
    0x4040008040800020ULL, 0x440003000200801ULL, 0x4200011004500ULL, 0x188020010100100ULL,
    0x14800401802800ULL, 0x2080040080800200ULL, 0x124080204001001ULL, 0x200046502000484ULL,
    0x480400080088020ULL, 0x1000422010034000ULL, 0x30200100110040ULL, 0x100021010009ULL,
    0x2002080100110004ULL, 0x202008004008002ULL, 0x20020004010100ULL, 0x2048440040820001ULL,
    0x101002200408200ULL, 0x40802000401080ULL, 0x4008142004410100ULL, 0x2060820c0120200ULL,
    0x1001004080100ULL, 0x20c020080040080ULL, 0x2935610830022400ULL, 0x44440041009200ULL,
    0x280001040802101ULL, 0x2100190040002085ULL, 0x80c0084100102001ULL, 0x4024081001000421ULL,
    0x20030a0244872ULL, 0x12001008414402ULL, 0x2006104900a0804ULL, 0x1004081002402ULL};

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