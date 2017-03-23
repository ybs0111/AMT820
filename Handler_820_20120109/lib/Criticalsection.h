#pragma once
//#include <afxmt.h>

class CTRCriticalSection
{
public:
	CTRCriticalSection(VOID)
	{
		InitializeCriticalSection(&mSync);
	}

	~CTRCriticalSection(VOID)
	{
		DeleteCriticalSection(&mSync);
	}

	inline VOID Enter(VOID)
	{
		EnterCriticalSection(&mSync);
	}
	
	inline VOID Leave(VOID)
	{
		LeaveCriticalSection(&mSync);
	}
	
private:
	CRITICAL_SECTION mSync;
};