#include <array>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <unordered_map>

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
        this->castlingRights = castlingRights;
        this->enPassantSquare = enPassantSquare;
        this->halfMoveClock = halfMoveClock;
        this->fullMoveClock = fullMoveClock;
    }
};

class Board
{
    // a1 = 0, h8 - 63;
    std::array<uint64_t, 12> bitboards;
    // 0-5 - white pieces. 0 - pawns, 1 - knights, 2 - bishops, 3 - rooks, 4 - queen, 5 - king
    // 6-11 - black pieces. 6 - pawns, 7 - knights, 8 - bishops, 9 - rooks, 10 - queen, 11 - king
    std::array<uint64_t, 3> occupancies;
    // 0 - all white pieces, 1 - all black pieces, 2 - all pieces

    uint8_t castlingRights = 0b0000; // 1st - white king, 2nd - white queen, 3rd - black king, 4th - black queen

    int enPassantSquare = -1;

    int halfMoveClock = 0;
    int fullMoveClock = 1;

    bool whiteToMove = true;

    Board()
    {
        bitboards.fill(0ULL);
        occupancies.fill(0ULL);
        uint8_t castlingRights = 0b0000;
        int enPassantSquare = -1;
        int halfMoveClock = 0;
        int fullMoveClock = 1;
        bool whiteToMove = true;
    }

    //
    //      Helper Methods:
    //
    void setBit(int index, int square)
    {
        bitboards[index] |= (1ULL << square);
    }

    void clearBit(int index, int square)
    {
        bitboards[index] &= ~(1ULL << square);
    }

    bool isBitSet(int index, int square)
    {
        return (bitboards[index] & (1ULL << square)) > 0;
    }
    uint64_t get_lsb(uint64_t bb)
    {
        // return (bb & ((~bb) + 1));
        return __builtin_ctzll(bb);
    }
    uint64_t pop_lsb(uint64_t &bb)
    {
        int square = __builtin_ctzll(bb);
        bb &= bb - 1;
        return square;
    }
    inline int popcount(uint64_t bb)
    {
        // int count=0;
        // while (bb) {
        //     bb&=bb-1;
        //     count++;
        // }
        return __builtin_popcount(bb);
    }

    //
    // FEN
    //

    std::vector<std::string> splitString(std::string str, char delimiter)
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

    int codeToIndex(std::string code)
    {
        char file = code[0] - 'a';
        char rank = code[1] - '1';
        return rank * 8 + file;
    }

    std::string indexToCode(int index)
    {
        int file = index % 8;
        int rank = index / 8;
        std::string s;
        s += char('a' + file);
        s += char('1' + rank);
        return s;
    }

    std::string getFEN()
    {
        return " ";
    }

    void setFEN(std::string s)
    {
        std::vector<std::string> vec = splitString(s, ' ');
        std::vector<std::string> rows = splitString(vec[0], '/');

        // Clear prev position:
        bitboards.fill(0ULL);
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
                    setBit(piece + color, i);
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
    void updateOccupancies()
    {
        occupancies[0] = 0ULL;
        occupancies[1] = 0ULL;
        occupancies[2] = 0ULL;
        for (int i = 0; i <= 5; i++)
        {
            occupancies[0] |= bitboards[i];
        }
        for (int i = 6; i <= 11; i++)
        {
            occupancies[1] |= bitboards[i];
        }
        occupancies[2] = occupancies[0] | occupancies[1];
    }

    //
    // Move representation
    //

    //
    //  First 6 bits - FROM square, next 6 bits - TO square, 3 bits - piece type that just moved,
    //  3 bits - piece type that got captured, 3 bits - promotion piece, and the rest 11 are for move flags
    //
};
