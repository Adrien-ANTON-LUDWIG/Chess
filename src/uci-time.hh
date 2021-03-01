#pragma once

#include <iostream>
#include <string>

#define DEFAULT_MOVETIME 10000

namespace ai
{
    struct UCITime
    {
        /*! \brief Initialize a UCITime object with default values. */
        UCITime();

        /*! \brief Sets the attribute named property to a given value. */
        bool set(const std::string& property, double value);

        /*! \brief Maximum amount of time for a move. */
        double movetime;

        /*! \brief Time left for white. */
        double wtime;

        /*! \brief Time left for black. */
        double btime;

        /*! \brief Per-move increment for white. */
        double winc;

        /*! \brief Per-move increment for black. */
        double binc;
    };
}