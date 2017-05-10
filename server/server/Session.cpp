#include "Session.h"

Session::Session()
{
	currentEncryptBufferLength = 0;
	currentEncryptBuffer = NULL_PTR;
	currentEncryptionKey = NULL_PTR;
	currentMechanismEncrypt = NULL_PTR;

	currentDecryptionKey = NULL_PTR;
	currentMechanismDecrypt = NULL_PTR;


	userLoggedIn = CK_FALSE;
	sessionInfo.flags = 0;
}
