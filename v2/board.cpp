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
#include <random>

Undo::Undo()
{
    castlingRights = 0b1111;
    capturedPiece = -1;
    enPassantSquare = -1;
    halfMoveClock = 0;
    fullMoveClock = 1;
}

Undo::Undo(uint8_t castlingRights, int capturedPiece, int8_t enPassantSquare, uint8_t halfMoveClock, uint16_t fullMoveClock)
{
    this->castlingRights = castlingRights;
    this->capturedPiece = capturedPiece;
    this->enPassantSquare = enPassantSquare;
    this->halfMoveClock = halfMoveClock;
    this->fullMoveClock = fullMoveClock;
}

std::ostream &operator<<(std::ostream &os, const Undo &u)
{
    os << "Undo castling rights:" << u.castlingRights << "captured piece " << u.capturedPiece << " en passant square: " << u.enPassantSquare << "\n";
    return os;
}

//

Move::Move()
{
    data = 0;
}
Move::Move(int from, int to, int status = 0)
{
    data = ((status << 12) | (to << 6) | from);
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
    return (((data >> 12) & 0xF));
}

bool Move::isCastling() const
{
    return (status() == 0x2);
}
bool Move::isEnPassant() const
{
    return (status() == 0x5); // 0b0101
}
bool Move::isPromotion() const
{
    return (status() & 0x8); // 0b1000
}
bool Move::isCapture() const
{
    return (status() & 0x4); // 0b0100
}

std::ostream &operator<<(std::ostream &os, const Move &m)
{
    os << "Move from " << m.from() << " to " << m.to() << " with status: " << m.status(); //<< "\n";
    return os;
}

//

uint64_t Board::pawn_masks[2][64];
uint64_t Board::pawn_push[2][64];
uint64_t Board::pawn_double_push[2][64];
uint64_t Board::knight_masks[64];
uint64_t Board::king_masks[64];
uint64_t Board::bishop_masks[64];
uint64_t Board::rook_masks[64];

uint64_t Board::bishop_relevant_bits[64];
uint64_t Board::rook_relevant_bits[64];

uint64_t Board::bishop_attacks[64][512];
uint64_t Board::rook_attacks[64][4096];

// uint64_t Board::BISHOP_MAGICS[64];
// uint64_t Board::ROOK_MAGICS[64];

//  Constructor
Board::Board()
{
    bbs.fill(0ULL);
    occupancies.fill(0ULL);
    mailbox.fill(0);
    castlingRights = 0b0000;
    enPassantSquare = -1;
    halfMoveClock = 0;
    fullMoveClock = 1;
    sideToMove = 0;
    std::cout << " Generating King moves\n";
    generateKingMoves();
    std::cout << " Generating pawn moves\n";
    generatePawnMoves();
    std::cout << " Generating knight moves\n";
    generateKnightMoves();
    // std::cout << "Searching for magic numbers\n";
    // searchAllMagics();
    std::cout << " Generating bishop moves\n";
    generateBishopMoves();
    std::cout << " Generating rook moves\n";
    generateRookMoves();
    std::cout << " Fin\n";
}

//
//      HELPER METHODS FOR BITWISE INTERACTION:
//
void Board::setBit(uint64_t &bb, int square)
{
    bb |= (1ULL << square);
    updateOccupancies();
}

void Board::clearBit(uint64_t &bb, int square)
{
    bb &= ~(1ULL << square);
    updateOccupancies();
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
    if (!bb)
    {
        throw std::runtime_error("BB is already empty, can't GET lsb");
    }

    return (bb & ((~bb) + 1));
}
uint64_t Board::pop_lsb_bb(uint64_t &bb)
{
    if (!bb)
    {
        throw std::runtime_error("BB is already empty, can't POP lsb");
    }
    int square = __builtin_ctzll(bb);
    bb &= bb - 1;
    updateOccupancies();
    return square;
}
int Board::get_lsb_index(uint64_t bb)
{
    updateOccupancies();
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

int Board::findPiece(int square, Move m)
{
    for (int i = 0; i < 12; i++)
    {
        if (bbs[i] & (1ULL << square))
        {
            return i;
        }
    }
    std::cout << "Move sequence that led to this error position: ";
    for (Move m : moveLog) {
        std::cout<<moveToCode(m)<<", ";
    }
    std::cout << "\n";
    for (int i=0; i<moveLog.size(); i++) {
        std::cout<< moveToCode(moveLog[i])<<", and corresponding board:\n";
        displayBB(boardLog[i]);
    }
    std::cout<<"\n";
    displayBoard();
    std::cout << "ERROR with square " << square <<" with move: " << m<<"\n";
    throw std::runtime_error("Piece not found");
}

int Board::findPieceKing(int square, Move m) {
    for (int i = 0; i < 12; i++)
    {
        if (bbs[i] & (1ULL << square))
        {
            return i;
        }
    }
    return -1;
}

void Board::validateBoard(int i) {
    std::string s="";
    if (i==1) {
        s+="perft after making move, ";
    } else if (i==2) {
        s+="perft after undoing move, ";
    } else if (i==3) {
        s+="perft divide after making move, ";
    } else if (i==4) {
        s+="perft divide after undoing move, ";
    }
    if (!bbs[5]) {
        throw std::runtime_error(s+"no white king found");
    }
    if (!bbs[11]) {
        throw std::runtime_error(s+"no black king found");
    }
}

uint64_t Board::getBB() {
    uint64_t bb;
    for (int i=0; i<12; i++) {
        bb|=bbs[i];
    }
    return bb;
}

//
//  DISPLAYING STUFF
//

void Board::displayBoard()
{
    std::cout << "\n";
    for (int rank = 7; rank > -1; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            if (!file)
            {
                printf(" %d ", rank+1);
            }
            printf(" %d", (isBitSet(occupancies[2], square) ? 1 : 0));
        }
        std::cout << "\n";
    }
    printf("\n    a b c d e f g h\n\n");
    printf("bitboard: %lld\n\n", occupancies[2]);
}

void Board::displayBB(uint64_t bb)
{
    std::cout << "\n";
    for (int rank = 7; rank > -1; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            if (!file)
            {
                printf(" %d ", rank+1);
            }
            printf(" %d", (isBitSet(bb, square) ? 1 : 0));
        }
        std::cout << "\n";
    }
    printf("\n    a b c d e f g h\n\n");
    printf("bitboard: %lld\n\n", bb);
}

void Board::displayMoves(std::array<Move, 256> moveList)
{
    for (int i = 0; i < moveList.size(); i++)
    {
        std::cout << moveList[i];
    }
}

//
// FEN PROCESSING
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

std::string Board::moveToCode(Move m)
{
    int r1 = m.from() / 8;
    int f1 = m.from() % 8;
    int r2 = m.to() / 8;
    int f2 = m.to() % 8;
    std::string s = "";
    // s += char('a' + f1) + char('1' + r1);
    // s += char('a' + f2) + char('1' + r2);
    auto l1 = fileMap.find(f1);
    auto l2 = fileMap.find(f2);
    s += char(l1->second);
    s += char('1' + r1);
    s += char(l2->second);
    s += char('1' + r2);
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
    sideToMove = 0;
    // whiteToMove = true;

    int rank = 7;
    int file = 0;
    for (std::string row : rows)
    {
        for (char let : row)
        {
            if (std::isalpha(let))
            {
                int color = (std::isupper(let) ? 0 : 6);
                char letConverted = std::tolower(let);
                int piece = pieceMap.at(letConverted);
                setBit(bbs[piece + color], rank * 8 + file);
                file++;
            }
            if (std::isdigit(let))
            {
                int letConverted = let - '0';
                file += letConverted;
            }
        }
        rank--;
        file = 0;
    }
    if (vec[1] == "w")
    {
        sideToMove = 0;
    }
    else
    {
        sideToMove = 1;
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

//
// INITIALIZATION/PREGENERATION STAGE
//

// Magic bbs gen
uint64_t Board::rand64()
{
    static std::mt19937_64 rng(std::random_device{}());
    return rng();
}

uint64_t Board::sparse_rand()
{
    return rand64() & rand64() & rand64();
}

uint64_t Board::find_magic(int sq, bool bishop)
{
    uint64_t mask = bishop ? mask_bishop_attacks(sq) : mask_rook_attacks(sq);
    int bits = popcount(mask);
    int size = 1 << bits;

    //  Build occupancy/attack pairs
    uint64_t occs[4096], atks[4096];
    for (int i = 0; i < size; i++)
    {
        occs[i] = set_occupancy(i, bits, mask);
        atks[i] = bishop ? bishop_attacks_from_occupancy(sq, occs[i]) : rook_attacks_from_occupancy(sq, occs[i]);
    }

    //  Trial and error:
    for (int attempt = 0; attempt < 100000000; attempt++)
    {
        uint64_t magic = sparse_rand();
        // if (popcount((mask * magic) >> 56) < 6)
        //     continue;
        uint64_t used[4096] = {};
        bool fail = false;
        for (int i = 0; i < size && !fail; i++)
        {
            int idx = (occs[i] * magic) >> (64 - bits);
            if (!used[idx])
            {
                used[idx] = atks[i];
            }
            else if (used[idx] != atks[i])
            {
                fail = true;
            }
        }
        if (!fail)
        {
            return magic;
        }
    }
    return 0ULL;
}

// void Board::searchAllMagics()
// {
//     for (int i = 0; i < 64; i++)
//     {
//         BISHOP_MAGICS[i] = find_magic(i, true);
//         ROOK_MAGICS[i] = find_magic(i, false);
//         if (!BISHOP_MAGICS[i])
//             throw std::runtime_error("BISHOP MAGIC FAILED");
//         if (!ROOK_MAGICS[i])
//             throw std::runtime_error("ROOK MAGIC FAILED");
//     }

//     // for (int i : BISHOP_MAGICS) {
//     //     std::cout<<i<<", ";
//     // }
//     // std::cout<< "\n\n\n\n";
//     // for (int i : ROOK_MAGICS) {
//     //     std::cout<<i<<", ";
//     // }
// }

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
    //     displayBB(attack);
    // }
    return attack;
}

uint64_t Board::mask_pawn_push(int side, int square, int push)
{
    uint64_t attack = 0ULL;
    int rank = square / 8;
    int file = square % 8;
    int dir = (side ? -1 : 1);
    int r = rank + (push * dir);
    if (r >= 0 && r < 8)
    {
        attack |= (1ULL << (r * 8 + file));
    }
    // if (attack)
    // {
    //     displayBB(attack);
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
        while (((r > 0 && r < 7) || ((rank == 0 || rank == 7) && df[i] != 0)) && ((f > 0 && f < 7) || ((file == 0 || file == 7) && dr[i] != 0)))
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
    return attacks;
}

//
// MAIN MOVE GENERATION
//

void Board::generateMoves(std::array<Move, 256> &moveList)
{
    // moveList.fill(Move());
    int count = 0;
    uint64_t from_bb;

    if (!sideToMove)
    {
        //
        //  WHITE PIECES MOVE GENERATION
        //

        // White pawns
        from_bb = bbs[0];
        while (from_bb > 0)
        {
            int from = pop_lsb_bb(from_bb);
            uint64_t pseudolegal = pawn_masks[0][from] & (occupancies[1]);
            while (pseudolegal > 0)
            {
                int to = pop_lsb_bb(pseudolegal);

                //  En passant
                if (to == enPassantSquare)
                {
                    moveList[count] = Move(from, to, 0b0101); // en passant status bitwise
                }
                else if ((0xff00000000000000 & (1ULL << to)) && ((1ULL << to) & occupancies[1]))
                { // If last row for white pawns(promotion)
                    moveList[count] = Move(from, to, 0b1100);
                    count++;
                    moveList[count] = Move(from, to, 0b1101);
                    count++;
                    moveList[count] = Move(from, to, 0b1110);
                    count++;
                    moveList[count] = Move(from, to, 0b1111);
                }
                else if (0xff00000000000000 & (1ULL << to))
                {
                    moveList[count] = Move(from, to, 0b1000);
                    count++;
                    moveList[count] = Move(from, to, 0b1001);
                    count++;
                    moveList[count] = Move(from, to, 0b1010);
                    count++;
                    moveList[count] = Move(from, to, 0b1011);
                }
                else
                {
                    moveList[count] = Move(from, to, 0b0100);
                }
                // std::cout << "Added move " << Move(from, to, 0b0000);

                count++;
            }
            pseudolegal = pawn_push[0][from] & (~occupancies[2]);
            while (pseudolegal)
            {
                int to = pop_lsb_bb(pseudolegal);
                //  Regular Push
                moveList[count] = Move(from, to, 0b0000);
                count++;
            }
            if ((1ULL << from) & 0xff00)
            {
                pseudolegal = pawn_double_push[0][from] & (~occupancies[2]);
                while (pseudolegal)
                {
                    int to = pop_lsb_bb(pseudolegal);
                    //  Double Push
                    if (((pawn_double_push[0][from] | pawn_push[0][from]) & occupancies[2])==0)
                    {
                        moveList[count] = Move(from, to, 0b0001);
                        count++;
                    }
                }
            }
        }

        //  White knights
        from_bb = bbs[1];
        while (from_bb > 0)
        {
            int from = pop_lsb_bb(from_bb);
            uint64_t pseudolegal = knight_masks[from] & (~occupancies[0]);
            while (pseudolegal > 0)
            {
                int to = pop_lsb_bb(pseudolegal);
                if ((1ULL << to) & occupancies[1])
                {
                    moveList[count] = Move(from, to, 0b0100);
                }
                else
                {
                    moveList[count] = Move(from, to, 0b0000);
                }
                count++;
            }
        }

        // White bishops
        from_bb = bbs[2];
        while (from_bb > 0)
        {
            int from = pop_lsb_bb(from_bb);
            uint64_t pseudolegal = get_bishop_attacks(from, occupancies[2]) & ~occupancies[0];
            while (pseudolegal > 0)
            {
                int to = pop_lsb_bb(pseudolegal);
                if ((1ULL << to) & occupancies[1])
                {
                    moveList[count] = Move(from, to, 0b0100);
                }
                else
                {
                    moveList[count] = Move(from, to, 0b0000);
                }
                count++;
            }
        }

        // White rooks
        from_bb = bbs[3];
        while (from_bb > 0)
        {
            int from = pop_lsb_bb(from_bb);
            uint64_t pseudolegal = get_rook_attacks(from, occupancies[2]) & ~occupancies[0];
            while (pseudolegal > 0)
            {
                int to = pop_lsb_bb(pseudolegal);
                if ((1ULL << to) & occupancies[1])
                {
                    moveList[count] = Move(from, to, 0b0100);
                }
                else
                {
                    moveList[count] = Move(from, to, 0b0000);
                }
                count++;
            }
        }

        // White queens
        from_bb = bbs[4];
        while (from_bb > 0)
        {
            int from = pop_lsb_bb(from_bb);
            uint64_t pseudolegal = (get_bishop_attacks(from, occupancies[2]) | get_rook_attacks(from, occupancies[2])) & ~occupancies[0];
            while (pseudolegal > 0)
            {
                int to = pop_lsb_bb(pseudolegal);
                if ((1ULL << to) & occupancies[1])
                {
                    moveList[count] = Move(from, to, 0b0100);
                }
                else
                {
                    moveList[count] = Move(from, to, 0b0000);
                }
                count++;
            }
        }

        // White kings
        from_bb = bbs[5];
        while (from_bb > 0)
        {
            int from = pop_lsb_bb(from_bb);
            uint64_t pseudolegal = king_masks[from] & ~occupancies[0];
            while (pseudolegal > 0)
            {
                int to = pop_lsb_bb(pseudolegal);
                if ((1ULL << to) & occupancies[1])
                {
                    moveList[count] = Move(from, to, 0b0100);
                }
                else
                {
                    moveList[count] = Move(from, to, 0b0000);
                }
                count++;
            }
        }
    }
    else
    {

        //
        //  BLACK PIECES MOVE GENERATION
        //

        // Black pawns
        from_bb = bbs[6];
        while (from_bb > 0)
        {

            int from = pop_lsb_bb(from_bb);
            uint64_t pseudolegal = pawn_masks[1][from] & (occupancies[0]);
            while (pseudolegal > 0)
            {
                int to = pop_lsb_bb(pseudolegal);
                if (to == enPassantSquare)
                {
                    moveList[count] = Move(from, to, 0b0101); // en passant status bitwise
                }
                else if ((0x00000000000000ff & (1ULL << to)) && ((1ULL << to) & occupancies[0]))
                { // If last row for white pawns(promotion)
                    moveList[count] = Move(from, to, 0b1100);
                    count++;
                    moveList[count] = Move(from, to, 0b1101);
                    count++;
                    moveList[count] = Move(from, to, 0b1110);
                    count++;
                    moveList[count] = Move(from, to, 0b1111);
                }
                else if (0x00000000000000ff & (1ULL << to))
                {
                    moveList[count] = Move(from, to, 0b0100);
                    count++;
                    moveList[count] = Move(from, to, 0b0101);
                    count++;
                    moveList[count] = Move(from, to, 0b0110);
                    count++;
                    moveList[count] = Move(from, to, 0b0111);
                }
                else
                {
                    moveList[count] = Move(from, to, 0b0000);
                }
                count++;
            }
            pseudolegal = pawn_push[1][from] & (~occupancies[2]);
            while (pseudolegal)
            {
                int to = pop_lsb_bb(pseudolegal);
                //  Regular Push
                moveList[count] = Move(from, to, 0b0000);
                count++;
            }
            if ((1ULL << from) & 0xff000000000000)
            {
                pseudolegal = pawn_double_push[1][from] & (~occupancies[2]);
                while (pseudolegal)
                {
                    int to = pop_lsb_bb(pseudolegal);
                    //  Double Push
                    if (((pawn_double_push[1][from] | pawn_push[1][from]) & occupancies[2])==0)
                    {
                        moveList[count] = Move(from, to, 0b0001);
                        count++;
                    }
                }
            }
        }

        //  Black knights
        from_bb = bbs[7];
        while (from_bb > 0)
        {
            int from = pop_lsb_bb(from_bb);
            uint64_t pseudolegal = knight_masks[from] & (~occupancies[1]);
            while (pseudolegal > 0)
            {
                int to = pop_lsb_bb(pseudolegal);
                if ((1ULL << to) & occupancies[0])
                {
                    moveList[count] = Move(from, to, 0b0100);
                }
                else
                {
                    moveList[count] = Move(from, to, 0b0000);
                }
                count++;
            }
        }

        // Black bishops
        from_bb = bbs[8];
        while (from_bb > 0)
        {
            int from = pop_lsb_bb(from_bb);
            uint64_t pseudolegal = get_bishop_attacks(from, occupancies[2]) & ~occupancies[1];
            while (pseudolegal > 0)
            {
                int to = pop_lsb_bb(pseudolegal);
                if ((1ULL << to) & occupancies[0])
                {
                    moveList[count] = Move(from, to, 0b0100);
                }
                else
                {
                    moveList[count] = Move(from, to, 0b0000);
                }
                count++;
            }
        }

        // Black rooks
        from_bb = bbs[9];
        while (from_bb > 0)
        {
            int from = pop_lsb_bb(from_bb);
            uint64_t pseudolegal = get_rook_attacks(from, occupancies[2]) & ~occupancies[1];
            while (pseudolegal > 0)
            {
                int to = pop_lsb_bb(pseudolegal);
                if ((1ULL << to) & occupancies[0])
                {
                    moveList[count] = Move(from, to, 0b0100);
                }
                else
                {
                    moveList[count] = Move(from, to, 0b0000);
                }
                count++;
            }
        }

        // Black queens
        from_bb = bbs[10];
        while (from_bb > 0)
        {
            int from = pop_lsb_bb(from_bb);
            uint64_t pseudolegal = (get_bishop_attacks(from, occupancies[2]) | get_rook_attacks(from, occupancies[2])) & ~occupancies[1];
            while (pseudolegal > 0)
            {
                int to = pop_lsb_bb(pseudolegal);
                if ((1ULL << to) & occupancies[0])
                {
                    moveList[count] = Move(from, to, 0b0100);
                }
                else
                {
                    moveList[count] = Move(from, to, 0b0000);
                }
                count++;
            }
        }

        //  Black kings
        from_bb = bbs[11];
        while (from_bb > 0)
        {
            int from = pop_lsb_bb(from_bb);
            uint64_t pseudolegal = king_masks[from] & ~occupancies[1];
            while (pseudolegal > 0)
            {
                int to = pop_lsb_bb(pseudolegal);
                if ((1ULL << to) & occupancies[0])
                {
                    moveList[count] = Move(from, to, 0b0100);
                }
                else
                {
                    moveList[count] = Move(from, to, 0b0000);
                }
                count++;
            }
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
        pawn_masks[1][i] = mask_pawn_attacks(1, i);
        pawn_push[0][i] = mask_pawn_push(0, i, 1);
        pawn_push[1][i] = mask_pawn_push(1, i, 1);
        pawn_double_push[0][i] = mask_pawn_push(0, i, 2);
        pawn_double_push[1][i] = mask_pawn_push(1, i, 2);
    }
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

    int index = (board_occupancy * ROOK_MAGICS[square]) >> (64 - rook_relevant_bits[square]);
    return rook_attacks[square][index];
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
        //  std::cout << "starting with i=" << i << "\n";
        // gen all attack masks and relevant bits
        rook_masks[i] = mask_rook_attacks(i);
        rook_relevant_bits[i] = popcount(rook_masks[i]);
        // std::cout << "rook relevant bits\n";

        int occupancy_count = 1 << rook_relevant_bits[i];
        // if (i == 0)
        // {
        //     std::cout << "mask rook attacks: " << rook_masks[i] << "\n";
        //     std::cout << "occupancy count: " << occupancy_count << "\n";
        // }
        for (uint64_t j = 0; j < occupancy_count; j++)
        {
            uint64_t occupancy = set_occupancy(j, rook_relevant_bits[i], rook_masks[i]);
            // std::cout << "occupancy\n";
            uint64_t attacks = rook_attacks_from_occupancy(i, occupancy);
            // std::cout << "attacks\n";

            int magic_index = (occupancy * ROOK_MAGICS[i]) >> (64 - rook_relevant_bits[i]);
            // std::cout << "magic index\n";
            rook_attacks[i][magic_index] = attacks;
            if (i == 0 && j == 72339069014639102)
                std::cout << "rook attacks " << attacks << "\n";
        }
        // std::cout << "successfully did i=" << i << "\n";
    }
}

// Make/undo move logic

void Board::makeMove(Move m, Undo &u)
{

    //std::cout << "Trying to find piece from for move: " << m << "\n";
    // if (m.from()==11) {
    //     std::cout << "Trying to find piece run makeMove() for move " << m << ".\n";
    //     displayBoard();
    // }
    int p = findPiece(m.from(), m); // bb index of piece that is getting moved

    // Move piece from source to target.
    int pc = -1; // Piece captured index bb
    clearBit(bbs[p], m.from());
    if (m.isEnPassant())
    {
        std::cout << "en passant\n";
        pc = (sideToMove ? 6 : 0);
        clearBit(
            bbs[!sideToMove ? 6 : 0], // if white
            (m.to() + (sideToMove ? +8 : -8)));
    }
    else if (m.isCapture())
    {
        // std::cout << "Trying to find piece 'to' for move: " << m << "\n";
        // if (m.to()==11) {
        //     std:: cout<< "Trying to find piece that is supposed to be captured in makeMove() for move " << m << "\n";
        //     displayBoard();
        // }
        pc = findPiece(m.to(), m);
        // std::cout << "pc is " << pc << "\n";
        clearBit(bbs[pc], m.to());
    }
    // std::cout << "pc now is : " << pc << "\n";

    //  Promotion
    if (!m.isPromotion()) // If it isn't promotion, same piece just gets moved to new square
    {
        setBit(bbs[p], m.to());
    }
    else // If it is promotion, new piece gets moved to new square
    {
        int promotedStatus = m.status();
        promotedStatus &= 0x3;
        if (promotedStatus == 3)
        {
            promotedStatus = 4;
        }
        else if (promotedStatus == 2)
        {
            promotedStatus = 3;
        }
        else if (promotedStatus == 1)
        {
            promotedStatus = 2;
        }
        else
        {
            promotedStatus = 1;
        }
        setBit(bbs[promotedStatus + (sideToMove ? +6 : +0)], m.to());
    }

    //  Castling
    if (m.status() == 0b0010 || m.status() == 0b0011)
    {
        int king = get_lsb_index(bbs[5 + (sideToMove ? 6 : 0)]);
        if (m.status() == 0b0010)
        {                                                      // King castle
            clearBit(bbs[3 + (sideToMove ? 6 : 0)], king + 3); //   Remove rook from old position
            setBit(bbs[3 + (sideToMove ? 6 : 0)], king + 1);   //  Add rook to the new position
        }
        else
        {                                                       // Queen castling
            clearBit(bbs[3 + (sideToMove ? 6 : 0)], king + -4); //  Remove rook from old pos
            setBit(bbs[3 + (sideToMove ? 6 : 0)], king - 1);    // Add it to new
        }
    }

    // Save undo info for full restoration.

    u = Undo(castlingRights, pc, enPassantSquare, halfMoveClock, fullMoveClock);
    // std::cout << u;

    // Castling rights
    if (p == 5 || p == 11)
    {
        castlingRights &= (sideToMove ? 0b1100 : 0b0011);
    }

    if ((castlingRights & 0b1100) && p == 5)
    {
        if (m.from() == 0)
        {
            castlingRights &= 0b1011;
        }
        else if (m.from() == 7)
        {
            castlingRights &= 0b0111;
        }
    }

    if ((castlingRights & 0b0011) && p == 11)
    {
        if (m.from() == 56)
        {
            castlingRights &= 0b1110;
        }
        else if (m.from() == 63)
        {
            castlingRights &= 0b1101;
        }
    }

    // Update game state
    enPassantSquare = -1;
    if (m.status() == 0b0001)
    {
        enPassantSquare = m.to() + (sideToMove ? +8 : -8);
    }

    // Halfmove/fullmove clocks
    fullMoveClock++;
    if (p == 0 || p == 6 || m.isCapture())
    {
        halfMoveClock = 0;
    }
    else
    {
        halfMoveClock++;
    }

    // Side to move switch
    sideToMove = !sideToMove;

    moveLog.push_back(m);
    boardLog.push_back(getBB());
}
void Board::undoMove(Move m, Undo &u)
{
    if (u.capturedPiece==11 || u.capturedPiece==5) {
        std::cout << "King was captured\n";
        std::string KING_CAPTURE_ERROR="King was captured, game is over!";
        throw std::runtime_error(KING_CAPTURE_ERROR);
    }

    enPassantSquare = u.enPassantSquare;
    castlingRights = u.castlingRights;
    halfMoveClock = u.halfMoveClock;
    fullMoveClock = u.fullMoveClock;

    sideToMove = !sideToMove;

    // if( m.to()==41) {
    //     std::cout << "Board before undoing the move" << m << ".\n";
    //     displayBoard();
    // }
    int p = findPiece(m.to(), m);

    clearBit(bbs[p], m.to());

    if (u.capturedPiece != -1)
    {
       // std::cout << "Piece was captured, trying to recover it from the " << u.capturedPiece << "\n";
        setBit(bbs[u.capturedPiece], m.to());
    }

    // Castling
    if (m.isCastling())
    { // WORKS ONLY BEFORE ORIGINAL PIECE WAS MOVED
        int king = get_lsb_index(bbs[5 + (sideToMove ? 6 : 0)]);
        if (m.status() == 0b0010)
        {                                                      // King castle
            clearBit(bbs[3 + (sideToMove ? 6 : 0)], king - 1); //   Remove rook from old position
            setBit(bbs[3 + (sideToMove ? 6 : 0)], king + 1);   //  Add rook to the new position
        }
        else
        {                                                      // Queen castling
            clearBit(bbs[3 + (sideToMove ? 6 : 0)], king + 1); //  Remove rook from old pos
            setBit(bbs[3 + (sideToMove ? 6 : 0)], king - 2);   // Add it to new
        }
    }

    // REMOVE PIECE FROM NEW POSITION
    clearBit(bbs[p], m.to());

    // Handle promotion
    if (!m.isPromotion()) // PUTS ORIGINAL PIECE ON THE OLD POSITION
    {
        setBit(bbs[p], m.from());
    }
    else // if IS promotion, but the pawn back.
    {
        setBit(bbs[sideToMove ? 6 : 0], m.from());
    }
    // std::cout << "after undoing move: " << m << "\n";
    // displayBoard();
    moveLog.pop_back();
}

// Is square attacked function

bool Board::isSquareAttacked(int square, int by)
{
    int color = (by ? 0 : 6);
    uint64_t res1 = knight_masks[square] & bbs[1 + color];
    uint64_t res2 = king_masks[square] & bbs[5 + color];
    uint64_t res3 = pawn_masks[by][square] & bbs[color];
    uint64_t res4 = get_rook_attacks(square, occupancies[2]) & ~occupancies[!by];
    uint64_t res5 = get_bishop_attacks(square, occupancies[2]) & ~occupancies[!by];
    return (1ULL << square) & res1 & res2 & res3 & res4 & res5;
}

bool Board::isKingAttacked(int by)
{
    int kingIndex = get_lsb_index(bbs[5 + !by]);
    return isSquareAttacked(kingIndex, by);
}

//
// INITIALIZING
//

void Board::startpos()
{
    setFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

//
//  TESTING
//

// usually, makeMove(int move) has a move parameter, but many times unmakeMove() has no parameter, because it pops the last move made from the move stack

int Board::perft(int depth)
{
    if (depth == 0)
    {
        return 1;
    }
    // std::cout << "STARTING PERFT WITH DEPTH " << depth << "\n";
    int nodes = 0;
    // std::cout << "Generating moves...\n";
    std::array<Move, 256> moveList;
    std::array<Undo, 256> undoList;
    // moveList.fill(Move());
    generateMoves(moveList);
    // std::cout << "Finished generating moves:";
    // displayMoves(moveList);
    for (int i = 0; i < moveList.size(); i++)
    {
        // std::cout << "Started looping over moves...\n";
        if (moveList[i].data == 0)
        {
            // std::cout << "BREAKING at i=" << i << "\n";
            break;
        }

        // if (undoList[i].capturedPiece==11 || undoList[i].capturedPiece==5) {
        //     continue;   // Never runs because undo is initialized in the makeMove(), which didn't run yet.
        // }
        if (findPieceKing(moveList[i].to(), moveList[i])==5 || findPieceKing(moveList[i].to(), moveList[i])==11) {
            continue;
        }

        // std::cout << "1\n";
        // if (moveList[i].from() == 48 && moveList[i].to() == 40)
        // {
        //     //std::cout << "BOARD BEFORE MAKING THAT MOVE\n";
        //     displayBoard();
        // }
        // if (moveList[i].from()==41) {
        //     std::cout << "Board before making that move: "<<moveList[i]<<"\n" ;
        //     displayBoard();
        // }

        // std::string debug_msg = "";
        // for (int iii=0; iii < 6-depth; iii++) {
        //     debug_msg += "   ";
        // }
        // if (depth > 1)
        //     std::cout << debug_msg << "making move: " << moveToCode(moveList[i]) << std::endl;

        makeMove(moveList[i], undoList[i]);
        
        validateBoard(1);
        // if (moveList[i].from() == 48 && moveList[i].to() == 40)
        // {
        //    // std::cout << "BOARD AFTER MAKING THAT MOVE\n";
        //     //displayBoard();
        // }
        // std::cout << "2 made move\n";
        if (!isKingAttacked(!sideToMove))
        {
            nodes += perft(depth - 1);
        }
        // std::cout << "3\n";
        
        // if (depth > 1)
        //     std::cout << debug_msg << "UNmaking move: " << moveToCode(moveList[i]) << std::endl;

        undoMove(moveList[i], undoList[i]);
        
        validateBoard(2);
        // std::cout << "4 unmade move\n";
    }
    return nodes;
}

int Board::perftDivide(int depth)
{
    // std::cout << "Trying to run new perft...\n";
    uint64_t totalNodes = 0;
    if (depth == 0)
    {
        return 1;
    }

    // std::cout << "Initializing new lists...\n";
    std::array<Move, 256> moveList;
    std::array<Undo, 256> undoList;
    // std::cout << "Generating moves...\n";
    generateMoves(moveList);
    // std::cout << "Finished generating moves\n";
    for (int i = 0; i < moveList.size(); i++)
    {
        //std::cout << "Index i=" << i << "\n";
        // std::cout << "Started looping over moves...\n";
        if (moveList[i].data == 0)
        {
            break;
        }
        if (findPieceKing(moveList[i].to(), moveList[i])==5 || findPieceKing(moveList[i].to(), moveList[i])==11) {
            continue;
        }
        //std::cout << "Trying to make move " << moveList[i] << " and undo " << undoList[i] << "\n";
        // std::cout << "1\n";
        // if (moveList[i].from()==41) {
        //     std::cout << "Board before making that move: "<<moveList[i]<<"\n" ;
        //     displayBoard();
        // }
        makeMove(moveList[i], undoList[i]);
        validateBoard(3);
        // std::cout << "2 made move\n";
        //std::cout << "Made move " << moveList[i] << " and undo " << undoList[i] << "\n";
        if (!isKingAttacked(!sideToMove))
        {
            // std::cout << "current depth is " << depth << " and condition 'is king attacked' run successfully for a move " << moveList[i] << " and undo " << undoList[i] << "\n";
            uint64_t nodes = perft(depth - 1);
            std::cout << moveToCode(moveList[i]) << ": " << nodes << std::endl;
            totalNodes += nodes;
        }
        // std::cout << "Doing perft on a lower level with a move " << moveList[i] << " and undo " << undoList[i] << "\n";
        //  std::cout << "3\n";

        //std::cout << "Trying to undo move " << moveList[i] << " and undo " << undoList[i] << "\n";
        undoMove(moveList[i], undoList[i]);
        std::cout<< "Board after unmaking a move "<< moveToCode(moveList[i])<<" :\n";
        displayBoard();
        validateBoard(4);
        //std::cout << "Undid move " << moveList[i] << " and undo " << undoList[i] << "\n";
    }
    std::cout << "\n Total nodes at depth " << depth << ": " << totalNodes << std::endl;
    return totalNodes;
}