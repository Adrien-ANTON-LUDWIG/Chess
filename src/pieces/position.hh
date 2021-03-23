#pragma once

#include <utility>

#include "utype.hh"

namespace board
{
    /* The file enum represent the colomns
     * on the board */
    enum File
    {
        A = 0,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
    };

    /* The rank enum represent the lines
     * on the board */
    enum Rank
    {
        ONE = 0,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
    };

    /* Position represent a coordinate on the board */
    class Position final
    {
    public:
        Position(File file, Rank rank);
        Position(int array_pos);
        Position(const Position &position) = default;
        Position() = default;

        File file_get() const;
        Rank rank_get() const;

        inline int to_index() const
        {
            return static_cast<int>(rank_) * 8 + 7 - static_cast<int>(file_);
        }

        bool operator==(const Position &pos) const;
        bool operator!=(const Position &pos) const;

    private:
        File file_;
        Rank rank_;
    };

} // namespace board

#include "position.hxx"
/*
8 | 63 62 61 60 59 58 57 56
7 | 55 54 53 52 51 50 49 48
6 | 47 46 45 44 43 42 41 40
5 | 39 38 37 36 35 34 33 32
4 | 31 30 29 28 27 26 25 24
3 | 23 22 21 20 19 18 17 16
2 | 15 14 13 12 11 10 09 08
1 | 07 06 05 04 03 02 01 00
---------------------------
  | A  B  C  D  E  F  G  H
*/