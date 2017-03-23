// IOBoardLibrary.cpp: implementation of the CIOBoardLibrary class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IOBoardLibrary.h"

#include "FAS_HSSI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define HSSI_DEBUGMODE 0
CIOBoardLibrary	IO;

CIOBoardLibrary::CIOBoardLibrary()
{

}

CIOBoardLibrary::~CIOBoardLibrary()
{

}

//HSSI DLL 호출과 관련한 Error 메세지를 리턴한다 
char* CIOBoardLibrary::ErrorOccurInfo(int iErrorNum) 
{
	CString sErrorMsg;
	static char cMsg[60];
/*	STATUS_SUCCESS = 0x0,
	STATUS_ERROR_NO_DEVICE,		// 동작중인 보드가 없을 때
	STATUS_ERROR_READ,			// Read동작 실패
	STATUS_ERROR_WRITE,			// Write동작 실패
	STATUS_ERROR_INITIALIZE,	// 초기화 실패 (Reset포함)
	STATUS_ERROR_COMMAND,		// CMD 사용에 문제있음
	STATUS_ERROR_FILE,			// File에 문제있음
	STATUS_ERROR_PORT,			// 포트가 존재하지 않음
	STATUS_ERROR_SLAVE,			// 슬레이브가 ON이 아님
	STATUS_NOTUSE_COMMAND,		// 지원하지 않는 명령어
	STATUS_INVALID_BOARDID,		// 해당 Board가 존재 X
	STATUS_INVALID_PORTNO,		// 해당 Port가 존재 X
	STATUS_INVALID_SLAVEID,		// 해당 Slave가 존재 X
	STATUS_INVALID_PARAMETER,	// 파라미터 입력 에러	
	STATUS_INVALID_DLL = 0x7fff	// 잘못된 DLL일 경우
*/
	if(iErrorNum == STATUS_ERROR_NO_DEVICE)
	{
		sErrorMsg = "동작중인 보드가 없을 때";
	}
	else if(iErrorNum == STATUS_ERROR_READ)
	{
		sErrorMsg = "Read동작 실패";
	}
	else if(iErrorNum == STATUS_ERROR_WRITE)
	{
		sErrorMsg = "Write동작 실패";
	}
	else if(iErrorNum == STATUS_ERROR_INITIALIZE)
	{
		sErrorMsg = "초기화 실패 (Reset포함)";
	}
	else if(iErrorNum == STATUS_ERROR_COMMAND)
	{
		sErrorMsg = "CMD 사용에 문제있음";
	}
	else if(iErrorNum == STATUS_ERROR_FILE)
	{
		sErrorMsg = "File에 문제있음";
	}
	else if(iErrorNum == STATUS_ERROR_PORT)
	{
		sErrorMsg = "포트가 존재하지 않음";
	}
	else if(iErrorNum == STATUS_ERROR_SLAVE)
	{
		sErrorMsg = "슬레이브가 ON이 아님";
	}
	else if(iErrorNum == STATUS_NOTUSE_COMMAND)
	{
		sErrorMsg = "지원하지 않는 명령어";
	}
	else if(iErrorNum == STATUS_INVALID_BOARDID)
	{
		sErrorMsg = "해당 Board가 존재않는다";
	}
	else if(iErrorNum == STATUS_INVALID_PORTNO)
	{
		sErrorMsg = "해당 Port가 존재않는다";
	}
	else if(iErrorNum == STATUS_INVALID_SLAVEID)
	{
		sErrorMsg = "해당 Slave가 존재않는다";
	}

	else if(iErrorNum == STATUS_INVALID_PARAMETER)
	{
		sErrorMsg = "파라미터 입력 에러";
	}
	else if(iErrorNum == STATUS_INVALID_DLL)
	{
		sErrorMsg = "잘못된 DLL일 경우";
	}
	else
	{
		sErrorMsg.Format("정의되지 않은 HSSI 에러 코드 = %d", iErrorNum);
	}

	strcpy(cMsg, sErrorMsg);
	return cMsg;
}

//HSSI와 관련 보드 정보를 확인 및 체크한다 
int CIOBoardLibrary::SearchBoardInformation(int iType, int iData)
{
	int iFlag = -1;
	int iBardData = 0;

	if(iData > 7127) //최대 I/O 포트에 대한 이름 정보가 PS or S7127 점으로 제한한다.
	{
		//최개 사용을 마스터 보드 2개로 설정 I/O Port 2048점을 의미 함 
		return iFlag;
	}

	switch(iType)
	{
/*	case 0: //Master 0,1,2,3,-0,4,5,6,7,-1,8,9,10,11,-2,12,13,14,15,-3,(3번째 마스터 보드)
		iBardData = iData / 4000; //0번 마스터 
		break;

	case 1: //port //PS3120 //0,1,2,3 port
		iBardData = iData / 1000;	//3번 포트 
		if(iBardData > 3) //마스터보드는 최대 2장 까지만 사용하자 
		{
			iBardData -= 4;
		}
		break;

	case 2: //slave number //PS3120
		iBardData = iData % 1000;	//120
		iBardData = iBardData / 16;//1
		break;

	case 3://bit num
		iBardData = iData % 1000;	//120
		iBardData = iBardData % 16;	//7==112 ~ 127까지의 포트 7.5
		break;
*/
	case 0: //Master 0,1,2,3,-0,4,5,6,7,-1,8,9,10,11,-2,12,13,14,15,-3,(3번째 마스터 보드)
		iBardData = iData / 4000; //0번 마스터 
		break;

	case 1: //port //PS3115 //0,1,2,3 port
		iBardData = iData / 1000;	//3번 포트 
		if(iBardData > 3) //마스터보드는 최대 2장 까지만 사용하자 
		{
			iBardData -= 4;
		}
		break;

	case 2: //slave number //PS3115
		iBardData = iData % 1000;	//115
		iBardData = iBardData / 100;//1
		break;

	case 3://bit num 
		iBardData = iData % 1000;	//115
		iBardData = iBardData % 100;//15 15번 포트
		break;
	default : 
		return iFlag;
		break;
	}

	return iBardData;
}

int CIOBoardLibrary::BoardOpen(INT* piMasterNo, int iOpen_Method)
{//I/O 보드 오픈
	int iRet = 0, iFlag = FALSE;
	INT iMasterNo=0;
	CString sMsg;

	//iopen_method 3가지 모드로 사용 
	iRet = HSSI_Open(&iMasterNo, iOpen_Method);
	if (iRet == STATUS_SUCCESS)
	{
		*piMasterNo = iMasterNo;
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

int CIOBoardLibrary::BoardClose(bool bClose_Method)
{//I/O 보드 닫음 
	int iRet = 0, iFlag = FALSE;
	CString sMsg;
	
	iRet = HSSI_Close(bClose_Method);
	if (iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

int CIOBoardLibrary::MasterSearch(INT* piMasterNo)
{//마스터 보드 수량을 구한다 
	int iRet = 0, iFlag = FALSE;
	INT iMasterNo=0;
	CString sMsg;

	iRet = HSSI_GetAttachedMasterBoard(&iMasterNo);
	if (iRet == STATUS_SUCCESS)
	{
		*piMasterNo = iMasterNo;
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}
	return iFlag;
}

int CIOBoardLibrary::PortSearch(int iMasterID, INT* piPortNo)	//master에 연결된 port 수를 구한다
{//마스터에 연결된 포트수를 구한다 
	int iRet = 0, iFlag = FALSE;
	INT iPortNo = 0;
	CString sMsg;

	iRet = HSSI_GetAttachedPort(iMasterID, &iPortNo);
	if (iRet == STATUS_SUCCESS)
	{
		*piPortNo = iPortNo;
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

int CIOBoardLibrary::SlaveSearch(int iMasterID, int iPortNo, INT* piSlaveNo)
{//슬레이브 갯수를 구한다 
	int iRet = 0, iFlag = FALSE;
	INT iSlaveNo =0;
	CString sMsg;

	iRet = HSSI_GetAttachedSlaveModule(iMasterID, iPortNo, &iSlaveNo);
	if(iRet == STATUS_SUCCESS)
	{
		*piSlaveNo = iSlaveNo;
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

BOOL CIOBoardLibrary::CheckSlave(int nMasterID, int nPortNo, int nSlaveID)
{	// 해당 Slave의 Enable, Disable 여부를 확인한다.
	int nSlaveInfo;

	FAS_STATUS fasResult;

	fasResult = HSSI_GetAttachedSlaveModule(nMasterID, nPortNo, &nSlaveInfo);

	if (((nSlaveInfo>>nSlaveID) & 0x1) != 1)
	{
		return FALSE;
	}
	return TRUE;
}

int CIOBoardLibrary::SetHSSISpeed(int iMasterID, int iPortNo, int iType) //0:0.5Mbps,1:1Mbps, 2:2Mbps
{//사용하려는 통신 속도를 구한다 //0:0.5Mbps,1:1Mbps, 2:2Mbps
	int iRet = 0, iFlag = FALSE;
	CString sMsg;

	iRet = HSSI_SetCommSpeed(iMasterID, iPortNo, iType);
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

int CIOBoardLibrary::GetHSSISpeed(int iMasterID, int iPortNo, INT* piType) //0:0.5Mbps,1:1Mbps, 2:2Mbps
{//사용중인 통신 속도를 구한다 
	int iRet = 0, iFlag = FALSE;
	INT iType;
	CString sMsg;

	iRet = HSSI_GetCommSpeed(iMasterID, iPortNo, &iType);
	if(iRet == STATUS_SUCCESS)
	{
		*piType = iType;
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

int CIOBoardLibrary::PortAutoEnable(int iMasterID, int iPortNo)
{//자동으로 연결된 모듈를 모두 Enable 시킨다 
	int iRet = 0, iFlag = FALSE;
	CString sMsg;

	iRet = HSSI_PortAutoEnable(iMasterID, iPortNo);
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

int CIOBoardLibrary::SlaveEnable(int iMasterID, int iPortNo, int iSlaveNo, BOOL bEnable) //bEnable TRUE enable start
{//개별적으로 모듈을 연결한다 
	int iRet = 0, iFlag = FALSE;
	CString sMsg;

	iRet = HSSI_EnableSlaveModule(iMasterID, iPortNo, iSlaveNo, bEnable); //TRUE = enable, FALSE = Dasible
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

int CIOBoardLibrary::SetIODefineWord(int iMasterID, int iPortNo, int iSlaveNo, WORD wData)
{//각 I/O 포트의 사용정보를 정의한다 wData가 0:입력, 1:출력 
	int iRet = 0, iFlag = FALSE;
	CString sMsg;

	iRet = HSSI_WriteDirectionWord(iMasterID, iPortNo, iSlaveNo, wData); //TRUE = enable, FALSE = Dasible
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

int CIOBoardLibrary::SetIODefineByte(int iMasterID, int iPortNo, int iSlaveNo, int iType, BYTE btData)
{//각 I/O 포트의 사용정보를 정의한다 btData가 0:입력, 1:출력 
	int iRet = 0, iFlag = FALSE;
	CString sMsg;

	iRet = HSSI_WriteDirectionByte(iMasterID, iPortNo, iSlaveNo, iType, btData); //TRUE = enable, FALSE = Dasible
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;

}

int CIOBoardLibrary::SetIODefineBit(int iIONo, BOOL bData)
{//각 I/O 포트의 사용정보를 정의한다 bData 0:입력, 1:출력 
	int iRet = 0, iFlag = FALSE;
	INT iMasterID=0, iPortNo=0, iSlaveNo=0, iBitNo = 0;
	CString sMsg;

	iMasterID  = SearchBoardInformation(0, iIONo);
	iPortNo    = SearchBoardInformation(1, iIONo);
	iSlaveNo   = SearchBoardInformation(2, iIONo);
	iBitNo     = SearchBoardInformation(3, iIONo);

	iRet = HSSI_WriteDirectionBit(iMasterID, iPortNo, iSlaveNo, iBitNo, bData); //TRUE = 출력사용, FALSE = 입력사용
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}


int CIOBoardLibrary::GetIODefineWord(int iMasterID, int iPortNo, int iSlaveNo, WORD* pwData)
{//각 I/O 포트의 사용정보를 Word 단위로 읽어들인다 
	int iRet = 0, iFlag = FALSE;
	WORD wData = 0;
	CString sMsg;

	iRet = HSSI_ReadDirectionWord(iMasterID, iPortNo, iSlaveNo, &wData); //TRUE = enable, FALSE = Dasible
	if(iRet == STATUS_SUCCESS)
	{
		*pwData = wData;
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

int CIOBoardLibrary::GetIODefineByte(int iMasterID, INT iPortNo, int iSlaveNo, int iType, BYTE* pbData)
{//각 I/O 포트의 사용정보를 Byte 단위로 읽어들인다 
	int iRet = 0, iFlag = FALSE;
	BYTE bData = 0;
	CString sMsg;

	iRet = HSSI_ReadDirectionByte(iMasterID, iPortNo, iSlaveNo, iType, &bData); //TRUE = enable, FALSE = Dasible
	if(iRet == STATUS_SUCCESS)
	{
		*pbData = bData;
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

int CIOBoardLibrary::GetIODefineBit(int iIONo, BOOL* pbData)
{//각 I/O 포트의 사용정보를 Bit 단위로 읽어들인다 
	int iRet = 0, iFlag = FALSE;
	INT iMasterID=0, iPortNo=0, iSlaveNo=0, iBitNo = 0;
	BOOL bData;
	CString sMsg;

	iMasterID  = SearchBoardInformation(0, iIONo);
	iPortNo    = SearchBoardInformation(1, iIONo);
	iSlaveNo   = SearchBoardInformation(2, iIONo);
	iBitNo     = SearchBoardInformation(3, iIONo);

	iRet = HSSI_ReadDirectionBit(iMasterID, iPortNo, iSlaveNo, iBitNo, &bData); //TRUE = 출력사용, FALSE = 입력사용
	if(iRet == STATUS_SUCCESS)
	{
		*pbData = bData;
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

int CIOBoardLibrary::SetLatchWord(int iMasterID, int iPortNo, int iSlaveNo, WORD wData)
{//각 I/O 포트를 Latch가 Word 단위로 사용가능하도록 I/O 를 Define 한다  
	int iRet = 0, iFlag = FALSE;
	CString sMsg;

	iRet = HSSI_SetLatchWord(iMasterID, iPortNo, iSlaveNo, wData); //TRUE = enable, FALSE = Dasible
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

int CIOBoardLibrary::SetLatchByte(int iMasterID, int iPortNo, int iSlaveNo, int iType, BYTE btData)
{//각 I/O 포트를 Latch가 Byte 단위로 사용가능하도록 I/O 를 Define 한다  
	int iRet = 0, iFlag = FALSE;
	CString sMsg;

	iRet = HSSI_SetLatchByte(iMasterID, iPortNo, iSlaveNo, iType, btData); //TRUE = enable, FALSE = Dasible
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

int CIOBoardLibrary::SetLatchBit(int iIONo, BOOL bData)
{//각 I/O 포트를 Latch가 Bit 단위로 사용가능하도록 I/O 를 Define 한다 
	int iRet = 0, iFlag = FALSE;
	INT iMasterID=0, iPortNo=0, iSlaveNo=0, iBitNo = 0;
	CString sMsg;

	iMasterID  = SearchBoardInformation(0, iIONo);
	iPortNo    = SearchBoardInformation(1, iIONo);
	iSlaveNo   = SearchBoardInformation(2, iIONo);
	iBitNo     = SearchBoardInformation(3, iIONo);

	iRet = HSSI_SetLatchBit(iMasterID, iPortNo, iSlaveNo, iBitNo, bData); //TRUE = 해당점을 latch로 사용, FALSE = 이전상태로 그대로 사용
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;

}


WORD CIOBoardLibrary::GetLatchWord(int iMasterID, int iPortNo, int iSlaveNo)
{//각 I/O 포트의 Define이 Word 단위로 되어있는 정보를 읽어들인다. 
	int iRet = 0, iFlag = FALSE;
	WORD wData = 0;
	CString sMsg;

	iRet = HSSI_ReadLatchWord(iMasterID, iPortNo, iSlaveNo, &wData); //TRUE = enable, FALSE = Dasible
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		wData = HSSI_ERROR;
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return wData;
}

BYTE CIOBoardLibrary::GetLatchByte(int iMasterID, INT iPortNo, int iSlaveNo, int iType)
{//각 I/O 포트의 Define이 Byte 단위로 되어있는 정보를 읽어들인다. 
	int iRet = 0, iFlag = FALSE;
	BYTE bData = 0;
	CString sMsg;

	iRet = HSSI_ReadLatchByte(iMasterID, iPortNo, iSlaveNo, iType, &bData); //TRUE = enable, FALSE = Dasible
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		bData = HSSI_ERROR;
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return bData;
}

BOOL CIOBoardLibrary::GetLatchBit(int iIONo)
{//각 I/O 포트의 Define이 Bit 단위로 되어있는 정보를 읽어들인다. 
	int iRet = 0, iFlag = FALSE;
	INT iMasterID=0, iPortNo=0, iSlaveNo=0, iBitNo = 0;
	BOOL bData;
	CString sMsg;

	iMasterID  = SearchBoardInformation(0, iIONo);
	iPortNo    = SearchBoardInformation(1, iIONo);
	iSlaveNo   = SearchBoardInformation(2, iIONo);
	iBitNo     = SearchBoardInformation(3, iIONo);

	iRet = HSSI_ReadLatchBit(iMasterID, iPortNo, iSlaveNo, iBitNo, &bData); //TRUE = 출력사용, FALSE = 입력사용
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		bData = HSSI_ERROR;
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return bData;
}

int CIOBoardLibrary::ResetLatchWord(int iMasterID, int iPortNo, int iSlaveNo, WORD wData)
{//각 I/O 포트의 Define이 Latch로 되어있는 Word 단위의 정보를 리셋한다. 
	int iRet = 0, iFlag = FALSE;
	CString sMsg;

	iRet = HSSI_ResetLatchWord(iMasterID, iPortNo, iSlaveNo, wData); //TRUE = enable, FALSE = Dasible
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

int CIOBoardLibrary::ResetLatchByte(int iMasterID, int iPortNo, int iSlaveNo, int iType, BYTE btData)
{//각 I/O 포트의 Define이 Latch로 되어있는 Byte 단위의 정보를 리셋한다. 
	int iRet = 0, iFlag = FALSE;
	CString sMsg;

	iRet = HSSI_ResetLatchByte(iMasterID, iPortNo, iSlaveNo, iType, btData); //TRUE = enable, FALSE = Dasible
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

int CIOBoardLibrary::ResetLatchBit(int iIONo, BOOL bData)
{//각 I/O 포트의 Define이 Latch로 되어있는 Bit 단위의 정보를 리셋한다. 
	int iRet = 0, iFlag = FALSE;
	INT iMasterID=0, iPortNo=0, iSlaveNo=0, iBitNo = 0;
	CString sMsg;

	iMasterID  = SearchBoardInformation(0, iIONo);
	iPortNo    = SearchBoardInformation(1, iIONo);
	iSlaveNo   = SearchBoardInformation(2, iIONo);
	iBitNo     = SearchBoardInformation(3, iIONo);

	iRet = HSSI_SetLatchBit(iMasterID, iPortNo, iSlaveNo, iBitNo, bData); //TRUE = 해당점을 latch로 사용, FALSE = 이전상태로 그대로 사용
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;	
}


int CIOBoardLibrary::MasterStatusCheck(INT iMasterID)  
{//iMasterID 마스터 보드의 존재 유무를 체크한다 
	int iRet = 0, iFlag = FALSE;
	CString sMsg;

	iRet = HSSI_IsExistMasterBoard(iMasterID); 
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

int CIOBoardLibrary::PortStatusCheck(int iMasterID, int iPortNo)  
{//iPortNo 마스터 보드의 각 통신포트가 정상적으로 동작하는지 체크한다 
	int iRet = 0, iFlag = FALSE;
	CString sMsg;

	iRet = HSSI_IsExistPort(iMasterID, iPortNo); 
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

int CIOBoardLibrary::SlaveStatusCheck(int iMasterID, int iPortNo, int iSlaveNo) //slave 동작상태 확인
{//iSlaveNo 통신포트의 각 슬레이브 모듈이 정상적으로 동작하는지 체크한다
	int iRet = 0, iFlag = FALSE;
	CString sMsg;

	iRet = HSSI_IsExistSlaveModule(iMasterID, iPortNo, iSlaveNo); 
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

int CIOBoardLibrary::OutportWord(int iIONo, WORD wData)	//16bit
{//wData의 WORD 단위로 Output 동작을 실행한다 
	int iRet = 0, iFlag = FALSE;
	INT iMasterID = 0, iPortNo = 0, iSlaveNo = 0;
	CString sMsg;
	
	iMasterID  = SearchBoardInformation(0, iIONo * 100); //3번마스터
	iPortNo    = SearchBoardInformation(1, iIONo * 100); //1번 포트
	iSlaveNo   = SearchBoardInformation(2, iIONo * 100); //
	
	iRet = HSSI_WriteOutputWord(iMasterID, iPortNo, iSlaveNo, wData); //TRUE = enable, FALSE = Dasible
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
}

int CIOBoardLibrary::OutportByte(int iIONo, int iType, BYTE btData)	//8bit
{//btData의 BYTE 단위로 Output 동작을 실행한다 
	int iRet = 0, iFlag = FALSE;
	INT iMasterID = 0, iPortNo = 0, iSlaveNo = 0;
	CString sMsg;
	
	iMasterID  = SearchBoardInformation(0, iIONo * 100); //3번마스터
	iPortNo    = SearchBoardInformation(1, iIONo * 100); //1번 포트
	iSlaveNo   = SearchBoardInformation(2, iIONo * 100); //
	
	iRet = HSSI_WriteOutputByte(iMasterID, iPortNo, iSlaveNo, iType, btData); //TRUE = enable, FALSE = Dasible
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;
	
}

int CIOBoardLibrary::OutportBit (int iIONo, BOOL bOnOff) //1bit
{//bOnOff의 BOOL 단위로 Output 동작을 실행한다
	int iRet = 0, iFlag = FALSE;
	INT iMasterID=0, iPortNo=0, iSlaveNo=0, iBitNo = 0;
	CString sMsg;
	
	iMasterID  = SearchBoardInformation(0, iIONo); //3번마스터
	iPortNo    = SearchBoardInformation(1, iIONo); //1번 포트
	iSlaveNo   = SearchBoardInformation(2, iIONo); //
	iBitNo     = SearchBoardInformation(3, iIONo);
	
	iRet = HSSI_WriteOutputBit(iMasterID, iPortNo, iSlaveNo, iBitNo, bOnOff); //TRUE = 해당점을 latch로 사용, FALSE = 이전상태로 그대로 사용
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return iFlag;	
}

WORD CIOBoardLibrary::InportOutputWord(int iIONo)
{//pwData의 WORD 단위로 input 상태를 체크한다
	int iRet = 0, iFlag = FALSE;
	INT iMasterID = 0, iPortNo = 0, iSlaveNo = 0;
	WORD wData = 0;
	CString sMsg;

	iMasterID  = SearchBoardInformation(0, iIONo * 100);
	iPortNo    = SearchBoardInformation(1, iIONo * 100);
	iSlaveNo   = SearchBoardInformation(2, iIONo * 100);

	iRet = HSSI_ReadOutputWord(iMasterID, iPortNo, iSlaveNo, &wData); //TRUE = enable, FALSE = Dasible
	
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		wData = HSSI_ERROR;
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return wData;
}

BYTE CIOBoardLibrary::InportOutputByte(int iIONo, int iType)
{//pbtData의 BYTE 단위로 input 상태를 체크한다
	int iRet = 0, iFlag = FALSE;
	INT iMasterID = 0, iPortNo = 0, iSlaveNo = 0;
	BYTE bData = 0;
	CString sMsg;

	iMasterID  = SearchBoardInformation(0, iIONo * 100);
	iPortNo    = SearchBoardInformation(1, iIONo * 100);
	iSlaveNo   = SearchBoardInformation(2, iIONo * 100);

	iRet = HSSI_ReadOutputByte(iMasterID, iPortNo, iSlaveNo, iType, &bData); //TRUE = enable, FALSE = Dasible
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		bData = HSSI_ERROR;
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return bData;
}

BOOL CIOBoardLibrary::InportOutputBit(int iIONo)
{//pbData의 BOOL 단위로 input 상태를 체크한다
	int iRet = 0, iFlag = FALSE;
	INT iMasterID = 0, iPortNo = 0, iSlaveNo = 0, iBitNo = 0;
	BOOL bData;
	CString sMsg;

	iMasterID  = SearchBoardInformation(0, iIONo);
	iPortNo    = SearchBoardInformation(1, iIONo);
	iSlaveNo   = SearchBoardInformation(2, iIONo);
	iBitNo     = SearchBoardInformation(3, iIONo);

	iRet = HSSI_ReadOutputBit(iMasterID, iPortNo, iSlaveNo, iBitNo, &bData); //TRUE = 출력사용, FALSE = 입력사용
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		bData = HSSI_ERROR;
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return bData;
}

WORD CIOBoardLibrary::InportInputWord(int iIONo)
{//pwData의 WORD 단위로 input 상태를 체크한다
	int iRet = 0, iFlag = FALSE;
	INT iMasterID = 0, iPortNo = 0, iSlaveNo = 0;
	WORD wData = 0;
	CString sMsg;
	
	iMasterID  = SearchBoardInformation(0, iIONo * 100);
	iPortNo    = SearchBoardInformation(1, iIONo * 100);
	iSlaveNo   = SearchBoardInformation(2, iIONo * 100);
	
	iRet = HSSI_ReadInputWord(iMasterID, iPortNo, iSlaveNo, &wData); //TRUE = enable, FALSE = Dasible
	
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		wData = HSSI_ERROR;
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return wData;
}

BYTE CIOBoardLibrary::InportInputByte(int iIONo, int iType)
{//pbtData의 BYTE 단위로 input 상태를 체크한다
	int iRet = 0, iFlag = FALSE;
	INT iMasterID = 0, iPortNo = 0, iSlaveNo = 0;
	BYTE bData = 0;
	CString sMsg;
	
	iMasterID  = SearchBoardInformation(0, iIONo * 100);
	iPortNo    = SearchBoardInformation(1, iIONo * 100);
	iSlaveNo   = SearchBoardInformation(2, iIONo * 100);
	
	iRet = HSSI_ReadInputByte(iMasterID, iPortNo, iSlaveNo, iType, &bData); //TRUE = enable, FALSE = Dasible
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		bData = HSSI_ERROR;
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return bData;
}

BOOL CIOBoardLibrary::InportInputBit(int iIONo)
{//pbData의 BOOL 단위로 input 상태를 체크한다
	int iRet = 0, iFlag = FALSE;
	INT iMasterID = 0, iPortNo = 0, iSlaveNo = 0, iBitNo = 0;
	BOOL bData;
	CString sMsg;
	
	iMasterID  = SearchBoardInformation(0, iIONo);
	iPortNo    = SearchBoardInformation(1, iIONo);
	iSlaveNo   = SearchBoardInformation(2, iIONo);
	iBitNo     = SearchBoardInformation(3, iIONo);
	
	iRet = HSSI_ReadInputBit(iMasterID, iPortNo, iSlaveNo, iBitNo, &bData); //TRUE = 출력사용, FALSE = 입력사용
	if(iRet == STATUS_SUCCESS)
	{
		iFlag = TRUE;
	}
	else
	{
		bData = HSSI_ERROR;
		sMsg = ErrorOccurInfo(iRet);
		if(HSSI_DEBUGMODE) AfxMessageBox(sMsg);
	}	
	return bData;
}