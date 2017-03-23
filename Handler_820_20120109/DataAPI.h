// DataAPI.h : main header file for the DATAAPI DLL
//

#if !defined(AFX_DATAAPI_H__D713DFD7_59F5_4FE4_9EB4_789B62AD2E25__INCLUDED_)
#define AFX_DATAAPI_H__D713DFD7_59F5_4FE4_9EB4_789B62AD2E25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDataAPIApp
// See DataAPI.cpp for the implementation of this class
//


//////////////// 050102 ////////////
#define		API_MAXREADCHR		200
#define		API_MACHINESTATUS	4	//{"STOP", "RUN", "JAM", "MAINT"}
#define		API_TOTALBIN		9
#define		API_BINGRADE		2	//0:good, 1:reject
#define		API_MAXSOCKET_PARA	256

#define		API_MAXJAMCOUNT		800
//#define		API_MAXJAMINFO		6
#define		API_TOTALPARTCNT	100

#define		API_MAXTESTCNT		5	//0:prime, 1:retest_1, 2:retest_2

#define		API_GOOD_BIN		1
#define		API_DEBUGMODE		1
#define		API_SUMMARYDEBUGMODE	1	//summary file save 여부
#define		Dll_MAXCHKDAY	400 //1year

#define		DLL_SEPARATEDATA_DEBUGMODE	1	//050422 summary file save 여부



class CDataAPIApp : public CWinApp
{
public:
	CDataAPIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataAPIApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDataAPIApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



////////////////////////////////////////
//mr lee js 2000.10 .27 
#define TOTALJAMCNT  800	//0:stop, run, jam, maint
#define DEBUGMODE		2		//0: x, 1:message box,2: file log
#define RUNMODE			4
#define DLL_MESSAGE_CREATE	(WM_USER+1700)

#define TOTALBIN		16	//0~15 040715 ljs 10 -> 16 변경 
#define TOTALRUNMODE	4	//0:stop, run, jam, maint
#define TOTALPARTCNT	100	//partly name define 
#define RUNMODEVALUE	4

//#define ERR				-1
#define OK				1

//040713 ljs 
#define MAXSOCKET_PARA	256
#define GOOD_BIN		1




//extern "C" __declspec(dllexport) int __stdcall Dll_Logfile_StartEnd(int iMode, char* cpWorkname);
//extern "C" __declspec(dllexport) int __stdcall Dll_ConfigfileRead(int iMode, int iPart);
//extern "C" __declspec(dllexport) void __stdcall Dll_GetJamMessage(stDll_JamMessage* Jammsg); //stDll_JamMessage구조체 변수 얻기 VC

extern "C" __declspec(dllexport) int __stdcall  Dll_JampartReadWrite(int iMode);
extern "C" __declspec(dllexport) int __stdcall  Dll_FileReadWrite(int iMode, char* cpFilename, char* cpMessage) ;
extern "C" __declspec(dllexport) int __stdcall  Dll_GetDayStartTime(void);
extern "C" __declspec(dllexport) void __stdcall Dll_LogFile(CString strAPI);//050417 API_ -> Dll_ 로 수정 
extern "C" __declspec(dllexport) int __stdcall  Dll_AutoFileDelete(char* cpFilepath, int iDeleteday);

///////////// 050102 //////////////
extern "C" __declspec(dllexport) int __stdcall   Dll_SearchSummaryData(int iMode, char* mWorkname, char* mLotname, char* mStarttime, char* mEndtime); //050417 API_ -> Dll_ 로 수정 
extern "C" __declspec(dllexport) int __stdcall   Dll_ConfigfileRead(int imode);
extern "C" __declspec(dllexport) int __stdcall   Dll_JamFileRead(int imode);
extern "C" __declspec(dllexport) int __stdcall   Dll_JamPartlyFileRead(int imode);
extern "C" __declspec(dllexport) char* __stdcall Dll_GetSecTimeToHMS(long lTotalSecond);

//log file file create
extern "C" __declspec(dllexport) int __stdcall Dll_LotDataLog(int iMode, char* mLotname, char* mStarttime, char* mEndtime);
extern "C" __declspec(dllexport) int __stdcall Dll_TimeDataLog(int iMode, int iStatus);
extern "C" __declspec(dllexport) int __stdcall Dll_ProductDataLog(int iMode, long lInsertcnt, int iBinnum, long lBincnt);
extern "C" __declspec(dllexport) int __stdcall Dll_JamDataLog(int iMode, char* cpJamcode, char* cpJamcount); //m1026 ljs 
extern "C" __declspec(dllexport) int __stdcall Dll_SocketDataLog(int iMode, int iNumOfSocket, int iBinnum, char *cpBindata, long lSocketOfBincnt[MAXSOCKET_PARA]); //040713 ljs 추가 

//050418 ljs 추가
extern "C" __declspec(dllexport) int __stdcall Dll_DataLogDecision(int iVariable); //iVariable == 0: log file 생성안함, 1:log file 생성 

struct stDll_JamMessage{
	CString mTotalmessage[TOTALJAMCNT];
	int iTotaljamcount;
	CString mJamcode[TOTALJAMCNT];
	CString mJamcount[TOTALJAMCNT];
	CString mJamcontinue[TOTALJAMCNT];
	CString mJampart[TOTALJAMCNT];
	CString mJambmp[TOTALJAMCNT];
	CString mJammesage[TOTALJAMCNT];
	int iJamcount[TOTALJAMCNT];
};	
extern stDll_JamMessage	stDllJammsg;

struct API_VariableHeader
{
	CString s_DllRunstatus[4] ;//= {"STOP", "RUN", "JAM", "MAINT"};
	CString  s_DllTemptime;
	//Variable Data Load 
	CString  s_DllName_Configfileinfo;
	CString  s_DllName_Jamfileinfo;
	CString  s_DllName_Jampartnameinfo;
	//Log File location data
	CString  s_DllJam_loginfo;
	CString  s_DllProduct_loginfo;
	CString  s_DllTime_loginfo;
	CString  s_DllLot_loginfo;
	CString  s_DllSocket_loginfo;	//V1.0 040618 ljs 

	CString  s_DllTemp_timedir;
	//Summary file locstion data
	CString  s_DllDaily_summaryinfo;
	CString  s_DllShift_summaryinfo;
	CString  s_DllPartly_summaryinfo;
	CString  s_DllWeekly_summaryinfo;
	CString  s_DllMonthly_summaryinfo;
	//etc.
	CString s_DllAppmcsname; //MCS Program name
	CString s_DllUsername;
	CString s_DllInsert_lotname;
	CString s_DllRemove_lotname;
	//file summary date
	CString s_DllRun_date;
	CString s_DllBack_date;
	CString s_DllWeekly_date;
	CString s_DllMonthly_date;
	//config parameter setting
	int i_DllBincountall;
	int i_DllDaystarttime;
	int i_DllDayshifttime;
	int i_DllLotdata_delete;
	int i_DllDaydata_delete;
	int i_DllWeeklydata_delete;
	int i_DllLogdata_delete;

	int i_DllSocketParacountall; //040715 ljs 현제 사용중인 소켓 숫자를 알자 

	int i_DllProductLogMethod; //040724 ljs 

	//2002.0620 Jamfile.cfg 파일을 사용 할지 00.ini 사용 할지 
	int i_DllJamFileType;

	CString s_DllPartlyname[TOTALPARTCNT];
	long l_DllPartlyjamcnt[TOTALPARTCNT];
	int i_DllMainStep;
	int i_DllStep;




	/////////////////////////////////////
	///// Data Manager file ////////////
	////////////////////////////////////
	//Log File location data
	 //Summary file locstion data

	//etc.

	//config parameter setting


	//extern int eiWeekly_date;

	//summary file 작성 여부 플레그 
	//extern bool eiFlag_Dailysumfile;
	//extern bool eiFlag_Shiftsumfile;
	//extern bool eiFlag_Partlysumfile;
	//extern bool eiFlag_Weeklysumfile;
	//extern bool eiFlag_Monthlysumfile;

	bool b_DllFlag_Daycreatefile;
	bool b_DllFlag_Weeklycreatefile;
	bool b_DllFlag_Monthlycreatefile;

	bool b_DllFlag_Initializeend;

	//각종 셋팅 데이터
	long l_DllStoptime;
	long l_DllRuntime;
	long l_DllJamtime;
	long l_DllMainttime;
	long l_DllJamtotalcnt;
	long l_DllProducttotalcnt;

	////// 040724 ljs
	long l_DllSockettotalcnt;

	//extern bool ebFlag_Summaryfileend;

	//extern bool ebFlag_Dailysummaryfileend;
	//extern bool ebFlag_Weeklysummaryfileend;
	//extern bool ebFlag_Monthlysummaryfileend;
	//extern CString MCS_handlerstatus;

	int i_BinCombine[TOTALBIN]; //041202 ljs 빈 정의 하자

	 
	int i_DataLog_Decision; //050418 SEC요청 0:log file 생성안함, 1:log file 진행 

};
extern API_VariableHeader VarH;	
// extern "C" __declspec(dllexport) API_VariableHeader VarH;	

///////////////////////////////////////////
////// 040719 ljs 가종 변수를 저장한다 ////
///////////////////////////////////////////
struct stDll_etcVariable
{
	//daily summary
	int  i_SocketParanumber; //040715 ljs 소켓 수를 알자 
	char c_Catchdata[MAXSOCKET_PARA]; //040715 ljs 
	char c_Bindata[MAXSOCKET_PARA];
	long l_Binlogsum[MAXSOCKET_PARA][TOTALBIN]; //TOTALBIN == 16

	//shift summary
	long l_TotalProductDVCCount;
	long l_TotalGoodCount;

 	long lSplitGoodBinCount;
 	long lSplitTotalCount;
	long lShiftTotalDVCCount[24]; 
	long lShiftGoodDVCCount[24]; 
	
	long iShiftBinCount[24][TOTALBIN] ; //040717 ljs 
	CString mBinResult;

	 
};	
extern stDll_etcVariable	Var_ect;

///////////////////////////////////////////
struct stApi_Var
{
	//050102 
	//all var
	int  i_TotalSocketParaNumber; //pata number
	int  i_TotalBinNumber;	//정해진 전체 빈 정보 
	int  i_BinCombine[API_TOTALBIN];
	int	 i_TotaJamCount;


	//Jam	
	CString s_JamCodeNum[API_MAXJAMCOUNT];
	CString s_JamCountCheck[API_MAXJAMCOUNT];
	CString s_JamPartlyNum[API_MAXJAMCOUNT];
	CString s_JamMessage[API_MAXJAMCOUNT];
	CString s_JamTotalMessage[API_MAXJAMCOUNT];
	long l_JamOccurCount[API_MAXJAMCOUNT]; //jam 순번 정의

	CString s_JamPartlyName[API_TOTALPARTCNT];
	long l_JamPartlyCount[API_TOTALPARTCNT];
	long l_TotalLogJamCount;

	CString s_MachineStatus[API_MACHINESTATUS]; //{"STOP", "RUN", "JAM", "MAINT"}

	//product	
	long l_TotalProductLoadingCount;
	long l_TotalProductPrimeBinCount[API_TOTALBIN]; //API_BINGRADE 0:good, 1:reject
	long l_TotalProductRetestBinCount[API_TOTALBIN];//API_BINGRADE 0:good, 1:reject 
	long l_TotalProductPrimeYieldCount[API_BINGRADE]; //API_BINGRADE 0:good, 1:reject
	long l_TotalProductRetestYieldCount[API_BINGRADE];//API_BINGRADE 0:good, 1:reject 
	
	
		
	//socket
	long l_SocketPrimeBinCount[API_MAXSOCKET_PARA][API_TOTALBIN]; //API_BINGRADE 0:good, 1:reject
	long l_SocketRetestBinCount[API_MAXSOCKET_PARA][API_TOTALBIN];//API_BINGRADE 0:good, 1:reject 

	long l_SocketPrimeYieldCount[API_MAXSOCKET_PARA][API_BINGRADE]; //API_BINGRADE 0:good, 1:reject
	long l_SocketRetestYieldCount[API_MAXSOCKET_PARA][API_BINGRADE];//API_BINGRADE 0:good, 1:reject 

	long l_TotalSocketPrimeYieldCount[API_BINGRADE]; //API_BINGRADE 0:good, 1:reject
	long l_TotalSocketRetestYieldCount[API_BINGRADE];//API_BINGRADE 0:good, 1:reject 
	
	//time
	long l_MachineStatusTime[API_MACHINESTATUS]; //{"STOP", "RUN", "JAM", "MAINT"}

	////// yield data /////
	float	f_ProductPrimeYield;
	float	f_ProductRetestYield;
	float	f_ProductCumYied;

	float	f_SocketTotalYield[API_MAXSOCKET_PARA];
	float	f_SocketPrimeYield[API_MAXSOCKET_PARA];
	float	f_SocketRetestYield[API_MAXSOCKET_PARA];
	float	f_SocketCumYied[API_MAXSOCKET_PARA];
	
	float	f_TotalSocketYied;
	float	f_TotalSocketCumYied;
	float	f_TotalSocketPrimeYield;
	float	f_TotalSocketRetestYield;	

	float	f_JamYield;
};	
extern stApi_Var	ApiVar;



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAAPI_H__D713DFD7_59F5_4FE4_9EB4_789B62AD2E25__INCLUDED_)
