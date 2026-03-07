#include "move.h"
#include "board.h"
#include <string>
Move::Move(int f, int t, Piece piece, Piece captured, Piece promoted, MoveFlag flag)
    : from(f), to(t), piece(piece), captured(captured), promoted(promoted), flag(flag)
{
}
std::ostream &operator<<(std::ostream &os, const MoveFlag flag)
{
    switch (flag)
    {
    case MoveFlag::Normal:
        return os << "Normal";
    case MoveFlag::Capture:
        return os << "Capture";
    case MoveFlag::DoublePawn:
        return os << "DoublePawn";
    case MoveFlag::KingCastle:
        return os << "KingCastle";
    case MoveFlag::QueenCastle:
        return os << "QueenCastle";
    case MoveFlag::EnPassant:
        return os << "EnPassant";
    case MoveFlag::Promotion:
        return os << "Promotion";
    }
    return os << "Unknown";
}
int Move::codeToIndex(std::string code)
{
    char let = code[0];
    char num = code[1];
    int index = 0;
    switch (let)
    {
    case 'a':
        index += 7;
        break;
    case 'b':
        index += 6;
        break;
    case 'c':
        index += 5;
        break;
    case 'd':
        index += 4;
        break;
    case 'e':
        index += 3;
        break;
    case 'f':
        index += 2;
        break;
    case 'g':
        index += 1;
        break;
    default:
        break;
    }
    // std::cout << "Reading number: " << num << "\n";
    int numConverted = num - '0';
    index = index + ((numConverted - 1) * 8);
    // std::cout << "Multiplication of num by 3: " << num*3 << "\n";
    return index;
}
std::string Move::indexToCode(int index)
{
    std::string whole = std::to_string(((index / 8) + 1));
    int remainder = index % 8;
    std::string let = "";
    switch (remainder)
    {
    case 0:
        let = "h";
        break;
    case 1:
        let = "g";
        break;
    case 2:
        let = "f";
        break;
    case 3:
        let = "e";
        break;
    case 4:
        let = "d";
        break;
    case 5:
        let = "c";
        break;
    case 6:
        let = "b";
        break;
    default:
        let = "a";
        break;
    }
    return (let + whole);
}
void Move::displayMove()
{
    std::cout << indexToCode(from) + indexToCode(to) << "\n";
}
std::string Move::getMoveCode()
{
    return indexToCode(from) + indexToCode(to);
}
std::ostream &operator<<(std::ostream &os, const Move &m)
{
    os << " Move from square " << m.from << " To square " << m.to << " 1)  Piece moved: " << m.piece << " 2) Piece captured: " << m.captured << " 3) Piece Promoted: " << m.promoted << ", Move's flag: " << m.flag;
    return os;
}