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
    for (int i = 0; i < 64; i++)
        board[i] = empty;
    state = GameState();
    history.clear();
}
Piece &Board::getPiece(int square)
{
    return board[square];
}
int Board::getEnPassantSquare()
{
    return state.enPassantSquare;
}
void Board::setSideToMove(Color c)
{
    state.sideToMove = c;
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
std::string Board::indexToCode(int index)
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
std::string Board::getFEN()
{
    return " ";
}

void Board::setFEN(std::string s)
{
    // std::cout << "setFEN method started\n";
    std::vector<std::string> vec = splitString(s, ' ');
    std::vector<std::string> rows = splitString(vec[0], '/');
    // std::cout << "Splitted the strings successfully\n";
    int i = 63;
    // Handle position of pieces on the board.
    state = GameState(false);
    for (std::string row : rows)
    {
        for (char let : row)
        {
            // std::cout<< "Handling letter " << let << "\n";
            //  Handle letters
            if (std::isalpha(let))
            {
                Color color = (std::isupper(let) ? Color::White : Color::Black);
                PieceType piece;
                if (std::tolower(static_cast<unsigned char>(let)) == 'p')
                {
                    // std::cout<< "pawn found\n";
                    piece = PieceType::Pawn;
                }
                else if (std::tolower(static_cast<unsigned char>(let)) == 'n')
                {
                    piece = PieceType::Knight;
                }
                else if (std::tolower(static_cast<unsigned char>(let)) == 'b')
                {
                    piece = PieceType::Bishop;
                }
                else if (std::tolower(static_cast<unsigned char>(let)) == 'r')
                {
                    piece = PieceType::Rook;
                }
                else if (std::tolower(static_cast<unsigned char>(let)) == 'k')
                {
                    piece = PieceType::King;
                }
                else
                {
                    piece = PieceType::Queen;
                }
                // std::cout << " current index: " << i << "\n";
                board[i] = Piece(piece, color);
                i--;
            }
            // Handle numbers
            if (std::isdigit(let))
            {
                int letConverted = let - '0';
                // std::cout<< "Let converted: " <<letConverted << "\n";
                i -= letConverted;
                // std::cout<< "index after hangling number: "<< i<<"\n";
            }
        }
    }
    // std::cout << " Finished running pieces handling\n";

    // Check which color needs to move
    // std::cout << "vec[1]: "<<vec[1]<<" vec[1] == 'w' result is: " << (vec[1] == "w") <<"\n";
    if (vec[1] == "w")
    {
        state.sideToMove = Color::White;
    }
    else
    {
        state.sideToMove = Color::Black;
    }
    // std::cout << " Finished setting color of side that is supposed to move, and it is "<< getSideToMove() << "\n";

    // Handle castling rights
    for (char let : vec[2])
    {
        if (static_cast<unsigned char>(let) == 'K')
        {
            state.whiteCastleKingSide = true;
        }
        else if (static_cast<unsigned char>(let) == 'k')
        {
            state.blackCastleKingSide = true;
        }
        else if ((static_cast<unsigned char>(let) == 'Q'))
        {
            state.whiteCastleQueenSide = true;
        }
        else if ((static_cast<unsigned char>(let) == 'q'))
        {
            state.blackCastleQueenSide = true;
        }
    }
    // std::cout << " Finished handling castling rights\n";

    // Handle en passant square
    int squareEnPassant = codeToIndex(vec[3]);
    if (vec[3] == "-")
    {
        state.enPassantSquare = -1;
    }
    else
    {
        state.enPassantSquare = squareEnPassant;
    }
    // std::cout << " Finished handling en passant square\n";

    // Halfmove clock
    if (vec.size() > 4)
    {
        state.halfMoveCount = std::stoi(vec[4]);

        // Fullmove count
        state.fullMoveCount = std::stoi(vec[5]);
    }
    // std::cout << " Finished half/fullmove clock\n";

    // std::cout << "Side to mive is set to : " << getSideToMove() << "\n";
}

void Board::displayBoardPieces()
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

void Board::displayBoard()
{
    for (int i = 0; i < 64; i++)
    {
        Piece p = board[i];
        if (p.color == Color::White)
        {
            if (p.type == PieceType::Pawn)
            {
                std::cout << "P ";
            }
            else if (p.type == PieceType::Knight)
            {
                std::cout << "N ";
            }
            else if (p.type == PieceType::Bishop)
            {
                std::cout << "B ";
            }
            else if (p.type == PieceType::Rook)
            {
                std::cout << "R ";
            }
            else if (p.type == PieceType::King)
            {
                std::cout << "K ";
            }
            else if (p.type == PieceType::Queen)
            {
                std::cout << "Q ";
            }
        }
        else if (p.color == Color::Black)
        {
            if (p.type == PieceType::Pawn)
            {
                std::cout << "p ";
            }
            else if (p.type == PieceType::Knight)
            {
                std::cout << "n ";
            }
            else if (p.type == PieceType::Bishop)
            {
                std::cout << "b ";
            }
            else if (p.type == PieceType::Rook)
            {
                std::cout << "r ";
            }
            else if (p.type == PieceType::King)
            {
                std::cout << "k ";
            }
            else if (p.type == PieceType::Queen)
            {
                std::cout << "q ";
            }
        }
        else
        {
            std::cout << "  ";
        }
        if (i % 8 == 7)
        {
            std::cout << "\n";
        }
    }
}

void Board::displayCastlingRights()
{
    std::cout << "White king side: " << state.whiteCastleKingSide << std::endl;
    std::cout << "White queen side: " << state.whiteCastleQueenSide << std::endl;
    std::cout << "Black king side: " << state.blackCastleKingSide << std::endl;
    std::cout << "Black queen side: " << state.blackCastleQueenSide << std::endl;
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
Color Board::getSideToMove()
{
    return state.sideToMove;
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
Color Board::reverseColor(Color c)
{
    if (c == Color::White)
    {
        return Color::Black;
    }
    else if (c == Color::Black)
    {
        return Color::White;
    }
    else
    {
        std::cout << " Error with attempt to change colors";
        throw "Attempt to change color::None";
    }
}

bool Board::isCapture(Piece p, int to)
{
    // Does NOT handle ally capturing
    return ((p.color != board[to].color) && (board[to].color != Color::None));
}

void Board::handlePawnCapture(int from, int to, Piece p, std::vector<Move> &moves)
{
    if (to == state.enPassantSquare)
    { // EN PASSANT LOGIC
        int back = to + (p.color == Color::White ? -8 : +8);
        if (isEnemy(p, back))
        {
            moves.push_back(Move(from, to, p, board[back], Piece(), MoveFlag::EnPassant));
            // board[back] = Piece(); // This empties the spot where the pawn was en passant'ed
            return;
        }
    }
    // if ((checkSpace(to) != p.color) && (checkSpace(to) != Color::None) && !crossesBorder(from, to))
    if (board[to].color != Color::None && board[to].color != p.color)
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
    int left, right;
    left = square + (p.color == Color::White ? +9 : -9);
    right = square + (p.color == Color::White ? +7 : -7);
    int back = state.enPassantSquare + (p.color == Color::White ? -8 : +8);
    if (!crossesBorderPawn(square, right) && right == state.enPassantSquare && isEnemy(p, back))
    {
        return true;
    }
    if (!crossesBorderPawn(square, left) && left == state.enPassantSquare && isEnemy(p, back))
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
        int back = m.to + ((m.piece.color == Color::White) ? -8 : +8);

        if (isOutOfBounds(back))
        {
            throw std::invalid_argument("var 'back' is out of bounds ");
        }
        board[back] = Piece();
    }
    // If the move is pawn double step, it sets en passant square as the piece's square
    if (m.piece.type == PieceType::Pawn && (std::abs(m.to - m.from) == 16))
    {
        // NORMALIZE AND FIND SQUARE BEHING A PAWN POSITION NOT ON IT
        int back = m.to + (m.piece.color == Color::White ? -8 : +8);
        state.enPassantSquare = back;
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
            finalRook = 2 + normalizer;
            board[normalizer] = Piece();
            board[finalRook] = Piece(PieceType::Rook, m.piece.color);
        }
        else if ((m.to - m.from) == 2)
        {
            normalizer = (m.piece.color == Color::White ? 0 : 56) + 7;
            finalRook = -3 + normalizer;
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
    if (history.empty())
    {
        std::cout << "ERROR: EMPTY VECTOR OF MOVES\n";
        throw std::invalid_argument("undoMove() requires history vector to NOT be empty");
    }
    MoveHistory h = history.back(); // Get the last element of the vector
    // Clear the final square by default
    board[h.to] = Piece();
    flipSideToMove();

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
        // std::cout << "UNDOING EN PASSANT\n";
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
        if ((h.to - h.from) == -2)
        {
            rookPos = h.to + 1;
            rookInit = h.to - 1;
        }
        else
        {
            rookPos = h.to - 1;
            rookInit = h.to + 2;
        }
        // std::cout <<"rookPos: "<<rookPos<<std::endl;
        // std::cout << "rookInit: "<<rookInit<<std::endl;
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
                // std::cout<<"Pawn moves are being generated\n";
                // std::cout <<"En passant square:" << state.enPassantSquare<<std::endl;
                generatePawnMoves(i, moves);
                // std::cout<<"Board after pawn moves are generated: \n";
                // displayBoard();
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
            // std::cout << "Board after generating for piece " << i << "\n";
            // displayBoard();
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
        if (!isOutOfBounds(square + n) && (!crossesBorderKnight(square, square + n)) && (!isAlly(p, square + n)))
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
    // PieceType type = p.type;
    for (int n : nums)
    {
        int mult = 1;
        while (true)
        {
            //  displayBoard();
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
                // std::cout << "Breaking geometrymoves method with a move from " << indexToCode(square) << " to " << indexToCode(final) << std::endl;
                break;
            }
            else
            {
                moves.push_back(Move(square, final, p, board[final], Piece(), MoveFlag::Normal));
                // std::cout << "Not breaking, didn't capture from " << indexToCode(square) << " to " << indexToCode(final) << std::endl;
                //  std::cout << "Result of isCapture; p.color and final " << isCapture(p, final) << "; " << p.color << "; " << indexToCode(final) << "; " << board[final].color << " I considered following move legal: ";
                //  Move(square, final, p, board[final], Piece(), MoveFlag::Normal).displayMove();
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
    Piece p = board[square];
    if (p.color == Color::White)
    {
        std::cout << "Result of the whole thing: " << (state.whiteCastleKingSide == true && board[square - 1].type == PieceType::None && board[square - 2].type == PieceType::None) &&
            (!isSquareAttacked(square, Color::Black) && !isSquareAttacked(square - 1, Color::Black) && !isSquareAttacked(square - 2, Color::Black));
        std::cout << "Result of this: " << (!isSquareAttacked(square, Color::Black) && !isSquareAttacked(square - 1, Color::Black) && !isSquareAttacked(square - 2, Color::Black) && !isSquareAttacked(square - 3, Color::Black)) << "\n";
        std::cout << "Result of square: " << isSquareAttacked(square, Color::Black) << std::endl;
        std::cout << "Result of square-1: " << isSquareAttacked(square - 1, Color::Black) << std::endl;
        std::cout << "Result of square-2: " << isSquareAttacked(square - 2, Color::Black) << std::endl;
        // std::cout<<"Result of square-3: " << isSquareAttacked(square-3,Color::Black)<<std::endl;
        // std::cout<<"Result of this: "<< (state.whiteCastleKingSide == true && board[square - 1].type == PieceType::None && board[square - 2].type == PieceType::None)<<"\n";
        if ((state.whiteCastleKingSide == true && board[square - 1].type == PieceType::None && board[square - 2].type == PieceType::None) &&
            (!isSquareAttacked(square, Color::Black) && !isSquareAttacked(square - 1, Color::Black) && !isSquareAttacked(square - 2, Color::Black)))
        { // Also need to check if not under attack
            moves.push_back(Move(square, square - 2, p, board[square - 2], Piece(), MoveFlag::KingCastle));
        }
        if ((state.whiteCastleQueenSide == true && board[square + 1].type == PieceType::None && board[square + 2].type == PieceType::None && board[square + 3].type == PieceType::None) &&
            (!isSquareAttacked(square, Color::Black) && !isSquareAttacked(square + 1, Color::Black) && !isSquareAttacked(square + 2, Color::Black)))
        {
            moves.push_back(Move(square, square + 2, p, board[square + 2], Piece(), MoveFlag::QueenCastle));
        }
    }
    if (p.color == Color::Black)
    {
        // std::cout << "0: "<< isSquareAttacked(square, Color::White)<<std::endl;
        // std::cout << "-1: "<< isSquareAttacked(square-1, Color::White)<<std::endl;
        // std::cout << "-2: "<< isSquareAttacked(square-2, Color::White)<<std::endl;
        // std::cout << "-3: "<< isSquareAttacked(square-3, Color::White)<<std::endl;

        if ((state.blackCastleKingSide == true && board[square - 1].type == PieceType::None && board[square - 2].type == PieceType::None) &&
            (!isSquareAttacked(square, Color::White) && !isSquareAttacked(square - 1, Color::White) && !isSquareAttacked(square - 2, Color::White)))
        { // Also need to check if not under attack
            moves.push_back(Move(square, square - 2, p, board[square - 2], Piece(), MoveFlag::KingCastle));
        }
        if ((state.blackCastleQueenSide == true && board[square + 1].type == PieceType::None && board[square + 2].type == PieceType::None && board[square + 3].type == PieceType::None) &&
            (!isSquareAttacked(square, Color::White) && !isSquareAttacked(square + 1, Color::White) && !isSquareAttacked(square + 2, Color::White)))
        {
            moves.push_back(Move(square, square + 2, p, board[square + 2], Piece(), MoveFlag::QueenCastle));
        }
    }
}

bool Board::isSquareAttacked(int square, Color by)
{
    // std::cout<<"Started running is square attacked\n";
    Piece p = board[square];

    // Checks for knight attacks
    std::array<int, 8> numsKnight = {-17, -15, -10, -6, 6, 10, 15, 17};
    for (int n : numsKnight)
    {
        int check = square + n;
        if (!isOutOfBounds(check))
        {
            if (!crossesBorderKnight(check, square) && by == board[check].color)
            {
                if (board[check].type == PieceType::Knight)
                {
                    // std::cout << "Crosses border knight:" << crossesBorderKnight(check, square) << "\n";
                    // std::cout << "Attacked by knight\n";
                    // std::cout<<"True by knight"<<std::endl;
                    return true;
                }
            }
        }
    }

    // Checks for bishop/queen attacks
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
            if (board[check].color == reverseColor(by))
            {
                break; // Breaks if there is an ally on this square
            }
            // displayBoard();
            // std::cout <<board[check].color<< " and " <<p.color<<std::endl;
            // std::cout<<"Move from " <<indexToCode(square) << " to " << indexToCode(check)<< " Showed "<<(board[check].color == p.color) <<std::endl;
            if (board[check].type != PieceType::None && board[check].type != PieceType::Bishop && board[check].type != PieceType::Queen)
            {
                break;
            }
            if (board[check].type == PieceType::Bishop || board[check].type == PieceType::Queen)
            {
                // std::cout << "Attacked by bishop/queen\n";
                return true;
            }
            mult++;
        }
    }

    // Checks for Rook/queen attacks
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
            if (board[check].color == reverseColor(by))
            {
                break; // Breaks if there is an ally on this square
            }
            if (board[check].type != PieceType::None && board[check].type != PieceType::Rook && board[check].type != PieceType::Queen)
            {
                break;
            }
            if (board[check].type == PieceType::Rook || board[check].type == PieceType::Queen)
            {
                // std::cout << " Attacked from square: " << indexToCode(check);
                // std::cout << "Attacked rook/queen\n";
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
        left = square + 9;
        right = square + 7;
    }
    else
    {
        left = square - 9;
        right = square - 7;
    }
    // std::cout << "left and right" << left << " " << right << "\n";
    std::vector<int> numsPawn;
    // std::cout << "Is out of bouds left " << isOutOfBounds(left) << "\n";
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
        // std::cout << "Looping over: " << n << "\n";
        if (board[n].color != by)
        {
            continue;
        }
        if (crossesBorderPawn(n, square))
        {
            continue;
        }
        if (board[n].type == PieceType::Pawn)
        {
            // std::cout << "Attacked by pawn\n";
            return true;
        }
    }

    // Add checking for en passant ONLY if p is pawn itself
    if (p.type == PieceType::Pawn && state.enPassantSquare != -1)
    {
        if (canEnPassant(square))
        {
            // std::cout << "Attacked by en passant\n";
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
            if (isSquareAttacked(i, reverseColor(board[i].color)))
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
        // std::cout << "Trying move m (inside of filterLegalMoves): " << m << "\n";
        // std::cout<<"Trying to make move..."<<m<<std::endl;
        makeMove(m);
        // std::cout << "Made move " << m<<std::endl;
        //  std::cout << "succesfully performed move m (inside of filterLegalMoves): " << m << "\n";
        if (!isKingInCheck(state.sideToMove == Color::White ? Color::Black : Color::White))
        {
            legal.push_back(m);
        }
        // std::cout << "Checked if king was in check;\n";
        undoMove(m);
        // std::cout<<"Undid move " << m << std::endl;
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
    // std::cout << "Attempt to generate pseudo legal moves...\n";
    generateMoves(pseudo);
    // std::cout << "Attempt to filter legal moves...\n";
    filterLegalMoves(pseudo, legal);
    // std::cout << "Successfully filtered legal moves\n";
    for (const Move &move : legal)
    {
        // std::cout << "Trying move Move: " << move << "\n";
        makeMove(move);
        // std::cout << "Performed move: " << move << "\n";
        if (!isKingInCheck(state.sideToMove == Color::White ? Color::Black : Color::White))
        {
            // std::cout << "Performing recursive call... \n";
            nodes += perft(depth - 1);
            // std::cout << " nodes var changed to: " << nodes;
            // std::cout << " Legality checked Move: " << move << "\n";
        }
        undoMove(move);
        // std::cout << "Undoing move: " << move << "\n";
    }
    return nodes;
}
void Board::perftDivide(int depth)
{
    uint64_t totalNodes = 0;
    std::vector<Move> pseudo;
    std::vector<Move> legal;
    generateMoves(pseudo);
    // std::cout << "Generated pseudo moves\n";
    filterLegalMoves(pseudo, legal);
    // std::cout << "Generated legal moves\n";
    //  for (Move m : legal)
    //  {
    //      m.displayMove();
    //  }
    for (Move &move : legal)
    {
        // std::cout << "this move is considered legal: ";
        // move.displayMove();
        makeMove(move);
        // std::cout << "Display board after making move ";
        // move.displayMove();
        // std::cout << move;
        // displayBoard();
        if (!isKingInCheck(state.sideToMove == Color::White ? Color::Black : Color::White))
        {
            uint64_t nodes = perft(depth - 1);

            std::cout << move.getMoveCode() << ": " << nodes << std::endl;
            totalNodes += nodes;
        }
        undoMove(move);
        // std::cout << "Display board after UNDOING move ";
        // move.displayMove();
        // displayBoard();
    }
    std::cout << "\n Total nodes at depth " << depth << ": " << totalNodes << std::endl;
}