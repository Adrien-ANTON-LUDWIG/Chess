#pragma once

#include <bitset>
#include <vector>

#include "board.hh"
#include "color.hh"
#include "piece-type.hh"
#include "position.hh"

namespace board
{
    class Chessboard;

    class Move
    {
    public:
        friend std::bitset<64> to_bitboard(const std::vector<Move> &moves);
        Move(const Color &color, const PieceType &piece_type,
             const Position &start, const Position &end);

        void execute_move(Chessboard &board);

        Color color_;
        PieceType piece_type_;
        Position start_;
        Position end_;
    };

    std::bitset<64> to_bitboard(const std::vector<Move> &moves);
} // namespace board