#pragma once

#include "move.hh"

namespace board
{
    class Chessboard;

    class ExceptionalMove : public Move
    {
    public:
        ExceptionalMove(const Color &color, const Position &start,
                        const Position &end);

        ExceptionalMove(const Color &color,
                        const ExceptionalMoveType &castling_type);

        void execute_move(Chessboard &board) override;

    private:
        ExceptionalMoveType move_type_;
    };

} // namespace board
