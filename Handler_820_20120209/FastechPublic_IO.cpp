// FastechPublic_IO.cpp: implementation of the CFastechPublic_IO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FastechPublic_IO.h"
#include "Ctlbd_Variable.h"
#include "FAS_HSSI.h"
#include "math.h"
#include "Variable.h"
//#include "io.h"     // 파일 존재 유무 검사 함수 호출하기 위해서는 반드시 INCLUDE 필요 


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFastechPublic_IO  FAS_IO;  

CFastechPublic_IO::CFastechPublic_IO()
{
	//mn_simulation_mode = 0;
}

CFastechPublic_IO::~CFastechPublic_IO()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
//2011.1019 james lee WORD 단위로 I/O를 제어한다
//Ver 0.0.1_2011.1019
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


//2011.1019 james //Word 단위로 제어시 사용하는 함수 
BOOL CFastechPublic_IO::get_in_bit(int n_IONo, int n_OnOff)  //2011.1019 Simulation 동작구현 목적으로 onoff 추가 
{
/*
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo = 0;
	BOOL    bData=0;
*/
	if(mn_simulation_mode == 1) //Simulation 모드이면 프로그램이 동작해야 하니 I/O 상태를 원하는 것으로 셋팅해 준다 
	{
		n_in[n_IONo] = n_OnOff;
		return n_in[n_IONo]; 
	}

	return n_in[n_IONo];

	//이하는 사용하지 않는다 
	/*
	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI 보드 데이터 확인 및 검사 함수
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_ReadInputBit(nMasterID, nPortNo, nSlaveNo, nBitNo, &bData);  // SLAVE I/O 모듈 입력 신호 READ 함수 [BIT 단위]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		bData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}	
	return bData;*/
}

int CFastechPublic_IO::set_out_bit(int n_IONo, BOOL b_OnOff) 
{
/*
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;
*/

	n_out[n_IONo] = b_OnOff;
	return STATUS_SUCCESS;


	//이하는 사용하지 않는다 
	/*
	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI 보드 데이터 확인 및 검사 함수
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_WriteOutputBit(nMasterID, nPortNo, nSlaveNo, nBitNo, b_OnOff);  // SLAVE I/O 모듈 출력 신호 발생 함수 [BIT 단위]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;*/
}

BOOL CFastechPublic_IO::get_out_bit(int n_IONo, int n_OnOff)
{
/*
	CString sMsg;
	int     nRet=0;	
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0,nBitNo=0;
	BOOL    bData;
*/

	if(mn_simulation_mode == 1) //Simulation 모드이면 프로그램이 동작해야 하니 I/O 상태를 원하는 것으로 셋팅해 준다 
	{		
		return n_out[n_IONo] = n_OnOff;
	}

	return n_out[n_IONo];
	//이하는 사용하지 않는다 
	/*
	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI 보드 데이터 확인 및 검사 함수
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);
	
	nRet = HSSI_ReadOutputBit(nMasterID, nPortNo, nSlaveNo, nBitNo, &bData);  // SLAVE I/O 모듈 입력 신호 READ 함수 [BIT 단위]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		bData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}	
	return bData;*/
}

int CFastechPublic_IO::IO_ReadWrite(int n_bd_num, int n_port_num, int n_slave_num, int n_rw_mode)
{//n_rw_mode : -1:변수값 초기화, 0=> initialize i/o 상태 read, 1:input, 2:output
	WORD	wData = 0;
	int     i=0, nRet=0, nBoardID=-1, nPortNo=0, nSlaveNo=0, nIO_Num = 0;
 
//#define MAX_PCI_SLOT 8	// 0~7 8개
//#define MAX_PORT_NO 4	// 0~3 4개
//#define MAX_SLAVE_NO 8	// 0~7 8개


	//보드 정보를 이용하여 io port 번호로 변경한다 
	if(n_bd_num < 0 || n_bd_num > MAX_PCI_SLOT - 6) //MAX_PCI_SLOT - 1)
	{//	보드는 실제 8장까지 사용가능하나 당사는 MAX를 2장으로 한다 
		AfxMessageBox("IO Board [0 ~ 1] Max Num Over");//[0 ~ 7] Max Num Over");
		return BD_ERROR;
	}
	else nBoardID = n_bd_num * 4000;

	if(n_port_num < 0 || n_port_num > MAX_PORT_NO - 1)
	{
		AfxMessageBox("IO Board Port [0 ~ 3] Max Num Over");
		return BD_ERROR;
	}
	else nPortNo   = n_port_num * 1000;

	if(n_slave_num < 0 || n_slave_num > MAX_SLAVE_NO - 1)
	{
		AfxMessageBox("IO Board Slave [0 ~ 7] Max Num Over");
		return BD_ERROR;
	}
	else nSlaveNo   = n_slave_num * 100;
	
	
    nIO_Num = nBoardID + nPortNo + nSlaveNo;
	if(nIO_Num > HSSI_MAX_IO - 1) //#define HSSI_MAX_IO		8716 //max io board 2장 까지만 선언하여 사용하자 //max io board 4장 까지만 처리한다 
	{
		AfxMessageBox("IO Num [8715] Max Num Over");
		return BD_ERROR;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//장비 I/O port를 define 및 통신포트 초기화 후 I/O 실제 제어전에 각 포트의 상태를 읽어 변수에 셋팅하여 준다 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if(n_rw_mode == -1) //input, output port 사용유무 처음 한번 초기화 할 것 
	{
		for(i=0; i<HSSI_MAX_IO; i++) //포트 사용 유무 초기화 
		{
			n_in[i]  = 0;//-1;
			n_out[i] = -1;
		}

		////////////////////////////////////////////////////////////////
		// 아래 내용은 AMT8312 I/O define 관련 정의 내용에 따라 변수를 셋팅했다 
		// 이 부분은 장비별로 틀리니 셋팅이 틀요할 것 임 
		////////////////////////////////////////////////////////////////

		//output 사용하는 것은 0 으로 재 초기화 하여 사용가능하게 한다 
		n_out[0]=0; n_out[1]=0;  n_out[15]=0;	//slave 0 0x8003);
		n_out[0]=0; n_out[1]=0;   //			//slave 1 0x0003)
		n_out[0]=0; n_out[1]=0; n_out[2]=0; n_out[3]=0; //slave 2 0x000f)
		n_out[0]=0; n_out[1]=0; n_out[2]=0; n_out[3]=0; //slave 3 0x000f)
		n_out[0]=0; n_out[1]=0; n_out[2]=0; n_out[3]=0; n_out[4]=0; n_out[5]=0; n_out[6]=0; n_out[7]=0; //slave 4 0x00ff)
		n_out[0]=0; n_out[1]=0; n_out[2]=0; n_out[3]=0; n_out[4]=0; n_out[5]=0; n_out[6]=0; n_out[7]=0; n_out[8]=0; n_out[9]=0; //slave 5 0x03ff)
		n_out[0]=0; n_out[1]=0;   //slave 6 0x0003)
		n_out[0]=0; n_out[1]=0; n_out[2]=0; n_out[3]=0; n_out[4]=0; n_out[5]=0; n_out[6]=0; n_out[7]=0; n_out[8]=0; n_out[9]=0; //slave 7 0x03ff)


		/* AMT8312 I/O define
		IO.SetIODefineWord(0, 0, 0, 0x8003); //0번 Master, 0번 포트, 0번 슬레이브 , 16bit 셋팅
		IO.SetIODefineWord(0, 0, 1, 0x0003); //0번 Master, 0번 포트, 1번 슬레이브 , 16bit 셋팅
		IO.SetIODefineWord(0, 0, 2, 0x000f); //0번 Master, 0번 포트, 2번 슬레이브 , 16bit 셋팅
		IO.SetIODefineWord(0, 0, 3, 0x000f); //0번 Master, 0번 포트, 3번 슬레이브 , 16bit 셋팅
		IO.SetIODefineWord(0, 0, 4, 0x00ff); //0번 Master, 0번 포트, 4번 슬레이브 , 16bit 셋팅
		IO.SetIODefineWord(0, 0, 5, 0x03ff); //0번 Master, 0번 포트, 5번 슬레이브 , 16bit 셋팅
		IO.SetIODefineWord(0, 0, 6, 0x0003); //0번 Master, 0번 포트, 6번 슬레이브 , 16bit 셋팅
		IO.SetIODefineWord(0, 0, 7, 0x03ff); //0번 Master, 0번 포트, 7번 슬레이브 , 16bit 셋팅
		*/
	}

	if(mn_simulation_mode == 1) //Simulation 모드이면 프로그램이 동작해야 하니 I/O 상태를 원하는 것으로 셋팅해 준다 
	{//장비없이 스뮬레이션 모드로 프로그램 테스트시 밑으로 내려가 읽으면 변수가 클리어 되니 리턴한다 
		return BD_GOOD; 
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 장비 초기화시 처음한번 Input 과 output port의 상태를 받는다 
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(n_rw_mode == 0) //initialize total input/ output read mode 
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//선된 변수 최초 한번 초기화
		// in/out 변수가 -1이면 사용하지 않는 포트이고, 0 또는 1이면 정상적으로 장비에서 시용하는 변수이다 
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		

		// ************************************************************************** //
		// 입력(모듈 번호 00번)								                          //
		// ************************************************************************** //
		nRet = HSSI_ReadInputWord(n_bd_num, n_port_num, n_slave_num, &wData); //(iMasterID, iPortNo, iSlaveNo, &wData); 
		if(nRet != STATUS_SUCCESS){return BD_ERROR;};//error

		n_in[nIO_Num + 0]	= (wData & DBIT0)   ? 1 : 0 ;		// PS000 (모듈: 0	포트: 0) 
		n_in[nIO_Num + 1]	= (wData & DBIT1)   ? 1 : 0 ;		// PS001 (모듈: 0	포트: 1) 
		n_in[nIO_Num + 2]	= (wData & DBIT2)   ? 1 : 0 ;		// PS002 (모듈: 0	포트: 2) 
		n_in[nIO_Num + 3]	= (wData & DBIT3)   ? 1 : 0 ;		// PS003 (모듈: 0	포트: 3) 
		n_in[nIO_Num + 4]	= (wData & DBIT4)   ? 1 : 0 ;		// PS004 (모듈: 0	포트: 4)
		n_in[nIO_Num + 5]	= (wData & DBIT5)   ? 1 : 0 ;		// PS005 (모듈: 0	포트: 5) 
		n_in[nIO_Num + 6]	= (wData & DBIT6)   ? 1 : 0 ;		// PS006 (모듈: 0	포트: 6) 
		n_in[nIO_Num + 7]	= (wData & DBIT7)   ? 1 : 0 ;		// PS007 (모듈: 0	포트: 7) 
		n_in[nIO_Num + 8]	= (wData & DBIT8)   ? 1 : 0 ;		// PS008 (모듈: 0	포트: 8) 
		n_in[nIO_Num + 9]	= (wData & DBIT9)   ? 1 : 0 ;		// PS009 (모듈: 0	포트: 9) 
		n_in[nIO_Num + 10]	= (wData & DBIT10)  ? 1 : 0 ;		// PS010 (모듈: 0	포트: 10) 
		n_in[nIO_Num + 11]	= (wData & DBIT11)  ? 1 : 0 ;		// PS011 (모듈: 0	포트: 11) 
		n_in[nIO_Num + 12]	= (wData & DBIT12)  ? 1 : 0 ;		// PS012 (모듈: 0	포트: 12) 
		n_in[nIO_Num + 13]	= (wData & DBIT13)  ? 1 : 0 ;		// PS013 (모듈: 0	포트: 13) 
		n_in[nIO_Num + 14]	= (wData & DBIT14)  ? 1 : 0 ;		// PS014 (모듈: 0	포트: 14) 
		n_in[nIO_Num + 15]	= (wData & DBIT15)  ? 1 : 0 ;		// PS015 (모듈: 0	포트: 15) 

		// ************************************************************************** //
		// 출력(모듈 번호 00번)								                          //
		// ************************************************************************** //
		nRet = HSSI_ReadOutputWord(n_bd_num, n_port_num, n_slave_num, &wData); //(iMasterID, iPortNo, iSlaveNo, &wData); 
		if(nRet != STATUS_SUCCESS){return BD_ERROR;};//error

		for(i=0; i<16; i++)//io port 16개
		{
			if(n_out[nIO_Num + i] == 0 || n_out[nIO_Num + i] == 1) //0:off, 1:on
			{
				if     (i == 0)	n_out[nIO_Num + 0]		= (wData & DBIT0)   ? 1 : 0 ;		// S000 (모듈: 0	포트: 0) 
				else if(i == 1)	n_out[nIO_Num + 1]		= (wData & DBIT1)   ? 1 : 0 ;		// S001 (모듈: 0	포트: 1) 
				else if(i == 2) n_out[nIO_Num + 2]		= (wData & DBIT2)   ? 1 : 0 ;		// S002 (모듈: 0	포트: 2) 
				else if(i == 3) n_out[nIO_Num + 3]		= (wData & DBIT3)   ? 1 : 0 ;		// S003 (모듈: 0	포트: 3) 
				else if(i == 4) n_out[nIO_Num + 4]		= (wData & DBIT4)   ? 1 : 0 ;		// S004 (모듈: 0	포트: 4)
				else if(i == 5) n_out[nIO_Num + 5]		= (wData & DBIT5)   ? 1 : 0 ;		// S005 (모듈: 0	포트: 5) 
				else if(i == 6) n_out[nIO_Num + 6]		= (wData & DBIT6)   ? 1 : 0 ;		// S006 (모듈: 0	포트: 6) 
				else if(i == 7) n_out[nIO_Num + 7]		= (wData & DBIT7)   ? 1 : 0 ;		// S007 (모듈: 0	포트: 7) 
				else if(i == 8) n_out[nIO_Num + 8]		= (wData & DBIT8)   ? 1 : 0 ;		// S008 (모듈: 0	포트: 8) 
				else if(i == 9) n_out[nIO_Num + 9]		= (wData & DBIT9)   ? 1 : 0 ;		// S009 (모듈: 0	포트: 9) 
				else if(i == 10) n_out[nIO_Num + 10]	= (wData & DBIT10)  ? 1 : 0 ;		// S010 (모듈: 0	포트: 10) 
				else if(i == 11) n_out[nIO_Num + 11]	= (wData & DBIT11)  ? 1 : 0 ;		// S011 (모듈: 0	포트: 11) 
				else if(i == 12) n_out[nIO_Num + 12]	= (wData & DBIT12)  ? 1 : 0 ;		// S012 (모듈: 0	포트: 12) 
				else if(i == 13) n_out[nIO_Num + 13]	= (wData & DBIT13)  ? 1 : 0 ;		// S013 (모듈: 0	포트: 13) 
				else if(i == 14) n_out[nIO_Num + 14]	= (wData & DBIT14)  ? 1 : 0 ;		// S014 (모듈: 0	포트: 14) 
				else if(i == 15) n_out[nIO_Num + 15]	= (wData & DBIT15)  ? 1 : 0 ;		// S015 (모듈: 0	포트: 15) 
			}
		}
	}	

	if(n_rw_mode == 1 || n_rw_mode == 2) //input read mode
	{
		// ************************************************************************** //
		// 입력(모듈 번호 00번)								                          //
		// ************************************************************************** //
		nRet = HSSI_ReadInputWord(n_bd_num, n_port_num, n_slave_num, &wData); //(iMasterID, iPortNo, iSlaveNo, &wData); 
		if(nRet != STATUS_SUCCESS){return BD_ERROR;};//error

		n_in[nIO_Num + 0]	= (wData & DBIT0)   ? 1 : 0 ;		// PS000 (모듈: 0	포트: 0) 
		n_in[nIO_Num + 1]	= (wData & DBIT1)   ? 1 : 0 ;		// PS001 (모듈: 0	포트: 1) 
		n_in[nIO_Num + 2]	= (wData & DBIT2)   ? 1 : 0 ;		// PS002 (모듈: 0	포트: 2) 
		n_in[nIO_Num + 3]	= (wData & DBIT3)   ? 1 : 0 ;		// PS003 (모듈: 0	포트: 3) 
		n_in[nIO_Num + 4]	= (wData & DBIT4)   ? 1 : 0 ;		// PS004 (모듈: 0	포트: 4)
		n_in[nIO_Num + 5]	= (wData & DBIT5)   ? 1 : 0 ;		// PS005 (모듈: 0	포트: 5) 
		n_in[nIO_Num + 6]	= (wData & DBIT6)   ? 1 : 0 ;		// PS006 (모듈: 0	포트: 6) 
		n_in[nIO_Num + 7]	= (wData & DBIT7)   ? 1 : 0 ;		// PS007 (모듈: 0	포트: 7) 
		n_in[nIO_Num + 8]	= (wData & DBIT8)   ? 1 : 0 ;		// PS008 (모듈: 0	포트: 8) 
		n_in[nIO_Num + 9]	= (wData & DBIT9)   ? 1 : 0 ;		// PS009 (모듈: 0	포트: 9) 
		n_in[nIO_Num + 10]	= (wData & DBIT10)  ? 1 : 0 ;		// PS010 (모듈: 0	포트: 10) 
		n_in[nIO_Num + 11]	= (wData & DBIT11)  ? 1 : 0 ;		// PS011 (모듈: 0	포트: 11) 
		n_in[nIO_Num + 12]	= (wData & DBIT12)  ? 1 : 0 ;		// PS012 (모듈: 0	포트: 12) 
		n_in[nIO_Num + 13]	= (wData & DBIT13)  ? 1 : 0 ;		// PS013 (모듈: 0	포트: 13) 
		n_in[nIO_Num + 14]	= (wData & DBIT14)  ? 1 : 0 ;		// PS014 (모듈: 0	포트: 14) 
		n_in[nIO_Num + 15]	= (wData & DBIT15)  ? 1 : 0 ;		// PS015 (모듈: 0	포트: 15) 
	}

	if(n_rw_mode == 1 || n_rw_mode == 2) //output mode
	{
		// ************************************************************************** //
		// 입력(모듈 번호 00번)								                          //
		// ************************************************************************** //
		wData = 0;
		for(i=0; i<16; i++)//io port 16개
		{
			if(n_out[nIO_Num + i] == 0 || n_out[nIO_Num + i] == 1) //0:off, 1:on
			{
				if     (i == 0)  wData += n_out[nIO_Num + i]		* DBIT0;  // S000 (모듈: 0	포트: 0)
				else if(i == 1)  wData += n_out[nIO_Num + i]		* DBIT1;  // S001 (모듈: 0	포트: 1)
				else if(i == 2)  wData += n_out[nIO_Num + i]		* DBIT2;  // S002 (모듈: 0	포트: 2)
				else if(i == 3)  wData += n_out[nIO_Num + i]		* DBIT3;  // S003 (모듈: 0	포트: 3)
				else if(i == 4)  wData += n_out[nIO_Num + i]		* DBIT4;  // S004 (모듈: 0	포트: 4)
				else if(i == 5)  wData += n_out[nIO_Num + i]		* DBIT5;  // S005 (모듈: 0	포트: 5)
				else if(i == 6)  wData += n_out[nIO_Num + i]		* DBIT6;  // S006 (모듈: 0	포트: 6)
				else if(i == 7)  wData += n_out[nIO_Num + i]		* DBIT7;  // S007 (모듈: 0	포트: 7)
				else if(i == 8)  wData += n_out[nIO_Num + i]		* DBIT8;  // S008 (모듈: 0	포트: 8)
				else if(i == 9)  wData += n_out[nIO_Num + i]		* DBIT9;  // S009 (모듈: 0	포트: 9)
				else if(i == 10) wData += n_out[nIO_Num + i]		* DBIT10; // S010 (모듈: 0	포트: 10)
				else if(i == 11) wData += n_out[nIO_Num + i]		* DBIT11; // S011 (모듈: 0	포트: 11)
				else if(i == 12) wData += n_out[nIO_Num + i]		* DBIT12; // S012 (모듈: 0	포트: 12)
				else if(i == 13) wData += n_out[nIO_Num + i]		* DBIT13; // S013 (모듈: 0	포트: 13)
				else if(i == 14) wData += n_out[nIO_Num + i]		* DBIT14; // S014 (모듈: 0	포트: 14)
				else if(i == 15) wData += n_out[nIO_Num + i]		* DBIT15; // S015 (모듈: 0	포트: 15)
				

			}
		}

		nRet = HSSI_WriteOutputWord(n_bd_num, n_port_num, n_slave_num, wData);//(iMasterID, iPortNo, iSlaveNo, wData); //TRUE = enable, FALSE = Dasible
		if(nRet != STATUS_SUCCESS){return BD_ERROR;};//error 
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//2011.1019 james lee WORD 단위로 I/O를 제어한다
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





// ******************************************************************************
// I/O 보드 라이브러리 초기화 함수                                                   
// ******************************************************************************
// ******************************************************************************
// I/O 보드 오픈 함수                                                            
// ******************************************************************************
int CFastechPublic_IO::Set_IO_BoardOpen(INT* pn_MasterNo, int n_OpenMethod) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterNo=0;

	nRet = HSSI_Open(&nMasterNo, n_OpenMethod);  // HSSI 초기화 함수 
	if (nRet == STATUS_SUCCESS)
	{
		*pn_MasterNo = nMasterNo;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}

	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// I/O 보드 해제 함수                                                            
// ******************************************************************************
int CFastechPublic_IO::Set_IO_BoardClose(bool b_CloseMethod) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_Close(b_CloseMethod);  // HSSI 리소스 해제 함수 
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}

	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈의 입출력 설정 함수 [WORD 단위]                                 
// ******************************************************************************
int CFastechPublic_IO::Set_IO_DefineWord(int n_MasterID, int n_PortNo, int n_SlaveNo, WORD w_Data) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_WriteDirectionWord(n_MasterID, n_PortNo, n_SlaveNo, w_Data);  // SLAVE I/O 모듈의 입출력 설정 함수 [WORD 단위]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈의 입출력 설정 함수 [BYTE 단위]                                 
// ******************************************************************************
int CFastechPublic_IO::Set_IO_DefineByte(int n_MasterID, int n_PortNo, int n_SlaveNo, int n_Type, BYTE bt_Data) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_WriteDirectionByte(n_MasterID, n_PortNo, n_SlaveNo, n_Type, bt_Data);  // SLAVE I/O 모듈의 입출력 설정 함수 [BYTE 단위]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈의 입출력 설정 함수 [BIT 단위]                                  
// ******************************************************************************
int CFastechPublic_IO::Set_IO_DefineBit(int n_IONo, BOOL b_Data) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI 보드 데이터 확인 및 검사 함수
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_WriteDirectionBit(nMasterID, nPortNo, nSlaveNo, nBitNo, b_Data);  // SLAVE I/O 모듈의 입출력 설정 함수 [BIT 단위]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈의 입출력 설정 상태 리턴 [WORD 단위]                            
// ******************************************************************************
int CFastechPublic_IO::Get_IO_DefineWord(int n_MasterID, int n_PortNo, int n_SlaveNo, WORD* pw_Data) 
{
	CString sMsg;
	int     nRet=0;
	WORD    wData=0;

	nRet = HSSI_ReadDirectionWord(n_MasterID, n_PortNo, n_SlaveNo, &wData);  // SLAVE I/O 모듈의 입출력 설정 상태 리턴 [WORD 단위]
	if (nRet == STATUS_SUCCESS)
	{
		*pw_Data = wData;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈의 입출력 설정 상태 리턴 [BYTE 단위]                            
// ******************************************************************************
int CFastechPublic_IO::Get_IO_DefineByte(int n_MasterID, INT n_PortNo, int n_SlaveNo, int n_Type, BYTE* pb_Data) 
{
	CString sMsg;
	int     nRet=0;
	BYTE    bData=0;

	nRet = HSSI_ReadDirectionByte(n_MasterID, n_PortNo, n_SlaveNo, n_Type, &bData);  // SLAVE I/O 모듈의 입출력 설정 상태 리턴 [BYTE 단위]
	if (nRet == STATUS_SUCCESS)
	{
		*pb_Data = bData;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈의 입출력 설정 상태 리턴 [BIT 단위]                             
// ******************************************************************************
int CFastechPublic_IO::Get_IO_DefineBit(int n_IONo, BOOL* pb_Data) 
{
	CString sMsg;
	int     nRet=0;
	BOOL    bData;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI 보드 데이터 확인 및 검사 함수
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_ReadDirectionBit(nMasterID, nPortNo, nSlaveNo, nBitNo, &bData);  // SLAVE I/O 모듈의 입출력 설정 상태 리턴 [BIT 단위]
	if (nRet == STATUS_SUCCESS)
	{
		*pb_Data = bData;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// PC에 연결된 마스터 보드 갯수 검사 함수                                        
// ******************************************************************************
int CFastechPublic_IO::Search_IO_Master(INT* pn_MasterNo) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterNo=0;

	nRet = HSSI_GetAttachedMasterBoard(&nMasterNo);  // 현재 PC에 연결되어 있는 마스터 보드 갯수 리턴 함수
	if (nRet == STATUS_SUCCESS)
	{
		*pn_MasterNo = nMasterNo;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}

	return nRet;
}
// ******************************************************************************


// ******************************************************************************
// 마스터 보드에 연결된 PORT 갯수 검사 함수                                      
// ******************************************************************************
int CFastechPublic_IO::Search_IO_Port(int n_MasterID, INT* pn_PortNo) 
{
	CString sMsg;
	int     nRet=0;
	INT     nPortNo=0;

	nRet = HSSI_GetAttachedPort(n_MasterID, &nPortNo);  // 마스터 보드의 정상 동작하는 PORT 갯수 리턴 함수
	if (nRet == STATUS_SUCCESS)
	{
		*pn_PortNo = nPortNo;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// PORT에 연결된 SLAVE 검사 함수                                                 
// ******************************************************************************
int CFastechPublic_IO::Search_IO_Slave(int n_MasterID, int n_PortNo, INT* pn_SlaveNo) 
{
	CString sMsg;
	int     nRet=0;	
	INT     nSlaveNo=0;

	nRet = HSSI_GetAttachedSlaveModule(n_MasterID, n_PortNo, &nSlaveNo);  // PORT에 연결된 SLAVE I/O 모듈 검사 함수
	if (nRet == STATUS_SUCCESS)
	{
		*pn_SlaveNo = nSlaveNo;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O MODULE이 ENABLE 상태로 마스터 보드에 연결되어 있는지 검사 함수      
// ******************************************************************************
int CFastechPublic_IO::Check_IO_SlaveStatus(int n_MasterID, int n_PortNo, int n_SlaveNo) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_IsExistSlaveModule(n_MasterID, n_PortNo, n_SlaveNo);  // SLAVE I/O MODULE이 ENABLE 상태로 마스터 보드에 연결되어 있는지 검사 함수
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// PORT에 연결된 SLAVE [ENABLE/DISABLE] 상태 검사 함수                           
// ******************************************************************************
BOOL CFastechPublic_IO::Check_IO_Slave(int n_MasterID, int n_PortNo, int n_SlaveID) 
{
	int nSlaveInfo;

	FAS_STATUS  fasResult;

	// **************************************************************************
	// nSlaveInfo : 최하위로부터 상위 비트 방향으로 8개의 BIT까지 각 SLAVE들의   
	//              연결 상태가 저장된 BCD가 들어있다                            
	// -> BIT 1   : SLAVE I/O 모듈이 연결되어 ENABLE된 상태                      
	// -> BIT 0   : SLAVE I/O 모듈이 연결되지 않아 DISABLE된 상태                
	// **************************************************************************
	fasResult = HSSI_GetAttachedSlaveModule(n_MasterID, n_PortNo, &nSlaveInfo);  // PORT에 연결된 SLAVE I/O 모듈 검사 함수
	if (((nSlaveInfo >> n_SlaveID) & 0x1) != 1)
	{
		return BD_ERROR;
	}
	// **************************************************************************

	return BD_GOOD;
}
// ******************************************************************************

// ******************************************************************************
// PORT와 SLAVE I/O 모듈과의 통신 속도 설정 함수                                 
// -> PORT_SPEED_05 : 0.5 Mbps                                                   
// -> PORT_SPEED_10 : 1.0 Mbps                                                   
// -> PORT_SPEED_20 : 2.0 Mbps                                                   
// ******************************************************************************
int CFastechPublic_IO::Set_IO_HSSISpeed(int n_MasterID, int n_PortNo, int n_Type) 
{
	CString sMsg;
	int     nRet=0, nFlag = BD_ERROR;

	nRet = HSSI_SetCommSpeed(n_MasterID, n_PortNo, n_Type);  // 해당 PORT와 I/O 모듈 사이의 통신 속도 설정 함수
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// PORT와 SLAVE I/O 모듈과의 통신 속도 리턴 함수                                 
// -> PORT_SPEED_05 : 0.5 Mbps                                                   
// -> PORT_SPEED_10 : 1.0 Mbps                                                   
// -> PORT_SPEED_20 : 2.0 Mbps                                                   
// ******************************************************************************
int CFastechPublic_IO::Get_IO_HSSISpeed(int n_MasterID, int n_PortNo, INT* pn_Type) 
{
	CString sMsg;
	int     nRet=0;
	INT     nType;

	nRet = HSSI_GetCommSpeed(n_MasterID, n_PortNo, &nType);  // 해당 PORT와 I/O 모듈 사이의 통신 속도 리턴 함수
	if (nRet == STATUS_SUCCESS)
	{
		*pn_Type = nType;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// 보드 장착 상태 검사 함수                                                      
// ******************************************************************************
int CFastechPublic_IO::Check_IO_BoardExist(int n_MaxSlot, int n_MaxPort, int n_MaxSlave) 
{
	int nMasterID, nPortNo, nSlaveID;
	int nRet=0;

	while(1)
	{
		for(nMasterID=0; nMasterID<n_MaxSlot; nMasterID++)
		{
			nRet = Check_IO_MasterStatus(nMasterID);  // 마스터 보드 장착 유무 검사 함수
			if (nRet != BD_GOOD)
			{
				continue ;
			}

			for(nPortNo=0; nPortNo<n_MaxPort; nPortNo)
			{
				nRet = Check_IO_PortStatus(nMasterID, nPortNo);  // PORT 존재 유무 및 정상 동작 검사 함수
				if (nRet != BD_GOOD)
				{
					continue ;
				}

				for(nSlaveID=0; nSlaveID<n_MaxSlave; nSlaveID++)
				{
					nRet = Check_IO_SlaveStatus(nMasterID, nPortNo, nSlaveID);  // SLAVE I/O MODULE이 ENABLE 상태로 마스터 보드에 연결되어 있는지 검사 함수
					if (nRet != BD_GOOD)
					{
						continue ;
					}
				}
			}
		}
	}

	return 0;
}
// ******************************************************************************

// ******************************************************************************
// PORT와 연결된 모든 SLAVE I/O 모듈 자동 ENABLE 함수                            
// ******************************************************************************
int CFastechPublic_IO::Set_IO_PortAutoEnable(int n_MasterID, int n_PortNo) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_PortAutoEnable(n_MasterID, n_PortNo);  // 해당 PORT에 연결된 모든 SLAVE I/O 모듈 자동 ENABLE 함수
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
		nRet = BD_ERROR;
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈 동작 ENABLE/DISABLE 설정 함수                                  
// ******************************************************************************
int CFastechPublic_IO::Set_IO_SlaveEnable(int n_MasterID, int n_PortNo, int n_SlaveNo, BOOL b_Enable) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_EnableSlaveModule(n_MasterID, n_PortNo, n_SlaveNo, b_Enable);  // 해당 SLAVE I/O 모듈 동작 ENABLE/DISABLE 설정 함수
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************


// ******************************************************************************

// ******************************************************************************
// 특정 위치 BIT값 검사 함수                                                     
// ******************************************************************************
BOOL CFastechPublic_IO::Check_Bit(long n_Value, int n_BitPos) 
{
	unsigned long nMask = 0x0001;  // 마스크 BIT 설정 
	nMask <<= n_BitPos;  // 변경할 BIT 위치로 이동 [SHIFT] 

	return (n_Value & nMask);  // 해당 BIT 변경 후 리턴 
}
// ******************************************************************************

// ******************************************************************************
// 특정 위치 BIT 변경 함수                                                       
// ******************************************************************************
void CFastechPublic_IO::Set_Bit(long &n_Value, int n_Bit, BOOL b_On) 
{
	unsigned long nMask = 0x0001;  // 마스크 BIT 설정 
	nMask <<= n_Bit;  // 변경할 BIT 위치로 이동 [SHIFT] 

	if ( b_On )
	{
		n_Value |= nMask;
	}
	else
	{
		n_Value &= ~nMask;
	}
}
// ******************************************************************************

// ******************************************************************************
// HSSI 보드 데이터 확인 및 검사 함수                                            
// -> 입력 파라메터는 일덩한 포맷으로 입력되어야 한다                            
//  : I/O MAP 정의 참조한다                                                      
//  : I/O MAP 구성은 4자리로 구성된다                                            
// ******************************************************************************
int CFastechPublic_IO::Search_IO_BoardInformation(int n_Type, int n_Data) 
{
	int nFlag     = BD_ERROR;
	int nBardData = 0;

	// **************************************************************************
	// I/O PORT에 대한 최대 입출력은 7127로 제한한다                             
	// -> 최대 사용 : 마스터 보드 2개로 제한 [I/O PORT는 2048]                   
	// **************************************************************************
	if (n_Data > HSSI_MAX_IO) //HSSI_MAX_IO = 7715
		return BD_ERROR;
	// **************************************************************************

	switch(n_Type)
	{
	case 0 :  // 마스터 보드 
		// **********************************************************************
		// 0[0, 1, 2, 3]    1[4, 5, 6, 7]    2[8, 9, 10, 11]    3[12, 13, 14, 15]
		// -> 마스터 보드 1장에 최대 4 PORT 연결된다                             
		// **********************************************************************
		nBardData = n_Data / 4000;
		// **********************************************************************
		break;

	case 1 :  // PORT 
		// **********************************************************************
		// I/O MAP 번호를 1000으로 나누면 몫이 PORT 번호가 되도록 정의되어 있다  
		// -> PORT 설정 범위는 최대 [0 - 15] 이다                                
		// -> 마스터 보드는 최대 2장으로 제한한다                                
		// **********************************************************************
		nBardData = n_Data / 1000;

		if (nBardData>3)
			nBardData -= 4;
		// **********************************************************************
		break;

	case 2 :  // SLAVE 
		// **********************************************************************
		// 한 PORT에 연결된 SLAVE 번호를 설정한다                                
		// -> PORT 당 최대 [0 - 7] 연결된다                                      
		// **********************************************************************
		nBardData = n_Data     % 1000;
		nBardData = nBardData / 100;
		// **********************************************************************
		break;

	case 3 :  // I/O BIT 
		// **********************************************************************
		// 해당 I/O BIT 위치 설정한다                                            
		// -> I/O BIT는 최대 [0 - 15] 설정할 수 있다                             
		//  : IN/OUT 상태는 임의로 조정 가능한다                                 
		// **********************************************************************
		nBardData = n_Data     % 1000;
		nBardData = nBardData % 100;
		// **********************************************************************
		break;

	default :
		return BD_ERROR;
		break;
	}

	return nBardData;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈의 입력을 LATCH 모드로 설정 함수 [WORD 단위]                    
// ******************************************************************************
int CFastechPublic_IO::Set_IO_LatchWord(int n_MasterID, int n_PortNo, int n_SlaveNo, WORD w_Data) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_SetLatchWord(n_MasterID, n_PortNo, n_SlaveNo, w_Data);  // SLAVE I/O 모듈의 입력을 LATCH 모드로 설정 함수 [WORD 단위]
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈의 입력을 LATCH 모드로 설정 함수 [BYTE 단위]                    
// ******************************************************************************
int CFastechPublic_IO::Set_IO_LatchByte(int n_MasterID, int n_PortNo, int n_SlaveNo, int n_Type, BYTE bt_Data) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_SetLatchByte(n_MasterID, n_PortNo, n_SlaveNo, n_Type, bt_Data);  // SLAVE I/O 모듈의 입력을 LATCH 모드로 설정 함수 [BYTE 단위]
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈의 입력을 LATCH 모드로 설정 함수 [BIT 단위]                     
// ******************************************************************************
int CFastechPublic_IO::Set_IO_LatchBit(int n_IONo, BOOL b_Data) 
{
	CString sMsg;
	int     nRet=0;	
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI 보드 데이터 확인 및 검사 함수
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_SetLatchBit(nMasterID, nPortNo, nSlaveNo, nBitNo, b_Data);  // SLAVE I/O 모듈의 입력을 LATCH 모드로 설정 함수 [BIT 단위]
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈의 LATCH 상태 리턴 함수 [WORD 단위]                             
// ******************************************************************************
WORD CFastechPublic_IO::Get_IO_LatchWord(int n_MasterID, int n_PortNo, int n_SlaveNo) 
{
	CString sMsg;
	int     nRet=0;
	
	WORD    wData=0;

	nRet = HSSI_ReadLatchWord(n_MasterID, n_PortNo, n_SlaveNo, &wData);  // SLAVE I/O 모듈의 LATCH 상태 리턴 함수 [WORD 단위]
	if (nRet == STATUS_SUCCESS)
	{
		//wData는 정상값이 들어갔다 //wData = BD_GOOD;
	}
	else
	{
		wData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return wData;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈의 LATCH 상태 리턴 함수 [BYTE 단위]                             
// ******************************************************************************
BYTE CFastechPublic_IO::Get_IO_LatchByte(int n_MasterID, INT n_PortNo, int n_SlaveNo, int n_Type) 
{
	CString sMsg;
	int     nRet=0;	
	BYTE    bData=0;

	nRet = HSSI_ReadLatchByte(n_MasterID, n_PortNo, n_SlaveNo, n_Type, &bData);  // SLAVE I/O 모듈의 LATCH 상태 리턴 함수 [BYTE 단위]
	if (nRet == STATUS_SUCCESS)
	{	 
	}
	else
	{
		bData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return bData;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈의 LATCH 상태 리턴 함수 [BIT 단위]                              
// ******************************************************************************
BOOL CFastechPublic_IO::Get_IO_LatchBit(int n_IONo) 
{
	CString sMsg;
	int     nRet=0;
	BOOL    bData=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI 보드 데이터 확인 및 검사 함수
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_ReadLatchBit(nMasterID, nPortNo, nSlaveNo, nBitNo, &bData);  // SLAVE I/O 모듈의 LATCH 상태 리턴 함수 [BIT 단위]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		bData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return bData;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈의 LATCH 동작 정지 함수 [WORD 단위]                             
// ******************************************************************************
int CFastechPublic_IO::Reset_IO_LatchWord(int n_MasterID, int n_PortNo, int n_SlaveNo, WORD w_Data) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_ResetLatchWord(n_MasterID, n_PortNo, n_SlaveNo, w_Data);  // SLAVE I/O 모듈의 LATCH 동작 정지 함수 [WORD 단위]
	if (nRet == STATUS_SUCCESS)
	{
		nRet = BD_GOOD;
	}
	else
	{
		nRet = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈의 LATCH 동작 정지 함수 [BYTE 단위]                             
// ******************************************************************************
int CFastechPublic_IO::Reset_IO_LatchByte(int n_MasterID, int n_PortNo, int n_SlaveNo, int n_Type, BYTE bt_Data) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_ResetLatchByte(n_MasterID, n_PortNo, n_SlaveNo, n_Type, bt_Data);  // SLAVE I/O 모듈의 LATCH 동작 정지 함수 [BYTE 단위]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈의 LATCH 동작 정지 함수 [BIT 단위]                              
// ******************************************************************************
int CFastechPublic_IO::Reset_IO_LatchBit(int n_IONo, BOOL b_Data) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI 보드 데이터 확인 및 검사 함수
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_SetLatchBit(nMasterID, nPortNo, nSlaveNo, nBitNo, b_Data);  // SLAVE I/O 모듈의 LATCH 동작 정지 함수 [BIT 단위]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// 마스터 보드 장착 유무 검사 함수                                               
// ******************************************************************************
int CFastechPublic_IO::Check_IO_MasterStatus(INT n_MasterID) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_IsExistMasterBoard(n_MasterID);  // 마스터 보드 장착 유무 검사 함수
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// PORT 존재 유무 및 정상 동작 검사 함수                                         
// ******************************************************************************
int CFastechPublic_IO::Check_IO_PortStatus(int n_MasterID, int n_PortNo) 
{
	CString sMsg;
	int     nRet=0;

	nRet = HSSI_IsExistPort(n_MasterID, n_PortNo);  // PORT 존재 유무 및 정상 동작 검사 함수
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//I/O를 직접 제어한다  james Lee 2008.9.5
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ******************************************************************************
// SLAVE I/O 모듈 출력 신호 발생 함수 [WORD 단위]                                
// ******************************************************************************
int CFastechPublic_IO::Set_Out_Word(int n_IONo, WORD w_Data) 
{
	CString sMsg;
	int     nRet=0;	
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo * 100);  // HSSI 보드 데이터 확인 및 검사 함수
	nPortNo   = Search_IO_BoardInformation(1, n_IONo * 100);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo * 100);

	nRet = HSSI_WriteOutputWord(nMasterID, nPortNo, nSlaveNo, w_Data);  // SLAVE I/O 모듈 출력 신호 발생 함수 [WORD 단위]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈 출력 신호 발생 함수 [BYTE 단위]                                
// ******************************************************************************
int CFastechPublic_IO::Set_Out_Byte(int n_IONo, int n_Type, BYTE bt_Data) 
{
	CString sMsg;
	int     nRet=0;	
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo * 100);  // HSSI 보드 데이터 확인 및 검사 함수
	nPortNo   = Search_IO_BoardInformation(1, n_IONo * 100);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo * 100);

	nRet = HSSI_WriteOutputByte(nMasterID, nPortNo, nSlaveNo, n_Type, bt_Data);  // SLAVE I/O 모듈 출력 신호 발생 함수 [BYTE 단위]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈 출력 신호 발생 함수 [BIT 단위]                                 
// ******************************************************************************
int CFastechPublic_IO::Set_Out_Bit(int n_IONo, BOOL b_OnOff) 
{
	
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI 보드 데이터 확인 및 검사 함수
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_WriteOutputBit(nMasterID, nPortNo, nSlaveNo, nBitNo, b_OnOff);  // SLAVE I/O 모듈 출력 신호 발생 함수 [BIT 단위]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return nRet;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈 입력 신호 READ 함수 [WORD 단위]                                
// ******************************************************************************
WORD CFastechPublic_IO::Get_In_Word(int n_IONo) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0;
	WORD    wData=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo * 100);  // HSSI 보드 데이터 확인 및 검사 함수
	nPortNo   = Search_IO_BoardInformation(1, n_IONo * 100);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo * 100);

	nRet = HSSI_ReadInputWord(nMasterID, nPortNo, nSlaveNo, &wData);  // SLAVE I/O 모듈 입력 신호 READ 함수 [WORD 단위]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		wData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return wData;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈 출력 신호 READ 함수 [WORD 단위]                                
// ******************************************************************************
WORD CFastechPublic_IO::Get_Out_Word(int n_IONo)
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0;
	WORD    wData=0;
	
	nMasterID = Search_IO_BoardInformation(0, n_IONo * 100);  // HSSI 보드 데이터 확인 및 검사 함수
	nPortNo   = Search_IO_BoardInformation(1, n_IONo * 100);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo * 100);
	
	nRet = HSSI_ReadOutputWord(nMasterID, nPortNo, nSlaveNo, &wData);  // SLAVE I/O 모듈 입력 신호 READ 함수 [WORD 단위]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		wData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return wData;	
}

// ******************************************************************************
// SLAVE I/O 모듈 입력 신호 READ 함수 [BYTE 단위]                                
// ******************************************************************************
BYTE CFastechPublic_IO::Get_In_Byte(int n_IONo, int n_Type) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0;
	BYTE    btData=0;

	nMasterID = Search_IO_BoardInformation(0, n_IONo * 100);  // HSSI 보드 데이터 확인 및 검사 함수
	nPortNo   = Search_IO_BoardInformation(1, n_IONo * 100);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo * 100);

	nRet = HSSI_ReadInputByte(nMasterID, nPortNo, nSlaveNo, n_Type, &btData);  // SLAVE I/O 모듈 입력 신호 READ 함수 [BYTE 단위]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		btData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return btData;
}
// ******************************************************************************

// ******************************************************************************
// SLAVE I/O 모듈 출력 신호 READ 함수 [BYTE 단위]                                
// ******************************************************************************
BYTE CFastechPublic_IO::Get_Out_Byte(int n_IONo, int n_Type)
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0;
	BYTE    btData=0;
	
	nMasterID = Search_IO_BoardInformation(0, n_IONo * 100);  // HSSI 보드 데이터 확인 및 검사 함수
	nPortNo   = Search_IO_BoardInformation(1, n_IONo * 100);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo * 100);
	
	nRet = HSSI_ReadOutputByte(nMasterID, nPortNo, nSlaveNo, n_Type, &btData);  // SLAVE I/O 모듈 입력 신호 READ 함수 [BYTE 단위]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		btData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return btData;
}

BOOL CFastechPublic_IO::Get_In_Bit(int nIONo, int n_SetTime)//james 2010,0128, long * np_GetTime) 
{
	CString sMsg;
	int     iRet=0;	
	INT     iMasterID=0, iPortNo=0, iSlaveNo=0, iBitNo=0;
	BOOL    bData;
	long long_Time = 0;
	
	iMasterID = Search_IO_BoardInformation(0, nIONo);  // HSSI 보드 데이터 확인 및 검사 함수
	iPortNo   = Search_IO_BoardInformation(1, nIONo);
	iSlaveNo  = Search_IO_BoardInformation(2, nIONo);
	iBitNo    = Search_IO_BoardInformation(3, nIONo);
	
	iRet = HSSI_ReadInputBit(iMasterID, iPortNo, iSlaveNo, iBitNo, &bData);  // SLAVE I/O 모듈 입력 신호 READ 함수 [BIT 단위]
	if (iRet == STATUS_SUCCESS)
	{
		//bData = MOT_FALSE;
	}
	else
	{
		bData = CTLBD_RET_ERROR;
		sMsg = ErrorOccurInfo(iRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (CTLBD_IO_DEBUGMODE)
		{
			AfxMessageBox(sMsg);
		}
	}
	//james test
	if(bData == CTL_ON)
	{
		
		mn_io_in_time_0[nIONo][0] = GetCurrentTime();
		
		mn_io_in_time_0[nIONo][1] = GetCurrentTime();
		long_Time = mn_io_in_time_0[nIONo][1] - mn_io_in_time_1[nIONo][0];
		if(long_Time > (n_SetTime * 3)) //091127 100))
		{
			mn_io_in_time_1[nIONo][0] = GetCurrentTime();
			bData = CTL_IO_PROCEED; //091127 
		}
		else if(long_Time > n_SetTime)
		{
			bData = CTL_ON; 
		}
		else if(long_Time < 0) //091010 추가 
		{
			mn_io_in_time_1[nIONo][0] = GetCurrentTime();
			bData = CTL_IO_PROCEED; //091127
		}		
		else
		{
			bData = CTL_IO_PROCEED; 
		}		 
	}
	else //CTL_OFF
	{
		mn_io_in_time_1[nIONo][0] = GetCurrentTime();
		
		mn_io_in_time_1[nIONo][1] = GetCurrentTime();
		long_Time = mn_io_in_time_1[nIONo][1] - mn_io_in_time_0[nIONo][0];
		
		if(long_Time > (n_SetTime * 3)) //091127 100))
		{
			mn_io_in_time_0[nIONo][0] = GetCurrentTime();
			bData = CTL_IO_PROCEED; //091127
		}
		else if(long_Time > n_SetTime)
		{
			bData = CTL_OFF; 			
		}
		else if(long_Time < 0) //091010 추가 
		{
			mn_io_in_time_0[nIONo][0] = GetCurrentTime();
			bData = CTL_IO_PROCEED; //091127
		}
		else
		{			
			bData = CTL_IO_PROCEED; 
		}		 
	}
	
	return bData;
}

// ******************************************************************************
// SLAVE I/O 모듈 입력 신호 READ 함수 [BIT 단위]                                 
// ******************************************************************************
BOOL CFastechPublic_IO::Get_In_Bit(int n_IONo) 
{
	CString sMsg;
	int     nRet=0;
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0, nBitNo = 0;
	BOOL    bData;

	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI 보드 데이터 확인 및 검사 함수
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);

	nRet = HSSI_ReadInputBit(nMasterID, nPortNo, nSlaveNo, nBitNo, &bData);  // SLAVE I/O 모듈 입력 신호 READ 함수 [BIT 단위]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		bData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return bData;
}




// ******************************************************************************
// SLAVE I/O 모듈 출력 신호 READ 함수 [BIT 단위]                                 
// ******************************************************************************
BOOL CFastechPublic_IO::Get_Out_Bit(int n_IONo)
{
	CString sMsg;
	int     nRet=0;	
	INT     nMasterID=0, nPortNo=0, nSlaveNo=0,nBitNo=0;
	BOOL    bData;
	
	nMasterID = Search_IO_BoardInformation(0, n_IONo);  // HSSI 보드 데이터 확인 및 검사 함수
	nPortNo   = Search_IO_BoardInformation(1, n_IONo);
	nSlaveNo  = Search_IO_BoardInformation(2, n_IONo);
	nBitNo    = Search_IO_BoardInformation(3, n_IONo);
	
	nRet = HSSI_ReadOutputBit(nMasterID, nPortNo, nSlaveNo, nBitNo, &bData);  // SLAVE I/O 모듈 입력 신호 READ 함수 [BIT 단위]
	if (nRet == STATUS_SUCCESS)
	{
	}
	else
	{
		bData = BD_ERROR;
		sMsg = ErrorOccurInfo(nRet);  // HSSI DLL 함수 호출 관련 에러 파싱 함수
		if (mn_errormsg_debug_mode)	AfxMessageBox(sMsg);
	}
	
	return bData;
}
// ******************************************************************************




// ******************************************************************************
// HSSI DLL 함수 호출 관련 에러 파싱 함수                                        
// -> nErrorNum                : 발생한 에러 번호                                
// -> STATUS_SUCCESS = 0x0     : 보드 존재                                       
// -> STATUS_ERROR_NO_DEVICE   : 동작 중인 보드 미존재                           
// -> STATUS_ERROR_READ        : READ 동작 실패                                  
// -> STATUS_ERROR_WRITE       : WRITE 동작 실패                                 
// -> STATUS_ERROR_INITIALIZE  : 초기화 실패 [RESET 포함]                        
// -> STATUS_ERROR_COMMAND     : CMD 사용에 문제 발생                            
// -> STATUS_ERROR_FILE        : FILE에 문제 발생                                
// -> STATUS_ERROR_PORT        : PORT 미존재                                     
// -> STATUS_ERROR_SLAVE       : SLAVE가 ON 아님                                 
// -> STATUS_NOTUSE_COMMAND    : 지원하지 않는 명령어                            
// -> STATUS_INVALID_BOARDID   : 해당 보드 미존재                                
// -> STATUS_INVALID_PORTNO    : 해당 PORT 미존재                                
// -> STATUS_INVALID_SLAVEID   : 해당 SLAVE 미존재                               
// -> STATUS_INVALID_PARAMETER : 파라메터 입력 실패                              
// -> STATUS_INVALID_DLL       : 잘못된 DLL 사용                                 
// ******************************************************************************
char* CFastechPublic_IO::ErrorOccurInfo(int nErrorNum) 
{
	CString sErrorMsg;  // 에러 메시지 임시 저장 변수 

	static char cMsg[60]={0,};

	if (nErrorNum == STATUS_ERROR_NO_DEVICE)
	{
		sErrorMsg = _T("동작 중인 보드 미존재");
	}
	else if (nErrorNum == STATUS_ERROR_READ)
	{
		sErrorMsg = _T("READ 동작 실패");
	}
	else if (nErrorNum == STATUS_ERROR_WRITE)
	{
		sErrorMsg = _T("WRITE 동작 실패");
	}
	else if (nErrorNum == STATUS_ERROR_INITIALIZE)
	{
		sErrorMsg = _T("초기화 실패 [RESET 포함]");
	}
	else if (nErrorNum == STATUS_ERROR_COMMAND)
	{
		sErrorMsg = _T("CMD 사용에 문제 발생");
	}
	else if (nErrorNum == STATUS_ERROR_FILE)
	{
		sErrorMsg = _T("FILE에 문제 발생");
	}
	else if (nErrorNum == STATUS_ERROR_PORT)
	{
		sErrorMsg = _T("PORT 미존재");
	}
	else if (nErrorNum == STATUS_ERROR_SLAVE)
	{
		sErrorMsg = _T("SLAVE가 ON 아님");
	}
	else if (nErrorNum == STATUS_NOTUSE_COMMAND)
	{
		sErrorMsg = _T("지원하지 않는 명령어");
	}
	else if (nErrorNum == STATUS_INVALID_BOARDID)
	{
		sErrorMsg = _T("해당 보드 미존재");
	}
	else if (nErrorNum == STATUS_INVALID_PORTNO)
	{
		sErrorMsg = _T("해당 PORT 미존재");
	}
	else if (nErrorNum == STATUS_INVALID_SLAVEID)
	{
		sErrorMsg = _T("해당 SLAVE 미존재");
	}
	else if (nErrorNum == STATUS_INVALID_PARAMETER)
	{
		sErrorMsg = _T("파라메터 입력 실패");
	}
	else if (nErrorNum == STATUS_INVALID_DLL)
	{
		sErrorMsg = _T("잘못된 DLL 사용");
	}
	else
	{
		sErrorMsg.Format(_T("정의되지 않은 HSSI 에러 코드 = %d"), nErrorNum);
	}

	strncpy(cMsg, sErrorMsg, sizeof(cMsg)-1);
	cMsg[sizeof(cMsg)-1] = '\0'; //2011.0518

	return cMsg;
}
// ******************************************************************************

/*
int CFastechPublic_IO::Check_IO_Port(int n_io, int n_time, int n_flag)
{
	int Ret = CTLBD_READY;	// 함수 리턴 플래그 설정 
	
	if (n_time == 0)	// 대기 시간없이 바로 리턴값을 받아야 한다.
	{
		if (Get_IO_InportBit(n_io) == TRUE)
		{
			Ret = CTLBD_YES;
		}
		else	Ret = CTLBD_NO;
	}
	else				// 설정 시간동안 확인하고, 리턴값을 받는다.
	{
		if (IOFlag[n_io] != true)
		{
			IOTime[n_io][0]	= GetCurrentTime();
			IOFlag[n_io] = true;
		}
		else if (Get_IO_InportBit(n_io) == TRUE && IOFlag[n_io] == true)
		{
			IOTime[n_io][1] = GetCurrentTime();
			IOTime[n_io][2] = IOTime[n_io][1] - IOTime[n_io][0];
			
			if (IOTime[n_io][2] >= n_time)
			{
				IOFlag[n_io] = false;
				Ret = CTLBD_YES;
			}
		}
		else if (Get_IO_InportBit(n_io) == FALSE && IOFlag[n_io] == true)
		{
			IOTime[n_io][1] = GetCurrentTime();
			IOTime[n_io][2] = IOTime[n_io][1] - IOTime[n_io][0];
			
			if (IOTime[n_io][2] >= n_time)
			{
				IOFlag[n_io] = false;
				Ret = CTLBD_NO;
			}
		}
	}
	
	return Ret;
}

int CFastechPublic_IO::Check_IO_Port_Yield(int n_io, int n_time, int n_yield)
{
	int Ret = CTLBD_READY;	// 함수 리턴 플래그 설정 
	
	if (IOYFlag[n_io] != true)
	{
		IOYTime[n_io][0] = GetCurrentTime();
		IOYFlag[n_io] = true;
		// Yield Check 초기화. 2K6/05/18/ViboX
		IOYCount[n_io][0] = 0;					// false
		IOYCount[n_io][1] = 0;					// true
		IOYCount[n_io][2] = 0;					// total
	}
	else if (Get_IO_InportBit(n_io) == TRUE && IOYFlag[n_io] == true)
	{
		IOYCount[n_io][1]++;
	}
	else if (Get_IO_InportBit(n_io) == FALSE && IOYFlag[n_io] == true)
	{
		IOYCount[n_io][0]++;
	}
	
	if (IOYFlag[n_io] == true)
	{
		IOYTime[n_io][1] = GetCurrentTime();
		IOYTime[n_io][2] = IOYTime[n_io][1] - IOYTime[n_io][0];
		
		if (IOYTime[n_io][2] >= n_time)
		{
			
			IOYCount[n_io][2] = IOYCount[n_io][1] + IOYCount[n_io][0];
			
			if (IOYCount[n_io][2] <= 0)
			{
				return CTLBD_NO;
			}
			IOYYield[n_io] = (float)IOYCount[n_io][1] / IOYCount[n_io][2];
			
			if (IOYYield[n_io] >= n_yield)
			{
				Ret = CTLBD_YES;
			}
			else
			{
				Ret = CTLBD_NO;
			}
			IOYFlag[n_io] = false;
		}
	}
	return Ret;
}

*/

// ******************************************************************************
//  실린더 동작시키는  함수 (단동)                                               
// ******************************************************************************
void CFastechPublic_IO::OnCylinderAction(int n_cyl, int n_act)
{
	//Set_Out_Bit(n_cyl, n_act);
	set_out_bit(n_cyl, n_act);
	
	CYLTime[n_cyl][0] = GetCurrentTime();
	
	CYLFlag[n_cyl] = false;
}

// ******************************************************************************
//  실린더 동작시키는  함수 (복복동)                                             
// ******************************************************************************
void CFastechPublic_IO::OnCylinderAction(int n_cyl, int n_cyl2, int n_act)
{
	//Set_Out_Bit(n_cyl, n_act);
	//Set_Out_Bit(n_cyl2, !n_act);
	set_out_bit(n_cyl, n_act);
	set_out_bit(n_cyl2, !n_act);
	
	CYLTime[n_cyl][0] = GetCurrentTime();
	
	CYLFlag[n_cyl] = false;
}

// ******************************************************************************
//  실린더 동작 시킨후 확인하는 함수 (센서 하나용)                               
// ******************************************************************************
int CFastechPublic_IO::OnCylinderActionCheck(int n_cyl, int n_sen, int n_act, int n_wait, int n_limit)
{
	int Ret = RET_PROCEED;  // 함수 리턴 플래그 설정 
	
	if (get_in_bit(n_sen, n_act) == n_act && CYLFlag[n_cyl] == false)
	{
		CYLTime[n_cyl][0]	= GetCurrentTime();
		CYLFlag[n_cyl] = true;
	}
	else if (get_in_bit(n_sen, n_act) == n_act && CYLFlag[n_cyl] == true)
	{
		CYLTime[n_cyl][1] = GetCurrentTime();
		CYLTime[n_cyl][2] = CYLTime[n_cyl][1] - CYLTime[n_cyl][0];
		
		if (CYLTime[n_cyl][2] >= n_wait)
		{
			Ret = RET_GOOD;
		}
	}
	else
	{
		CYLTime[n_cyl][1] = GetCurrentTime();
		CYLTime[n_cyl][2] = CYLTime[n_cyl][1] - CYLTime[n_cyl][0];
		
		if (CYLTime[n_cyl][2] >= n_limit)
		{
			Ret = RET_ERROR;
		}
	}
	return Ret;
}

// ******************************************************************************
//  실린더 동작 시킨후 확인하는 함수 (센서 두개용)                               
// ******************************************************************************
int CFastechPublic_IO::OnCylinderActionCheck(int n_cyl, int n_sen1, int n_sen2, int n_act, int n_wait, int n_limit)
{
	int Ret = RET_PROCEED;  // 함수 리턴 플래그 설정 
	
	if (get_in_bit(n_sen1, n_act) == n_act && 
		get_in_bit(n_sen2, !n_act) == !n_act && 
		CYLFlag[n_cyl] == false)
	{
		CYLTime[n_cyl][0]	= GetCurrentTime();
		CYLFlag[n_cyl] = true;
	}
	else if (get_in_bit(n_sen1, n_act) == n_act && 
		get_in_bit(n_sen2, !n_act) == !n_act && 
		CYLFlag[n_cyl] == true)
	{
		CYLTime[n_cyl][1] = GetCurrentTime();
		CYLTime[n_cyl][2] = CYLTime[n_cyl][1] - CYLTime[n_cyl][0];
		
		if (CYLTime[n_cyl][2] >= n_wait)
		{
			Ret = RET_GOOD;
		}
	}
	else
	{
		CYLTime[n_cyl][1] = GetCurrentTime();
		CYLTime[n_cyl][2] = CYLTime[n_cyl][1] - CYLTime[n_cyl][0];
		
		if (CYLTime[n_cyl][2] >= n_limit)
		{
			Ret = RET_ERROR;
		}
	}
	return Ret;
}


void CFastechPublic_IO::Set_IO_OnOff(int nIONo, int OnOff)
{
	 
	set_out_bit(nIONo, OnOff); 
	
	CYLTime[nIONo][0] = GetCurrentTime();
	
	CYLFlag[nIONo] = false;
}
//센서 없이 동작 시키는 실린더 등을 제어하기 위해 추가 
int CFastechPublic_IO::Chk_IO_OnOff(int nIONo, int OnOff, int nWaitTime, int nLimitTime)
{
	 
	int Ret = RET_PROCEED;  // 함수 리턴 플래그 설정 
	int nIO;

	nIO = get_in_bit(nIONo, OnOff);
	if (CYLFlag[nIONo] == false && nIO	== OnOff)
	{
		CYLTime[nIONo][0]	= GetCurrentTime();
		CYLFlag[nIONo] = true;
	}
	else if (CYLFlag[nIONo] == true && nIO	== OnOff)
	{
		CYLTime[nIONo][1] = GetCurrentTime();
		CYLTime[nIONo][2] = CYLTime[nIONo][1] - CYLTime[nIONo][0];
		//if(CYLTime[nIONo][2] >= 20000 || CYLTime[nIONo][2] < 0) //wait 시간 limit 설정 
		if(CYLTime[nIONo][2] < 0) //wait 시간 limit 설정 
		{
			CYLTime[nIONo][0] = GetCurrentTime();
			CYLFlag[nIONo] = false;
			return RET_PROCEED;
		}

		if (CYLTime[nIONo][2] >= nWaitTime)
		{
			Ret = RET_GOOD;
			CYLFlag[nIONo] = false;
		}
	} 
	else
	{
		CYLTime[nIONo][1] = GetCurrentTime();
		CYLTime[nIONo][2] = CYLTime[nIONo][1] - CYLTime[nIONo][0];
		//if(CYLTime[nIONo][2] >= 20000 || CYLTime[nIONo][2] < 0) //wait 시간 limit 설정 
		if(CYLTime[nIONo][2] <= 0) //wait 시간 limit 설정 
		{
			CYLFlag[nIONo] = false;
			return RET_PROCEED;
		}
		if (CYLTime[nIONo][2] >= nLimitTime)
		{
			Ret = RET_ERROR;
			CYLFlag[nIONo] = false;
		}
	}
	return Ret;
}
//센서 없이 동작 시키는 실린더 등을 제어하기 위해 추가 
int CFastechPublic_IO::Wait_IO_OnOff(int nIONo, int OnOff, int nWaitTime)
{
	 
	int Ret = RET_PROCEED;  // 함수 리턴 플래그 설정 
	
	if (CYLFlag[nIONo] == false)
	{
		CYLTime[nIONo][0]	= GetCurrentTime();
		CYLFlag[nIONo] = true;
	}
	else if (CYLFlag[nIONo] == true)
	{
		CYLTime[nIONo][1] = GetCurrentTime();
		CYLTime[nIONo][2] = CYLTime[nIONo][1] - CYLTime[nIONo][0];

		if(  CYLTime[nIONo][2] <= 0) //wait 시간 limit 설정 
		{
			CYLFlag[nIONo] = false;
			return RET_PROCEED;
		}
		
		if (CYLTime[nIONo][2] >= nWaitTime)
		{
			Ret = RET_GOOD;
			CYLFlag[nIONo] = false;
		}
	} 
	return Ret;
}