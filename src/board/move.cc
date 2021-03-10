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

    std::bitset<64> to_bitboard(const std::vector<Move> &moves)
    {
        std::bitset<64> bitboard;

        for (auto move : moves)
            bitboard[move.end_.to_index()] = 1;

        return bitboard;
    }
} // namespace board