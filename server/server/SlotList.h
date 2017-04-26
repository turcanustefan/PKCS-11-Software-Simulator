#ifndef H_SLOT
#define H_SLOT

#include "Token.h"

#define SLOT_COUNT 10
#define TOKEN_COUNT 2

struct SlotList
{
	static CK_ULONG ulSlotCount;
	CK_SLOT_ID_PTR slotIdList;
	CK_SLOT_INFO_PTR slotInfoList;
	Token** tokenList;

public:
	SlotList();
	~SlotList();
	/* Gets */

	CK_ULONG C_GetSlotCount() const { return ulSlotCount; }
	CK_SLOT_ID_PTR C_GetSlotIdList() const { return slotIdList; }
	CK_SLOT_INFO_PTR C_GetSlotInfoList() const { return slotInfoList; }

	/* Slot and Token Management */

	CK_RV C_InitToken(CK_SLOT_ID slotID, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen, CK_UTF8CHAR_PTR pLabel);
};

#endif