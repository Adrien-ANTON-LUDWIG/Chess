#pragma once

#include <map>
#include <vector>

#include "color.hh"
#include "move.hh"
#include "piece-type.hh"
#include "position.hh"

namespace board
{
    class Chessboard
    {
    public:
        Chessboard();
        Chessboard(std::string string);

        std::vector<Move> generate_legal_moves();
        bool is_move_legal(Move move);
        void do_move(Move move);
        bool is_check();
        bool is_checkmate();
        bool is_draw();

        std::pair<PieceType, Color> operator[](Position postion);

        void print_chessboard();

    private:
        uint64_t black_pawn_;
        uint64_t black_rook_;
        uint64_t black_bishop_;
        uint64_t black_knight_;
        uint64_t black_queen_;
        uint64_t black_king_;
        uint64_t white_pawn_;
        uint64_t white_rook_;
        uint64_t white_bishop_;
        uint64_t white_knight_;
        uint64_t white_queen_;
        uint64_t white_king_;
    };
} // namespace board