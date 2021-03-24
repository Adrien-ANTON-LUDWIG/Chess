#pragma once

#include <optional>

#include "color.hh"
#include "piece-type.hh"
#include "position.hh"
#include "report-type.hh"

namespace board
{
    /* The move symbolize a movement of a piece. Every turn should
     * correspond to one move. It contains every informations related
     * to the action */
    class PgnMove final
    {
    public:
        /* initialize it with PgnMove::opt_piece_t { [PieceType] } */
        using opt_piece_t = std::optional<PieceType>;

        PgnMove(const Position &start, const Position &end, PieceType piece,
                bool capture, ReportType report,
                const opt_piece_t &promotion = std::nullopt);

        /*! \brief Generate a castling given a color and a side */
        static PgnMove generate_castling(bool queen_side, Color color);

        void report_set(ReportType report);

        PieceType get_piece() const;
        const Position get_start() const;
        const Position get_end() const;
        opt_piece_t get_promotion() const;
        bool get_capture() const;
        ReportType get_report() const;

    private:
        // The original position of the piece
        const Position start_;

        // The destination of the piece
        const Position end_;

        // The type of the moving piece
        PieceType piece_;

        // if promotion, contains the piecetype generated by the promotion
        opt_piece_t promotion_;

        // if the move is a capture
        bool capture_;

        // type of report given by the pgn file
        ReportType report_;
    };
} // namespace board
