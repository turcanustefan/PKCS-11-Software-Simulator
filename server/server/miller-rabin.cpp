#include "miller-rabin.h"
/*
* calculates (a * b) % c taking into account that a * b might overflow
*/
MRTYPE mulmod(MRTYPE a, MRTYPE b, MRTYPE mod)
{
#ifndef LARGE_NUMBERS
	MRTYPE x = 0, y = a % mod;
	while (b > 0)
	{
		if (b % 2 == 1)
		{
			x = (x + y) % mod;
		}
		y = (y * 2) % mod;
		b /= 2;
	}
	return x % mod;
#else

#endif
}
/*
* modular exponentiation
*/
MRTYPE modulo(MRTYPE base, MRTYPE exponent, MRTYPE mod)
{
#ifndef LARGE_NUMBERS
	MRTYPE x = 1;
	MRTYPE y = base;
	while (exponent > 0)
	{
		if (exponent % 2 == 1)
			x = (x * y) % mod;
		y = (y * y) % mod;
		exponent = exponent / 2;
	}
	return x % mod;
#else

#endif
}

/*
* Miller-Rabin Primality test, iteration signifies the accuracy
* Shows if number is prime
*/
int Miller(MRTYPE p, int iteration)
{
#ifndef LARGE_NUMBERS
	int i;
	MRTYPE s;
	if (p < 2)
	{
		return 1;
	}
	if (p != 2 && p % 2 == 0)
	{
		return 1;
	}
	s = p - 1;
	while (s % 2 == 0)
	{
		s /= 2;
	}
	for (i = 0; i < iteration; i++)
	{
		MRTYPE a = rand() % (p - 1) + 1, temp = s;
		MRTYPE mod = modulo(a, temp, p);
		while (temp != p - 1 && mod != 1 && mod != p - 1)
		{
			mod = mulmod(mod, mod, p);
			temp *= 2;
		}
		if (mod != p - 1 && temp % 2 == 0)
		{
			return 1;
		}
	}
	return 0;
#else

#endif
}

