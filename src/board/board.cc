#include "board.hh"

#include <iostream>
#include <string>

namespace board
{
    Chessboard::Chessboard()
    {
        black_pawn_ = 0x00FF000000000000;
        black_rook_ = 0x8100000000000000;
        black_knight_ = 0x4200000000000000;
        black_bishop_ = 0x2400000000000000;
        black_queen_ = 0x1000000000000000;
        black_king_ = 0x0800000000000000;
        white_pawn_ = 0x000000000000FF00;
        white_rook_ = 0x0000000000000081;
        white_bishop_ = 0x0000000000000024;
        white_knight_ = 0x0000000000000042;
        white_queen_ = 0x0000000000000010;
        white_king_ = 0x0000000000000008;
    }

    void Chessboard::print_chessboard()
    {
        std::string characters[] = { "♟", "♜", "♞", "♝", "♛", "♚",
                                     "♙", "♖", "♘", "♗", "♕", "♔" };
        std::vector pieces = { black_pawn_,   black_rook_,  black_knight_,
                               black_bishop_, black_queen_, black_king_,
                               white_pawn_,   white_rook_,  white_knight_,
                               white_bishop_, white_queen_, white_king_ };

        std::string board = "";
        bool white = false;

        uint64_t i = 1;
        for (size_t j = 0; j < 64; j++)
        {
            if (j % 8 == 0)
                std::cout << std::to_string(8 - (j + 1) / 8) + " ";
            bool filled = false;

            if (white)
                std::cout << "\x1B[44m";

            for (int p = 0; p < 12; p++)
            {
                if (i & pieces[p])
                {
                    std::cout << " " << characters[p] << " ";
                    filled = true;
                }
            }
            if (!filled)
                std::cout << "   ";

            if (white)
                std::cout << "\x1B[0m";

            if ((j + 1) % 8 == 0)
            {
                std::cout << '\n';
                white = !white;
            }

            i = i * 2;
            white = !white;
        }
        std::cout << "   A  B  C  D  E  F  G  H\n";
        return;
    }
} // namespace board