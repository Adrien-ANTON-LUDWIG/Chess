#include "exceptional_move.hh"

namespace board
{
    void ExceptionalMove::execute_move(Chessboard &board)
    {
        board.update_piece(color_, PieceType::KING, start_.to_index(), 0);
        board.update_piece(color_, PieceType::ROOK, end_.to_index(), 0);
        if (move_type_ == ExceptionalMoveType::CASTLING_SMALL)
        {
            Position new_rook_pos = Position(
                static_cast<File>(static_cast<int>(end_.file_get()) - 2),
                end_.rank_get());
            Position new_king_pos = Position(
                static_cast<File>(static_cast<int>(start_.file_get()) + 2),
                start_.rank_get());
            board.update_piece(color_, PieceType::KING, new_king_pos.to_index(),
                               1);
            board.update_piece(color_, PieceType::ROOK, new_rook_pos.to_index(),
                               1);
        }
        else if (move_type_ == ExceptionalMoveType::CASTLING_BIG)
        {
            Position new_rook_pos = Position(
                static_cast<File>(static_cast<int>(end_.file_get()) + 3),
                end_.rank_get());
            Position new_king_pos = Position(
                static_cast<File>(static_cast<int>(start_.file_get()) - 2),
                start_.rank_get());
            board.update_piece(color_, PieceType::KING, new_king_pos.to_index(),
                               1);
            board.update_piece(color_, PieceType::ROOK, new_rook_pos.to_index(),
                               1);
        }
    }
} // namespace board