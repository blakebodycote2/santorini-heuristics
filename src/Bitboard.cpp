// Bitboard.cpp
#include "Bitboard.hpp"


Bitboard::Bitboard() : bitboard(0U) {}

Bitboard::Bitboard(U32 board) : bitboard(board) {}

// Get bit value at a specific square
bool Bitboard::getBit(int square) const 
{
    return (bitboard & (1U << square)) != 0;
}

U32 Bitboard::getBitboard()
{
    return bitboard;
}

// Set bit at a specific square
void Bitboard::setBit(int square) 
{
    bitboard |= (1U << square);
}

// Remove bit at a specific square
void Bitboard::removeBit(int square) 
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
void Bitboard::printBitboard() const 
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

Bitboard Bitboard::getAdjSquares() const
{
    // boundaries to prevent board-wrapping
    const U32 left_mask  = 0b0111101111011110111101111;
    const U32 right_mask = 0b1111011110111101111011110;


    U32 adj = 0U;
    adj |= (bitboard >> 6) & left_mask;   // Up-left
    adj |= (bitboard >> 5);               // Up
    adj |= (bitboard >> 4) & right_mask;  // Up-right
    adj |= (bitboard >> 1) & left_mask;   // Left
    adj |= (bitboard << 1) & right_mask;  // Right
    adj |= (bitboard << 4) & left_mask;   // Down-left
    adj |= (bitboard << 5);               // Down
    adj |= (bitboard << 6) & right_mask;  // Down-right

    return Bitboard(adj);
}

Bitboard Bitboard::clearWithMask(Bitboard mask)
{
    return Bitboard(bitboard & ~mask.getBitboard());
}

void Bitboard::combine(Bitboard other)
{
    bitboard |= other.getBitboard();
}