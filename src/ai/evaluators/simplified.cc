#include "simplified.hh"

#include <cmath>

namespace ai
{
    bool Simplified_Evaluator::is_endgame(const board::Chessboard &board)
    {
        int c = static_cast<int>(board.get_side_turn());

        bool no_more_queen = !board.pieces_[c][0].to_ullong()
            && !board.pieces_[(c + 1) % 2][0].to_ullong();

        bool one_minor_piece = board.pieces_[c][1].to_ullong() // Rook
                    + board.pieces_[c][2].to_ullong() // Bishop
                    + board.pieces_[c][3].to_ullong() // Knight
                == 1
            && board.pieces_[(c + 1) % 2][1].to_ullong() // Rook
                    + board.pieces_[(c + 1) % 2][2].to_ullong() // Bishop
                    + board.pieces_[(c + 1) % 2][3].to_ullong() // Knight
                == 1;

        return no_more_queen || one_minor_piece;
    }

    float Simplified_Evaluator::evaluate_board(const board::Chessboard &board)
    {
        board::Chessboard board_copy = board;
        if (board_copy.is_checkmate(
                static_cast<board::Color>(!board_copy.get_side_turn())))
            return __FLT_MAX__;

        float pieces_sum = 0.0f;
        float values[] = { 900.0f, 500.0f, 330.0f, 320.0f, 100.0f, 0.0f };
        int c = static_cast<int>(board.get_side_turn());
        for (int p = 0; p < 6; p++)
        {
            pieces_sum += board.pieces_[c][p].count() * values[p];
            pieces_sum -=
                1.5f * board.pieces_[(c + 1) % 2][p].count() * values[p];

            for (size_t i = 0; i < 64; i++)
            {
                if (p < 5)
                    pieces_sum += board.pieces_[c][p][i] * heuristics[c][p][i];
                else if (is_endgame(board))
                    pieces_sum +=
                        board.pieces_[c][p][i] * heuristics[c][p + 1][i];
                else
                    pieces_sum += board.pieces_[c][p][i] * heuristics[c][p][i];
            }
        }
        return pieces_sum;
    }
} // namespace ai