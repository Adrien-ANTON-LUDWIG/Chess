#pragma once

#include "AI.hh"

namespace ai
{
    class Negamax : public AI
    {
    public:
        Negamax(std::unique_ptr<Evaluator> &eval);
        float negaMax(board::Chessboard &board, int depth);
        board::Move best_move() override;
    };
} // namespace ai