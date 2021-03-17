#include <boost/program_options.hpp>
#include <iostream>

#include "board.hh"
#include "color.hh"
#include "game-tracer.hh"
#include "move.hh"
#include "perft.hh"
#include "pgn-parser.hh"
#include "piece-type.hh"

namespace po = boost::program_options;

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
    }
    catch (const po::error &e)
    {
        std::cerr << e.what() << '\n';
    }
}
