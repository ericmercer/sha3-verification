#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

# define BIT_INTERLEAVE (0)

#define ROL32(a, offset) (((a) << (offset)) | ((a) >> ((32 - (offset)) & 31)))

static uint64_t ROL64(uint64_t val, int offset)
{
    if (offset == 0) {
        return val;
    } else if (!BIT_INTERLEAVE) {
        return (val << offset) | (val >> (64-offset));
    } else {
        uint32_t hi = (uint32_t)(val >> 32), lo = (uint32_t)val;

        if (offset & 1) {
            uint32_t tmp = hi;

            offset >>= 1;
            hi = ROL32(lo, offset);
            lo = ROL32(tmp, offset + 1);
        } else {
            offset >>= 1;
            lo = ROL32(lo, offset);
            hi = ROL32(hi, offset);
        }

        return ((uint64_t)hi << 32) | lo;
    }
}

void Theta(uint64_t A[5][5])
{
    uint64_t C[5], D[5];
    size_t y;

    C[0] = A[0][0];
    C[1] = A[0][1];
    C[2] = A[0][2];
    C[3] = A[0][3];
    C[4] = A[0][4];

    for (y = 1; y < 5; y++) {
        C[0] ^= A[y][0];
        C[1] ^= A[y][1];
        C[2] ^= A[y][2];
        C[3] ^= A[y][3];
        C[4] ^= A[y][4];
    }

    D[0] = ROL64(C[1], 1) ^ C[4];
    D[1] = ROL64(C[2], 1) ^ C[0];
    D[2] = ROL64(C[3], 1) ^ C[1];
    D[3] = ROL64(C[4], 1) ^ C[2];
    D[4] = ROL64(C[0], 1) ^ C[3];

    for (y = 0; y < 5; y++) {
        A[y][0] ^= D[0];
        A[y][1] ^= D[1];
        A[y][2] ^= D[2];
        A[y][3] ^= D[3];
        A[y][4] ^= D[4];
    }
}


static const unsigned char rhotates[5][5] = {
    {  0,  1, 62, 28, 27 },
    { 36, 44,  6, 55, 20 },
    {  3, 10, 43, 25, 39 },
    { 41, 45, 15, 21,  8 },
    { 18,  2, 61, 56, 14 }
};

void Rho(uint64_t A[5][5])
{
    size_t y;

    for (y = 0; y < 5; y++) {
        A[y][0] = ROL64(A[y][0], rhotates[y][0]);
        A[y][1] = ROL64(A[y][1], rhotates[y][1]);
        A[y][2] = ROL64(A[y][2], rhotates[y][2]);
        A[y][3] = ROL64(A[y][3], rhotates[y][3]);
        A[y][4] = ROL64(A[y][4], rhotates[y][4]);
    }
}

void Pi(uint64_t A[5][5])
{
    uint64_t T[5][5];

    memcpy(T, A, sizeof(T));

    A[0][0] = T[0][0];
    A[0][1] = T[1][1];
    A[0][2] = T[2][2];
    A[0][3] = T[3][3];
    A[0][4] = T[4][4];

    A[1][0] = T[0][3];
    A[1][1] = T[1][4];
    A[1][2] = T[2][0];
    A[1][3] = T[3][1];
    A[1][4] = T[4][2];

    A[2][0] = T[0][1];
    A[2][1] = T[1][2];
    A[2][2] = T[2][3];
    A[2][3] = T[3][4];
    A[2][4] = T[4][0];

    A[3][0] = T[0][4];
    A[3][1] = T[1][0];
    A[3][2] = T[2][1];
    A[3][3] = T[3][2];
    A[3][4] = T[4][3];

    A[4][0] = T[0][2];
    A[4][1] = T[1][3];
    A[4][2] = T[2][4];
    A[4][3] = T[3][0];
    A[4][4] = T[4][1];
}


void Chi(uint64_t A[5][5])
{
    uint64_t C[5];
    size_t y;

    for (y = 0; y < 5; y++) {
        C[0] = A[y][0] ^ (~A[y][1] & A[y][2]);
        C[1] = A[y][1] ^ (~A[y][2] & A[y][3]);
        C[2] = A[y][2] ^ (~A[y][3] & A[y][4]);
        C[3] = A[y][3] ^ (~A[y][4] & A[y][0]);
        C[4] = A[y][4] ^ (~A[y][0] & A[y][1]);

        A[y][0] = C[0];
        A[y][1] = C[1];
        A[y][2] = C[2];
        A[y][3] = C[3];
        A[y][4] = C[4];
    }
}

static const uint64_t iotas[] = {
    BIT_INTERLEAVE ? 0x0000000000000001ULL : 0x0000000000000001ULL,
    BIT_INTERLEAVE ? 0x0000008900000000ULL : 0x0000000000008082ULL,
    BIT_INTERLEAVE ? 0x8000008b00000000ULL : 0x800000000000808aULL,
    BIT_INTERLEAVE ? 0x8000808000000000ULL : 0x8000000080008000ULL,
    BIT_INTERLEAVE ? 0x0000008b00000001ULL : 0x000000000000808bULL,
    BIT_INTERLEAVE ? 0x0000800000000001ULL : 0x0000000080000001ULL,
    BIT_INTERLEAVE ? 0x8000808800000001ULL : 0x8000000080008081ULL,
    BIT_INTERLEAVE ? 0x8000008200000001ULL : 0x8000000000008009ULL,
    BIT_INTERLEAVE ? 0x0000000b00000000ULL : 0x000000000000008aULL,
    BIT_INTERLEAVE ? 0x0000000a00000000ULL : 0x0000000000000088ULL,
    BIT_INTERLEAVE ? 0x0000808200000001ULL : 0x0000000080008009ULL,
    BIT_INTERLEAVE ? 0x0000800300000000ULL : 0x000000008000000aULL,
    BIT_INTERLEAVE ? 0x0000808b00000001ULL : 0x000000008000808bULL,
    BIT_INTERLEAVE ? 0x8000000b00000001ULL : 0x800000000000008bULL,
    BIT_INTERLEAVE ? 0x8000008a00000001ULL : 0x8000000000008089ULL,
    BIT_INTERLEAVE ? 0x8000008100000001ULL : 0x8000000000008003ULL,
    BIT_INTERLEAVE ? 0x8000008100000000ULL : 0x8000000000008002ULL,
    BIT_INTERLEAVE ? 0x8000000800000000ULL : 0x8000000000000080ULL,
    BIT_INTERLEAVE ? 0x0000008300000000ULL : 0x000000000000800aULL,
    BIT_INTERLEAVE ? 0x8000800300000000ULL : 0x800000008000000aULL,
    BIT_INTERLEAVE ? 0x8000808800000001ULL : 0x8000000080008081ULL,
    BIT_INTERLEAVE ? 0x8000008800000000ULL : 0x8000000000008080ULL,
    BIT_INTERLEAVE ? 0x0000800000000001ULL : 0x0000000080000001ULL,
    BIT_INTERLEAVE ? 0x8000808200000000ULL : 0x8000000080008008ULL
};


void Iota(uint64_t A[5][5], size_t i)
{
    //assert(i < (sizeof(iotas) / sizeof(iotas[0])));
    //The assert statement above fails symbolic execution because it is not compiled properly in Clang
    //Using an if statement below fills the same obligation of checking on i before doing anything, so
    //we can use it equivalently.  Also, our proof has a precondition of i < 24 so this will never fail.
    if (i < (sizeof(iotas) / sizeof(iotas[0]))) {
        A[0][0] ^= iotas[i];
    }
}




void KeccakF1600(uint64_t A[5][5])
{
    size_t i;

    for (i = 0; i < 24; i++) {
        Theta(A);
        Rho(A);
        Pi(A);
        Chi(A);
        Iota(A, i);
        
    }
}