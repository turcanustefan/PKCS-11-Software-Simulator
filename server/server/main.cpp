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
	CK_OBJECT_HANDLE obj;
	CK_MECHANISM mec = { CKM_AES_KEY_GEN, NULL, 0 };
	CK_ATTRIBUTE_PTR att = new CK_ATTRIBUTE[3];
	CK_BYTE id[] = { 0xa1 };
	att[0].type = CKA_LABEL;
	att[0].pValue = "TOKEN LABEL";
	att[0].ulValueLen = strlen((char*)att[0].pValue);
	att[1].type = CKA_SIGN;
	att[2].type = CKA_ID;
	att[2].pValue = id;
	att[2].ulValueLen = sizeof(id);
	token->C_InsertKey(&mec, att, 3, &obj);
	return CKR_OK;
}
//
//#include <stdio.h>
//#include <malloc.h>
//#include <stdlib.h>
//#include <windows.h>
//#include "cryptoki.h"
//
//
//#define MAX_COUNT 20
//
//#define E_BASE 0x200
//#define E_PKCS11_TEST_LIBRARY_NOT_FOUND E_BASE+1
//#define E_PKCS11_TEST_CRYPTOKIFUNCTIONS E_BASE+2
//#define E_PKCS11_TEST_NO_TOKENS_PRESENT E_BASE+3
//#define E_PKCS11_TEST_ALLOC				E_BASE+4
//#define E_PKCS11_TEST_NOT_FOUND			E_BASE+5
//#define E_PKCS11_TEST_IO				E_BASE+6
//
//
//
//
//CK_RV execute_p11_jobs(char *p11Library, char *p11PinCode,
//	unsigned char *iv, unsigned char leniv,
//	char *infile, char *outfile)
//{
//	CK_RV					rv;
//	CK_SLOT_ID_PTR			pSlotList = NULL;
//	CK_ULONG				ulSlotCount;
//	CK_SESSION_HANDLE		hSession;
//
//	HINSTANCE				hDll;
//	CK_FUNCTION_LIST_PTR	pFunctionList = NULL;
//	CK_C_GetFunctionList	pC_GetFunctionList = NULL;
//
//
//	CK_BYTE					*plainData = NULL;
//	CK_BYTE					*encryptedData = NULL;
//	FILE					*fd = NULL;
//
//
//
//	do
//	{
//		//incarcare librarie dinamica si obtin adresa functiei C_GetFunctionList (functie disponibila in DLL)
//		printf("\nIncarcare DLL de PKCS#11.....");
//		hDll = LoadLibrary(p11Library);
//		if (!hDll)
//		{
//			printf("EROARE");
//			rv = E_PKCS11_TEST_LIBRARY_NOT_FOUND;
//
//			break;
//		}
//		printf("OK");
//
//
//		pC_GetFunctionList = (CK_C_GetFunctionList)GetProcAddress(hDll, "C_GetFunctionList");
//		if (pC_GetFunctionList == NULL)
//		{
//			printf("\nEROARE adresare C_GetFunctionList");
//			rv = E_PKCS11_TEST_CRYPTOKIFUNCTIONS;
//
//			break;
//		}
//
//
//		// obtin adresele celorlalte functii din API -ul de p11
//		printf("\nIncarcare lista de functii PKCS#11.....");
//		rv = (*pC_GetFunctionList)(&pFunctionList);
//		if (rv != CKR_OK)
//		{
//			printf("EROARE");
//			break;
//		}
//		printf("OK");
//
//
//
//
//
//
//
//		// initializare librarie pkcs11
//		printf("\nInitializare biblioteca de PKCS#11.....");
//		rv = pFunctionList->C_Initialize(NULL);
//		if ((rv != CKR_OK) && (rv != CKR_CRYPTOKI_ALREADY_INITIALIZED))
//		{
//			printf("EROARE");
//
//			break;
//		}
//		printf("OK");
//
//
//
//		// obtin nr de sloturi (ocupate cu tokenuri)
//		printf("\nObtinere lista sloturi de PKCS#11.....");
//		rv = pFunctionList->C_GetSlotList(TRUE, NULL, &ulSlotCount);
//		if (rv != CKR_OK)
//		{
//			printf("EROARE");
//
//			break;
//		}
//
//		if (ulSlotCount == 0)
//		{
//			printf("%d slot(uri)", ulSlotCount);
//			rv = E_PKCS11_TEST_NO_TOKENS_PRESENT;
//
//			break;
//		}
//
//
//
//		//obtin lista de sloturi (doar cele cu tokenuri)	
//		pSlotList = (CK_SLOT_ID_PTR)malloc(ulSlotCount * sizeof(CK_SLOT_ID));
//		if (pSlotList == 0)
//		{
//			printf("EROARE");
//			rv = E_PKCS11_TEST_ALLOC;
//
//			break;
//		}
//		rv = pFunctionList->C_GetSlotList(TRUE, pSlotList, &ulSlotCount);
//		if (rv)
//		{
//			printf("EROARE");
//
//			break;
//		}
//		printf("gasit %d slot(uri)", ulSlotCount);
//
//
//		if (ulSlotCount == 0)
//		{
//			rv = E_PKCS11_TEST_NO_TOKENS_PRESENT;
//			break;
//		}
//
//
//
//		// deschid o sesiune PKCS11 de lucru cu tokenul (read-write)
//		printf("\nDeschidere sesiune PKCS11 de lucru pe token.....");
//		rv = pFunctionList->C_OpenSession(pSlotList[0], CKF_RW_SESSION | CKF_SERIAL_SESSION, NULL, NULL, &hSession);
//		if (rv != CKR_OK)
//		{
//			printf("EROARE");
//			break;
//		}
//		printf("OK");
//
//
//
//
//		// loghez sesiunea (dau codul PIN) 
//		printf("\nAutentificare.............");
//		if (p11PinCode != NULL)
//		{
//			rv = (pFunctionList)->C_Login(hSession, CKU_USER, (CK_CHAR_PTR)p11PinCode, (USHORT)strlen(p11PinCode));
//			if ((rv != CKR_OK) && (rv != CKR_USER_ALREADY_LOGGED_IN))
//			{
//				printf("Eroare (0x%08X)");
//				break;
//			}
//			printf("OK");
//			rv = CKR_OK;
//		}
//
//
//		//caut o cheie DES pe token
//		CK_OBJECT_CLASS desKeyClass = CKO_SECRET_KEY;
//		CK_KEY_TYPE		desKeyType = CKK_DES;
//		CK_BBOOL		desKeyToken = TRUE;
//
//		CK_ATTRIBUTE	desKeyTemplate[] =   // template-ul de cautare 
//		{
//			{ CKA_CLASS,			&desKeyClass,	sizeof(desKeyClass) },
//			{ CKA_TOKEN,			&desKeyToken,	sizeof(desKeyToken) },
//			{ CKA_KEY_TYPE,		&desKeyType,	sizeof(desKeyType) },
//		};
//
//		CK_OBJECT_HANDLE	desKeyInfo[MAX_COUNT]; // pt rezultatele cautarii 
//		CK_ULONG			objectDesKeyFound = 0;
//
//
//
//
//
//		printf("\nCautare chei DES pe token.....");
//		rv = pFunctionList->C_FindObjectsInit(hSession,
//			desKeyTemplate,
//			sizeof(desKeyTemplate) / sizeof(CK_ATTRIBUTE));
//		if (rv != CKR_OK)
//		{
//			printf("EROARE");
//			break;
//		}
//
//		rv = pFunctionList->C_FindObjects(hSession,
//			desKeyInfo,
//			sizeof(desKeyInfo) / sizeof(CK_OBJECT_HANDLE), /* numarul maxim de obiecte ce va fi gasit */
//			&objectDesKeyFound);
//		if (rv != CKR_OK)
//		{
//			printf("EROARE");
//			break;
//		}
//
//
//		rv = pFunctionList->C_FindObjectsFinal(hSession);
//		if (rv != CKR_OK)
//		{
//			printf("EROARE");
//			break;
//		}
//		printf("gasit %d chei", objectDesKeyFound);
//
//
//
//		CK_OBJECT_HANDLE	hKey = 0;
//
//		if (objectDesKeyFound > 0)
//		{
//			hKey = desKeyInfo[0];
//		}
//		else
//		{
//
//			// generez o cheie DES pe token 			
//			CK_MECHANISM keyGenMechanism = {
//				CKM_DES_KEY_GEN, NULL_PTR, 0
//			};
//
//
//			printf("\nGenerare cheie DES pe token.....");
//			rv = pFunctionList->C_GenerateKey(hSession,
//				&keyGenMechanism,
//				desKeyTemplate,
//				sizeof(desKeyTemplate) / sizeof(CK_ATTRIBUTE),
//				&hKey);
//			if (rv != CKR_OK)
//			{
//				printf("EROARE");
//				break;
//			}
//			printf("OK");
//		}
//
//
//
//		// citesc datele din fisierul de intrare 
//		printf("\nCitire date in clar din fisierul de intrare.....");
//		fd = fopen(infile, "rb");
//		if (fd == NULL)
//		{
//			printf("EROARE");
//			rv = E_PKCS11_TEST_IO;
//
//			break;
//		}
//
//
//		fseek(fd, 0, SEEK_END);
//		CK_ULONG ulInLen = ftell(fd);
//		fseek(fd, 0, SEEK_SET);
//
//		plainData = (CK_BYTE_PTR)malloc(ulInLen);
//		fread(plainData, 1, ulInLen, fd);
//		fclose(fd);
//		printf("OK");
//
//
//		CK_ULONG	ulOutLen = (ulInLen / 8) * 8 + 8;
//		encryptedData = (CK_BYTE_PTR)malloc(ulOutLen);
//		memset(encryptedData, 0x00, ulOutLen);
//
//
//
//
//
//		// criptarea datelor de intrare
//		CK_MECHANISM encMechanism = {
//			CKM_DES_CBC_PAD, (CK_BYTE_PTR)iv, leniv  //mecanismul de criptare: DES_CBC cu Padding
//		};
//
//
//		printf("\nCriptare date de intrare cu cheia secreta de pe token.....");
//		rv = pFunctionList->C_EncryptInit(hSession, &encMechanism, hKey);
//		if (rv != CKR_OK)
//		{
//			printf("EROARE");
//			break;
//		}
//
//		rv = pFunctionList->C_Encrypt(hSession, plainData, ulInLen, encryptedData, &ulOutLen);
//		if (rv != CKR_OK)
//		{
//			printf("EROARE");
//			break;
//		}
//		printf("OK");
//
//
//
//
//
//
//
//		// scriu rezultatul in fisierul de iesire
//		printf("\nScriere date criptate in fisierul de iesire.....");
//		fd = fopen(outfile, "wb");
//		if (fd == NULL)
//		{
//			printf("EROARE");
//			rv = E_PKCS11_TEST_IO;
//
//			break;
//		}
//
//		fwrite(encryptedData, 1, ulOutLen, fd);
//		fclose(fd);
//		printf("OK");
//
//
//
//	} while (0);
//
//
//
//
//
//	// finalizez lucrul cu biblioteca p11, inchid sesiuni, eliberez spatiu de memorie, etc
//
//	if (plainData != NULL)
//	{
//		free(plainData);
//		plainData = NULL;
//	}
//
//	if (encryptedData != NULL)
//	{
//		free(encryptedData);
//		encryptedData = NULL;
//	}
//
//	if (pSlotList != NULL)
//	{
//		free(pSlotList);
//		pSlotList = NULL;
//	}
//
//	printf("\nInchidere sesiune PKCS#11.....");
//	pFunctionList->C_Logout(hSession);
//	pFunctionList->C_CloseSession(hSession);
//	printf("OK");
//
//
//	printf("\nInchidere lucru cu biblioteca PKCS#11.....");
//	pFunctionList->C_Finalize(NULL);
//	pFunctionList = NULL;
//	printf("OK");
//
//	FreeLibrary(hDll);
//	hDll = NULL;
//
//
//	return rv;
//}
//
//
//
//int main(int argc, char* argv[])
//{
//	int status = 0;
//	unsigned char IV[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//
//
//	printf("\nStart !\n");
//	status = execute_p11_jobs("eTpkcs11.dll", "ctrl+f12", IV, sizeof(IV), "1.txt", "out.des");
//	printf("\n\nGata (status = 0x%08X) !\n\n\n", status);
//
//
//
//	return status;
//}