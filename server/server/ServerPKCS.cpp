#include "ServerPKCS.h"

ServerPKCS* ServerPKCS::pServer = NULL_PTR;

ServerPKCS::ServerPKCS()
{
	slotList = new SlotList();
	serverInfo.cryptokiVersion.major = CRYPTOKI_VERSION_MAJOR;
	serverInfo.cryptokiVersion.minor = CRYPTOKI_VERSION_MINOR;
	serverInfo.flags = 0;
	for (int i = 0; i < 32; i++)
		serverInfo.libraryDescription[i] = 0;
	serverInfo.libraryVersion.major = 1;
	serverInfo.libraryVersion.minor = 0;
	for(int i=0;i<32;i++)
		serverInfo.manufacturerID[i] = 0;
	sessionCounter = 0;
	slotCounter = 0;
}

ServerPKCS::~ServerPKCS()
{
	if (slotList)
		delete[] slotList;
	slotList = NULL_PTR;
}

CK_RV ServerPKCS::C_CreateInstance()
{
	if (pServer == NULL_PTR)
	{
		pServer = new ServerPKCS();
		return CKR_OK;
	}
	return CKR_CRYPTOKI_ALREADY_INITIALIZED;
}

CK_RV ServerPKCS::C_GetInstance(ServerPKCS*& pInfo)
{
	if (pServer == NULL_PTR)
		return CKR_CRYPTOKI_NOT_INITIALIZED;
	pInfo = pServer;
	return CKR_OK;
}

CK_RV ServerPKCS::C_DeleteInstance()
{
	if (pServer)
		delete[] pServer;
	pServer = NULL_PTR;
	return CKR_OK;
}

CK_RV ServerPKCS::C_GetFunctionList(CK_FUNCTION_LIST_PTR_PTR ppFunctionList)
{
	CK_FUNCTION_LIST_PTR pFunctionList = new CK_FUNCTION_LIST();
	ppFunctionList = &pFunctionList;
	return CKR_OK;
}

CK_RV ServerPKCS::C_Initialize(CK_VOID_PTR pInitArgs)
{
	if (pInitArgs == NULL_PTR)
	{
		ServerPKCS::C_CreateInstance();
		return CKR_OK;
	}
	return CKR_ARGUMENTS_BAD;
}

CK_RV ServerPKCS::C_Finalize(CK_VOID_PTR pReserved)
{
	if (pReserved == NULL_PTR)
		return CKR_OK;
	else
		return CKR_ARGUMENTS_BAD;
}

CK_RV ServerPKCS::C_GetInfo(CK_INFO_PTR& pInfo)
{
	pInfo = &serverInfo;
	return CKR_OK;
}

CK_RV ServerPKCS::C_GetSlotList(CK_BBOOL tokenPresent, CK_SLOT_ID_PTR & pSlotList, CK_ULONG_PTR pulCount)
{
	if (tokenPresent == CK_FALSE)
	{
		*pulCount = 0;
		pSlotList = NULL_PTR;
		return CKR_TOKEN_NOT_PRESENT;
	}
	else
	{
		*pulCount = slotList->C_GetSlotCount();
		pSlotList = slotList->C_GetSlotIdList();
		return CKR_OK;
	}
}

CK_RV ServerPKCS::C_GetSlotInfo(CK_SLOT_ID slotID, CK_SLOT_INFO_PTR & pInfo)
{
	int count = slotList->C_GetSlotCount();
	for (int i = 0; i < count; i++)
	{
		if (slotID == slotList->C_GetSlotIdList()[i])
		{
			pInfo = &slotList->C_GetSlotInfoList()[i];
			return CKR_OK;
		}
	}
	return CKR_SLOT_ID_INVALID;
}

CK_RV ServerPKCS::C_OpenSession(CK_SLOT_ID slotID, CK_FLAGS flags, CK_VOID_PTR pApplication, CK_NOTIFY Notify, CK_SESSION_HANDLE_PTR phSession)
{
	for (unsigned int i = 0; i < slotList->ulSlotCount; i++)
	{
		if (slotID == slotList->slotIdList[i])
		{
			Session* s = new Session();
			srand(time(NULL));

			s->sessionInfo.flags = flags;
			s->sessionInfo.slotID = slotID;
			if (slotList->tokenList[slotID]->tokenInfo.flags & CKF_USER_PIN_INITIALIZED)
				s->isPinEnabled = CK_TRUE;
			else
				s->isPinEnabled = CK_FALSE;
			if (C_VerifyFlag(flags, CKF_RW_SESSION))
				s->sessionInfo.state = CKS_RW_PUBLIC_SESSION;
			else
				s->sessionInfo.state = CKS_RO_PUBLIC_SESSION;
			sessionCounter++;
			s->sessionHandler = rand()% RAND_MAX;
			*phSession = s->sessionHandler;
			sessionList.push_back(s);
			return CKR_OK;
		}
	}
	return CKR_SLOT_ID_INVALID;
}

CK_RV ServerPKCS::C_Login(CK_SESSION_HANDLE hSession, CK_USER_TYPE userType, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen)
{
	Session* pSession;
	if (C_VerifySession(hSession, pSession))
	{
		Token* pToken = slotList->tokenList[pSession->sessionInfo.slotID];
		if (pToken->tokenInfo.flags & CKF_USER_PIN_LOCKED)
			return CKR_PIN_LOCKED;
		if (pSession->isPinEnabled)
		{
			if (pToken->wrongPinCounter == CKU_PIN_TRYOUTS)
			{
				pToken->tokenInfo.flags |= CKF_USER_PIN_LOCKED;
				return CKR_PIN_LOCKED;
			}
			if (memcmp(pToken->pin, pPin, ulPinLen)==0)
			{
				if (pSession->userLoggedIn == CK_TRUE)
					return CKR_USER_ALREADY_LOGGED_IN;
				else
				{
					pSession->userLoggedIn = CK_TRUE;
					return CKR_OK;
				}
			}
			else
			{
				pToken->wrongPinCounter++;
				if (pToken->wrongPinCounter == CKU_PIN_TRYOUTS - CKU_PIN_COUNT_LOW)
				{
					pToken->tokenInfo.flags |= CKF_USER_PIN_COUNT_LOW;
				}
				if (pToken->wrongPinCounter == CKU_PIN_TRYOUTS - 1)
				{
					pToken->tokenInfo.flags |= CKF_USER_PIN_FINAL_TRY;
				}
				return CKR_PIN_INCORRECT;
			}

			return CKR_OK;
		}
		else
		{
			return CKR_USER_PIN_NOT_INITIALIZED;
		}
	}
	else
		return CKR_SESSION_HANDLE_INVALID;
}

CK_RV ServerPKCS::C_GenerateKey(CK_SESSION_HANDLE hSession, CK_MECHANISM_PTR pMechanism, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phKey)
{
	Session* pSession;
	if (C_VerifySession(hSession, pSession))
	{
		Token* pToken = slotList->tokenList[pSession->sessionInfo.slotID];
		for (int i = 0; i < ulCount; i++)
		{
			if (pToken->C_IsMechanismAvailable(pMechanism->mechanism))
			{
				break;
			}
			else
				return CKR_MECHANISM_INVALID;
		}
		pToken->C_InsertKey(pMechanism, pTemplate, ulCount, phKey);
	}
	return CKR_OK;
}

CK_BBOOL ServerPKCS::C_VerifySession(CK_SESSION_HANDLE hSession, Session*& pSession)
{
	for (CK_ULONG i = 0; i < sessionCounter; i++)
	{
		if (hSession == sessionList[i]->sessionHandler)
		{
			pSession = sessionList[i];
			return CK_TRUE;
		}
	}
	return CK_FALSE;
}

CK_RV ServerPKCS::C_FindObjectsInit(CK_SESSION_HANDLE hSession, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
	Session* pSession;
	if (C_VerifySession(hSession, pSession))
	{
		Token* pToken = slotList->tokenList[pSession->sessionInfo.slotID];
		pToken->C_FindObjects(pTemplate, ulCount);
	}
	else
		return CKR_SESSION_HANDLE_INVALID;
	return CKR_OK;
}

CK_RV ServerPKCS::C_FindObjects(CK_SESSION_HANDLE hSession, CK_OBJECT_HANDLE_PTR phObject, CK_ULONG ulMaxObjectCount, CK_ULONG_PTR pulObjectCount)
{
	Session* pSession;
	if (C_VerifySession(hSession, pSession))
	{
		Token* pToken = slotList->tokenList[pSession->sessionInfo.slotID];
		if (pToken->objectCounter > ulMaxObjectCount)
			*pulObjectCount = ulMaxObjectCount;
		else
			*pulObjectCount = pToken->objectCounter;
		for (int i = 0; i < *pulObjectCount; i++)
		{
			phObject[i] = pToken->objectList[i];
		}
	}
	else
		return CKR_SESSION_HANDLE_INVALID;
	return CKR_OK;
}

CK_RV ServerPKCS::C_FindObjectsFinal(CK_SESSION_HANDLE hSession)
{
	Session* pSession;
	if (C_VerifySession(hSession, pSession))
	{
		Token* pToken = slotList->tokenList[pSession->sessionInfo.slotID];
		delete[] pToken->objectList;
		pToken->objectList = NULL;
	}
	else
		return CKR_SESSION_HANDLE_INVALID;
	return CKR_OK;
}
