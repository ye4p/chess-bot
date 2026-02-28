#pragma once
#include "piece.h"
#include "move.h"
#include <array>
#include <vector>
#include "gamestate.h"
class Board
{
    std::array<Piece, 64> board;
    GameState state;

public:
    Board();
    Piece getPiece(int square);
    int getEnPassantSquare();
    void assignDefaultRow(int row, Color color);
    void assignPawns(int row, Color color);
    void startingPosition();
    void displayBoard();
    Color checkSpace(int square);
    bool isEndOfTheBoard(int square, Piece p);
    bool crossesBorder(int from, int to);
    bool crossesBorderKnight(int from, int to);
    bool isAlly(Piece p, int to);
    bool isCapture(Piece p, int to);
    void handlePawnCapture(int from, int to, Piece p, std::vector<Move> &moves);
    bool isOutOfBounds(int to);
    bool crossesBorderBishop(int from, int to, int n, int multiplier);
    bool crossesBorderPawn(int from, int to);
    bool isEnemy(Piece p, int where);
    bool canEnPassant(int square);
    void FakeMove(Piece p, int to);
    void makeMove(const Move &m);
    void undoMove(const Move &m);
    void generateMoves(std::vector<Move> &moves);
    void generatePawnMoves(int square, std::vector<Move> &moves);
    void generateKnightMoves(int square, std::vector<Move> &moves);
    void generateGeometryMoves(int square, std::vector<Move> &moves, std::vector<int> nums);
    void generateBishopMoves(int square, std::vector<Move> &moves);
    void generateRookMoves(int square, std::vector<Move> &moves);
    void generateQueenMoves(int square, std::vector<Move> &moves);
    void generateKingMoves(int square, std::vector<Move> &moves);
    bool isSquareAttacked(int square);
};