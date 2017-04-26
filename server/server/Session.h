#ifndef H_SESSION
#define H_SESSION

#include "cryptoki.h"

#define CK_MECHANISM_COUNT 3

struct Session
{
	CK_SESSION_HANDLE sessionHandler;
	CK_SESSION_INFO sessionInfo;
	CK_BBOOL userLoggedIn;
	CK_BBOOL isPinEnabled;
	/* Constructors and Destructors */
	Session();

};

#endif