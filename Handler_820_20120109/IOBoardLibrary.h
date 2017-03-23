// IOBoardLibrary.h: interface for the CIOBoardLibrary class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOBOARDLIBRARY_H__C0FD5100_0977_49A6_BEF2_A78BA860E191__INCLUDED_)
#define AFX_IOBOARDLIBRARY_H__C0FD5100_0977_49A6_BEF2_A78BA860E191__INCLUDED_

#include "AxtLIBDef.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define HSSI_ERROR	-1

class CIOBoardLibrary  
{
public:
	CIOBoardLibrary();
	virtual ~CIOBoardLibrary();

public:
	BOOL CheckSlave(int nMasterID, int nPortNo, int nSlaveID);
	char* ErrorOccurInfo(int ErrorNum);
	int SearchBoardInformation(int iType, int iData);
	int BoardOpen(INT* piMasterNo, int iopen_method);
	int BoardClose(bool bclose_method);
	int MasterSearch(INT* piMasterNo);
	int PortSearch(int iMasterID, INT* piPotNo);	//master에 연결된 slave 수를 구한다
	int SlaveSearch(int iMasterID, int iPortNo, INT* piSlaveNo);

	int SetHSSISpeed(int iMasterID, int iPortNo, int iType); //0:0.5Mbps,1:1Mbps, 2:2Mbps
	int GetHSSISpeed(int iMasterID, int iPortNo, INT* piType); //0:0.5Mbps,1:1Mbps, 2:2Mbps

	int PortAutoEnable(int iMasterID, int iPortNo);
	int SlaveEnable(int iMasterID, int iPortNo, int iSlaveNo, BOOL bEnable); //bEnable TRUE enable start
	int MasterStatusCheck(int iMasterID);
	int PortStatusCheck(int iMasterID, int iPortNo); //slave 동작상태 확인
	int SlaveStatusCheck(int iMasterID, int iPortNo, int iSlaveNo);

	int SetIODefineWord(int iMasterID, int iPortNo, int iSlaveNo, WORD wData);
	int SetIODefineByte(int iMasterID, int iPortNo, int iSlaveNo, int iType, BYTE btData);
	int SetIODefineBit(int iIONo, BOOL bData);
	int GetIODefineWord(int iMasterID, int iPortNo, int iSlaveNo, WORD* pwData);
	int GetIODefineByte(int iMasterID, INT iPortNo, int iSlaveNo, int iType, BYTE* pbData);
	int GetIODefineBit(int iIONo, BOOL* pbOnOff);

	int SetLatchWord(int iMasterID, int iPortNo, int iSlaveNo, WORD wData);
	int SetLatchByte(int iMasterID, int iPortNo, int iSlaveNo, int iType, BYTE btData);
	int SetLatchBit(int iIONo, BOOL bData);

	WORD GetLatchWord(int iMasterID, int iPortNo, int iSlaveNo);
	BYTE GetLatchByte(int iMasterID, INT iPortNo, int iSlaveNo, int iType);
	BOOL GetLatchBit(int iIONo);

	int ResetLatchWord(int iMasterID, int iPortNo, int iSlaveNo, WORD wData);
	int ResetLatchByte(int iMasterID, int iPortNo, int iSlaveNo, int iType, BYTE btData);
	int ResetLatchBit(int iIONo, BOOL bData);	
	
	int OutportWord(int iIONo, WORD wData);	//16bit
	int OutportByte(int iIONo, int iType, BYTE btData);	//8bit
	int OutportBit (int iIONo, BOOL bOnOff); //1bit
	
	// 파스텍쪽 변경으로 인한 변경. 2K6/09/19/ViboX
	WORD InportOutputWord (int iIONo);	//
	BYTE InportOutputByte (int iIONo, int iType);
	BOOL InportOutputBit  (int iIONo);
	
	WORD InportInputWord (int iIONo);	//
	BYTE InportInputByte (int iIONo, int iType);
	BOOL InportInputBit  (int iIONo);
};
extern CIOBoardLibrary	IO;

#endif // !defined(AFX_IOBOARDLIBRARY_H__C0FD5100_0977_49A6_BEF2_A78BA860E191__INCLUDED_)
