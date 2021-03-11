#pragma once

#include "move.hh"

namespace board
{
    enum class ExceptionalMoveType
    {
        CASTLING_SMALL,
        CASTLING_BIG
    };

    class ExceptionalMove : public Move
    {
    public:
        ExceptionalMove(const Color &color, const Position &start,
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

        void execute_move(Chessboard &board);

    private:
        ExceptionalMoveType move_type_;
    };

} // namespace board