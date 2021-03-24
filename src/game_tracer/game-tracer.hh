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

        void handle_castling(const PgnMove &move, const bool &color);
        void handle_move(const PgnMove &move, const bool &color);
        void play_pgn_move(PgnMove &move, bool &color);
        void play_pgn_game();
        void interractive(std::string &fen);

        void check_if_legal(const Move &mv, const bool &color);
        void check_state(const PgnMove &move, const bool &color);
        void check_draw();
        void check_checkmate(const PgnMove &move, const bool &color);
        void check_check_and_stalemate(const PgnMove &move, const bool &color);
        bool is_draw_repetition();

    private:
        Chessboard board_;
        std::string pgn_filepath_;
        std::vector<Listener *> listeners_;

        // History
        std::vector<Chessboard> history_;
    };

} // namespace board