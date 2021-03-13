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

    Move::Move(const Color &color, const Position &start, const Position &end)
    {
        color_ = color;
        piece_type_ = PieceType::KING;
        start_ = start;
        end_ = end;
        if (start_.file_get() < end_.file_get())
        {
            move_type_ = Castling::SMALL;
            if (color_ == Color::WHITE)
                end_ = Position(File::H, Rank::ONE);
            else
                end_ = Position(File::H, Rank::EIGHT);
        }
        else
        {
            move_type_ = Castling::BIG;
            if (color == Color::WHITE)
                end_ = Position(File::A, Rank::ONE);
            else
                end_ = Position(File::A, Rank::EIGHT);
        }

        castling_ = true;
    }

    Move::Move(const Color &color, const Castling &castling_type)
    {
        static std::array<Position, 2> king_start = {
            Position(File::E, Rank::ONE), Position(File::E, Rank::EIGHT)
        };

        // rook_rank_start[Color]
        static std::array<Rank, 2> rook_rank_start = { Rank::ONE, Rank::EIGHT };

        // rook_file_start[ExceptionMoveType]
        static std::array<File, 2> rook_file_start = { File::H, File::A };

        color_ = color;
        piece_type_ = PieceType::KING;
        start_ = king_start[color];
        end_ = Position(rook_file_start[static_cast<int>(castling_type)],
                        rook_rank_start[color]);
        castling_ = true;
        move_type_ = castling_type;
    }

    bool Move::set_capture(Chessboard &board)
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

    bool Move::set_promotion(const PieceType &promotion_type)
    {
        static std::array<Rank, 2> promotion_rank = { Rank::EIGHT, Rank::ONE };
        if (piece_type_ != PieceType::PAWN
            || end_.rank_get() != promotion_rank[color_]
            || promotion_type == PieceType::KING)
            return false;

        promotion_ = true;
        promotion_type_ = promotion_type;
        return true;
    }

    void Move::update_castling(Chessboard &board)
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

    void Move::execute_move(Chessboard &board)
    {
        if (castling_)
            return execute_castling(board);

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

    std::bitset<64> to_bitboard(const std::vector<Move> &moves)
    {
        std::bitset<64> bitboard;

        for (size_t i = 0; i < moves.size(); i++)
            bitboard[moves[i].end_.to_index()] = 1;

        return bitboard;
    }

    void Move::execute_castling(Chessboard &board)
    {
        board.update_piece(color_, PieceType::KING, start_.to_index(), 0);
        board.update_piece(color_, PieceType::ROOK, end_.to_index(), 0);
        if (move_type_ == Castling::SMALL)
        {
            Position new_rook_pos = Position(
                static_cast<File>(static_cast<int>(end_.file_get()) - 2),
                end_.rank_get());
            Position new_king_pos = Position(
                static_cast<File>(static_cast<int>(start_.file_get()) + 2),
                start_.rank_get());
            board.update_piece(color_, PieceType::KING, new_king_pos.to_index(),
                               1);
            board.update_piece(color_, PieceType::ROOK, new_rook_pos.to_index(),
                               1);
        }
        else if (move_type_ == Castling::BIG)
        {
            Position new_rook_pos = Position(
                static_cast<File>(static_cast<int>(end_.file_get()) + 3),
                end_.rank_get());
            Position new_king_pos = Position(
                static_cast<File>(static_cast<int>(start_.file_get()) - 2),
                start_.rank_get());
            board.update_piece(color_, PieceType::KING, new_king_pos.to_index(),
                               1);
            board.update_piece(color_, PieceType::ROOK, new_rook_pos.to_index(),
                               1);
        }

        if (board.side_turn_)
            board.turn_++;
        board.side_turn_ = static_cast<Color>(!board.side_turn_);
    }

    // Note: o = autre 🇫🇷🥖
    bool Move::operator==(const Move &o)
    {
        return color_ == o.color_ && piece_type_ == o.piece_type_
            && start_ == o.start_ && end_ == o.end_ && capture_ == o.capture_
            && promotion_ == o.promotion_ && castling_ == o.castling_
            && move_type_ == o.move_type_;
    }
} // namespace board