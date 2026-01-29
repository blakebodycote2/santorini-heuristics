// Bitboard.h
#ifndef BITBOARD_H
#define BITBOARD_H

#include <iostream>

// Define bitboard data type
using U32 = unsigned int;

// board squares
enum {
    a5, b5, c5, d5, e5,
    a4, b4, c4, d4, e4,
    a3, b3, c3, d3, e3,
    a2, b2, c2, d2, e2,
    a1, b1, c1, d1, e1
};

class Bitboard {
private:
    U32 bitboard; // 32-bit bitboard to represent the 5x5 grid

public:
    // Constructor to initialize bitboard to 0
    Bitboard() : bitboard(0U) {}

    Bitboard(U32 board) : bitboard(board) {}

    // Get bit value at a specific square
    bool getBit(int square) const 
    {
        return (bitboard & (1U << square)) != 0;
    }

    // Set bit at a specific square
    void setBit(int square) 
    {
        bitboard |= (1U << square);
    }

    // Remove bit at a specific square
    void removeBit(int square) 
    {
        if (getBit(square)) 
        {
            bitboard ^= (1U << square);
        }
        else
        {
            std::cout << "ERROR: NOTHING REMOVED AT " << square << std::endl;
        }
    }

    // Print the current bitboard
    void printBitboard() const 
    {
        std::cout << "\n";

        for (int row = 0; row < 5; row++) {
            for (int column = 0; column < 5; column++) {
                // Convert column & row into square index
                int square = row * 5 + column;

                // Print row labels
                if (column == 0)
                    std::cout << " " << 5 - row << " ";

                // Print bit state (either 1 or 0)
                std::cout << " " << (getBit(square) ? 1 : 0);
            }
            std::cout << "\n";
        }

        // Print column labels
        std::cout << "\n    a b c d e\n\n";

        // Print bitboard as unsigned decimal number
        std::cout << "Bitboard: " << bitboard << "\n\n";
    }
};

#endif