#include <boost/program_options.hpp>
#include <iostream>

#include "board.hh"
#include "move.hh"
#include "pgn-parser.hh"

namespace po = boost::program_options;

int main(int argc, const char *argv[])
{
    board::Chessboard *board = new board::Chessboard();
    board->print_chessboard();
    try
    {
        po::options_description desc{ "Options" };
        desc.add_options()("help,h", "UsAgE")("pgn", po::value<std::string>(),
                                              "path to the PGN game file")(
            "listener,l", po::value<std::vector<std::string>>()->multitoken(),
            "list of paths to listener plugin")(
            "perft", po::value<std::string>(),
            "path to a perft file")("play", "Stdin waits for moves");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("pgn"))
        {
            auto file = vm["pgn"].as<std::string>();
            auto moves = pgn_parser::parse_pgn(file);
            bool color = false;
            for (auto move : moves)
            {
                board->do_move(board::Move(static_cast<board::Color>(color),
                                           move.get_piece(), move.get_start(),
                                           move.get_end()));
                color = !color;
            }

            std::cout << "Pgn - " << file << " :\n";
            board->print_chessboard();

            // std::cout << "pgn: " << vm["pgn"].as<std::string>() << '\n';
        }
        else if (vm.count("listener"))
            std::cout << "listener: "
                      << (vm["listener"].as<std::vector<std::string>>()).size()
                      << '\n';
        else if (vm.count("perft"))
            std::cout << "perft: " << vm["perft"].as<std::string>() << '\n';

        else if (vm.count("play"))
        {
            bool color = false;

            while (1)
            {
                std::vector<std::string> move_string = { "" };
                std::cin >> move_string[0];

                auto moves = pgn_parser::string_to_move(move_string);

                board->do_move(board::Move(
                    static_cast<board::Color>(color), moves[0].get_piece(),
                    moves[0].get_start(), moves[0].get_end()));

                color = !color;
                board->is_check(static_cast<board::Color>(color));
            }
        }
    }
    catch (const po::error &e)
    {
        std::cerr << e.what() << '\n';
    }
    delete board;
}
