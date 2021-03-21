#include "sum.hh"

#include <cmath>

namespace ai
{
    float Sum_Evaluator::evaluate_board(const board::Chessboard &board)
    {
        board::Chessboard board_copy = board;
        if (board_copy.is_checkmate(
                static_cast<board::Color>(!board_copy.get_side_turn())))
            return std::numeric_limits<float>::infinity();

        float pieces_sum = 0.0f;
        float values[] = { 900.0f, 500.0f, 330.0f, 320.0f, 100.0f, 20000.f };
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
        return pieces_sum;
    }
} // namespace ai