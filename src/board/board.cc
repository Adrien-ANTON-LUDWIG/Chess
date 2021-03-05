#include "board.hh"

#include <iostream>
#include <string>

namespace board
{
    Chessboard::Chessboard()
    {
        pieces_[static_cast<int>(Color::WHITE)]
               [static_cast<int>(PieceType::QUEEN)] = 0x0000000000000010;
        pieces_[static_cast<int>(Color::WHITE)]
               [static_cast<int>(PieceType::ROOK)] = 0x0000000000000081;
        pieces_[static_cast<int>(Color::WHITE)]
               [static_cast<int>(PieceType::BISHOP)] = 0x0000000000000024;
        pieces_[static_cast<int>(Color::WHITE)]
               [static_cast<int>(PieceType::KNIGHT)] = 0x0000000000000042;
        pieces_[static_cast<int>(Color::WHITE)]
               [static_cast<int>(PieceType::PAWN)] = 0x000000000000FF00;
        pieces_[static_cast<int>(Color::WHITE)]
               [static_cast<int>(PieceType::KING)] = 0x0000000000000008;

        pieces_[static_cast<int>(Color::BLACK)]
               [static_cast<int>(PieceType::QUEEN)] = 0x1000000000000000;
        pieces_[static_cast<int>(Color::BLACK)]
               [static_cast<int>(PieceType::ROOK)] = 0x8100000000000000;
        pieces_[static_cast<int>(Color::BLACK)]
               [static_cast<int>(PieceType::BISHOP)] = 0x2400000000000000;
        pieces_[static_cast<int>(Color::BLACK)]
               [static_cast<int>(PieceType::KNIGHT)] = 0x4200000000000000;
        pieces_[static_cast<int>(Color::BLACK)]
               [static_cast<int>(PieceType::PAWN)] = 0x00FF000000000000;
        pieces_[static_cast<int>(Color::BLACK)]
               [static_cast<int>(PieceType::KING)] = 0x0800000000000000;
    }

    // Print the Chessboard in terminal
    void Chessboard::print_chessboard()
    {
        std::string characters[] = { "♛", "♜", "♝", "♞", "♟", "♚",
                                     "♕", "♖", "♗", "♘", "♙", "♔" };

        bool white = false;

        for (size_t j = 0; j < 64; j++)
        {
            if (j % 8 == 0)
                std::cout << std::to_string(8 - (j + 1) / 8) + " ";
            bool filled = false;

            if (white)
                std::cout << "\x1B[44m";

            for (int c = 0; c < 2; c++)
                for (int p = 0; p < 6; p++)
                {
                    if (pieces_[c][p][63 - j])
                    {
                        std::cout << " " << characters[c * 6 + p] << " ";
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

            white = !white;
        }
        std::cout << "   A  B  C  D  E  F  G  H\n\n";
        return;
    }

    // Update the board with a move
    void Chessboard::do_move(Move move)
    {
        int color = static_cast<int>(move.color_);
        int type = static_cast<int>(move.piece_type_);
        int end = move.end_.to_index();
        pieces_[color][type][move.start_.to_index()] = 0;

        for (int c = 0; c < 2; c++)
            for (int p = 0; p < 6; p++)
                if (pieces_[c][p][end])
                {
                    pieces_[c][p][end] = 0;
                    last_fifty_turn_ = 0;
                }

        pieces_[color][type][end] = 1;

        print_chessboard();
    }

    // Return the color of the piece at position
    std::optional<Color> Chessboard::piece_to_position(const Position &position)
    {
        for (int c = 0; c < 2; c++)
            for (int p = 0; p < 6; p++)
                if (pieces_[c][p][position.to_index()])
                    return static_cast<Color>(c);
        return std::nullopt;
    }

    /**
     * @brief Generates vector all possible moves for a king.
     *
     * @param position
     * @param color
     * @return std::vector<Move>
     */
    std::vector<Move>
    Chessboard::generate_legal_moves_knight(const Position &position,
                                            const Color &color)
    {
        std::vector<Move> moves;
        auto rank = position.rank_get();
        auto file = position.file_get();

        for (int i = -2; i <= 2; i++)
        {
            if (i == 0)
                continue;

            int j = 3 - abs(i);
            for (int k = 0; k < 2; k++)
            {
                if (i == 0 && j == 0)
                    continue;

                auto rank_int = static_cast<int>(rank) + i;
                auto file_int = static_cast<int>(file) + j;

                if (rank_int < 0 || rank_int > 7 || file_int < 0
                    || file_int > 7) // Add vérif case pas en échec
                    continue;

                auto new_rank = static_cast<Rank>(rank_int);
                auto new_file = static_cast<File>(file_int);
                Position move(new_file, new_rank);
                auto move_color = piece_to_position(move);
                if (move_color == std::nullopt
                    || move_color != color) // Add vérif case pas en échec
                    moves.push_back(
                        Move(color, PieceType::KING, position, move));

                j = -j;
            }
        }
        return moves;
    }

    // Generate a vector of all legal moves
    /**
     * @brief
     *
     * @param position
     * @param color
     * @return std::vector<Move>
     */
    std::vector<Move>
    Chessboard::generate_legal_moves_pawn(const Position &position,
                                          const Color &color)
    {
        // TODO en passant
        // TODO capture
        std::vector<Move> moves;
        static std::array pawns_rank = { Rank::TWO, Rank::SEVEN };
        int color_int = static_cast<int>(color);
        Position front(position.file_get(),
                       static_cast<Rank>(static_cast<int>(position.rank_get())
                                         + 1 * (-color_int)));
        auto front_color = piece_to_position(front);

        if (front_color == color)
            return moves;

        moves.push_back(Move(color, PieceType::PAWN, position, front));

        if (front_color != std::nullopt
            || position.rank_get() != pawns_rank[color_int])
            return moves;

        Position front2(position.file_get(),
                        static_cast<Rank>(static_cast<int>(position.rank_get())
                                          + 2 * (-color_int)));

        auto front2_color = piece_to_position(front2);

        if (front2_color == color)
            return moves;

        moves.push_back(Move(color, PieceType::PAWN, position, front2));
        return moves;
    }

    void Chessboard::generate_legal_moves_diagonal(
        const Move &move, const size_t &distance,
        std::vector<Move> &legal_moves, int dir_x, int dir_y)
    {
        if (!distance)
            return;
        if (dir_x == 0 && dir_y == 0)
        {
            generate_legal_moves_diagonal(move, distance, legal_moves, -1, -1);
            generate_legal_moves_diagonal(move, distance, legal_moves, 1, -1);
            generate_legal_moves_diagonal(move, distance, legal_moves, -1, 1);
            generate_legal_moves_diagonal(move, distance, legal_moves, 1, 1);
        }
        else
        {
            int pos_x = static_cast<int>(move.end_.file_get());
            int pos_y = static_cast<int>(move.end_.rank_get());
            int new_x = pos_x + dir_x;
            int new_y = pos_y + dir_y;
            if (new_y > static_cast<int>(Rank::EIGHT)
                || new_y < static_cast<int>(Rank::ONE)
                || new_x > static_cast<int>(File::H)
                || new_x < static_cast<int>(File::A))
            {
                return;
            }
            else
            {
                auto new_pos = Position(static_cast<File>(new_x),
                                        static_cast<Rank>(new_y));
                auto new_move =
                    Move(move.color_, move.piece_type_, move.start_, new_pos);
                auto piece = piece_to_position(new_pos);
                if (piece != std::nullopt && *piece == move.color_)
                    legal_moves.push_back(new_move);
                if (piece != std::nullopt)
                    return;
                legal_moves.push_back(new_move);
                generate_legal_moves_diagonal(new_move, distance - 1,
                                              legal_moves, dir_x, dir_y);
            }
        }
    }

    void
    Chessboard::generate_legal_moves_forward(const Move &move,
                                             const size_t &distance,
                                             std::vector<Move> &legal_moves)
    {
        generate_legal_moves_diagonal(move, distance, legal_moves, 1, 0);
        generate_legal_moves_diagonal(move, distance, legal_moves, 0, 1);
        generate_legal_moves_diagonal(move, distance, legal_moves, -1, 0);
        generate_legal_moves_diagonal(move, distance, legal_moves, 0, -1);
    }

    std::vector<Move>
    Chessboard::generate_legal_moves_generic(const Position &position,
                                             Color piece_color, PieceType type)
    {
        int distances[] = { 256, 256, 256, 0, 0, 1 };
        bool forwards[] = { true, true, false, false, false, true };
        bool diagonals[] = { true, false, true, false, false, true };

        bool forward = forwards[static_cast<int>(type)];
        int distance = distances[static_cast<int>(type)];
        bool diagonal = diagonals[static_cast<int>(type)];

        Move m = Move(piece_color, type, position, position);
        std::vector<Move> vec = std::vector<Move>();
        if (forward)
            generate_legal_moves_forward(m, distance, vec);
        if (diagonal)
            generate_legal_moves_diagonal(m, distance, vec, 0, 0);
        return vec;
    }

    bool is_move_legal(Move move)
    {
        (void)move;
        // Check if destination is same color
        // Check if destination is check
        // Check if there is another piece on the way (Knight ignore)
        // En passant
        // Castle
        return true;
    }

    std::vector<Move> Chessboard::generate_legal_moves()
    {
        std::vector<Move> moves;

        for (int c = 0; c < 2; c++)
            for (int p = 0; p < 6; p++)
                for (int i = 0; i < 64; i++)
                    if (pieces_[c][p][i])
                    {
                        Position position(i);
                        auto type = static_cast<PieceType>(p);
                        auto color = static_cast<Color>(c);

                        if (type == PieceType::PAWN)
                        {
                            auto new_moves =
                                generate_legal_moves_pawn(position, color);
                            moves.insert(moves.end(), new_moves.begin(),
                                         new_moves.end());
                        }
                        else if (type == PieceType::KNIGHT)
                        {
                            auto new_moves =
                                generate_legal_moves_knight(position, color);
                            moves.insert(moves.end(), new_moves.begin(),
                                         new_moves.end());
                        }
                        else
                        {
                            auto new_moves = generate_legal_moves_generic(
                                position, color, type);
                            moves.insert(moves.end(), new_moves.begin(),
                                         new_moves.end());
                        }
                    }

        return moves;
    }
} // namespace board

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