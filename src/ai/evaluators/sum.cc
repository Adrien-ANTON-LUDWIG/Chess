#include "sum.hh"

#include <cmath>

namespace ai
{
    float Sum_Evaluator::evaluate_board(const board::Chessboard &board)
    {
        board::Chessboard board_copy = board;
        if (board_copy.is_checkmate(
                static_cast<board::Color>(!board_copy.get_side_turn())))
            return __FLT_MAX__;

        float pieces_sum = 0.0f;
        float values[] = { 10.0f, 5.0f, 3.0f, 3.0f, 1.0f, 0.f };
        for (int i = 0; i < 6; i++)
        {
            pieces_sum +=
                board.pieces_[static_cast<int>(board.get_side_turn())][i]
                    .count()
                * values[i];
            pieces_sum -=
                board
                    .pieces_[(static_cast<int>(board.get_side_turn()) + 1) % 2]
                            [i]
                    .count()
                * values[i];
        }
        return pieces_sum
            + log2(board.color_boards_[board.get_side_turn()].to_ullong()) - 32;
    }
} // namespace ai