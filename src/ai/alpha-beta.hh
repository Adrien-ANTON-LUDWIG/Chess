#pragma once

#include "AI.hh"

namespace ai
{
    class AlphaBeta : public AI
    {
    public:
        AlphaBeta(std::unique_ptr<Evaluator> &eval);
        float alphaBeta(board::Chessboard &board, float alpha, float beta,
                        int depth);
        float quiesce(board::Chessboard &board, float alpha, float beta);

        board::Move best_move() override;
    };
} // namespace ai