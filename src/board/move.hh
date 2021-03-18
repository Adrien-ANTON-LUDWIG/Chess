#pragma once

#include <bitset>
#include <iostream>
#include <vector>

#include "board.hh"
#include "castling_type.hh"
#include "color.hh"
#include "piece-type.hh"
#include "position.hh"

namespace board
{
    class Chessboard;

    class Move
    {
    public:
        friend class Chessboard;
        friend class GameTracer;
        friend void operator<<(std::ostream &o, const Move &move);
        friend std::bitset<64> to_bitboard(const std::vector<Move> &moves);

        Move() = default;
        Move(const Color &color, const PieceType &piece_type,
             const Position &start, const Position &end);

        // Constructeur de Roque 🇫🇷🥖
        Move(const Color &color, const Position &start, const Position &end);
        Move(const Color &color, const Castling &castling_type);

        // Constructeur EBNF
        Move(const std::string &ebnf, Chessboard &board);

        // TODO void set_capture()
        bool set_capture(Chessboard &board);
        bool set_promotion(const PieceType &promotion_type);
        void set_castling();
        void update_castling(Chessboard &board);
        void execute_move(Chessboard &board);
        void execute_castling(Chessboard &board);

        // Algebraic notation EBNF
        std::string to_ebnf();

        bool operator==(const Move &o);

    private:
        Color color_;
        PieceType piece_type_;
        Position start_;
        Position end_;

        // Basique
        std::optional<std::pair<PieceType, Position>> capture_ = std::nullopt;
        bool promotion_ = false;
        PieceType promotion_type_ = PieceType::QUEEN;

        // Roque
        bool castling_ = false;
        Castling move_type_ = Castling::SMALL;
    };

    void operator<<(std::ostream &o, const Move &move);
    std::bitset<64> to_bitboard(const std::vector<Move> &moves);
} // namespace board