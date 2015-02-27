// Implement your solutions in this file
#include "hamming.h"

unsigned int hamming(uint64_t x, uint64_t y)
{
    int d = 0;
    for (int i = 0; i < 64; i++) {
    	if (x%2 != y%2) {
    		d++;
    	}
    	x >>= 1;
    	y >>= 1;
    }
    return d;
    return 0;
}
