#include "move.hh"

/*

Rook: up -8, right +1, left -1, down +8;
Knight: -17, -15, -10, -6, +6, +10, +15, +17;
Bishop: -9,-7,+7,+9;

/!\ Border

*/
namespace board
{
    Move::Move(const Color &color, const PieceType &piece_type,
               const Position &start, const Position &end)
        : color_(color)
        , piece_type_(piece_type)
        , start_(start)
        , end_(end)
    {}
} // namespace board