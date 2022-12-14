#pragma once

#include "evaluator.hh"

namespace ai
{
    class Simplified_Evaluator : public Evaluator
    {
    public:
        bool is_endgame(const board::Chessboard &board);
        float evaluate_board(const board::Chessboard &board) override;
        float controlled_ground(const board::Chessboard &board);

        float heuristics[2][7][64] = {
            {
                // white queen
                { -20, -10, -10, -5,  -5,  -10, -10, -20, -10, 0,   0,   0,  0,
                  0,   0,   -10, -10, 0,   5,   5,   5,   5,   0,   -10, -5, 0,
                  5,   5,   5,   5,   0,   -5,  0,   0,   5,   5,   5,   5,  0,
                  -5,  -10, 5,   5,   5,   5,   5,   0,   -10, -10, 0,   5,  0,
                  0,   0,   0,   -10, -20, -10, -10, -5,  -5,  -10, -10, -20 },

                // white rook
                { 0,  0, 0, 0, 0, 0, 0, 0,  5,  10, 10, 10, 10, 10, 10, 5,
                  -5, 0, 0, 0, 0, 0, 0, -5, -5, 0,  0,  0,  0,  0,  0,  -5,
                  -5, 0, 0, 0, 0, 0, 0, -5, -5, 0,  0,  0,  0,  0,  0,  -5,
                  -5, 0, 0, 0, 0, 0, 0, -5, 0,  0,  0,  5,  5,  0,  0,  0 },

                // white bishop
                { -20, -10, -10, -10, -10, -10, -10, -20, -10, 0,   0,   0,   0,
                  0,   0,   -10, -10, 0,   5,   10,  10,  5,   0,   -10, -10, 5,
                  5,   10,  10,  5,   5,   -10, -10, 0,   10,  10,  10,  10,  0,
                  -10, -10, 10,  10,  10,  10,  10,  10,  -10, -10, 5,   0,   0,
                  0,   0,   5,   -10, -20, -10, -10, -10, -10, -10, -10, -20 },

                // white knight
                { -50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0,   0,   0,
                  0,   -20, -40, -30, 0,   10,  15,  15,  10,  0,   -30, -30, 5,
                  15,  20,  20,  15,  5,   -30, -30, 0,   15,  20,  20,  15,  0,
                  -30, -30, 5,   10,  15,  15,  10,  5,   -30, -40, -20, 0,   5,
                  5,   0,   -20, -40, -50, -40, -30, -30, -30, -30, -40, -50 },

                // white pawn
                { 0.0f,  0.0f,  0.0f,   0.0f,   0.0f,   0.0f,   0.0f,  0.0f,
                  50.0f, 50.0f, 50.0f,  50.0f,  50.0f,  50.0f,  50.0f, 50.0f,
                  10.0f, 10.0f, 20.0f,  30.0f,  30.0f,  20.0f,  10.0f, 10.0f,
                  5.0f,  5.0f,  10.0f,  25.0f,  25.0f,  10.0f,  5.0f,  5.0f,
                  0.0f,  0.0f,  0.0f,   20.0f,  20.0f,  0.0f,   0.0f,  0.0f,
                  5.0f,  -5.0f, -10.0f, 0.0f,   0.0f,   -10.0f, -5.0f, 5.0f,
                  5.0f,  10.0f, 10.0f,  -20.0f, -20.0f, 10.0f,  10.0f, 5.0f,
                  0.0f,  0.0f,  0.0f,   0.0f,   0.0f,   0.0f,   0.0f,  0 },

                // white king middle game
                { -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40,
                  -50, -50, -40, -40, -30, -30, -40, -40, -50, -50, -40,
                  -40, -30, -30, -40, -40, -50, -50, -40, -40, -30, -20,
                  -30, -30, -40, -40, -30, -30, -20, -10, -20, -20, -20,
                  -20, -20, -20, -10, 20,  20,  0,   0,   0,   0,   20,
                  20,  20,  30,  10,  0,   0,   10,  30,  20 },

                // white king end game
                { -50, -40, -30, -20, -20, -30, -40, -50, -30, -20, -10,
                  0,   0,   -10, -20, -30, -30, -10, 20,  30,  30,  20,
                  -10, -30, -30, -10, 30,  40,  40,  30,  -10, -30, -30,
                  -10, 30,  40,  40,  30,  -10, -30, -30, -10, 20,  30,
                  30,  20,  -10, -30, -30, -30, 0,   0,   0,   0,   -30,
                  -30, -50, -30, -30, -30, -30, -30, -30, -50 },
            },
            {

                // black queen
                { -20, -10, -10, -5,  -5,  -10, -10, -20, -10, 0,   5,   0,  0,
                  0,   0,   -10, -10, 5,   5,   5,   5,   5,   0,   -10, 0,  0,
                  5,   5,   5,   5,   0,   -5,  -5,  0,   5,   5,   5,   5,  0,
                  -5,  -10, 0,   5,   5,   5,   5,   0,   -10, -10, 0,   0,  0,
                  0,   0,   0,   -10, -20, -10, -10, -5,  -5,  -10, -10, -20 },

                // black rook
                { 0,  0,  0,  5,  5,  0,  0,  0,  -5, 0, 0, 0, 0, 0, 0, -5,
                  -5, 0,  0,  0,  0,  0,  0,  -5, -5, 0, 0, 0, 0, 0, 0, -5,
                  -5, 0,  0,  0,  0,  0,  0,  -5, -5, 0, 0, 0, 0, 0, 0, -5,
                  5,  10, 10, 10, 10, 10, 10, 5,  0,  0, 0, 0, 0, 0, 0, 0 },

                // black bishop
                { -20, -10, -10, -10, -10, -10, -10, -20, -10, 5,   0,   0,   0,
                  0,   5,   -10, -10, 10,  10,  10,  10,  10,  10,  -10, -10, 0,
                  10,  10,  10,  10,  0,   -10, -10, 5,   5,   10,  10,  5,   5,
                  -10, -10, 0,   5,   10,  10,  5,   0,   -10, -10, 0,   0,   0,
                  0,   0,   0,   -10, -20, -10, -10, -10, -10, -10, -10, -20 },

                // black knight
                { -50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0,   5,   5,
                  0,   -20, -40, -30, 5,   10,  15,  15,  10,  5,   -30, -30, 0,
                  15,  20,  20,  15,  0,   -30, -30, 5,   15,  20,  20,  15,  5,
                  -30, -30, 0,   10,  15,  15,  10,  0,   -30, -40, -20, 0,   0,
                  0,   0,   -20, -40, -50, -40, -30, -30, -30, -30, -40, -50 },

                // black pawn
                { 0.0f,  0.0f,  0.0f,   0.0f,   0.0f,   0.0f,   0.0f,  0.0f,
                  5.0f,  10.0f, 10.0f,  -20.0f, -20.0f, 10.0f,  10.0f, 5.0f,
                  5.0f,  -5.0f, -10.0f, 0.0f,   0.0f,   -10.0f, -5.0f, 5.0f,
                  0.0f,  0.0f,  0.0f,   20.0f,  20.0f,  0.0f,   0.0f,  0.0f,
                  5.0f,  5.0f,  10.0f,  25.0f,  25.0f,  10.0f,  5.0f,  5.0f,
                  10.0f, 10.0f, 20.0f,  30.0f,  30.0f,  20.0f,  10.0f, 10.0f,
                  50.0f, 50.0f, 50.0f,  50.0f,  50.0f,  50.0f,  50.0f, 50.0f,
                  0.0f,  0.0f,  0.0f,   0.0f,   0.0f,   0.0f,   0.0f,  0 },

                // black king start
                { 20,  30,  10,  0,   0,   10,  30,  20,  20,  20,  0,
                  0,   0,   0,   20,  20,  -10, -20, -20, -20, -20, -20,
                  -20, -10, -20, -30, -30, -40, -40, -30, -30, -20, -30,
                  -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50,
                  -50, -40, -40, -30, -30, -40, -40, -50, -50, -40, -40,
                  -30, -30, -40, -40, -50, -50, -40, -40, -30 },

                // black king end game
                { -50, -30, -30, -30, -30, -30, -30, -50, -30, -30, 0,
                  0,   0,   0,   -30, -30, -30, -10, 20,  30,  30,  20,
                  -10, -30, -30, -10, 30,  40,  40,  30,  -10, -30, -30,
                  -10, 30,  40,  40,  30,  -10, -30, -30, -10, 20,  30,
                  30,  20,  -10, -30, -30, -20, -10, 0,   0,   -10, -20,
                  -30, -50, -40, -30, -20, -20, -30, -40, -50 } }
        };
    };
} // namespace ai