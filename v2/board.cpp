#include "board.h"
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

Undo::Undo()
{
    castlingRights = 0b1111;
    capturedPiece = -1;
    enPassantSquare = -1;
    halfMoveClock = 0;
    fullMoveClock = 1;
}

Undo::Undo(uint8_t castlingRights, uint8_t capturedPiece, int8_t enPassantSquare, uint8_t halfMoveClock, uint16_t fullMoveClock)
{
    this->castlingRights = castlingRights;
    this->capturedPiece = capturedPiece;
    this->enPassantSquare = enPassantSquare;
    this->halfMoveClock = halfMoveClock;
    this->fullMoveClock = fullMoveClock;
}

//

Move::Move()
{
    data = 0;
}
Move::Move(int from, int to, int status=0)
{
    data = ((status << 12) || (to << 6) || from);
}

int Move::from() const
{
    return (data & 0x3F);
}
int Move::to() const
{
    return ((data >> 6) & 0x3F);
}
int Move::status() const
{
    return (((data >> 12) & 0xE));
}

bool Move::isCastling() const
{
    return (status() == 0x2);
}
bool Move::isEnPassant() const
{
    return (status() == 0x5); //0b0101
}
bool Move::isPromotion() const
{
    return (status() & 0x8); //0b1000
}
bool Move::isCapture() const {
    return (status() & 0x4 ); //0b0100
}
//

Board::Board()
{
    bbs.fill(0ULL);
    occupancies.fill(0ULL);
    castlingRights = 0b0000;
    enPassantSquare = -1;
    halfMoveClock = 0;
    fullMoveClock = 1;
    whiteToMove = true;
}

//
//      Helper Methods:
//
void Board::setBit(uint64_t &bb, int square)
{
    bb |= (1ULL << square);
}

void Board::clearBit(uint64_t &bb, int square)
{
    bb &= ~(1ULL << square);
}

bool Board::isBitSet(uint64_t bb, int square)
{
    return (bb & (1ULL << square)) > 0;
}
int Board::pieceOn(int square)
{
    for (int i = 0; i < 12; i++)
    {
        if (isBitSet(bbs[i], square))
        {
            return i;
        }
    }
    return -1;
}
uint64_t Board::get_lsb_bb(uint64_t bb)
{
    return (bb & ((~bb) + 1));
}
uint64_t Board::pop_lsb_bb(uint64_t &bb)
{
    int square = __builtin_ctzll(bb);
    bb &= bb - 1;
    return square;
}
int Board::get_lsb_index(uint64_t bb)
{
    return __builtin_ctzll(bb);
}

inline int Board::popcount(uint64_t bb)
{
    // int count=0;
    // while (bb) {
    //     bb&=bb-1;
    //     count++;
    // }
    return __builtin_popcountll(bb);
}

void Board::displayBoard(uint64_t bb)
{
    std::cout << "\n";
    for (int rank = 7; rank > -1; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            if (!file)
            {
                printf(" %d ", 8 - rank);
            }
            printf(" %d", (isBitSet(bb, square) ? 1 : 0));
        }
        std::cout << "\n";
    }
    printf("\n    a b c d e f g h\n\n");
    printf("bitboard: %lld\n\n", bb);
}

//
// FEN
//

std::vector<std::string> Board::splitString(std::string str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(str);
    while (std::getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

int Board::codeToIndex(std::string code)
{
    char file = code[0] - 'a';
    char rank = code[1] - '1';
    return rank * 8 + file;
}

std::string Board::indexToCode(int index)
{
    int file = index % 8;
    int rank = index / 8;
    std::string s;
    s += char('a' + file);
    s += char('1' + rank);
    return s;
}

std::string Board::getFEN()
{
    return " ";
}

void Board::setFEN(std::string s)
{
    std::vector<std::string> vec = splitString(s, ' ');
    std::vector<std::string> rows = splitString(vec[0], '/');

    // Clear prev position:
    bbs.fill(0ULL);
    castlingRights = 0b0000;
    enPassantSquare = -1;
    halfMoveClock = 0;
    fullMoveClock = 0;
    whiteToMove = true;

    int i = 0;
    for (std::string row : rows)
    {
        for (char let : row)
        {
            if (std::isalpha(let))
            {
                int color = (std::isupper(let) ? 0 : 6);
                char letConverted = std::tolower(let);
                int piece = pieceMap.at(letConverted);
                setBit(bbs[piece + color], i);
                i++;
            }
            if (std::isdigit(let))
            {
                int letConverted = let - '0';
                i += letConverted;
            }
        }
    }
    if (vec[1] == "w")
    {
        whiteToMove = true;
    }
    else
    {
        whiteToMove = false;
    }

    for (char let : vec[2])
    {
        switch (let)
        {
        case 'K':
        {
            castlingRights |= (1 << 3);
            break;
        }
        case 'Q':
        {
            castlingRights |= (1 << 2);
            break;
        }
        case 'k':
        {
            castlingRights |= (1 << 1);
            break;
        }
        case 'q':
        {
            castlingRights |= (1 << 0);
            break;
        }
        }
    }
    int squareEnPassant = codeToIndex(vec[3]);
    if (vec[3] == "-")
    {
        enPassantSquare = -1;
    }
    else
    {
        enPassantSquare = squareEnPassant;
    }
    if (vec.size() > 4)
    {
        halfMoveClock = std::stoi(vec[4]);
        fullMoveClock = std::stoi(vec[5]);
    }
    updateOccupancies();
}
void Board::updateOccupancies()
{
    occupancies[0] = 0ULL;
    occupancies[1] = 0ULL;
    occupancies[2] = 0ULL;
    for (int i = 0; i <= 5; i++)
    {
        occupancies[0] |= bbs[i];
    }
    for (int i = 6; i <= 11; i++)
    {
        occupancies[1] |= bbs[i];
    }
    occupancies[2] = occupancies[0] | occupancies[1];
}

//
// Move generation & representation
//

//
//  First 6 bits - FROM square, next 6 bits - TO square, 3 bits - piece type that just moved,
//  3 bits - piece type that got captured, 3 bits - promotion piece, and the rest 11 are for move flags
//

uint64_t Board::mask_pawn_attacks(int side, int square)
{
    uint64_t attack = 0ULL;

    int rank = square / 8;
    int file = square % 8;

    int f1 = file + 1;
    int f2 = file - 1;
    int r = rank + (!side ? +1 : -1);

    if (r >= 0 && r < 8 && f1 >= 0 && f1 < 8)
    {
        attack |= (1ULL << (r * 8 + f1));
    }
    if (r >= 0 && r < 8 && f2 >= 0 && f2 < 8)
    {
        attack |= (1ULL << (r * 8 + f2));
    }
    // if (attack > 0)
    // {
    //     displayBoard(attack);
    // }
    return attack;
}

uint64_t Board::mask_knight_attacks(int square)
{
    uint64_t attacks = 0ULL;

    int rank = square / 8;
    int file = square % 8;

    int dr[] = {-2, -1, 1, 2, 2, 1, -1, -2};
    int df[] = {1, 2, 2, 1, -1, -2, -2, -1};

    for (int i = 0; i < 8; i++)
    {
        int r = rank + dr[i];
        int f = file + df[i];

        if (r >= 0 && r < 8 && f >= 0 && f < 8)
        {
            attacks |= (1ULL << (r * 8 + f));
        }
    }
    // if (attacks > 0)
    // {
    //     displayBoard(attacks);
    // }
    return attacks;
}
uint64_t Board::mask_king_attacks(int square)
{
    uint64_t attacks = 0ULL;

    int rank = square / 8;
    int file = square % 8;

    int dr[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int df[] = {-1, -1, -1, 0, 0, 1, 1, 1};

    for (int i = 0; i < 8; i++)
    {
        int r = rank + dr[i];
        int f = file + df[i];
        if (r >= 0 && r < 8 && f >= 0 && f < 8)
        {
            attacks |= (1ULL << (r * 8 + f));
        }
    }
    // if (attacks > 0)
    // {
    //     displayBoard(attacks);
    // }
    return attacks;
}
uint64_t Board::mask_bishop_attacks(int square)
{
    uint64_t attacks = 0ULL;

    int rank = square / 8;
    int file = square % 8;

    int dr[] = {-1, -1, 1, 1};
    int df[] = {-1, 1, -1, 1};

    for (int i = 0; i < 4; i++)
    {
        int r = rank + dr[i];
        int f = file + df[i];
        while (r > 0 && r < 7 && f > 0 && f < 7)
        {
            attacks |= (1ULL << (r * 8 + f));
            r += dr[i];
            f += df[i];
        }
    }
    // if (attacks>0) {
    //     displayBoard(attacks);
    // }
    return attacks;
}

uint64_t Board::mask_rook_attacks(int square)
{
    uint64_t attacks = 0ULL;

    int rank = square / 8;
    int file = square % 8;

    int dr[] = {0, 0, -1, 1};
    int df[] = {-1, 1, 0, 0};

    for (int i = 0; i < 4; i++)
    {
        int r = rank + dr[i];
        int f = file + df[i];
        while (r > 0 && r < 7 && f > 0 && f < 7)
        {
            attacks |= (1ULL << (r * 8 + f));
            r += dr[i];
            f += df[i];
        }
    }
    // if (attacks>0) {
    //     displayBoard(attacks);
    // }
    return attacks;
}

uint64_t Board::get_bishop_attacks(int square, uint64_t board_occupancy)
{
    board_occupancy &= bishop_masks[square];

    int index = (board_occupancy * BISHOP_MAGICS[square]) >> (64 - bishop_relevant_bits[square]);
    return bishop_attacks[square][index];
}

uint64_t Board::get_rook_attacks(int square, uint64_t board_occupancy)
{
    board_occupancy &= rook_masks[square];

    int index = (board_occupancy * BISHOP_MAGICS[square]) >> (64 - rook_relevant_bits[square]);
    return rook_attacks[square][index];
}

uint64_t Board::set_occupancy(int index, int bits, uint64_t mask)
{
    uint64_t occupancy = 0ULL;
    for (int i = 0; i < bits; i++)
    {
        int square = get_lsb_index(mask);
        mask &= mask - 1;
        if (index & (1 << i))
        {
            occupancy |= (1ULL << square);
        }
    }
    return occupancy;
}

uint64_t Board::bishop_attacks_from_occupancy(int square, uint64_t blockers)
{
    uint64_t attacks = 0ULL;
    int r = square / 8;
    int f = square % 8;

    // NE
    for (int i = r + 1, j = f + 1; i <= 7 && j <= 7; i++, j++)
    {
        attacks |= (1ULL << (i * 8 + j));
        if (blockers & (1ULL << (i * 8 + j)))
            break;
    }
    // NW
    for (int i = r + 1, j = f - 1; i <= 7 && j >= 0; i++, j--)
    {
        attacks |= (1ULL << (i * 8 + j));
        if (blockers & (1ULL << (i * 8 + j)))
            break;
    }
    // SE
    for (int i = r - 1, j = f + 1; i >= 0 && j <= 7; i--, j++)
    {
        attacks |= (1ULL << (i * 8 + j));
        if (blockers & (1ULL << (i * 8 + j)))
            break;
    }
    // SW
    for (int i = r - 1, j = f - 1; i >= 0 && j >= 0; i--, j--)
    {
        attacks |= (1ULL << (i * 8 + j));
        if (blockers & (1ULL << (i * 8 + j)))
            break;
    }
    return attacks;
}

uint64_t Board::rook_attacks_from_occupancy(int square, uint64_t blockers)
{
    uint64_t attacks = 0ULL;
    int r = square / 8;
    int f = square % 8;
    // N
    for (int i = r + 1; i <= 7; i++)
    {
        attacks |= (1ULL << (i * 8 + f));
        if (blockers & (1ULL << (i * 8 + f)))
            break;
    }
    // S
    for (int i = r - 1; i >= 0; i--)
    {
        attacks |= (1ULL << (i * 8 + f));
        if (blockers & (1ULL << (i * 8 + f)))
            break;
    }
    // E
    for (int j = f + 1; j <= 7; j++)
    {
        attacks |= (1ULL << (r * 8 + j));
        if (blockers & (1ULL << (r * 8 + j)))
            break;
    }
    // W
    for (int j = f - 1; j >= 0; j--)
    {
        attacks |= (1ULL << (r * 8 + j));
        if (blockers & (1ULL << (r * 8 + j)))
            break;
    }
}

void Board::generateMoves()
{
    std::vector<Move> moves;

    uint64_t from_bb;

    from_bb = bbs[0];
    while (from_bb > 0)
    {
        // White pawns
        int from = pop_lsb_bb(from_bb);
        uint64_t pseudolegal=pawn_masks[0][from] & (!occupancies[0]);
        while (pseudolegal>0) {
            int to = pop_lsb_bb(pseudolegal);
            if (to==enPassantSquare) {
                moves.push_back(Move(from, to, 0b0101)); // en passant status bitwise
            } else if ( (0xff00000000000000 & to ) && (to & occupancies[1])) { // If last row for white pawns(promotion)
                moves.push_back(Move(from, to, 0b1100));
                moves.push_back(Move(from, to, 0b1101)); 
                moves.push_back(Move(from, to, 0b1110)); 
                moves.push_back(Move(from, to, 0b1111));  
            } else if (0xff00000000000000 & to ) {
                moves.push_back(Move(from, to, 0b1000));
                moves.push_back(Move(from, to, 0b1001)); 
                moves.push_back(Move(from, to, 0b1010)); 
                moves.push_back(Move(from, to, 0b1011));  
            } else {
                moves.push_back(Move(from, to, 0b0000));
            }
        } 
    }

    //  White knights
    from_bb= bbs[1];
    while (from_bb>0) {
        int from = pop_lsb_bb(from_bb);
        uint64_t pseudolegal=knight_masks[from] & (!occupancies[0]);
        while (pseudolegal>0) {
            int to =pop_lsb_bb(pseudolegal);
            if (to & occupancies[1]) {
                moves.push_back(Move(from,to, 0b0100));
            }   else {
                moves.push_back(Move(from,to, 0b0000));
            }
        }
    }

    // White bishops
    from_bb=bbs[2];
    while   (from_bb>0) {
        int from =pop_lsb_bb(from_bb);
        uint64_t pseudolegal=get_bishop_attacks(from, occupancies[2]);
        while (pseudolegal>0) {
            int to = pop_lsb_bb(pseudolegal);
            if (to & occupancies[1]) {
                moves.push_back(Move(from, to, 0b0100));
            } else {
                moves.push_back(Move(from, to, 0b0000));
            }
        }
    }

    // White rooks
    from_bb=bbs[3];
    while (from_bb>0) {
        int from = pop_lsb_bb(from_bb);
        uint64_t pseudolegal = get_rook_attacks(from, occupancies[2]);
        while (pseudolegal>0) {
            
        }
    }
}
void Board::generateKnightMoves()
{
    for (int i = 0; i < 64; i++)
    {
        knight_masks[i] = mask_knight_attacks(i);
    }
}
void Board::generateKingMoves()
{
    for (int i = 0; i < 64; i++)
    {
        king_masks[i] = mask_king_attacks(i);
    }
}
void Board::generatePawnMoves()
{
    for (int i = 0; i < 64; i++)
    {
        pawn_masks[0][i] = mask_pawn_attacks(0, i);
    }
    for (int i = 0; i < 64; i++)
    {
        pawn_masks[1][i] = mask_pawn_attacks(1, i);
    }
}
void Board::generateBishopMoves()
{
    for (int i = 0; i < 64; i++)
    {
        bishop_masks[i] = mask_bishop_attacks(i);
        bishop_relevant_bits[i] = popcount(bishop_masks[i]);

        int occupancy_count = 1 << bishop_relevant_bits[i];

        // Generating all occupancies and attacks
        for (int j = 0; j < occupancy_count; j++)
        {
            uint64_t occupancy = set_occupancy(j, bishop_relevant_bits[i], bishop_masks[i]);

            // Computing exact attacks for this occupancy (smhw related to ray tracing)
            uint64_t attacks = bishop_attacks_from_occupancy(i, occupancy);

            // Filling magic attack table
            int magic_index = (occupancy * BISHOP_MAGICS[i]) >> (64 - bishop_relevant_bits[i]);
            bishop_attacks[i][magic_index] = attacks;
        }
    }
}
void Board::generateRookMoves()
{
    for (int i = 0; i < 64; i++)
    {
        // gen all attack masks and relevant bits
        rook_masks[i] = mask_rook_attacks(i);
        rook_relevant_bits[i] = popcount(rook_masks[i]);

        int occupancy_count = 1 << rook_relevant_bits[i];
        for (int j = 0; j < occupancy_count; j++)
        {
            uint64_t occupancy = set_occupancy(j, rook_relevant_bits[i], rook_masks[i]);

            uint64_t attacks = rook_attacks_from_occupancy(i, occupancy);

            int magic_index = (occupancy * ROOK_MAGICS[i]) >> (64 - rook_relevant_bits[i]);
            rook_attacks[i][magic_index] = attacks;
        }
    }
}
void Board::generateSlidingMoves()
{
}

// Make/unmake move logic

void Board::makeMove(Move m, Undo &undo)
{
    undo.enPassantSquare = enPassantSquare;
    undo.castlingRights = castlingRights;
    undo.fullMoveClock = fullMoveClock;
    undo.halfMoveClock = halfMoveClock;
    undo.capturedPiece = pieceOn(m.to());
}
void Board::unmakeMove(Move m, Undo &undo)
{
    castlingRights = undo.castlingRights;
    enPassantSquare = undo.enPassantSquare;
    fullMoveClock = undo.fullMoveClock;
    halfMoveClock = undo.halfMoveClock;
    if (undo.capturedPiece != -1)
    {
        setBit(bbs[undo.capturedPiece], m.to());
        mailbox[m.to()] = undo.capturedPiece;
    }
}