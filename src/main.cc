#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>

#include "board.hh"
#include "color.hh"
#include "game-tracer.hh"
#include "move.hh"
#include "perft.hh"
#include "pgn-parser.hh"
#include "piece-type.hh"
#include "uci-time.hh"
#include "uci.hh"

namespace po = boost::program_options;

board::Chessboard fen_to_board(std::istream &fen)
{
    std::string token;

    fen >> token;
    board::Chessboard board = init_board_from_perft(token);
    token = "";

    fen >> token;
    auto side_turn = complete_board_information(token);
    token = "";

    fen >> token;
    configurer_roque(token, board);
    token = "";

    fen >> token;
    set_enpassant(token, board, side_turn);
    token = "";

    // Halfmove clock
    fen >> token;
    int black_cpt = std::stoi(token);
    (void)black_cpt;
    token = "";

    // Fullmove
    fen >> token;
    int white_cpt = std::stoi(token);
    (void)white_cpt;
    token = "";

    return board;
}

int main(int argc, const char *argv[])
{
    try
    {
        po::options_description desc{ "Options" };
        desc.add_options()("help,h", "UsAgE")("pgn", po::value<std::string>(),
                                              "path to the PGN game file")(
            "listener,l", po::value<std::vector<std::string>>()->multitoken(),
            "list of paths to listener plugin")(
            "perft", po::value<std::string>(), "path to a perft file")(
            "play", po::value<std::string>(), "Stdin waits for moves");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        std::string pgn_filepath = "";
        std::vector<std::string> listeners;

        if (!vm["listener"].empty())
            listeners = vm["listener"].as<std::vector<std::string>>();

        if (!vm["pgn"].empty())
            pgn_filepath = vm["pgn"].as<std::string>();

        board::GameTracer game_tracer(pgn_filepath, listeners);

        if (vm.count("pgn"))
            game_tracer.play_pgn_game();
        else if (vm.count("perft"))
            std::cout << run_from_perft(vm["perft"].as<std::string>())
                      << std::endl;
        else if (vm.count("play"))
        {
            std::string fen = "";
            if (!vm["play"].empty())
                fen = vm["play"].as<std::string>();
            game_tracer.interractive(fen);
        }

        std::string logs;
        std::ofstream log_file("logs");

        board::Chessboard board;
        ai::UCITime uci_time;
        std::string token;

        while (1)
        {
            std::string uci = ai::get_board("PandaISniper", uci_time);
            std::stringstream uci_ss(uci);

            logs += uci + "\n";
            log_file << logs << std::endl;

            uci_ss >> token;

            if (token != "position")
                throw std::invalid_argument("Expected 'position' got '" + token
                                            + "'");

            uci_ss >> token;

            if (token == "startpos")
                board = board::Chessboard();
            else if (token == "fen")
                board = fen_to_board(uci_ss);
            else
                throw std::invalid_argument("Expected 'startpos' or 'fen got '"
                                            + token + "'");

            uci_ss >> token;

            if (token == "moves")
            {
                while (uci_ss >> token)
                    board::Move(token, board).execute_move(board);
            }

            std::vector<board::Move> moves =
                board.generate_legal_moves(board.get_side_turn());

            if (moves.size() == 0)
                throw std::logic_error("No legal move available");

            srand(time(NULL));
            board::Move move = moves[rand() % moves.size()];
            // board::Move move = moves[0];

            ai::play_move(move.to_ebnf());
        }
    }
    catch (const po::error &e)
    {
        std::cerr << e.what() << '\n';
    }
}
