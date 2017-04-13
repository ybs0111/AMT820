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

//HSSI DLL ȣ��� ������ Error �޼����� �����Ѵ� 
char* CIOBoardLibrary::ErrorOccurInfo(int iErrorNum) 
{
	CString sErrorMsg;
	static char cMsg[60];
/*	STATUS_SUCCESS = 0x0,
	STATUS_ERROR_NO_DEVICE,		// �������� ���尡 ���� ��
	STATUS_ERROR_READ,			// Read���� ����
	STATUS_ERROR_WRITE,			// Write���� ����
	STATUS_ERROR_INITIALIZE,	// �ʱ�ȭ ���� (Reset����)
	STATUS_ERROR_COMMAND,		// CMD ��뿡 ��������
	STATUS_ERROR_FILE,			// File�� ��������
	STATUS_ERROR_PORT,			// ��Ʈ�� �������� ����
	STATUS_ERROR_SLAVE,			// �����̺갡 ON�� �ƴ�
	STATUS_NOTUSE_COMMAND,		// �������� �ʴ� ��ɾ�
	STATUS_INVALID_BOARDID,		// �ش� Board�� ���� X
	STATUS_INVALID_PORTNO,		// �ش� Port�� ���� X
	STATUS_INVALID_SLAVEID,		// �ش� Slave�� ���� X
	STATUS_INVALID_PARAMETER,	// �Ķ���� �Է� ����	
	STATUS_INVALID_DLL = 0x7fff	// �߸��� DLL�� ���
*/
	if(iErrorNum == STATUS_ERROR_NO_DEVICE)
	{
		sErrorMsg = "�������� ���尡 ���� ��";
	}
	else if(iErrorNum == STATUS_ERROR_READ)
	{
		sErrorMsg = "Read���� ����";
	}
	else if(iErrorNum == STATUS_ERROR_WRITE)
	{
		sErrorMsg = "Write���� ����";
	}
	else if(iErrorNum == STATUS_ERROR_INITIALIZE)
	{
		sErrorMsg = "�ʱ�ȭ ���� (Reset����)";
	}
	else if(iErrorNum == STATUS_ERROR_COMMAND)
	{
		sErrorMsg = "CMD ��뿡 ��������";
	}
	else if(iErrorNum == STATUS_ERROR_FILE)
	{
		sErrorMsg = "File�� ��������";
	}
	else if(iErrorNum == STATUS_ERROR_PORT)
	{
		sErrorMsg = "��Ʈ�� �������� ����";
	}
	else if(iErrorNum == STATUS_ERROR_SLAVE)
	{
		sErrorMsg = "�����̺갡 ON�� �ƴ�";
	}
	else if(iErrorNum == STATUS_NOTUSE_COMMAND)
	{
		sErrorMsg = "�������� �ʴ� ��ɾ�";
	}
	else if(iErrorNum == STATUS_INVALID_BOARDID)
	{
		sErrorMsg = "�ش� Board�� ����ʴ´�";
	}
	else if(iErrorNum == STATUS_INVALID_PORTNO)
	{
		sErrorMsg = "�ش� Port�� ����ʴ´�";
	}
	else if(iErrorNum == STATUS_INVALID_SLAVEID)
	{
		sErrorMsg = "�ش� Slave�� ����ʴ´�";
	}

	else if(iErrorNum == STATUS_INVALID_PARAMETER)
	{
		sErrorMsg = "�Ķ���� �Է� ����";
	}
	else if(iErrorNum == STATUS_INVALID_DLL)
	{
		sErrorMsg = "�߸��� DLL�� ���";
	}
	else
	{
		sErrorMsg.Format("���ǵ��� ���� HSSI ���� �ڵ� = %d", iErrorNum);
	}

	strcpy(cMsg, sErrorMsg);
	return cMsg;
}

//HSSI�� ���� ���� ������ Ȯ�� �� üũ�Ѵ� 
int CIOBoardLibrary::SearchBoardInformation(int iType, int iData)
{
	int iFlag = -1;
	int iBardData = 0;

	if(iData > 7127) //�ִ� I/O ��Ʈ�� ���� �̸� ������ PS or S7127 ������ �����Ѵ�.
	{
		//�ְ� ����� ������ ���� 2���� ���� I/O Port 2048���� �ǹ� �� 
		return iFlag;
	}

	switch(iType)
	{
/*	case 0: //Master 0,1,2,3,-0,4,5,6,7,-1,8,9,10,11,-2,12,13,14,15,-3,(3��° ������ ����)
		iBardData = iData / 4000; //0�� ������ 
		break;

	case 1: //port //PS3120 //0,1,2,3 port
		iBardData = iData / 1000;	//3�� ��Ʈ 
		if(iBardData > 3) //�����ͺ���� �ִ� 2�� ������ ������� 
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
		iBardData = iBardData % 16;	//7==112 ~ 127������ ��Ʈ 7.5
		break;
*/
	case 0: //Master 0,1,2,3,-0,4,5,6,7,-1,8,9,10,11,-2,12,13,14,15,-3,(3��° ������ ����)
		iBardData = iData / 4000; //0�� ������ 
		break;

	case 1: //port //PS3115 //0,1,2,3 port
		iBardData = iData / 1000;	//3�� ��Ʈ 
		if(iBardData > 3) //�����ͺ���� �ִ� 2�� ������ ������� 
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
		iBardData = iBardData % 100;//15 15�� ��Ʈ
		break;
	default : 
		return iFlag;
		break;
	}

	return iBardData;
}

int CIOBoardLibrary::BoardOpen(INT* piMasterNo, int iOpen_Method)
{//I/O ���� ����
	int iRet = 0, iFlag = FALSE;
	INT iMasterNo=0;
	CString sMsg;

	//iopen_method 3���� ���� ��� 
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
{//I/O ���� ���� 
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
{//������ ���� ������ ���Ѵ� 
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

int CIOBoardLibrary::PortSearch(int iMasterID, INT* piPortNo)	//master�� ����� port ���� ���Ѵ�
{//�����Ϳ� ����� ��Ʈ���� ���Ѵ� 
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
{//�����̺� ������ ���Ѵ� 
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
{	// �ش� Slave�� Enable, Disable ���θ� Ȯ���Ѵ�.
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
{//����Ϸ��� ��� �ӵ��� ���Ѵ� //0:0.5Mbps,1:1Mbps, 2:2Mbps
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
{//������� ��� �ӵ��� ���Ѵ� 
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
{//�ڵ����� ����� ��⸦ ��� Enable ��Ų�� 
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
{//���������� ����� �����Ѵ� 
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
{//�� I/O ��Ʈ�� ��������� �����Ѵ� wData�� 0:�Է�, 1:��� 
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
{//�� I/O ��Ʈ�� ��������� �����Ѵ� btData�� 0:�Է�, 1:��� 
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
{//�� I/O ��Ʈ�� ��������� �����Ѵ� bData 0:�Է�, 1:��� 
	int iRet = 0, iFlag = FALSE;
	INT iMasterID=0, iPortNo=0, iSlaveNo=0, iBitNo = 0;
	CString sMsg;

	iMasterID  = SearchBoardInformation(0, iIONo);
	iPortNo    = SearchBoardInformation(1, iIONo);
	iSlaveNo   = SearchBoardInformation(2, iIONo);
	iBitNo     = SearchBoardInformation(3, iIONo);

	iRet = HSSI_WriteDirectionBit(iMasterID, iPortNo, iSlaveNo, iBitNo, bData); //TRUE = ��»��, FALSE = �Է»��
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
{//�� I/O ��Ʈ�� ��������� Word ������ �о���δ� 
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
{//�� I/O ��Ʈ�� ��������� Byte ������ �о���δ� 
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
{//�� I/O ��Ʈ�� ��������� Bit ������ �о���δ� 
	int iRet = 0, iFlag = FALSE;
	INT iMasterID=0, iPortNo=0, iSlaveNo=0, iBitNo = 0;
	BOOL bData;
	CString sMsg;

	iMasterID  = SearchBoardInformation(0, iIONo);
	iPortNo    = SearchBoardInformation(1, iIONo);
	iSlaveNo   = SearchBoardInformation(2, iIONo);
	iBitNo     = SearchBoardInformation(3, iIONo);

	iRet = HSSI_ReadDirectionBit(iMasterID, iPortNo, iSlaveNo, iBitNo, &bData); //TRUE = ��»��, FALSE = �Է»��
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
{//�� I/O ��Ʈ�� Latch�� Word ������ ��밡���ϵ��� I/O �� Define �Ѵ�  
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
{//�� I/O ��Ʈ�� Latch�� Byte ������ ��밡���ϵ��� I/O �� Define �Ѵ�  
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
{//�� I/O ��Ʈ�� Latch�� Bit ������ ��밡���ϵ��� I/O �� Define �Ѵ� 
	int iRet = 0, iFlag = FALSE;
	INT iMasterID=0, iPortNo=0, iSlaveNo=0, iBitNo = 0;
	CString sMsg;

	iMasterID  = SearchBoardInformation(0, iIONo);
	iPortNo    = SearchBoardInformation(1, iIONo);
	iSlaveNo   = SearchBoardInformation(2, iIONo);
	iBitNo     = SearchBoardInformation(3, iIONo);

	iRet = HSSI_SetLatchBit(iMasterID, iPortNo, iSlaveNo, iBitNo, bData); //TRUE = �ش����� latch�� ���, FALSE = �������·� �״�� ���
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
{//�� I/O ��Ʈ�� Define�� Word ������ �Ǿ��ִ� ������ �о���δ�. 
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
{//�� I/O ��Ʈ�� Define�� Byte ������ �Ǿ��ִ� ������ �о���δ�. 
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
{//�� I/O ��Ʈ�� Define�� Bit ������ �Ǿ��ִ� ������ �о���δ�. 
	int iRet = 0, iFlag = FALSE;
	INT iMasterID=0, iPortNo=0, iSlaveNo=0, iBitNo = 0;
	BOOL bData;
	CString sMsg;

	iMasterID  = SearchBoardInformation(0, iIONo);
	iPortNo    = SearchBoardInformation(1, iIONo);
	iSlaveNo   = SearchBoardInformation(2, iIONo);
	iBitNo     = SearchBoardInformation(3, iIONo);

	iRet = HSSI_ReadLatchBit(iMasterID, iPortNo, iSlaveNo, iBitNo, &bData); //TRUE = ��»��, FALSE = �Է»��
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
{//�� I/O ��Ʈ�� Define�� Latch�� �Ǿ��ִ� Word ������ ������ �����Ѵ�. 
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
{//�� I/O ��Ʈ�� Define�� Latch�� �Ǿ��ִ� Byte ������ ������ �����Ѵ�. 
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
{//�� I/O ��Ʈ�� Define�� Latch�� �Ǿ��ִ� Bit ������ ������ �����Ѵ�. 
	int iRet = 0, iFlag = FALSE;
	INT iMasterID=0, iPortNo=0, iSlaveNo=0, iBitNo = 0;
	CString sMsg;

	iMasterID  = SearchBoardInformation(0, iIONo);
	iPortNo    = SearchBoardInformation(1, iIONo);
	iSlaveNo   = SearchBoardInformation(2, iIONo);
	iBitNo     = SearchBoardInformation(3, iIONo);

	iRet = HSSI_SetLatchBit(iMasterID, iPortNo, iSlaveNo, iBitNo, bData); //TRUE = �ش����� latch�� ���, FALSE = �������·� �״�� ���
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
{//iMasterID ������ ������ ���� ������ üũ�Ѵ� 
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
{//iPortNo ������ ������ �� �����Ʈ�� ���������� �����ϴ��� üũ�Ѵ� 
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

int CIOBoardLibrary::SlaveStatusCheck(int iMasterID, int iPortNo, int iSlaveNo) //slave ���ۻ��� Ȯ��
{//iSlaveNo �����Ʈ�� �� �����̺� ����� ���������� �����ϴ��� üũ�Ѵ�
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
{//wData�� WORD ������ Output ������ �����Ѵ� 
	int iRet = 0, iFlag = FALSE;
	INT iMasterID = 0, iPortNo = 0, iSlaveNo = 0;
	CString sMsg;
	
	iMasterID  = SearchBoardInformation(0, iIONo * 100); //3��������
	iPortNo    = SearchBoardInformation(1, iIONo * 100); //1�� ��Ʈ
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
{//btData�� BYTE ������ Output ������ �����Ѵ� 
	int iRet = 0, iFlag = FALSE;
	INT iMasterID = 0, iPortNo = 0, iSlaveNo = 0;
	CString sMsg;
	
	iMasterID  = SearchBoardInformation(0, iIONo * 100); //3��������
	iPortNo    = SearchBoardInformation(1, iIONo * 100); //1�� ��Ʈ
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
{//bOnOff�� BOOL ������ Output ������ �����Ѵ�
	int iRet = 0, iFlag = FALSE;
	INT iMasterID=0, iPortNo=0, iSlaveNo=0, iBitNo = 0;
	CString sMsg;
	
	iMasterID  = SearchBoardInformation(0, iIONo); //3��������
	iPortNo    = SearchBoardInformation(1, iIONo); //1�� ��Ʈ
	iSlaveNo   = SearchBoardInformation(2, iIONo); //
	iBitNo     = SearchBoardInformation(3, iIONo);
	
	iRet = HSSI_WriteOutputBit(iMasterID, iPortNo, iSlaveNo, iBitNo, bOnOff); //TRUE = �ش����� latch�� ���, FALSE = �������·� �״�� ���
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
{//pwData�� WORD ������ input ���¸� üũ�Ѵ�
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
{//pbtData�� BYTE ������ input ���¸� üũ�Ѵ�
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
{//pbData�� BOOL ������ input ���¸� üũ�Ѵ�
	int iRet = 0, iFlag = FALSE;
	INT iMasterID = 0, iPortNo = 0, iSlaveNo = 0, iBitNo = 0;
	BOOL bData;
	CString sMsg;

	iMasterID  = SearchBoardInformation(0, iIONo);
	iPortNo    = SearchBoardInformation(1, iIONo);
	iSlaveNo   = SearchBoardInformation(2, iIONo);
	iBitNo     = SearchBoardInformation(3, iIONo);

	iRet = HSSI_ReadOutputBit(iMasterID, iPortNo, iSlaveNo, iBitNo, &bData); //TRUE = ��»��, FALSE = �Է»��
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
{//pwData�� WORD ������ input ���¸� üũ�Ѵ�
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
{//pbtData�� BYTE ������ input ���¸� üũ�Ѵ�
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
{//pbData�� BOOL ������ input ���¸� üũ�Ѵ�
	int iRet = 0, iFlag = FALSE;
	INT iMasterID = 0, iPortNo = 0, iSlaveNo = 0, iBitNo = 0;
	BOOL bData;
	CString sMsg;
	
	iMasterID  = SearchBoardInformation(0, iIONo);
	iPortNo    = SearchBoardInformation(1, iIONo);
	iSlaveNo   = SearchBoardInformation(2, iIONo);
	iBitNo     = SearchBoardInformation(3, iIONo);
	
	iRet = HSSI_ReadInputBit(iMasterID, iPortNo, iSlaveNo, iBitNo, &bData); //TRUE = ��»��, FALSE = �Է»��
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