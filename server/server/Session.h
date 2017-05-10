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


	CK_VOID_PTR currentEncryptionKey;
	CK_MECHANISM_TYPE currentMechanismEncrypt;
	CK_BYTE_PTR currentEncryptBuffer;
	CK_ULONG currentEncryptBufferLength;
	
	CK_VOID_PTR currentDecryptionKey;
	CK_MECHANISM_TYPE currentMechanismDecrypt;
 
	/* Constructors and Destructors */
	Session();

};

#endif