#include "uci.hh"

#include <fnmatch.h>
#include <iostream>
#include <regex>
#include <sstream>

namespace ai
{
    namespace
    {
        /**
         * Handles the `uci` UCI command.
         * \param name The name of the AI to print.
        */
        void uci(const std::string& name)
        {
            std::cout << "id name " << name << '\n';
            std::cout << "id author " << name << '\n';
            std::cout << "uciok" << std::endl;
        }

        /**
         * Handles the `isready` UCI command.
        */
        void isready()
        {
            std::cout << "readyok" << std::endl;
        }

        /**
         * Read std::cin until a string matching the given regex is matched.
         * Exits the program when reading `quit`, `stopuci`, or an empty line.
         * Calls the `uci` and `isready` functions when needed.
         * Ignores any unrecognized command.
         * \param expected The string of the regex to match.
         * \param name The name to print when a `uci` command is read.
        */
        std::string get_input(const std::string& expected = "*",
            const std::string& name = "")
        {
            std::regex re(expected);
            std::string buffer;
            do
            {
                std::getline(std::cin, buffer);
                if (buffer == "quit" || buffer == "stopuci" || buffer == "")
                    exit(0);
                if (buffer == "uci")
                    uci(name);
                if (buffer == "isready")
                    isready();
            } while (!std::regex_match(buffer, re));
            return buffer;
        }

        /**
         * Stores the time information found following a `go` command.
         * \param uci_time The structure to store the times to.
         * \param command The line to extra the time information from.
        */
        void parse_times(UCITime& uci_time, const std::string& command)
        {
            std::stringstream ss(command);
            std::string param;
            ss >> param;

            while (ss >> param)
            {
                size_t value;
                ss >> value;

                if (!uci_time.set(param, value))
                    std::cerr << "UCI: unrecognized go parameter: "
                        << param << std::endl;
            }
        }
    } // namespace

    void play_move(const std::string& move)
    {
        // Send the computed move
        std::cout << "bestmove " << move << std::endl;
    }

    std::string get_board(const std::string& name, UCITime& uci_time)
    {
        std::string board("");
        std::string go_command("");

        while (go_command.empty())
        {
            // Get either a `position` or `go` command
            std::string got = get_input("(position .*)|(go.*)", name);

            if (!fnmatch("go*", got.c_str(), 0))
            {
                // A `go` command was read, make sure a position was given.
                if (board.empty())
                {
                    std::cerr << "Got `go' command before postion. "
                              << "Skipping..." << std::endl;
                    continue;
                }

                // Get the time information from the command.
                parse_times(uci_time, got);
                break;
            }
            else
                board = got;
        }

        return board;
    }
} // namespace ai
