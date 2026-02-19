#include "board.h"
#include "piece.h"
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
Board::Board()
{
    Piece empty;
    for (int i = 0; i < 8; i++)
        board[i] = empty;
}

void Board::assignDefaultRow(int row, Color color)
{
    int start = (row - 1) * 8;
    board[start] = Piece(PieceType::Rook, color);
    board[start + 1] = Piece(PieceType::Knight, color);
    board[start + 2] = Piece(PieceType::Bishop, color);
    board[start + 3] = Piece(PieceType::King, color);
    board[start + 4] = Piece(PieceType::Queen, color);
    board[start + 5] = Piece(PieceType::Bishop, color);
    board[start + 6] = Piece(PieceType::Knight, color);
    board[start + 7] = Piece(PieceType::Rook, color);
}

void Board::assignPawns(int row, Color color)
{
    int start = (row - 1) * 8;
    for (int i = start; i < start + 8; i++)
        board[i] = Piece(PieceType::Pawn, color);
}

void Board::startingPosition()
{
    assignDefaultRow(1, Color::White);
    assignPawns(2, Color::White);
    assignPawns(7, Color::Black);
    assignDefaultRow(8, Color::Black);
}

void Board::displayBoard()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            std::cout << board[i * 8 + j] << " ";
        }
        std::cout << "\n";
    }
}
Color Board::checkSpace(int square)
// Returns 0 if empty, 1 if white piece, 2 if black piece.
{
    Piece p = board[square];
    if (p.type == PieceType::None && p.color == Color::None)
    {
        return Color::None;
    }
    else
    {
        return p.color;
    }
}
bool Board::isEndOfTheBoard(int square, Piece p)
{
    if (p.color == Color::White)
    {
        return square > 55;
    }
    else
    {
        return square < 8;
    }
}
bool Board::crossesBorder(int from, int to)
{
    return (from % 7 == 0 && to % 8 == 0) || (to % 7 == 0 && from % 8 == 0);
}
bool Board::crossesBorderKnight(int from, int to)
{
    if (to > 63 || to < 0)
    {
        return true;
    }
    return (                                                //.
        (                                                   //.
            ((from + 1) % 8 == 0 || (from + 2) % 8 == 0) && //.
            (to % 8 == 0 || (to + 1) % 8 == 0)              //.
            )                                               //.
        ||                                                  //.
        (                                                   //.
            ((to + 1) % 8 == 0 || (to + 2) % 8 == 0) &&     //.
            (from % 8 == 0 || (from - 1) % 8 == 0)          // the initial square of 1 evaluates as true
            )                                               //.
    );                                                      //.
}
bool Board::isAlly(Piece p, int to)
{
    return p.color == board[to].color;
}
bool Board::isCapture(Piece p, int to)
{
    // Does NOT handle ally capturing
    return ((p.color != board[to].color) && (board[to].color != Color::None));
}
void Board::handlePawnCapture(int from, int to, Piece p, std::vector<Move> &moves)
{
    if ((checkSpace(to) != p.color) && (checkSpace(to) != Color::None) && !crossesBorder(from, to))
    {
        if (isEndOfTheBoard(to, p))
        {
            moves.push_back(Move(from, to, p, board[to], Piece(PieceType::Queen, p.color), MoveFlag::Promotion));
        }
        else
        {
            moves.push_back(Move(from, to, p, board[to], Piece(), MoveFlag::Capture));
        }
    }
}
bool Board::isOutOfBounds(int to)
{
    return (to > 63 || to < 0);
}
bool Board::crossesBorderBishop(int from, int to, int n, int multiplier)
{
   return ((from%8==0 && to % 8==7) || (from%8==7 && to % 8==0));
}
void Board::FakeMove(Piece p, int to)
{
    board[to] = p;
}
void Board::makeMove(const Move &m)
{
    board[m.from] = Piece();
    if (m.promoted.type != PieceType::None && m.promoted.color != Color::None)
    {
        board[m.to] = m.promoted;
    }
    else
    {
        board[m.to] = m.piece;
    }
}
void Board::undoMove(const Move &m)
{
    // TODO
}
void Board::generateMoves(std::vector<Move> &moves) {};

void Board::generatePawnMoves(int square, std::vector<Move> &moves)
{
    Piece p = board[square];
    int forward;
    int left;
    int right;
    int forward2;

    // Normalizes moving for pawn of both colors
    if (p.color == Color::White)
    {
        forward = square + 8;
        left = square + 9;
        right = square + 7;
        forward2 = square + 16;
        ;
    }
    else
    {
        forward = square - 8;
        left = square - 9;
        right = square - 7;
        forward2 = square - 16;
    }

    // Handles double step
    bool startingPosition;
    if ((square > 7 && square < 16 && p.color == Color::White) || (square > 47 && square < 56 && p.color == Color::Black))
    {
        startingPosition = true;
    }
    else
    {
        startingPosition = false;
    }
    // Handles moving forward
    if (checkSpace(forward) == Color::None)
    {
        if (isEndOfTheBoard(forward, p))
        {
            moves.push_back(Move(square, forward, p, Piece(), Piece(PieceType::Queen, p.color), MoveFlag::Promotion)); // Make default promotion to Queen but later need to change
        }
        else
        {
            moves.push_back(Move(square, forward, p, Piece(), Piece()));
        }
    }

    // Handles double step if pawn is in the starting position
    if (startingPosition && checkSpace(forward2) == Color::None && checkSpace(forward) == Color::None)
    {
        moves.push_back(Move(square, forward2, p, Piece(), Piece()));
    }

    // Handles pawn capturing from the left side
    handlePawnCapture(square, left, p, moves);
    // Handles pawn capturing from the right pawn
    handlePawnCapture(square, right, p, moves);
    // Check for border
}
void Board::generateKnightMoves(int square, std::vector<Move> &moves)
{
    Piece p = board[square];
    // -17, -15,-10, -6, 6, 10, 15, 17
    int nums[8] = {-17, -15, -10, -6, 6, 10, 15, 17};
    for (int n : nums)
    {
        if ((!crossesBorderKnight(square, square + n)) && (!isAlly(p, square + n)))
        {
            if (isCapture(p, square + n))
            {
                moves.push_back(Move(square, square + n, p, board[square + n], Piece(), MoveFlag::Capture));
            }
            else
            {
                moves.push_back(Move(square, square + n, p, board[square + n], Piece()));
            }
        }
    }
}
void Board::generateBishopMoves(int square, std::vector<Move> &moves)
{
    Piece p = board[square];
    std::array<int, 4> nums = {-9, -7, 7, 9};
    // +-  7, 9 and its multiples
    // Stop sliding when hit any piece. Capture if enemy
    for (int n : nums)
    {
        int mult=1;
        while (true) {
        int final=square+n*mult;
        // Do all the checks(if fails, break the loop)
        // add the element in the loop
        if (isOutOfBounds(final)) {
            break;
        }
        if (isAlly(p, final)) {
            break;
        }
        if (crossesBorderBishop(square, final, n, mult)) {
            break;
        }
        if (isCapture(p, final)) {
            moves.push_back(Move(square, final, p, board[final], Piece(), MoveFlag::Normal));
        } else {
            moves.push_back(Move(square, final, p, board[final], Piece(), MoveFlag::Capture));
        }
        mult++;
    }
    }
}
void Board::generateRookMoves(int square, std::vector<Move> &moves) {}
void Board::generateQueenMoves(int square, std::vector<Move> &moves) {}
void Board::generateKingMoves(int square, std::vector<Move> &moves) {}