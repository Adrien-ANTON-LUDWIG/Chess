#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>

#include "AI.hh"
#include "alpha-beta.hh"
#include "board.hh"
#include "color.hh"
#include "game-tracer.hh"
#include "move.hh"
#include "negamax.hh"
#include "perft.hh"
#include "pgn-parser.hh"
#include "piece-type.hh"
#include "random.hh"
#include "simplified.hh"
#include "sum.hh"
#include "uci-time.hh"
#include "uci.hh"

namespace po = boost::program_options;

int main(int argc, const char *argv[])
{
    try
    {
        po::options_description desc{ "Options" };
        desc.add_options()("help,h", "UsAgE")("pgn", po::value<std::string>(),
                                              "path to the PGN game file")(
            "listeners,l", po::value<std::vector<std::string>>()->multitoken(),
            "list of paths to listener plugin")(
            "perft", po::value<std::string>(), "path to a perft file")(
            "play", po::value<std::string>(), "Stdin waits for moves")(
            "bench", po::value<std::string>(), "fen to test ai time on");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        std::string pgn_filepath = "";
        std::vector<std::string> listeners;

        if (!vm["listeners"].empty())
            listeners = vm["listeners"].as<std::vector<std::string>>();

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
        else
        {
            std::unique_ptr<ai::Evaluator> e =
                //   std::make_unique<ai::Sum_Evaluator>();
                std::make_unique<ai::Simplified_Evaluator>();

            // ai::Negamax ai(e);
            // ai::Random ai;
            ai::AlphaBeta ai(e);
            if (vm.count("bench"))
                ai.bench_uci(vm["bench"].as<std::string>());
            else
                ai.play_uci();
        }
    }
    catch (const po::error &e)
    {
        std::cerr << e.what() << '\n';
    }
}
