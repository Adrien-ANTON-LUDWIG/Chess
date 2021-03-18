#include "minimax.hh"

namespace ai
{
    board::Move Minimax::best_move()
    {
        std::vector<board::Move> moves =
            board_.generate_legal_moves(board_.get_side_turn());

        if (moves.size() == 0)
            throw std::logic_error("No legal move available");

        return moves[std::rand() % moves.size()];
    }
} // namespace ai