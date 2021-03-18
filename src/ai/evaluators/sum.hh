#pragma once

#include "evaluator.hh"

namespace ai
{
    class Sum_Evaluator : Evaluator
    {
    public:
        float evaluate_board(const board::Chessboard &board) override;
    };

} // namespace ai