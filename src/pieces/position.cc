#include "position.hh"

namespace board
{
    Position::Position(int array_pos)
    {
        int y = array_pos / 8;
        int x = 7 - array_pos % 8;

        this->file_ = static_cast<File>(x);
        this->rank_ = static_cast<Rank>(y);
    }

    int Position::to_index() const
    {
        return static_cast<int>(rank_) * 8 + 7 - static_cast<int>(file_);
    }
} // namespace board