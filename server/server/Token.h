#ifndef H_TOKEN
#define H_TOKEN
#include "cryptoki.h"
#include "Session.h"
#include "sqlite3.h"
#include <vector>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/engine.h>
#include <openssl/rand.h>

#define DB_NAME "db.sqlite"
#define CKU_MAX_PIN_LEN 32
#define CKU_MIN_PIN_LEN 10
#define CKU_PIN_TRYOUTS 10
#define CKU_PIN_COUNT_LOW 3
#define CKU_MAX_SESSION_COUNT 5
#define CKU_MAX_RW_SESSION_COUNT 2
#define CKU_TOTAL_PRIVATE_MEMORY 4096
#define CKU_TOTAL_PUBLIC_MEMORY 8192
#define CKU_MAX_TOKEN_OBJECTS 128

struct Token
{
	CK_ULONG tokenID;
	CK_TOKEN_INFO tokenInfo;
	CK_UTF8CHAR_PTR pin;
	CK_ULONG pinLen;
	CK_ULONG wrongPinCounter;
	CK_ULONG mechanismCounter;
	CK_MECHANISM_PTR mechanismList;
	sqlite3 *pDatabase;
	CK_OBJECT_HANDLE_PTR objectList;
	CK_ULONG objectCounter;

	/* Constructors and Destructors */

	Token(CK_ULONG tokenID);
	~Token();

	CK_RV C_InsertAttributes(CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_KEY_TYPE keyType, CK_ULONG keyHandler);
	CK_RV C_InsertKey(CK_MECHANISM_PTR pMechanism, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phKey);
	CK_RV C_InsertKeyPair(CK_MECHANISM_PTR pMechanism, CK_ATTRIBUTE_PTR pPublicKeyTemplate, CK_ULONG ulPublicKeyAttributeCount, CK_ATTRIBUTE_PTR pPrivateKeyTemplate, CK_ULONG ulPrivateKeyAttributeCount, CK_OBJECT_HANDLE_PTR phPublicKey, CK_OBJECT_HANDLE_PTR phPrivateKey);
	CK_RV C_FindObjects(CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount);
	CK_BBOOL C_IsMechanismAvailable(CK_MECHANISM_TYPE);
	
};

#endif