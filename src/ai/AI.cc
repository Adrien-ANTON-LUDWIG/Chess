#include "AI.hh"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "board.hh"
#include "perft.hh"
#include "uci-time.hh"
#include "uci.hh"

namespace ai
{
    void AI::play_uci()
    {
        ai::UCITime uci_time;
        std::string token;

        while (1)
        {
            std::string uci = ai::get_board("PandaISniper", uci_time);
            std::stringstream uci_ss(uci);

            uci_ss >> token;

            if (token != "position")
                throw std::invalid_argument("Expected 'position' got '" + token
                                            + "'");

            uci_ss >> token;

            if (token == "startpos")
                board_ = board::Chessboard();
            else if (token == "fen")
                board_ = fen_to_board(uci_ss);
            else
                throw std::invalid_argument("Expected 'startpos' or 'fen got '"
                                            + token + "'");

            uci_ss >> token;

            if (token == "moves")
            {
                while (uci_ss >> token)
                    board::Move(token, board_).execute_move(board_);
            }

            board::Move move = best_move();

            play_move(move.to_ebnf());
        }
    }

    void AI::bench_uci(const std::string &fen)
    {
        std::stringstream fen_ss(fen);
        board_ = fen_to_board(fen_ss);
        board::Move move = best_move();
        play_move(move.to_ebnf());
    }
} // namespace ai