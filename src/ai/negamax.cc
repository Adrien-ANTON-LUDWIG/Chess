#include "negamax.hh"

#include <algorithm>
#include <iostream>

#define DEPTH 3

namespace ai
{
    Negamax::Negamax(std::unique_ptr<Evaluator> &eval)
        : AI(eval)
    {}

    float Negamax::negaMax(board::Chessboard &board, int depth)
    {
        if (!depth)
            return eval_->evaluate_board(board);
        float max = -std::numeric_limits<float>::infinity();
        std::vector<board::Move> moves =
            board.generate_legal_moves(board.get_side_turn());

        for (size_t i = 0; i < moves.size(); i++)
        {
            board::Chessboard fake_board = board;
            moves[i].execute_move(fake_board);
            float score = -negaMax(fake_board, depth - 1);
            // std::cout << "Score : " << score << " Score > max ? " << (score >
            // max) << " Max: " << max << '\n';

            max = std::max(score, max);
        }

        return max;
    }

    board::Move Negamax::best_move()
    {
        float max = -std::numeric_limits<float>::infinity();
        size_t max_index = 0;
        std::vector<board::Move> moves =
            board_.generate_legal_moves(board_.get_side_turn());

        for (size_t i = 0; i < moves.size(); i++)
        {
            board::Chessboard fake_board = board_;
            moves[i].execute_move(fake_board);
            float score = -negaMax(fake_board, DEPTH - 1);

            if (score > max)
            {
                max = score;
                max_index = i;
            }
        }

        return moves[max_index];
    }
} // namespace ai