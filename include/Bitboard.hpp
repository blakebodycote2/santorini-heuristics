#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <iostream>

using U32 = unsigned int;

// Enum for board squares
enum 
{
    a5, b5, c5, d5, e5,
    a4, b4, c4, d4, e4,
    a3, b3, c3, d3, e3,
    a2, b2, c2, d2, e2,
    a1, b1, c1, d1, e1
};

class Bitboard 
{
private:
    U32 bitboard;

public:
    Bitboard();
    Bitboard(U32 board);

    U32 getBitboard();
    bool getBit(int square) const;
    void setBit(int square);
    void removeBit(int square);
    void printBitboard() const;

    Bitboard getAdjSquares() const;
    void combine(Bitboard board);
    Bitboard clearWithMask(Bitboard mask);
    
};

#endif // BITBOARD_HPP

