#pragma once

#include "AI.hh"

namespace ai
{
    class Minimax : public AI
    {
    public:
        board::Move best_move() override;
    };
} // namespace ai