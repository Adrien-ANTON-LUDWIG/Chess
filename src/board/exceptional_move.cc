#include "exceptional_move.hh"

namespace board
{
    ExceptionalMove::ExceptionalMove(const Color &color, const Position &start,
                                     const Position &end)
        : Move(color, PieceType::KING, start, end)
    {
        if (start.file_get() < end.file_get())
        {
            move_type_ = ExceptionalMoveType::CASTLING_SMALL;
            if (color == Color::WHITE)
                end_ = Position(File::H, Rank::ONE);
            else
                end_ = Position(File::H, Rank::EIGHT);
        }
        else
        {
            move_type_ = ExceptionalMoveType::CASTLING_BIG;
            if (color == Color::WHITE)
                end_ = Position(File::A, Rank::ONE);
            else
                end_ = Position(File::A, Rank::EIGHT);
        }
    }

    ExceptionalMove::ExceptionalMove(const Color &color,
                                     const ExceptionalMoveType &castling_type)
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
        move_type_ = castling_type;
    }

    void ExceptionalMove::execute_move(Chessboard &board)
    {
        board.update_piece(color_, PieceType::KING, start_.to_index(), 0);
        board.update_piece(color_, PieceType::ROOK, end_.to_index(), 0);
        if (move_type_ == ExceptionalMoveType::CASTLING_SMALL)
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
        else if (move_type_ == ExceptionalMoveType::CASTLING_BIG)
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
} // namespace board