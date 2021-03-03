#include "position.hh"

namespace board
{
    Position::Position(File file, Rank rank)
        : file_(file)
        , rank_(rank)
    {}

    File Position::file_get() const
    {
        return file_;
    }

    Rank Position::rank_get() const
    {
        return rank_;
    }

    bool Position::operator==(const Position &pos) const
    {
        return file_ == pos.file_ && rank_ == pos.rank_;
    }

    bool Position::operator!=(const Position &pos) const
    {
        return !(*this == pos);
    }
} // namespace board