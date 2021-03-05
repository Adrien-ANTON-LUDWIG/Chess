#pragma once

#include <bitset>
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

        std::optional<Color> piece_to_position(const Position &position);

        std::vector<Move> generate_legal_moves_knight(const Position &position,
                                                      const Color &color);
        std::vector<Move> generate_legal_moves_pawn(const Position &position,
                                                    const Color &color);

        void generate_legal_moves_diagonal(const Move &move,
                                           const size_t &distance,
                                           std::vector<Move> &legal_moves,
                                           int dir_x, int dir_y);

        void generate_legal_moves_forward(const Move &move,
                                          const size_t &distance,
                                          std::vector<Move> &legal_moves);

        std::vector<Move> generate_legal_moves_generic(const Position &position,
                                                       Color piece_color,
                                                       PieceType type);

    private:
        // pieces_[Color][PieceType]
        std::array<std::array<std::bitset<64>, 6>, 2> pieces_;

        int turn_;
        bool white_turn_;
        bool white_king_castling_;
        bool white_queen_castling_;
        bool black_king_castling_;
        bool black_queen_castling_;
        // en_passant_
        int last_fifty_turn_;
    };
} // namespace board