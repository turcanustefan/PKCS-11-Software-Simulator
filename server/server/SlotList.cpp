#include "SlotList.h"

CK_ULONG SlotList::ulSlotCount = 0;

SlotList::SlotList()
{
	slotIdList = new CK_SLOT_ID[SLOT_COUNT];
	slotInfoList = new CK_SLOT_INFO[SLOT_COUNT];
	tokenList = new Token*[TOKEN_COUNT];
	for (int k = 0; k < TOKEN_COUNT; k++)
	{
		tokenList[k] = new Token(k);
	}
	for (int k = 0; k < SLOT_COUNT; k++)
	{
		slotIdList[k] = ulSlotCount++;
		slotInfoList[k].firmwareVersion.major = 1;
		slotInfoList[k].firmwareVersion.minor = 0;
		slotInfoList[k].flags = CKF_TOKEN_PRESENT;
		slotInfoList[k].hardwareVersion.major = 1;
		slotInfoList[k].hardwareVersion.minor = 0;
		char man[] = "TuAv School Project";
		char desc[] = "TuAv Virtual SlotList";
		memcpy(slotInfoList[k].manufacturerID, man, strnlen_s(man, 32));
		memcpy(slotInfoList[k].slotDescription, desc, strnlen_s(desc, 32));
	}
}

SlotList::~SlotList()
{
	for (int k = 0; k < SLOT_COUNT; k++)
	{
		if(tokenList[k])
			delete[] tokenList[k];
		tokenList[k] = NULL_PTR;
	}
	if (tokenList)
		delete[] tokenList;
	if (slotIdList)
		delete[] slotIdList;
	if (slotInfoList)
		delete[] slotInfoList;
	tokenList = NULL_PTR;
	slotIdList = NULL_PTR;
	slotInfoList = NULL_PTR;
	
}

CK_RV SlotList::C_InitToken(CK_SLOT_ID slotID, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen, CK_UTF8CHAR_PTR pLabel)
{
	for (int i = 0; i < ulSlotCount; i++)
	{
		if (slotID == slotIdList[i])
		{
			return CKR_OK;
		}
	}
	return CKR_SLOT_ID_INVALID;
	
}
