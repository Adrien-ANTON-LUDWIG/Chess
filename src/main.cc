#include <boost/program_options.hpp>
#include <iostream>

#include "board.hh"

namespace po = boost::program_options;

int main(int argc, const char *argv[])
{
    board::Chessboard *ch = new board::Chessboard();
    ch->print_chessboard();
    delete ch;
    try
    {
        po::options_description desc{ "Options" };
        desc.add_options()("help,h", "UsAgE")("pgn", po::value<std::string>(),
                                              "path to the PGN game file")(
            "listener,l", po::value<std::vector<std::string>>()->multitoken(),
            "list of paths to listener plugin")(
            "perft", po::value<std::string>(), "path to a perft file");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("pgn"))
            std::cout << "pgn: " << vm["pgn"].as<std::string>() << '\n';
        else if (vm.count("listener"))
            std::cout << "listener: "
                      << (vm["listener"].as<std::vector<std::string>>()).size()
                      << '\n';
        else if (vm.count("perft"))
            std::cout << "perft: " << vm["perft"].as<std::string>() << '\n';
    }
    catch (const po::error &e)
    {
        std::cerr << e.what() << '\n';
    }
}
