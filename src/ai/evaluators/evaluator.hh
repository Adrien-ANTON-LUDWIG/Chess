#pragma once

#include <limits>

#include "board.hh"
#include "color.hh"

namespace board
{
    class Chessboard;
} // namespace board

namespace ai
{
    class Evaluator
    {
    public:
        ~Evaluator() = default;
        virtual float evaluate_board(const board::Chessboard &board) = 0;
    };
} // namespace ai