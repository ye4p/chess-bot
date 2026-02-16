#pragma once
#include "piece.h"
#include "move.h"
#include <array>
#include <vector>
class Board
{
    std::array<Piece, 64> board;

public:
    Board();
    void assignDefaultRow(int row, Color color);
    void assignPawns(int row, Color color);
    void startingPosition();
    void displayBoard();
    Color checkSpace(int square);
    bool isEndOfTheBoard(int square, Piece p);
    bool crossesBorder(int from, int to);
    void handlePawnCapture(int from, int to, Piece p, std::vector<Move> &moves);
    void FakeMove(Piece p, int to);
    void makeMove(const Move &m);
    void undoMove(const Move &m);
    void generateMoves(std::vector<Move> &moves);
    void generatePawnMoves(int square, std::vector<Move> &moves);
    void generateKnightMoves(int square, std::vector<Move> &moves);
    void generateBishopMoves(int square, std::vector<Move> &moves);
    void generateRookMoves(int square, std::vector<Move> &moves);
    void generateQueenMoves(int square, std::vector<Move> &moves);
    void generateKingMoves(int square, std::vector<Move> &moves);
};