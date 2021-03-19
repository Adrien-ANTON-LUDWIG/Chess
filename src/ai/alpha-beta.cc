#include "alpha-beta.hh"

#include <algorithm>
#include <iostream>

#define DEPTH 4

namespace ai
{
    AlphaBeta::AlphaBeta(std::unique_ptr<Evaluator> &eval)
        : AI(eval)
    {}

    float AlphaBeta::alphaBeta(board::Chessboard &board, float alpha,
                               float beta, int depth)
    {
        if (!depth)
            return eval_->evaluate_board(board);
        std::vector<board::Move> moves =
            board.generate_legal_moves(board.get_side_turn());

        for (size_t i = 0; i < moves.size(); i++)
        {
            board::Chessboard fake_board = board;
            moves[i].execute_move(fake_board);
            float score = -alphaBeta(fake_board, -beta, -alpha, depth - 1);

            if (score >= beta)
                return beta;
            if (score > alpha)
                alpha = score;
        }

        return alpha;
    }

    board::Move AlphaBeta::best_move()
    {
        float alpha = -std::numeric_limits<float>::infinity();
        float beta = std::numeric_limits<float>::infinity();
        size_t alpha_index = 0;
        std::vector<board::Move> moves =
            board_.generate_legal_moves(board_.get_side_turn());

        for (size_t i = 0; i < moves.size(); i++)
        {
            board::Chessboard fake_board = board_;
            moves[i].execute_move(fake_board);
            float score = -alphaBeta(fake_board, -beta, -alpha, DEPTH - 1);

            if (score > alpha)
            {
                alpha = score;
                alpha_index = i;
            }
        }

        return moves[alpha_index];
    }
} // namespace ai