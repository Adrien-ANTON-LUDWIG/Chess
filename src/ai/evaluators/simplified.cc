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

    float
    Simplified_Evaluator::controlled_ground(const board::Chessboard &board)
    {
        // All my homies hate optimization
        auto newboard = board::Chessboard(board);

        // Pure ground control
        auto moves = newboard.generate_legal_moves(board.get_side_turn());
        std::bitset<64> ground;
        for (auto m : moves)
            ground[m.get_end().to_index()] = true;
        float controlled = ground.count();
        if (board.get_side_turn() == board::Color::WHITE)
            controlled +=
                (ground & std::bitset<64>(0xFFFFFFFF00000000)).count();
        else
            controlled +=
                (ground & std::bitset<64>(0x00000000FFFFFFFF)).count();

        // Self-protection
        float values[] = { 900.0f, 500.0f, 330.0f, 320.0f, 100.0f, 0.0f };
        for (int i = 0; i < 6; i++)
        {
            controlled +=
                (ground & board.pieces_[board.get_side_turn()][i]).count()
                * values[i] * 0.2f;
        }

        return controlled;
    }

    float Simplified_Evaluator::evaluate_board(const board::Chessboard &board)
    {
        board::Chessboard board_copy = board;
        if (board_copy.is_checkmate(
                static_cast<board::Color>(!board_copy.get_side_turn())))
            return std::numeric_limits<float>::infinity();

        float pieces_sum = 0.0f;
        float values[] = { 900.0f, 500.0f, 330.0f, 320.0f, 100.0f, 0.0f };
        int c = static_cast<int>(board.get_side_turn());
        for (int p = 0; p < 6; p++)
        {
            pieces_sum += board.pieces_[c][p].count() * values[p];
            pieces_sum -= board.pieces_[(c + 1) % 2][p].count() * values[p];

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

        if (board_copy.is_draw())
            return (pieces_sum >= 0 ? -1 : 1) * 100;

        return pieces_sum + controlled_ground(board);
    }
} // namespace ai