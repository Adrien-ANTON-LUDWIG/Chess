#include "move.hh"

namespace board
{
    Move::Move(const Color &color, const PieceType &piece_type,
               const Position &start, const Position &end)
        : color_(color)
        , piece_type_(piece_type)
        , start_(start)
        , end_(end)
    {}

    void Move::execute_move(Chessboard &board)
    {
        static std::array en_passant_start = { Rank::TWO, Rank::SEVEN };
        static std::array en_passant_end = { Rank::FOUR, Rank::FIVE };
        int color = static_cast<int>(color_);
        int opponent_color = static_cast<int>(!static_cast<bool>(color));
        int type = static_cast<int>(piece_type_);
        int end = end_.to_index();
        int start = start_.to_index();
        board.update_piece(color, type, start, 0);

        board.check_eating_en_passant(*this);
        if (board.color_boards_[opponent_color][end])
        {
            for (int p = 0; p < 6; p++)
                if (board.pieces_[opponent_color][p][end])
                    board.update_piece(opponent_color, p, end, 0);
            board.last_fifty_turn_ = 0;
        }

        board.update_piece(color, type, end, 1);

        if (piece_type_ == PieceType::PAWN
            && start_.rank_get() == en_passant_start[color]
            && end_.rank_get() == en_passant_end[color])
            board.en_passant_ = end_;
        else
            board.en_passant_ = std::nullopt;

        if (board.side_turn_)
            board.turn_++;
        board.side_turn_ = static_cast<Color>(!board.side_turn_);
    }

    std::bitset<64> to_bitboard(const std::vector<Move> &moves)
    {
        std::bitset<64> bitboard;

        for (auto move : moves)
            bitboard[move.end_.to_index()] = 1;

        return bitboard;
    }
} // namespace board