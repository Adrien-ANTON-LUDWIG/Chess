#pragma once

#include <cctype>

#include "board.hh"

board::Chessboard init_board_from_perft(std::string perft_string);
board::Color complete_board_information(std::string perft_string);
int start_game_perft(board::Chessboard board, int &white_cpt, int &black_cpt,
                     int depth, board::Color side_turn);
int run_from_perft(std::string perft_file);
void configurer_roque(std::string perft_string, board::Chessboard &board);
void set_enpassant(std::string perft_string, board::Chessboard &board);