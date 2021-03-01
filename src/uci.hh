#pragma once

#include <string>

#include "uci-time.hh"

namespace ai
{
    /** Send a move to GUI
     * \param move: String following EBNF
     * Eg:
     * - e2e4
     * - e1g1
     */
    void play_move(const std::string& move);

    /** Reads lines until both a `position` and `go` commands are entered.
     * This function handles a subset of the `UCI` protocol.
     * If a `uci` or `isready` command is entered, it will answer accordingly.
     * \param name The name to return if asked
     * \param uci_time A structure that will contain the time information
     *      given by the `go` command.
     * \returns a string following the format:
     *      position [startpos | fen FEN] (moves ...)
     * Eg:
     * - position startpos
     * - position startpos moves e2e4
     * - position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
     * - position fen rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1
     *      moves f7f5
     */
    std::string get_board(const std::string& name, UCITime& uci_time);
} // namespace ai
