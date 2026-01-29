#ifndef POSITION_H
#define POSITION_H

#include "Bitboard.hpp"
#include <array>
#include <iostream>

/*
* A class storing a Connect 4 position.
* Function are relative to the current player to play.
* Position containing aligment are not supported by this class.
*/
class Position {
public:
    Bitboard workers1;
    Bitboard workers2;
    std::array<Bitboard, 5> board;

    Position()
        : workers1(0U),
          workers2(0U)
    {
        board.fill(Bitboard(0U));
        board[0] = Bitboard((1ULL << 25) - 1); // All squares start at level 0

        workers1.setBit(b4);
        workers1.setBit(d2);

        workers2.setBit(b2);
        workers2.setBit(d4);
    }

    int getLevel(int square) const 
    {
        for (int i = 0; i < 4; i++)
            if (board[i].getBit(square)) return i;
        return 4; // If none match, it must be level 4.
    }

    void setLevel(int square, int level) 
    {
        board[level].removeBit(square);
        board[level+1].setBit(square);
    }

   /**
    * Indicates whether a column is playable.
    * @param col: 0-based index of column to play
    * @return true if the column is playable, false if the column is already full.
    */
    bool canMove(int from, int to) const
    {
        int fromLevel = getLevel(from);
        int toLevel = getLevel(to);
        return !(toLevel == 4) && !(fromLevel + 1 < toLevel);
    }


    bool canBuild(int square) const 
    {
        return !(board[4].getBit(square));
    }

   /**
    * Plays a playable column.
    * This function should not be called on a non-playable column or a column making an alignment.
    *
    * @param col: 0-based index of a playable column.
    */
    void play(int player, int from, int to, int build)
    {
        // Move
        if (player == 1)
        {
            workers1.removeBit(from);
            workers1.setBit(to);
        } 
        else if (player == 2)
        {
            workers2.removeBit(from);
            workers2.setBit(to);
        }
        // Build
        int buildLevel = getLevel(build);
        setLevel(build, buildLevel);
    }

   /**
    * Indicates whether the current player wins by playing a given column.
    * This function should never be called on a non-playable column.
    * @param col: 0-based index of a playable column.
    * @return true if current player makes an alignment by playing the corresponding column col.
    */
   bool isWinningMove(int square) const
   {
        return (board[3].getBit(square));
   }


   unsigned int nbMoves() const;
};

#endif