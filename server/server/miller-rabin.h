#ifndef H_MILLER_RABIN
#define H_MILLER_RABIN

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

#ifndef LARGE_NUMBERS
#define MRTYPE long long
#else
#define MRTYPE std::string
#endif

int Miller(MRTYPE p, int iteration);
MRTYPE modulo(MRTYPE base, MRTYPE exponent, MRTYPE mod);
MRTYPE mulmod(MRTYPE a, MRTYPE b, MRTYPE mod);

#endif
