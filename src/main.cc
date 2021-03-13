#include <algorithm>
#include <boost/program_options.hpp>
#include <iostream>

#include "board.hh"
#include "color.hh"
#include "move.hh"
#include "pgn-parser.hh"
#include "piece-type.hh"

namespace po = boost::program_options;

void play_pgn_move(board::Chessboard &board, board::PgnMove &move, bool &color)
{
    if (move.get_piece() == board::PieceType::KING
        && abs(move.get_start().file_get() - move.get_end().file_get()) == 2)
    {
        if (move.get_capture() || move.get_promotion() != std::nullopt)
        {
            std::cerr << "Disqualification" << std::endl;
            exit(0);
        }

        auto mv = board::Move(static_cast<board::Color>(color),
                              move.get_start(), move.get_end());

        auto legal_moves =
            board.generate_legal_moves(static_cast<board::Color>(color));
        if (std::find(legal_moves.begin(), legal_moves.end(), mv)
            == legal_moves.end())
        {
            std::cerr << "Disqualification" << std::endl;
            exit(0);
        }

        mv.execute_move(board);
    }
    else
    {
        auto mv =
            board::Move(static_cast<board::Color>(color), move.get_piece(),
                        move.get_start(), move.get_end());

        if (move.get_capture())
            if (!mv.set_capture(board))
            {
                std::cerr << "Disqualification" << std::endl;
                exit(0);
            }

        auto promotion = move.get_promotion();
        if (promotion != std::nullopt)
            if (!mv.set_promotion(*promotion))
            {
                std::cerr << "Disqualification" << std::endl;
                exit(0);
            }

        auto legal_moves =
            board.generate_legal_moves(static_cast<board::Color>(color));
        if (std::find(legal_moves.begin(), legal_moves.end(), mv)
            == legal_moves.end())
        {
            std::cerr << "Disqualification" << std::endl;
            exit(0);
        }

        mv.execute_move(board);
    }

    if ((move.get_report() == ReportType::CHECK
         && !board.is_check(static_cast<board::Color>(!color)))
        || (move.get_report() == ReportType::CHECKMATE
            && !board.is_checkmate(static_cast<board::Color>(!color)))
        || (move.get_report() == ReportType::NONE
            && (board.is_check(static_cast<board::Color>(!color)))))
    {
        std::cerr << "Disqualification" << std::endl;
        exit(0);
    }

    color = !color;
    board.print_chessboard(static_cast<board::Color>(color));
}

void play_pgn_game(const std::string &filename, board::Chessboard &board)
{
    auto moves = pgn_parser::parse_pgn(filename);
    bool color = false;
    for (auto move : moves)
        play_pgn_move(board, move, color);

    std::cout << "Pgn - " << filename << " :\n";
    board.print_chessboard(static_cast<board::Color>(color));
}

void interractive(board::Chessboard &board)
{
    bool color = false;

    while (1)
    {
        std::vector<std::string> move_string = { "" };
        std::cin >> move_string[0];

        auto moves = pgn_parser::string_to_move(move_string);

        play_pgn_move(board, moves[0], color);
        board.is_check(static_cast<board::Color>(color));
    }
}

int main(int argc, const char *argv[])
{
    board::Chessboard board;
    board.print_chessboard(board::Color::WHITE);
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
            play_pgn_game(vm["pgn"].as<std::string>(), board);
        else if (vm.count("listener"))
            std::cout << "listener: "
                      << (vm["listener"].as<std::vector<std::string>>()).size()
                      << '\n';
        else if (vm.count("perft"))
            std::cout << "perft: " << vm["perft"].as<std::string>() << '\n';

        else if (vm.count("play"))
            interractive(board);
    }
    catch (const po::error &e)
    {
        std::cerr << e.what() << '\n';
    }
}
