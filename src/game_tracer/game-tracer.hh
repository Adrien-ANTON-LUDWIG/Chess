#pragma once

#include "board.hh"
#include "listener.hh"
#include "pgn-parser.hh"

namespace board
{
    using Listener = listener::Listener;

    class GameTracer
    {
    public:
        GameTracer(std::string pgn_filepath,
                   std::vector<std::string> listeners);
        ~GameTracer();

        void play_pgn_move(PgnMove &move, bool &color);
        void play_pgn_game();
        void interractive(std::string &fen);

        bool is_draw_repetition();

    private:
        Chessboard board_;
        std::string pgn_filepath_;
        std::vector<Listener *> listeners_;

        // History
        std::vector<Chessboard> history_;
    };

} // namespace board