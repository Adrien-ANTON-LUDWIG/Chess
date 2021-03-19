#include "game-tracer.hh"

#include <algorithm>
#include <dlfcn.h>
#include <iostream>

#include "perft.hh"

namespace board
{
    GameTracer::GameTracer(std::string pgn_filepath,
                           std::vector<std::string> listeners)
        : board_()
        , pgn_filepath_(pgn_filepath)
    {
        (void)listeners;
        history_.push_back(board_);
#ifndef _STATIC
        for (auto l : listeners)
        {
            auto handle = dlopen(l.c_str(), RTLD_LAZY);

            auto symbol = dlsym(handle, "listener_create");
            Listener *listener = reinterpret_cast<Listener *(*)()>(symbol)();
            listeners_.push_back(listener);
        }
#endif
    }

    GameTracer::~GameTracer()
    {
#ifndef _STATIC
        for (size_t i = 0; i < listeners_.size(); i++)
            dlclose(listeners_[i]);
#endif
    }

    void GameTracer::play_pgn_move(PgnMove &move, bool &color)
    {
        if (move.get_piece() == PieceType::KING
            && abs(move.get_start().file_get() - move.get_end().file_get())
                == 2)
        {
            if (move.get_capture() || move.get_promotion() != std::nullopt)
            {
                for (auto l : listeners_)
                {
                    l->on_player_disqualified(static_cast<Color>(color));
                    l->on_game_finished();
                }

                exit(0);
            }

            auto mv = Move(static_cast<Color>(color), move.get_start(),
                           move.get_end());

            auto legal_moves =
                board_.generate_legal_moves(static_cast<Color>(color));
            if (std::find(legal_moves.begin(), legal_moves.end(), mv)
                == legal_moves.end())
            {
                for (auto l : listeners_)
                {
                    l->on_player_disqualified(static_cast<Color>(color));
                    l->on_game_finished();
                }

                exit(0);
            }

            mv.execute_move(board_);

            for (auto l : listeners_)
            {
                l->on_piece_moved(mv.piece_type_, mv.start_, move.get_end());

                if (mv.move_type_ == Castling::SMALL)
                    l->on_kingside_castling(mv.color_);
                else
                    l->on_queenside_castling(mv.color_);
            }
        }
        else
        {
            auto mv = Move(static_cast<Color>(color), move.get_piece(),
                           move.get_start(), move.get_end());

            if (move.get_capture())
                if (!mv.set_capture(board_))
                {
                    for (auto l : listeners_)
                    {
                        l->on_player_disqualified(static_cast<Color>(color));
                        l->on_game_finished();
                    }

                    exit(0);
                }

            auto promotion = move.get_promotion();
            if (promotion != std::nullopt)
                if (!mv.set_promotion(*promotion))
                {
                    for (auto l : listeners_)
                    {
                        l->on_player_disqualified(static_cast<Color>(color));
                        l->on_game_finished();
                    }

                    exit(0);
                }

            auto legal_moves =
                board_.generate_legal_moves(static_cast<Color>(color));
            if (std::find(legal_moves.begin(), legal_moves.end(), mv)
                == legal_moves.end())
            {
                for (auto l : listeners_)
                {
                    l->on_player_disqualified(static_cast<Color>(color));
                    l->on_game_finished();
                }

                exit(0);
            }

            mv.execute_move(board_);

            for (auto l : listeners_)
            {
                l->on_piece_moved(mv.piece_type_, mv.start_, mv.end_);

                if (mv.capture_ != std::nullopt)
                    l->on_piece_taken(mv.capture_->first, mv.end_);

                if (mv.promotion_)
                    l->on_piece_promoted(mv.promotion_type_, mv.end_);
            }
        }

        history_.push_back(board_);

        if ((move.get_report() == ReportType::CHECK
             && !board_.is_check(static_cast<Color>(!color)))
            || (move.get_report() == ReportType::CHECKMATE
                && !board_.is_checkmate(static_cast<Color>(!color)))
            || (move.get_report() == ReportType::NONE
                && (board_.is_check(static_cast<Color>(!color)))))
        {
            for (auto l : listeners_)
            {
                l->on_player_disqualified(static_cast<Color>(color));
                l->on_game_finished();
            }

            exit(0);
        }

        if (move.get_report() == ReportType::CHECKMATE)
        {
            for (auto l : listeners_)
            {
                l->on_player_mat(static_cast<Color>(!color));
                l->on_game_finished();
            }

            exit(0);
        }
        else if (move.get_report() == ReportType::CHECK)
            for (auto l : listeners_)
                l->on_player_check(static_cast<Color>(!color));
        else if (board_.is_stalemate(static_cast<Color>(!color)))
        {
            for (auto l : listeners_)
            {
                l->on_player_pat(static_cast<Color>(!color));
                l->on_draw();
                l->on_game_finished();
            }

            exit(0);
        }
        if (board_.is_draw() || is_draw_repetition())
        {
            for (auto l : listeners_)
            {
                l->on_draw();
                l->on_game_finished();
            }

            exit(0);
        }

        color = !color;
        // board_.print_chessboard(static_cast<Color>(color));
    }

    void GameTracer::play_pgn_game()
    {
        for (auto l : listeners_)
            l->on_game_started();

        auto moves = pgn_parser::parse_pgn(pgn_filepath_);
        bool color = false;
        // board_.print_chessboard(static_cast<Color>(color));

        for (auto move : moves)
            play_pgn_move(move, color);

        // std::cout << "Pgn - " << pgn_filepath_ << " :\n";
        // board_.print_chessboard(static_cast<Color>(color));
    }

    void GameTracer::interractive(std::string &fen)
    {
        bool color = false;

        if (!fen.empty())
        {
            board_ = init_board_from_perft(fen);
            board_.set_castling(Color::WHITE, PieceType::QUEEN);
            board_.set_castling(Color::BLACK, PieceType::KING);
        }

        board_.print_chessboard(static_cast<Color>(color));

        while (1)
        {
            std::vector<std::string> move_string = { "" };
            std::cin >> move_string[0];

            auto moves = pgn_parser::string_to_move(move_string);

            play_pgn_move(moves[0], color);
            board_.print_chessboard(static_cast<Color>(color));
            board_.is_check(static_cast<Color>(color));
        }
    }

    bool GameTracer::is_draw_repetition()
    {
        int count = 0;

        for (auto state : history_)
        {
            if (board_.is_equivalent(state))
            {
                count++;
            }
            if (count == 3)
                return true;
        }

        return false;
    }
} // namespace board