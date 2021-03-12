#include "basic_move.hh"

#include "move.hh"

namespace board
{
    class Chessboard;

    BasicMove::BasicMove(const Color &color, const PieceType &piece_type,
                         const Position &start, const Position &end)
        : Move(color, piece_type, start, end)
    {}

    bool BasicMove::set_capture(Chessboard &board)
    {
        size_t end_index = end_.to_index();

        int dead_piece = -1;
        for (int p = 0; p < 6; p++)
            if (board.pieces_[!color_][p][end_index])
            {
                dead_piece = p;
                break;
            }

        if (dead_piece == -1)
        {
            if (!board.check_eating_en_passant(*this))
                return false;

            end_index = board.en_passant_->to_index();
            dead_piece = PieceType::PAWN;
        }

        capture_ = std::optional<std::pair<PieceType, Position>>(
            std::pair(static_cast<PieceType>(dead_piece), Position(end_index)));
        return true;
    }

    bool BasicMove::set_promotion(const std::optional<PieceType> &promotion)
    {
        static std::array<Rank, 2> promotion_rank = { Rank::EIGHT, Rank::ONE };
        if (piece_type_ != PieceType::PAWN
            || end_.rank_get() != promotion_rank[color_]
            || *promotion == PieceType::KING)
            return false;

        promotion_ = promotion;
        return true;
    }

    void BasicMove::update_castling(Chessboard &board)
    {
        if (piece_type_ == PieceType::KING)
        {
            board.king_castling_[color_] = 0;
            board.queen_castling_[color_] = 0;
            return;
        }

        // rook positions = WHITE = 0, 7 BLACK = 56, 63
        if (color_ == Color::WHITE)
        {
            if (start_.rank_get() == Rank::ONE)
            {
                if (start_.file_get() == File::A)
                    board.queen_castling_[Color::WHITE] = false;
                else if (start_.file_get() == File::H)
                    board.king_castling_[Color::WHITE] = false;
            }
            else if (end_.rank_get() == Rank::EIGHT)
            {
                if (end_.file_get() == File::A)
                    board.queen_castling_[Color::BLACK] = false;
                else if (end_.file_get() == File::H)
                    board.king_castling_[Color::BLACK] = false;
            }
        }
        else
        {
            if (end_.rank_get() == Rank::ONE)
            {
                if (end_.file_get() == File::A)
                    board.queen_castling_[Color::WHITE] = false;
                else if (end_.file_get() == File::H)
                    board.king_castling_[Color::WHITE] = false;
            }
            else if (start_.rank_get() == Rank::EIGHT)
            {
                if (start_.file_get() == File::A)
                    board.queen_castling_[Color::BLACK] = false;
                else if (start_.file_get() == File::H)
                    board.king_castling_[Color::BLACK] = false;
            }
        }
    }

    void BasicMove::execute_move(Chessboard &board)
    {
        int color = static_cast<int>(color_);
        int opponent_color = static_cast<int>(!static_cast<bool>(color));
        int type = static_cast<int>(piece_type_);
        int end = end_.to_index();
        int start = start_.to_index();
        board.update_piece(color, type, start, 0);

        if (capture_ != std::nullopt)
        {
            board.update_piece(opponent_color, capture_->first,
                               capture_->second.to_index(), 0);
            board.last_fifty_turn_ = 0;
        }

        if (board.color_boards_[color_][end])
            throw std::logic_error(
                "There should not be 2 pieces at the position, color : "
                + std::to_string(color_)
                + ", piece : " + std::to_string(piece_type_));
        board.update_piece(color, type, end, 1);

        board.update_en_passant(*this);
        update_castling(board);

        if (board.side_turn_)
            board.turn_++;
        board.side_turn_ = static_cast<Color>(!board.side_turn_);
    }
} // namespace board
