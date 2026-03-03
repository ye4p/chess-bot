#include "board.h"
#include "piece.h"
#include "gamestate.h"
#include "movehistory.h"
#include <iostream>
#include <vector>
#include <array>
#include <cstdint>
#include <sstream>
#include <cctype>

Board::Board()
{
    Piece empty;
    for (int i = 0; i < 8; i++)
        board[i] = empty;
    GameState state;
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
void Board::setSideToMove(Color c)
{
    state.sideToMove == c;
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
std::vector<std::string> Board::splitString(std::string str, char delimiter) {
    std::vector <std::string> tokens;
    std::string token;
    std::stringstream ss(str);
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
std::string Board::getFEN() {

}

void Board::setFEN(std::string s) {
    std::vector <std::string> vec=splitString(s, ' ');
    std::vector <std::string> rows =splitString(vec[0], '/');
    int i =63;
    for (std::string row : rows) {
        for (char let : row) {
            
            // Handle letters
            if (std::isalpha(let)) {
                Color color= (std::isupper(let) ? Color::White : Color::Black);
                PieceType piece;
                if (std::tolower(static_cast<unsigned char>(let))=='p') {
                    piece=PieceType::Pawn;
                } else if (std::tolower(static_cast<unsigned char>(let))=='n') {
                    piece=PieceType::Knight;
                } else if (std::tolower(static_cast<unsigned char>(let))=='b')  {
                    piece=PieceType::Bishop;
                } else if (std::tolower(static_cast<unsigned char>(let))=='r') {
                    piece=PieceType::Rook;
                } else if (std::tolower(static_cast<unsigned char>(let))=='k') {
                    piece=PieceType::King;
                } else {
                    piece = PieceType::Queen;
                }
                board[i]=Piece(piece, color);
                i--;
            }
            // Handle numbers
            if (std::isdigit(let)) {
                i-=let;
            }
        }
    }
    
    // Check which color needs to move
    if (vec[1]=="w") {
        state.sideToMove=Color::White;
    } else {
        state.sideToMove=Color::Black;
    }

    //Handle castling rights
    state=GameState(false);
    for (char let : vec[2]) {
        if (static_cast<unsigned char>(let)=='K') {
            state.whiteCastleKingSide=  true;  
        } else if (static_cast<unsigned char>(let)=='k') {
            state.blackCastleKingSide=true;
        } else if ((static_cast<unsigned char>(let)=='Q')) {
            state.whiteCastleQueenSide=true;
        } else if ((static_cast<unsigned char>(let)=='q')) {
            state.blackCastleQueenSide=true;
        }
    }

    // Handle en passant square
    char let=vec[3][0];
    char num=vec[3][1];
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
void Board::flipSideToMove()
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
    if (isOutOfBounds(m.from) || isOutOfBounds(m.to))
    {
        throw std::invalid_argument("One of the numbers in move is out of bounds ");
    }
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
    // if (state.enPassantSquare != -1) -- as I understand, this makes it think that every move after double step from pawn is en passant
    if (m.flag == MoveFlag::EnPassant)
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
        if (isOutOfBounds(back))
        {
            throw std::invalid_argument("var 'back' is out of bounds ");
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
    MoveHistory h = history.back(); // Get the last element of the vector
    // Clear the final square by default
    board[h.to] = Piece();
    flipSideToMove();
    if (history.empty())
    {
        std::cout << "ERROR: EMPTY VECTOR OF MOVES\n";
        throw std::invalid_argument("undoMove() requires history vector to NOT be empty");
    }

    if (h.moved.color == Color::Black)
    {
        state.fullMoveCount--;
    }
    state.halfMoveCount = h.prevHalfMoveCount;
    state.whiteCastleKingSide = h.prevWhiteCastleKingSide;
    state.whiteCastleQueenSide = h.prevWhiteCastleQueenSide;
    state.blackCastleKingSide = h.prevBlackCastleKingSide;
    state.blackCastleQueenSide = h.prevBlackCastleQueenSide;
    state.enPassantSquare = h.prevEnPassantSquare;
    // Handle promotion
    if (h.promoted.type != PieceType::None && h.promoted.color != Color::None)
    {
        board[h.to] = Piece();
    }
    // Handle capture
    if (h.flag == MoveFlag::Capture)
    {
        board[h.to] = h.captured;
    }
    // Handle en passant
    else if (h.flag == MoveFlag::EnPassant)
    {
        int dir;
        if (h.moved.color == Color::White)
        {
            dir = h.to - 8;
        }
        else
        {
            dir = h.to + 8;
        }
        board[dir] = h.captured;
    }
    // Handle castling
    if (h.moved.type == PieceType::King && std::abs(h.to - h.from) == 2)
    {
        // int rookPos=h.to + ((h.to - h.from) == 2 ? -1 : 1);
        int rookPos;
        int rookInit;
        if ((h.to - h.from) == 2)
        {
            rookPos = h.to - 1;
            rookInit = h.to + 2;
        }
        else
        {
            rookPos = h.to + 1;
            rookInit = h.to - 2;
        }
        board[rookPos] = Piece();
        board[rookInit] = Piece(PieceType::Rook, h.moved.color);
    }
    // Move piece back
    board[h.from] = h.moved;

    // Pop the history entry
    history.pop_back();
}
void Board::generateMoves(std::vector<Move> &moves)
{
    // Check side to move:
    Color c = state.sideToMove;
    for (int i = 0; i < 64; i++)
    {
        Piece p = board[i];
        if (p.color == c)
        {
            if (board[i].type == PieceType::Pawn)
            {
                generatePawnMoves(i, moves);
            }
            else if (p.type == PieceType::Knight)
            {
                generateKnightMoves(i, moves);
            }
            else if (p.type == PieceType::Bishop)
            {
                generateBishopMoves(i, moves);
            }
            else if (p.type == PieceType::Rook)
            {
                generateRookMoves(i, moves);
            }
            else if (p.type == PieceType::King)
            {
                generateKingMoves(i, moves);
            }
            else if (p.type == PieceType::Queen)
            {
                generateQueenMoves(i, moves);
            }
        }
    }
};

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
                moves.push_back(Move(square, final, p, board[final], Piece(), MoveFlag::Capture));
            }
            else
            {
                moves.push_back(Move(square, final, p, board[final], Piece(), MoveFlag::Normal));
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
bool Board::isKingInCheck(Color c)
{
    for (int i = 0; i < 64; i++)
    {
        if (board[i].type == PieceType::King && board[i].color == c)
        {
            if (isSquareAttacked(i))
            {
                return true;
            }
        }
    }
    return false;
}
void Board::filterLegalMoves(const std::vector<Move> &pseudo, std::vector<Move> &legal)
{
    for (const Move &m : pseudo)
    {
        //std::cout << "Trying move m (inside of filterLegalMoves): " << m << "\n";
        makeMove(m);
        //std::cout << "succesfully performed move m (inside of filterLegalMoves): " << m << "\n";
        if (!isKingInCheck(state.sideToMove == Color::White ? Color::Black : Color::White))
        {
            legal.push_back(m);
        }
        undoMove(m);
    }
}
uint64_t Board::perft(int depth)
{
    // std::cout << "Got to perft on depth " << depth << "\n";
    if (depth == 0)
    {
        return 1;
    }
    // std::cout << "(2) Perf testing on depth " << depth << "\n";
    uint64_t nodes = 0;
    std::vector<Move> pseudo;
    std::vector<Move> legal;
    //std::cout << "Attempt to generate pseudo legal moves...\n";
    generateMoves(pseudo);
    //std::cout << "Attempt to filter legal moves...\n";
    filterLegalMoves(pseudo, legal);
    //std::cout << "Successfully filtered legal moves\n";
    for (const Move &move : legal)
    {
        //std::cout << "Trying move Move: " << move << "\n";
        makeMove(move);
        //std::cout << "Performed move: " << move << "\n";
        if (!isKingInCheck(state.sideToMove == Color::White ? Color::Black : Color::White))
        {
            //std::cout << "Performing recursive call... \n";
            nodes += perft(depth - 1);
            //std::cout << " nodes var changed to: " << nodes;
            //std::cout << " Legality checked Move: " << move << "\n";
        }
        undoMove(move);
        // std::cout << "Undoing move: " << move << "\n";
    }
    return nodes;
}