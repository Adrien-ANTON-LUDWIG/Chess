#pragma once

#include <bitset>
#include <map>
#include <vector>

#include "castling_type.hh"
#include "color.hh"
#include "move.hh"
#include "piece-type.hh"
#include "position.hh"

namespace board
{
    class Move;

    class Chessboard
    {
    public:
        Chessboard(bool empty = false);
        Chessboard(std::string string);

        friend class Move;

        Chessboard();

        std::vector<Move> generate_legal_moves(const Color &side_turn);
        bool is_move_legal(const Move &move);

        void update_piece(const int &color, const int &type,
                          const int &position, const bool &arrive);

        bool check_eating_en_passant(const Move &move);
        void update_en_passant(const Move &move);
        bool is_check(const Color &color);
        bool is_checkmate(const Color &color);
        bool is_stalemate(const Color &color);
        bool is_draw();

        std::pair<PieceType, Color> operator[](Position postion);

        void print_chessboard(const Color &color);

        std::optional<Color> is_piece_to_position(const Position &position);

        std::vector<Move> generate_legal_moves_knight(const Position &position,
                                                      const Color &color);

        void generate_pawn_capture(std::vector<Move> &moves,
                                   const Position &position, const Color &color,
                                   const int &color_side, const File &file,
                                   const int &side);

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

        std::bitset<64> generate_move_king_like_other(const PieceType &p,
                                                      const Color &c);

        std::vector<Move> generate_castling_moves(const Color &side_turn);

        bool is_king_collision(const PieceType &piecetype, const Color &color);
        bool is_castling_legal(const Color &side_turn,
                               const Castling &castling_type);
        void set_castling(Color color, PieceType type);
        void set_en_passant(Position pos);
        Color get_side_turn();

        bool is_equivalent(const Chessboard &other);

    private:
        // pieces_[Color][PieceType]
        std::array<std::array<std::bitset<64>, 6>, 2> pieces_;
        std::array<std::bitset<64>, 2> color_boards_;
        std::bitset<64> board_;

        int turn_ = 0;
        Color side_turn_ = Color::WHITE;
        bool white_turn_;

        // Castling
        std::array<bool, 2> king_castling_ = { true, true };
        std::array<bool, 2> queen_castling_ = { true, true };
        std::optional<Position> en_passant_ = std::nullopt;
        int last_fifty_turn_ = 0;
    };
} // namespace board