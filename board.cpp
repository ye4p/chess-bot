#include "board.h"
#include "piece.h"
#include <iostream>
#include <vector>
#include <array>
#include <movehistory.h>
Board::Board()
{
    Piece empty;
    for (int i = 0; i < 8; i++)
        board[i] = empty;
    GameState state = GameState();
    std::vector<MoveHistory> history;
}
Piece Board::getPiece(int square)
{
    return board[square];
}
int Board::getEnPassantSquare()
{
    return state.enPassantSquare;
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
Color Board::flipSideToMove()
{
    if (state.sideToMove == Color::White)
    {
        state.sideToMove = Color::Black;
    }
    else
    {
        state.sideToMove = Color::White;
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
    return (                                               //
        (from % 8 == 6 && to % 8 == 0) ||                  //
        (from % 8 == 7 && (to % 8 == 0 || to % 8 == 1)) || //
        (from % 8 == 1 && to % 8 == 7) ||                  //
        (from % 8 == 0 && (to % 8 == 6 || to % 8 == 7))    //
                                                           //
    );                                                     //
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
    // TODO: add en passant logic
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
    return (                                         //
        (((from + (multiplier - 1) * n) % 8 == 0) && //
         (to % 8 == 7))                              //
        ||                                           //
        (((from + (multiplier - 1) * n) % 8 == 7) && //
         (to % 8 == 0))                              //
    );                                               //
}

bool Board::crossesBorderPawn(int from, int to)
{
    return (
        ((from % 8 == 0) && (to % 8 == 7)) || ((to % 8 == 0) && (from % 8 == 7)));
}

bool Board::isEnemy(Piece p, int where)
{
    return (p.color != board[where].color && board[where].color != Color::None);
}

bool Board::canEnPassant(int square)
{
    Piece p = board[square];
    if (state.enPassantSquare == -1)
    {
        return false;
    }
    if ((square - 1) % 8 != 7 && board[square - 1].type == PieceType::Pawn && isEnemy(p, square - 1))
    {
        return true;
    }
    if ((square + 1) % 8 != 0 && board[square + 1].type == PieceType::Pawn && isEnemy(p, square + 1))
    {
        return true;
    }
    return false;
}

void Board::FakeMove(Piece p, int to)
{
    board[to] = p;
}

void Board::makeMove(const Move &m)
{

    // Saving move into memory stack:
    MoveHistory h = MoveHistory(m.from, m.to, m.piece, m.captured, m.promoted, m.flag, state.enPassantSquare, state.whiteCastleKingSide, state.whiteCastleQueenSide, state.blackCastleKingSide, state.blackCastleQueenSide, state.halfMoveCount);
    history.push_back(h);
    //
    //          1) BOARD MANIPULATION
    //

    board[m.from] = Piece();
    // Handles promotion
    if (m.promoted.type != PieceType::None && m.promoted.color != Color::None)
    {
        board[m.to] = m.promoted;
    }
    else
    {
        board[m.to] = m.piece;
    }
    // Handles eating pawn with en passant
    if (state.enPassantSquare != -1)
    {
        int back;
        if (m.piece.color == Color::White)
        {
            back = m.to - 8;
        }
        else
        {
            back = m.to + 8;
        }
        board[back] = Piece();
    }
    // If the move is pawn double step, it sets en passant square as the piece's square
    if (m.piece.type == PieceType::Pawn && (std::abs(m.to - m.from) == 16))
    {
        state.enPassantSquare = m.to;
    }
    else
    {
        state.enPassantSquare = -1;
    }

    // Handle castle
    if (m.piece.type == PieceType::King)
    {
        int finalRook;
        int normalizer;
        if ((m.to - m.from) == -2)
        {
            normalizer = m.piece.color == Color::White ? 0 : 56;
            finalRook = 3 + normalizer;
            board[normalizer] = Piece();
            board[finalRook] = Piece(PieceType::Rook, m.piece.color);
        }
        else if ((m.to - m.from) == 2)
        {
            normalizer = m.piece.color == Color::White ? 0 : 56 + 7;
            finalRook = -2 + normalizer;
            board[normalizer] = Piece();
            board[finalRook] = Piece(PieceType::Rook, m.piece.color);
        }
    }

    //
    //          2) MANAGE STATES
    //

    // 2.1 Manipulating castling rights if rook/king are moved
    if (m.piece.type == PieceType::Rook)
    {
        if (m.piece.color == Color::White)
        {
            if (m.from == 0)
            {
                state.whiteCastleQueenSide = false;
            }
            else if (m.from == 7)
            {
                state.whiteCastleKingSide = false;
            }
        }
        else
        {
            if (m.from == 56)
            {
                state.blackCastleQueenSide = false;
            }
            else if (m.from == 63)
            {
                state.blackCastleKingSide = false;
            }
        }
    }
    if (m.captured.type == PieceType::Rook)
    {
        if (m.to == 0)
            state.whiteCastleQueenSide = false;
        if (m.to == 7)
            state.whiteCastleKingSide = false;
        if (m.to == 56)
            state.blackCastleQueenSide = false;
        if (m.to == 63)
            state.blackCastleKingSide = false;
    }

    if (m.piece.type == PieceType::King)
    {
        if (m.piece.color == Color::White)
        {
            state.whiteCastleKingSide = false;
            state.whiteCastleQueenSide = false;
        }
        else
        {
            state.blackCastleKingSide = false;
            state.blackCastleQueenSide = false;
        }
    }

    // 2.2 Manages other state properties
    if (m.piece.type == PieceType::Pawn || m.flag == MoveFlag::Capture)
    {
        state.halfMoveCount = 0;
    }
    else
    {
        state.halfMoveCount++;
    }
    if (m.piece.color == Color::Black)
    {
        state.fullMoveCount++;
    }
    flipSideToMove();
}

void Board::undoMove(const Move &m)
{
    flipSideToMove();
    if (history.empty())
    {
        throw std::invalid_argument("undoMove() requires history vector to NOT be empty");
    }
    board[m.to] = m.captured;
    board[m.from] = m.piece;
    if (m.flag == MoveFlag::Promotion)
    {
        board[m.from] = Piece(PieceType::Pawn, m.piece.color);
    }
    if (m.piece.type == PieceType::Pawn && (std::abs(m.to - m.from) == 16))
    {
        state.enPassantSquare = -1;
    }
    flipSideToMove();
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
    if (!crossesBorderPawn(square, left))
    {
        handlePawnCapture(square, left, p, moves);
    }
    // Handles pawn capturing from the right pawn
    if (!crossesBorderPawn(square, right))
    {
        handlePawnCapture(square, right, p, moves);
    }
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

void Board::generateGeometryMoves(int square, std::vector<Move> &moves, std::vector<int> nums)
{
    Piece p = board[square];
    for (int n : nums)
    {
        int mult = 1;
        while (true)
        {
            int final = square + n * mult;
            if (isOutOfBounds(final))
            {
                break;
            }
            if (isAlly(p, final))
            {
                break;
            }
            if (crossesBorderBishop(square, final, n, mult))
            {
                break;
            }
            if (isCapture(p, final))
            {
                moves.push_back(Move(square, final, p, board[final], Piece(), MoveFlag::Normal));
            }
            else
            {
                moves.push_back(Move(square, final, p, board[final], Piece(), MoveFlag::Capture));
            }
            if (p.type == PieceType::King)
            {
                break;
            }
            mult++;
        }
    }
}

void Board::generateBishopMoves(int square, std::vector<Move> &moves)
{
    std::vector<int> nums = {-9, -7, 7, 9};
    generateGeometryMoves(square, moves, nums);
}

void Board::generateRookMoves(int square, std::vector<Move> &moves)
{
    std::vector<int> nums = {-8, -1, 1, 8};
    generateGeometryMoves(square, moves, nums);
}

void Board::generateQueenMoves(int square, std::vector<Move> &moves)
{
    std::vector<int> nums = {-9, -8, -7, -1, 1, 7, 8, 9};
    generateGeometryMoves(square, moves, nums);
}

void Board::generateKingMoves(int square, std::vector<Move> &moves)
{
    std::vector<int> nums = {-9, -8, -7, -1, 1, 7, 8, 9};
    generateGeometryMoves(square, moves, nums);
}

bool Board::isSquareAttacked(int square)
{
    Piece p = board[square];
    std::array<int, 8> numsKnight = {-17, -15, -10, -6, 6, 10, 15, 17};
    for (int n : numsKnight)
    {
        int check = square + n;
        if (!isOutOfBounds(check))
        {
            if (!crossesBorderKnight(check, square) && isEnemy(p, check))
            {
                if (board[check].type == PieceType::Knight)
                {
                    // std::cout << "Crosses border knight:" << crossesBorderKnight(check, square) << "\n";
                    return true;
                }
            }
        }
    }
    std::array<int, 4> numsBishop = {-9, -7, 7, 9};
    for (int n : numsBishop)
    {
        int mult = 1;
        while (true)
        {
            int check = square + n * mult;
            if (isOutOfBounds(check))
            {
                break;
            }
            if (crossesBorderBishop(square, check, n, mult))
            { // If doesn't work can just change to when its on the borders (?)
                break;
            }
            if (board[check].color == p.color)
            {
                break; // Breaks if there is an ally on this square
            }
            if (board[check].type == PieceType::Bishop || board[check].type == PieceType::Queen)
            {
                return true;
            }
            mult++;
        }
    }
    std::array<int, 4> numsRook = {-8, -1, 1, 8};
    for (int n : numsRook)
    {
        int mult = 1;
        while (true)
        {
            int check = square + n * mult;
            if (isOutOfBounds(check))
            {
                break;
            }
            if (crossesBorderBishop(square, check, n, mult))
            { // If doesn't work can just change to when its on the borders (?)
                break;
            }
            if (board[check].color == p.color)
            {
                break; // Breaks if there is an ally on this square
            }
            if (board[check].type == PieceType::Rook || board[check].type == PieceType::Queen)
            {
                return true;
            }
            mult++;
        }
    }

    // Add checking for enemy pawn
    // Normalizes moving for pawn of both colors
    int left, right;
    if (p.color == Color::White)
    {
        left = square - 9;
        right = square - 7;
    }
    else
    {
        left = square + 9;
        right = square + 7;
    }
    std::vector<int> numsPawn;
    if (!isOutOfBounds(left))
    {
        numsPawn.push_back(left);
    }
    if (!isOutOfBounds(right))
    {
        numsPawn.push_back(right);
    }
    for (int n : numsPawn)
    {
        if (board[n].color == p.color || board[n].color == Color::None)
        {
            break;
        }
        if (crossesBorderPawn(n, square))
        {
            break;
        }
        if (board[n].type == PieceType::Pawn)
        {
            return true;
        }
    }

    // Add checking for en passant ONLY if p is pawn itself
    if (p.type == PieceType::Pawn && state.enPassantSquare != -1)
    {
        if (canEnPassant(square))
        {
            return true;
        }
    }
    return false;
}