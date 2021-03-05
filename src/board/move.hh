#pragma once

#include "color.hh"
#include "piece-type.hh"
#include "position.hh"

namespace board
{
    class Move
    {
    public:
        friend class Chessboard;

        Move(const Color &color, const PieceType &piece_type,
             const Position &start, const Position &end);

    private:
        Color color_;
        PieceType piece_type_;
        Position start_;
        Position end_;
    };
} // namespace board