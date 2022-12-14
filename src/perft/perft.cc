#include "perft.hh"

#include <fstream>

board::Chessboard fen_to_board(std::istream &fen)
{
    std::string token;

    fen >> token;
    board::Chessboard board = init_board_from_perft(token);
    token = "";

    fen >> token;
    auto side_turn = complete_board_information(token);
    board.set_side_turn(side_turn);
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

int run_from_perft(std::string perft_file)
{
    std::string line;
    std::ifstream file(perft_file);
    if (file.is_open())
    {
        board::Chessboard board = fen_to_board(file);

        file >> line;
        int depth = std::stoi(line);
        return start_game_perft(board, depth, board.get_side_turn());
    }
    return 0;
}

#include <iostream>

int start_game_perft(board::Chessboard board, int depth, board::Color side_turn)
{
    if (!depth)
        return 1;
    // std::cout << board.to_fen() << " " << depth << "\n";
    int nodes = 0;
    auto moves = board.generate_legal_moves(side_turn);
    // std::cout << "Legal moves: " << moves.size() << "\n";
    // std::cout << "Depth value: " << depth << "\n";
    for (size_t i = 0; i < moves.size(); i++)
    {
        auto temp_board = board::Chessboard(board);
        moves[i].execute_move(temp_board);

        /*
                {
                    std::cout << "\n---\n";
                    board.print_chessboard(side_turn);
                    temp_board.print_chessboard(side_turn);
                    std::cout << moves[i];
                    std::string unused;
                    std::cin >> unused;
                }
        */

        nodes += start_game_perft(
            temp_board, depth - 1,
            static_cast<board::Color>(!static_cast<int>(side_turn)));
    }
    return nodes;
}
// rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 1

void configurer_roque(std::string perft_string, board::Chessboard &board)
{
    if (perft_string[0] == '-')
        return;
    for (size_t i = 0; i < perft_string.size(); i++)
    {
        if (perft_string[i] == 'K')
            board.set_castling(board::Color::WHITE, board::PieceType::KING);
        else if (perft_string[i] == 'Q')
            board.set_castling(board::Color::WHITE, board::PieceType::QUEEN);
        else if (perft_string[i] == 'k')
            board.set_castling(board::Color::BLACK, board::PieceType::KING);
        else if (perft_string[i] == 'q')
            board.set_castling(board::Color::BLACK, board::PieceType::QUEEN);
    }
}

void set_enpassant(std::string perft_string, board::Chessboard &board,
                   board::Color color)
{
    if (perft_string[0] == '-')
        return;
    if (color == board::Color::WHITE) // So our ennemy is black
        perft_string[1] -= 1;
    else
        perft_string[1] += 1;
    board::Position pos(
        static_cast<board::File>(tolower(perft_string[0]) - 'a'),
        static_cast<board::Rank>(perft_string[1] - '1'));
    board.set_en_passant(pos);
}

board::Color complete_board_information(std::string perft_string)
{
    size_t index = 0;
    board::Color side = (perft_string[index] == 'w') ? board::Color::WHITE
                                                     : board::Color::BLACK;

    return side;
}

board::Chessboard init_board_from_perft(std::string perft_string)
{
    board::Chessboard b(true);
    int pos = 63;
    for (size_t i = 0; i < perft_string.size() && pos >= 0; i++)
    {
        board::Color color;
        board::PieceType type;

        if (perft_string[i] == '/')
            continue;
        if (isdigit(perft_string[i]))
            pos -= perft_string[i] - '0';
        else if (isalpha(perft_string[i]))
        {
            if (islower(perft_string[i]))
                color = board::Color::BLACK;
            else
                color = board::Color::WHITE;
            if (tolower(perft_string[i]) == 'r')
                type = board::PieceType::ROOK;
            else if (tolower(perft_string[i]) == 'n')
                type = board::PieceType::KNIGHT;
            else if (tolower(perft_string[i]) == 'b')
                type = board::PieceType::BISHOP;
            else if (tolower(perft_string[i]) == 'q')
                type = board::PieceType::QUEEN;
            else if (tolower(perft_string[i]) == 'k')
                type = board::PieceType::KING;
            else
                type = board::PieceType::PAWN;
            b.update_piece(color, type, pos, 1);
            pos--;
        }
    }
    return b;
}

/*
8 | 63 62 61 60 59 58 57 56
7 | 55 54 53 52 51 50 49 48
6 | 47 46 45 44 43 42 41 40
5 | 39 38 37 36 35 34 33 32
4 | 31 30 29 28 27 26 25 24
3 | 23 22 21 20 19 18 17 16
2 | 15 14 13 12 11 10 09 08
1 | 07 06 05 04 03 02 01 00
---------------------------
  | A  B  C  D  E  F  G  H
*/