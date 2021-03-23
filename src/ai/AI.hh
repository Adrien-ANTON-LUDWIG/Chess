#pragma once

#include <limits>
#include <memory>

#include "board.hh"
#include "evaluator.hh"
#include "move.hh"

namespace ai
{
    class AI
    {
    public:
        AI(std::unique_ptr<Evaluator> &eval)
            : board_()
        {
            eval_ = std::move(eval);
        }

        AI() = default;

        void play_uci();
        void bench_uci(const std::string &fen);

        virtual board::Move best_move() = 0;

    protected:
        board::Chessboard board_;
        std::unique_ptr<ai::Evaluator> eval_;
    };
} // namespace ai