#pragma once

#include <bitset>
#include <vector>

#include "color.hh"
#include "piece-type.hh"
#include "position.hh"

namespace board
{
    class Move
    {
    public:
        friend class Chessboard;
        friend std::bitset<64> to_bitboard(const std::vector<Move> &moves);

        Move(const Color &color, const PieceType &piece_type,
             const Position &start, const Position &end);

    private:
        Color color_;
        PieceType piece_type_;
        Position start_;
        Position end_;
    };

    std::bitset<64> to_bitboard(const std::vector<Move> &moves);
} // namespace board