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

Undo::Undo(uint8_t castlingRights, int8_t enPassantSquare, uint8_t halfMoveClock, uint64_t fullMoveClock)
{
    this->castlingRights = castlingRights;
    this->enPassantSquare = enPassantSquare;
    this->halfMoveClock = halfMoveClock;
    this->fullMoveClock = fullMoveClock;
}

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
uint64_t Board::get_lsb(uint64_t bb)
{
    // return (bb & ((~bb) + 1));
    return __builtin_ctzll(bb);
}
uint64_t Board::pop_lsb(uint64_t &bb)
{
    int square = __builtin_ctzll(bb);
    bb &= bb - 1;
    return square;
}
inline int Board::popcount(uint64_t bb)
{
    // int count=0;
    // while (bb) {
    //     bb&=bb-1;
    //     count++;
    // }
    return __builtin_popcount(bb);
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
                int fullPiece = piece + color;
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
    // attack bb
    uint64_t attack = 0ULL;
    // piece bb
    uint64_t bb = 0ULL;
    // set piece on bb
    // set_bit()
    setBit(bb, square);
    displayBoard(bb);
    displayBoard(bb << 7);
    if (!side)
    {
        attack |= (bb << 7);
    }
    else
    {
    }
    return 0;
}

void Board::generateMoves()
{
}
void Board::generateKnightMoves()
{
}
void Board::generateKingMoves()
{
}
void Board::generateSlidingMoves()
{
}
