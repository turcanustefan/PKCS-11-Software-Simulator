#include "ServerPKCS.h"

int main()
{
	/* C_Initialise */
	ServerPKCS* p = NULL_PTR;
	ServerPKCS::C_CreateInstance();
	ServerPKCS::C_GetInstance(p);
	p->C_Initialize(NULL_PTR);
	CK_INFO_PTR pi = NULL_PTR;
	p->C_GetInfo(pi);
	/* C_GetSlotList */
	CK_SLOT_ID_PTR freeSlots;
	CK_ULONG slotCount;
	p->C_GetSlotList(TRUE, freeSlots, &slotCount);
	/* C_OpenSession */
	CK_SESSION_HANDLE hSession;
	p->C_OpenSession(freeSlots[0], CKF_RW_SESSION | CKF_SERIAL_SESSION, NULL, NULL, &hSession);
	char p11PinCode[] = "1234567890";
	p->C_Login(hSession, CKU_USER, (CK_CHAR_PTR)p11PinCode, (CK_ULONG)strlen(p11PinCode));
	

	Token* token = new Token(0);
	/*CK_OBJECT_HANDLE obj;
	CK_MECHANISM mec = { CKM_AES_KEY_GEN, NULL, 0 };
	CK_ATTRIBUTE_PTR att = new CK_ATTRIBUTE[3];
	CK_BYTE id[] = { 0xa1 };
	CK_KEY_TYPE type = CKK_AES;
	att[0].type = CKA_LABEL;
	att[0].pValue = "TOKEN LABEL";
	att[0].ulValueLen = strlen((char*)att[0].pValue);
	att[1].type = CKA_KEY_TYPE;
	att[1].pValue = (void*)&type;
	att[2].type = CKA_ID;
	att[2].pValue = static_cast<CK_VOID_PTR>(id);
	att[2].ulValueLen = sizeof(id);
	token->C_InsertKey(&mec, att, 3, &obj);
	token->C_FindObjects(att, 3);*/
	CK_OBJECT_HANDLE hPublicKey, hPrivateKey;
	CK_MECHANISM mechanism = {
		CKM_RSA_PKCS_KEY_PAIR_GEN, NULL_PTR, 0
	};

	CK_ULONG modulusBits = 768;
	CK_BYTE publicExponent[] = { 3 };
	CK_BYTE id[] = { 123 };
	CK_BBOOL encrypt = CK_TRUE, verify = CK_TRUE, wrap = CK_TRUE, tok = CK_TRUE, privat = CK_TRUE, decrypt = CK_TRUE, sign = CK_TRUE, unwrap = CK_TRUE;
	CK_ATTRIBUTE publicKeyTemplate[] = {
		{ CKA_ENCRYPT, &encrypt, sizeof(true) }
		,
		{ CKA_VERIFY, &verify, sizeof(true) }
		,
		{ CKA_WRAP, &wrap, sizeof(true) }
		,
		{ CKA_MODULUS_BITS, &modulusBits, sizeof(modulusBits) }
		,
		{ CKA_PUBLIC_EXPONENT, publicExponent, sizeof(publicExponent) }
	};
	
	CK_ATTRIBUTE privateKeyTemplate[] = {
		{ CKA_TOKEN, &tok, sizeof(true) }
		,
		{ CKA_PRIVATE, &privat, sizeof(true) }
		,
		{ CKA_ID, id, sizeof(id) }
		,
		{ CKA_DECRYPT, &decrypt, sizeof(true) }
		,
		{ CKA_SIGN, &sign, sizeof(true) }
		,
		{ CKA_UNWRAP, &unwrap, sizeof(true) }
	};
	CK_RV rv;
	rv = token->C_InsertKeyPair(&mechanism,
		publicKeyTemplate, 5,
		privateKeyTemplate, 8, &hPublicKey, &hPrivateKey);

	/* C_FindObjects */

	CK_BYTE rsaKeyID[] = { 123 };
	CK_KEY_TYPE		rsaKeyType = CKK_RSA;
	CK_ATTRIBUTE	rsaKeyTemplate[] =   // Searching Template
	{
		{ CKA_ID,		&rsaKeyID,	sizeof(rsaKeyID) },
		{ CKA_KEY_TYPE,		&rsaKeyType,	sizeof(rsaKeyType) },
	};
	p->C_FindObjectsInit(hSession, rsaKeyTemplate, sizeof(rsaKeyTemplate) / sizeof(CK_ATTRIBUTE));

	CK_OBJECT_HANDLE	rsaKeyInfo[CKU_MAX_TOKEN_OBJECTS];
	CK_ULONG			objectRSAKeyFound = 0;

	p->C_FindObjects(hSession, rsaKeyInfo,
		sizeof(rsaKeyInfo) / sizeof(CK_OBJECT_HANDLE), /* Maximum number of objects */
		&objectRSAKeyFound);
	p->C_FindObjectsFinal(hSession);
	
	return CKR_OK;
}
