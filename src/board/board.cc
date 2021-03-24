#include "board.hh"

#include <cmath>
#include <iostream>
#include <string>

namespace board
{
    Chessboard::Chessboard(bool empty)
    {
        if (empty)
        {
            king_castling_ = { false, false };
            queen_castling_ = { false, false };
            return;
        }
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

        color_boards_[static_cast<int>(Color::WHITE)] = 0x000000000000FFFF;
        color_boards_[static_cast<int>(Color::BLACK)] = 0xFFFF000000000000;
        board_ = 0xFFFF00000000FFFF;
    }

    // Print the Chessboard in terminal

    void Chessboard::print_chessboard(const Color &color)
    {
        std::string characters[] = { "♛", "♜", "♝", "♞", "♟", "♚",
                                     "♕", "♖", "♗", "♘", "♙", "♔" };

        std::bitset<64> possible_moves;
        auto legal_moves = generate_legal_moves(color);

        for (size_t i = 0; i < legal_moves.size(); i++)
            possible_moves[legal_moves[i].end_.to_index()] = 1;

        bool white = false;

        for (size_t j = 0; j < 64; j++)
        {
            if (j % 8 == 0)
                std::cout << std::to_string(8 - (j + 1) / 8) + " ";
            bool filled = false;

            if (possible_moves[63 - j])
                std::cout << "\x1B[45m";
            else if (white)
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

            if (white || possible_moves[63 - j])
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

    Color Chessboard::get_side_turn() const
    {
        return side_turn_;
    }

    void Chessboard::set_side_turn(const bool &side_turn)
    {
        side_turn_ = static_cast<Color>(side_turn);
    }

    void Chessboard::set_castling(Color color, PieceType type)
    {
        if (type == PieceType::KING)
            king_castling_[color] = true;
        else
            queen_castling_[color] = true;
    }

    void Chessboard::set_en_passant(Position pos)
    {
        en_passant_ = pos;
    }

    void Chessboard::update_piece(const int &color, const int &type,
                                  const int &position, const bool &arrive)
    {
        pieces_[color][type][position] = arrive;
        color_boards_[color][position] = arrive;
        board_[position] = arrive;
    }

    bool Chessboard::check_eating_en_passant(const Move &move)
    {
        static std::array behind = { 1, -1 };
        return (en_passant_ != std::nullopt
                && move.piece_type_ == PieceType::PAWN
                && move.end_.file_get() == en_passant_->file_get()
                && move.end_.rank_get() - en_passant_->rank_get()
                    == behind[move.color_]);
    }

    void Chessboard::update_en_passant(const Move &move)
    {
        static std::array en_passant_start = { Rank::TWO, Rank::SEVEN };
        static std::array en_passant_end = { Rank::FOUR, Rank::FIVE };

        if (move.piece_type_ == PieceType::PAWN
            && move.start_.rank_get() == en_passant_start[move.color_]
            && move.end_.rank_get() == en_passant_end[move.color_])
            en_passant_ = move.end_;
        else
            en_passant_ = std::nullopt;
    }

    // Return the color of the piece at position
    std::optional<Color>
    Chessboard::is_piece_to_position(const Position &position)
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
            for (int k = 0; k < 2; k++, j = -j)
            {
                auto rank_int = static_cast<int>(rank) + i;
                auto file_int = static_cast<int>(file) + j;

                if (rank_int < 0 || rank_int > 7 || file_int < 0
                    || file_int > 7)
                    continue;

                auto new_rank = static_cast<Rank>(rank_int);
                auto new_file = static_cast<File>(file_int);
                Position pos(new_file, new_rank);
                auto move_color = is_piece_to_position(pos);
                auto mv = Move(color, PieceType::KNIGHT, position, pos);
                mv.set_capture(*this);
                if (move_color == std::nullopt || move_color != color)
                    moves.push_back(mv);
            }
        }
        return moves;
    }

    int opposite_number(int a, bool neg)
    {
        if (neg)
            return -a;
        return a;
    }

    void Chessboard::check_promotion(std::vector<Move> &moves,
                                     const Color &color,
                                     const Position &position, const Move &move)
    {
        static std::array promotion_rank = { Rank::EIGHT, Rank::ONE };

        // Promotion
        if (position.rank_get() == promotion_rank[color])
            for (int i = 0; i < 4; i++)
            {
                Move prom_move = Move(move);
                prom_move.set_promotion(
                    static_cast<PieceType>(PieceType::QUEEN + i));
                moves.push_back(prom_move);
            }
        else
            moves.push_back(move);
    }

    void Chessboard::generate_pawn_capture(std::vector<Move> &moves,
                                           const Position &position,
                                           const Color &color, const File &file,
                                           const int &side)
    {
        if (position.file_get() != file)
        {
            Position capture(
                static_cast<File>(static_cast<int>(position.file_get()) + side),
                static_cast<Rank>(static_cast<int>(position.rank_get()) + 1
                                  - 2 * static_cast<int>(color)));
            auto capture_color = is_piece_to_position(capture);

            if (capture_color != std::nullopt && capture_color != color)
            {
                auto mv = Move(color, PieceType::PAWN, position, capture);
                mv.set_capture(*this);

                check_promotion(moves, color, capture, mv);
            }
        }
    }

    // Generate a vector of all legal moves
    std::vector<Move>
    Chessboard::generate_legal_moves_pawn(const Position &position,
                                          const Color &color)
    {
        std::vector<Move> moves;
        static std::array pawns_rank = { Rank::TWO, Rank::SEVEN };
        int color_int = static_cast<int>(color);
        int side = opposite_number(1, color_int);

        if (en_passant_ != std::nullopt
            && en_passant_->rank_get() == position.rank_get()
            && abs(en_passant_->file_get() - position.file_get()) == 1
            && color_boards_[!color][en_passant_->to_index()])
        {
            auto mv = Move(color, PieceType::PAWN, position,
                           Position(en_passant_->file_get(),
                                    static_cast<Rank>(en_passant_->rank_get()
                                                      + 1 * side)));
            mv.set_capture(*this);
            moves.push_back(mv);
        }

        // Capture
        generate_pawn_capture(moves, position, color, File::A, -1);
        generate_pawn_capture(moves, position, color, File::H, 1);

        // Move front
        Position front(position.file_get(),
                       static_cast<Rank>(static_cast<int>(position.rank_get())
                                         + 1 * side));

        auto front_color = is_piece_to_position(front);

        if (front_color != std::nullopt)
            return moves;

        auto mv = Move(color, PieceType::PAWN, position, front);

        check_promotion(moves, color, front, mv);

        if (position.rank_get() != pawns_rank[color_int])
            return moves;

        Position front2(position.file_get(),
                        static_cast<Rank>(static_cast<int>(position.rank_get())
                                          + 2 * side));

        auto front2_color = is_piece_to_position(front2);

        if (front2_color != std::nullopt)
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
                new_move.set_capture(*this);
                auto piece = is_piece_to_position(new_pos);

                if (piece != move.color_)
                    legal_moves.emplace_back(new_move);
                if (piece != std::nullopt)
                    return;
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
        // Q,R,B,P,N,K

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

    bool is_move_legal(const Move &move)
    {
        (void)move;
        return true;
    }

    bool Chessboard::is_castling_legal(const Color &side_turn,
                                       const Castling &castling_type)
    {
        static std::array<Position, 2> king_pos = {
            Position(File::E, Rank::ONE), Position(File::E, Rank::EIGHT)
        };
        int side = (castling_type == Castling::SMALL) ? 1 : -1;

        for (int i = 1; i < 3; i++)
        {
            Chessboard fake_board(*this);
            Move tmp =
                Move(side_turn, PieceType::KING, king_pos[side_turn],
                     Position(static_cast<File>(king_pos[side_turn].file_get()
                                                + i * side),
                              king_pos[side_turn].rank_get()));
            tmp.execute_move(fake_board);
            if (fake_board.is_check(side_turn))
                return false;
        }

        return true;
    }

    std::vector<Move>
    Chessboard::generate_castling_moves(const Color &side_turn)
    {
        static std::array<std::bitset<64>, 2> kc_mask = { 0x0000000000000006,
                                                          0x0600000000000000 };
        static std::array<std::bitset<64>, 2> qc_mask = { 0x0000000000000070,
                                                          0x7000000000000000 };

        std::vector<Move> castling_moves;
        if (Chessboard::is_check(side_turn))
            return castling_moves;

        if (king_castling_[side_turn] && (board_ & kc_mask[side_turn]) == 0
            && is_castling_legal(side_turn, Castling::SMALL))
        {
            castling_moves.push_back(Move(side_turn, Castling::SMALL));
        }
        if (queen_castling_[side_turn] && (board_ & qc_mask[side_turn]) == 0
            && is_castling_legal(side_turn, Castling::BIG))
        {
            castling_moves.push_back(Move(side_turn, Castling::BIG));
        }

        return castling_moves;
    }

    std::vector<Move> Chessboard::generate_legal_moves(const Color &side_turn)
    {
        std::vector<Move> moves;

        for (int p = 0; p < 6; p++)
            for (int i = 0; i < 64; i++)
                if (pieces_[side_turn][p][i])
                {
                    Position position(i);
                    auto type = static_cast<PieceType>(p);
                    auto color = side_turn;

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
                        auto new_moves =
                            generate_legal_moves_generic(position, color, type);
                        moves.insert(moves.end(), new_moves.begin(),
                                     new_moves.end());
                    }
                }

        if (!is_check(side_turn))
        {
            auto new_moves = generate_castling_moves(side_turn);
            moves.insert(moves.end(), new_moves.begin(), new_moves.end());
        }

        for (auto move = moves.begin(); move < moves.end(); move++)
        {
            Chessboard board(*this);
            move->execute_move(board);
            if (board.is_check(side_turn))
            {
                moves.erase(move);
                move--;
            }
        }

        return moves;
    }

    std::bitset<64>
    Chessboard::generate_move_king_like_other(const PieceType &p,
                                              const Color &c)
    {
        Position king = Position(log2(pieces_[c][PieceType::KING].to_ullong()));

        std::vector<Move> legal_moves;
        if (p == PieceType::PAWN)
            legal_moves = generate_legal_moves_pawn(king, c);
        else if (p == PieceType::KNIGHT)
            legal_moves = generate_legal_moves_knight(king, c);
        else
            legal_moves = generate_legal_moves_generic(king, c, p);

        std::bitset<64> possible_moves;
        for (size_t i = 0; i < legal_moves.size(); i++)
            possible_moves[legal_moves[i].end_.to_index()] = 1;

        return possible_moves;
    }

    bool Chessboard::is_king_collision(const PieceType &piecetype,
                                       const Color &color)
    {
        std::bitset<64> movement =
            generate_move_king_like_other(piecetype, color);
        std::bitset<64> ennemy = pieces_[!color][piecetype];
        std::bitset<64> collision = movement & ennemy;
        return collision != 0;
    }

    bool Chessboard::is_check(const Color &color)
    {
        int i = 0;
        while (i < 6)
        {
            if (is_king_collision(static_cast<PieceType>(i), color))
                return true;
            i++;
        }
        return false;
    }

    bool Chessboard::is_checkmate(const Color &color)
    {
        return is_check(color) && generate_legal_moves(color).size() == 0;
    }

    bool Chessboard::is_stalemate(const Color &color)
    {
        return !is_check(color) && generate_legal_moves(color).size() == 0;
    }

    bool Chessboard::is_draw()
    {
        return halfmove_clock_ >= 100;
    }

    bool Chessboard::is_equivalent(const Chessboard &other)
    {
        return pieces_ == other.pieces_;
    }

    std::string Chessboard::to_fen()
    {
        static std::string characters[] = { "Q", "R", "B", "N", "P", "K",
                                            "q", "r", "b", "n", "p", "k" };
        std::string fen;

        for (ssize_t i = 7; i >= 0; i--)
        {
            size_t empty = 0;
            for (ssize_t j = 7; j >= 0; j--)
            {
                bool found = false;

                for (size_t c = 0; c < 2; c++)
                {
                    for (size_t p = 0; p < 6; p++)
                    {
                        size_t index = i * 8 + j;
                        if (pieces_[c][p][index])
                        {
                            found = true;
                            if (empty)
                                fen += std::to_string(empty);
                            empty = 0;
                            fen += characters[c * 6 + p];
                        }
                    }
                }
                if (!found)
                    empty++;
            }

            if (empty)
                fen += std::to_string(empty);
            empty = 0;
            if (i)
            {
                fen += '/';
            }
        }

        fen += ' ';
        if (side_turn_ == Color::WHITE)
            fen += "w ";
        else
            fen += "b ";

        if (king_castling_[0])
            fen += 'K';
        if (queen_castling_[0])
            fen += 'Q';
        if (king_castling_[1])
            fen += 'k';
        if (queen_castling_[1])
            fen += 'q';
        if (fen[fen.size() - 1] == ' ')
            fen += '-';

        fen += ' ';

        if (en_passant_ == std::nullopt)
            fen += "- ";
        else
        {
            fen += static_cast<int>(en_passant_->file_get()) + 'a';
            fen += static_cast<int>(en_passant_->rank_get()) + '1';
            fen += " ";
        }

        fen += "0 1";
        return fen;
    }

    PieceType Chessboard::get_piece_type(const Color &color,
                                         const Position &position)
    {
        int index = position.to_index();
        for (size_t p = 0; p < 6; p++)
            if (pieces_[static_cast<int>(color)][p][index])
                return static_cast<PieceType>(p);

        throw std::logic_error("No piece here, circulez y a rien à voir");
        return PieceType::KING;
    }
} // namespace board

// rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 1

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