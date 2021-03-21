#pragma once

#include "AI.hh"

namespace ai
{
    class Random : public AI
    {
    public:
        Random() = default;
        board::Move best_move() override;
    };
} // namespace ai