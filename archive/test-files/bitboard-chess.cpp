#include <stdio.h>

// define bitboard data type
using U32 = unsigned int;

// board squares
enum {
    a5, b5, c5, d5, e5,
    a4, b4, c4, d4, e4,
    a3, b3, c3, d3, e3,
    a2, b2, c2, d2, e2,
    a1, b1, c1, d1, e1
};

// ------- MACROS -------
bool getBit(U32 bitboard, int square) 
{
    return (bitboard & (1ULL << square));
}

void setBit(U32 &bitboard, int square) 
{
    bitboard |= (1ULL << square);
}

void removeBit(U32 &bitboard, int square) 
{
    if (getBit(bitboard, square)) 
        bitboard ^= (1ULL << square);
}

// print bitboard
void printBitboard(U32 bitboard)
{
    printf("\n");

    // loop over board ranks
    for (int rank = 0; rank < 5; rank++)
    {
        // loop over board files
        for (int file = 0; file < 5; file++)
        {
            // convert file & rank into square index
            int square = rank * 5 + file;

            // print ranks
            if (file == 0)
                printf(" %d ", 5 - rank);

            // print bit state (either 1 or 0)
            printf(" %d ", getBit(bitboard, square) ? 1 : 0);
        }

        // print new line every rank
        printf("\n");
    }

    // print board files
    printf("\n    a  b  c  d  e\n\n");

    // print bitboard as unsigned decimal number
    printf("Bitboard : %llud\n\n", bitboard);
}



int main()
{
    // define bitboard
    U32 bitboard = 0ULL;

    // set bits
    setBit(bitboard, e5);
    setBit(bitboard, c3);

    // print bitboard
    printBitboard(bitboard);

    // remove bit
    removeBit(bitboard, e4);

    // print bitboard
    printBitboard(bitboard);

    return 0;
}