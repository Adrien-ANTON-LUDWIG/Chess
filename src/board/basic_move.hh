#pragma once

#include <bitset>
#include <vector>

#include "board.hh"
#include "color.hh"
#include "move.hh"
#include "piece-type.hh"
#include "position.hh"

namespace board
{
    class Chessboard;

    class BasicMove : public Move
    {
    public:
        friend std::bitset<64> to_bitboard(const std::vector<BasicMove> &moves);

        BasicMove(const Color &color, const PieceType &piece_type,
                  const Position &start, const Position &end);
        BasicMove() = default;

        void execute_move(Chessboard &board) override;
        bool set_capture(Chessboard &board);
        bool set_promotion(const std::optional<PieceType> &promotion);
        void update_castling(Chessboard &board);

        std::optional<std::pair<PieceType, Position>> capture_;
        std::optional<PieceType> promotion_;
    };
} // namespace board