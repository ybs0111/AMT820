// CtlBd_Function.cpp: implementation of the CCtlBd_Function class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "CtlBd_Function.h"

#include "Cmmsdk.h"
#include "CmmsdkDef.h"
#include "ComizoaPublic.h"
#include "COMI_Manager.h"

#include "FastechPublic_IO.h"

#include "FAS_HSSI.h"
#include "math.h"

#include "Public_Function.h"
#include "Variable.h"
#include "MyJamData.h"
#include "IO_Manager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCtlBd_Function  CtlBdFunc;
CCtlBd_Function::CCtlBd_Function()
{

}

CCtlBd_Function::~CCtlBd_Function()
{
//st_ctlbd.n_TotalMotBoard_Number
}

int CCtlBd_Function::Board_Initialize(int n_mode)
{
	int nRet = 0;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//Motor Board Initialize 
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	if(CTLBD_MOTBD_TYPE == 0) //0:파스텍 보드를 사용하는 장비이면, 1:커미조아 모터를 사용하는 장비 
	{
		st_ctlbd.n_TotalMotBoard_Number = 3;  //모터보드가 3개 있다 
		st_ctlbd.n_TotalMotorAxis_Number= 18; //모터의 총 수량은 18개이다  
		st_ctlbd.n_MotorAxis_Number[0] = 8;   //0번 모터보드는 8축용 보드이다  
		st_ctlbd.n_MotorAxis_Number[1] = 6;   //1번 모터보드는 6축용 보드이다 
		st_ctlbd.n_MotorAxis_Number[2] = 4;   //2번 모터보드는 4축용 보드이다 
/* 2011.0113 
		nRet = FAS_MOT.Initialize_MotBoard();
		if(nRet = CTLBD_RET_ERROR)
		{
			return CTLBD_RET_ERROR;
		}
*/
	}
	else if(CTLBD_MOTBD_TYPE == 1) //1:커미조아 보드를 사용하는 장비이면, 1:커미조아 모터를 사용하는 장비 
	{


	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//I/O Board Initialize 
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	if(CTLBD_IOBD_TYPE == 0) //0: 파스텍 I/O 보드를 처리할때 사용 
	{
		nRet = Initialize_FASIO_Board();
		if(nRet = CTLBD_RET_ERROR)
		{
			return CTLBD_RET_ERROR;
		}
	}

	return  CTLBD_RET_GOOD;
}

int CCtlBd_Function::Robot_Clash_Safety_Check(int n_Mode, int n_SourceAxis, int n_ReferenceAxis, double d_Safety_Gap, double d_TargetPos)
{//d_Safety_Gap = 300; //300mm 이상 떨어져 있어야 한다 //DC Loading 사이트 

	//n_Mode => 0: 모터 이동전 순수하게 체크만 하는 상태, 1: 모터가 이동중일때 체크하는 상태 
	int nRet = CTL_ERROR;
	char cJamcode[10] = {NULL};
	double dCurrentPos[2] = {0,0}, dCalPos[3] = {0,0,0}, dSafepos_gap[3] = {0,0,0}, dCurrMotPos = 0; 
	int nSafetyPos_Flag = CTL_NO; 

	//090914 모터보드 인터럽트 발생 시점이 문제가 있어 조건 추가  && dCurrentPos[1] < st_motor[n_ReferenceAxis].n_interrupt_position + 1
	
// 	dCurrentPos[0] = COMI.Get_MotCurrentPos(n_SourceAxis);    //현재 이동하는 축
// 	dCurrentPos[1] = COMI.Get_MotCurrentPos(n_ReferenceAxis); //참고되는 축이 정보 
// 	//COMI.Get_Deviation_Position(n_SourceAxis, st_motor[n_SourceAxis].n_allow,st_motor[n_SourceAxis].d_deviation) ;
// 	if(n_SourceAxis == M_DCLDRBT_Y && n_ReferenceAxis == M_DCULDRBT_Y) //LOAD Robot가 DC Test Site로 이동하는 동작시 사용  
// 	{
// 		//각자 충돌 영역인 DC TEST 사이트를 기준으로 현재의 위치를 빼주어 실제 간섭 영역의 범위를 알수 있게 한다 
// 		dCalPos[0] = (st_motor[n_SourceAxis].md_pos[P_DCLDRBT_DCTEST] - dCurrentPos[0]);	//로딩 로보트 
// 		dCalPos[1] = (st_motor[n_ReferenceAxis].md_pos[P_DCULDRBT_DCTEST] - dCurrentPos[1]);//언로딩 로보트 
// 		dCalPos[2] = dCalPos[1] + dCalPos[0];
// 
// 		//090915 조건 추가 
// 		if(d_TargetPos > st_motor[n_SourceAxis].md_pos[P_DCLDRBT_DCSAFETY_POS]) 
// 		{
// 			dCurrMotPos = COMI.Get_MotCurrentPos(M_DCULDRBT_Y);
// 			if(dCurrMotPos > st_motor[M_DCULDRBT_Y].n_interrupt_position)
// 			{//상대편 모터가 인터럽트 위치도 벗어나지 못한상태이면 무조건 에러를 발생시켜 source motor를 정지해야 한다 
// 				nSafetyPos_Flag = CTL_NO;
// 			}
// 			else
// 			{//레퍼런스 모터가 위험 위치라도 인터럽트 위치를 벗어난 상태이면 이동하고 있을것이라고 생각하고 조건을 긍정으로 본다, 이동한다 
// 				nSafetyPos_Flag = CTL_YES;
// 			}			
// 		}
// 		else
// 		{//타겟 위치의 값이 안전 위치를 벗어나지 않으면 안전하니 그냥 이동해도 된다  
// 			dCalPos[2] = d_Safety_Gap + 1;//090925 어차피 안전 영역으로 읻오하니 조건 체크를 제외한다 
// 			nSafetyPos_Flag = CTL_YES;
// 		}
// 
// 		/////////////////////////////////////////////////////////////
// 		//2011.0123 조건 추가, 잘못된 티칭값 발생시 확인위해 추가 
// 		/////////////////////////////////////////////////////////////
// 		if(d_TargetPos != 0) //d_TargetPos == 0 홈체크시는 보지 말자 
// 		{
// 			dSafepos_gap[0] = st_motor[n_SourceAxis].md_pos[P_DCLDRBT_DCTEST] - st_motor[n_SourceAxis].md_pos[P_DCLDRBT_DCSAFETY_POS];
// 			dSafepos_gap[1] = st_motor[n_ReferenceAxis].md_pos[P_DCULDRBT_DCTEST] - st_motor[n_ReferenceAxis].md_pos[P_DCULDRBT_DCSAFETY_POS];
// 			if(dSafepos_gap[0] > dSafepos_gap[1]) //source 쪽 안전 위치 갭이 큰 상태 
// 			{
// 				dSafepos_gap[2] = dSafepos_gap[0];
// 			}
// 			else if(dSafepos_gap[0] < dSafepos_gap[1]) //reference 쪽 안전 위치 갭이 큰 상태 
// 			{
// 				dSafepos_gap[2] = dSafepos_gap[1];
// 			}
// 			else 
// 			{
// 				dSafepos_gap[2] = dSafepos_gap[0];
// 			}
// 
// 			if(dSafepos_gap[2] < (d_Safety_Gap - 10)) //2011.0123 추가 //현재 설정되어 있는 안전위치 값이 clash safety pos - 10mm 값보다 작으면 에러 발생 
// 			{
// 				nSafetyPos_Flag = CTL_NO;
// 
// 				st_msg.mstr_abnormal_msg.Format("[Safety]Safety Pos[%lf] < Clash Pos[%lf - 10mm]", dSafepos_gap[2], d_Safety_Gap);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
// 			}
// 		}
// 
// 
// 		//각 모토 Y의 속도 정보는 같아야 한다 
// 		if(dCalPos[2] > d_Safety_Gap && nSafetyPos_Flag == CTL_YES) //상호 계산된 갭 값이 일정 값(d_Safety_Gap) 이상으로 안전한 위치에 있어 동작이 가능한 상태이다 
// 		{
// 			nRet = CTL_GOOD; //정상 간섭 영역이 아니다 
// 		}
// 		else
// 		{ //정해진 위치보다 더 가까이 있어 안전한 상태가 아니다 동작 중이면 멈추어야 한다 
// 			//000009 0 A "Robot 충돌영역 간섭 문제가 발생-Safety Position Check Error - (Load Robot Y & Unload Robot Y)."
// 			nRet = CTL_ERROR;
// 
// 			if(n_Mode == 1) //모터가 이동중 안전을 체크하여 에러가 발생하면 해당 이동 모터를 급 정지 한다 
// 			{
// 				cmmSxStopEmg(n_SourceAxis);
// 				CtlBdFunc.ms_ErrMsg.Format("Clash Error Motor E_Stop MotorNum:%d", n_SourceAxis);
// 				CtlBdFunc.Send_Error_Message(MOT_ERR_MSG, n_SourceAxis, alarm.mstr_code, CtlBdFunc.ms_ErrMsg); //2011.0123 
// 			}
// 
// 			sprintf(cJamcode, "%02d0009", n_SourceAxis);  
// 			alarm.mstr_code = _T(cJamcode); 	alarm.mn_type_mode = CTL_dWARNING; //st_work.m_iRunStatus = CTL_dWARNING;
// 			CtlBdFunc.ms_ErrMsg.Format("Clash_Axis:%d, Mode:%d, Source:%4.3f, Reference:%4.3f, TS:%4.3f, TR:%4.3f, TT:%4.3f",n_SourceAxis, n_Mode, dCurrentPos[0], dCurrentPos[1], dCalPos[0], dCalPos[1], dCalPos[2]);
// 			if (st_handler.cwnd_list != NULL && n_Mode == 1)  // 리스트 바 화면 존재
// 			{
// 				st_msg.mstr_abnormal_msg = CtlBdFunc.ms_ErrMsg;
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
// 			}
// 			CtlBdFunc.Send_Error_Message(MOT_ERR_MSG, n_SourceAxis, alarm.mstr_code, CtlBdFunc.ms_ErrMsg); //2011.0123 
// 		}
// 	}
// 	else if(n_SourceAxis == M_DCULDRBT_Y && n_ReferenceAxis == M_DCLDRBT_Y) //UNLOAD Robot가 DC Test Site로 이동하는 동작시 사용  
// 	{
// 		//각자 충돌 영역인 DC TEST 사이트를 기준으로 현재의 위치를 빼주어 실제 간섭 영역의 범위를 알수 있게 한다 
// 		dCalPos[0] = (st_motor[n_SourceAxis].md_pos[P_DCULDRBT_DCTEST] - dCurrentPos[0]);	//로딩 로보트 
// 		dCalPos[1] = (st_motor[n_ReferenceAxis].md_pos[P_DCLDRBT_DCTEST] - dCurrentPos[1]);//언로딩 로보트 
// 		dCalPos[2] = dCalPos[1] + dCalPos[0];
// 
// 		//090915 조건 추가 
// 		if(d_TargetPos > st_motor[n_SourceAxis].md_pos[P_DCULDRBT_DCSAFETY_POS]) 
// 		{
// 			dCurrMotPos = COMI.Get_MotCurrentPos(M_DCLDRBT_Y);
// 			if(dCurrMotPos > st_motor[M_DCLDRBT_Y].n_interrupt_position)
// 			{
// 				nSafetyPos_Flag = CTL_NO;
// 			}
// 			else
// 			{
// 				nSafetyPos_Flag = CTL_YES;
// 			}
// 		}
// 		else
// 		{
// 			dCalPos[2] = d_Safety_Gap + 1;//090925 어차피 안전 영역으로 읻오하니 조건 체크를 제외한다 
// 			nSafetyPos_Flag = CTL_YES;
// 		}
// 
// 		/////////////////////////////////////////////////////////////
// 		//2011.0123 조건 추가, 잘못된 티칭값 발생시 확인위해 추가 
// 		/////////////////////////////////////////////////////////////
// 		if(d_TargetPos != 0) //d_TargetPos == 0 홈체크시는 보지 말자 
// 		{
// 			dSafepos_gap[0] = st_motor[n_SourceAxis].md_pos[P_DCULDRBT_DCTEST] - st_motor[n_SourceAxis].md_pos[P_DCULDRBT_DCSAFETY_POS];
// 			dSafepos_gap[1] = st_motor[n_ReferenceAxis].md_pos[P_DCLDRBT_DCTEST] - st_motor[n_ReferenceAxis].md_pos[P_DCLDRBT_DCSAFETY_POS];
// 			if(dSafepos_gap[0] > dSafepos_gap[1]) //source 쪽 안전 위치 갭이 큰 상태 
// 			{
// 				dSafepos_gap[2] = dSafepos_gap[0];
// 			}
// 			else if(dSafepos_gap[0] < dSafepos_gap[1]) //reference 쪽 안전 위치 갭이 큰 상태 
// 			{
// 				dSafepos_gap[2] = dSafepos_gap[1];
// 			}
// 			else 
// 			{
// 				dSafepos_gap[2] = dSafepos_gap[0];
// 			}
// 
// 			if(dSafepos_gap[2] < (d_Safety_Gap - 10)) //2011.0123 추가 //현재 설정되어 있는 안전위치 값이 clash safety pos - 10mm 값보다 작으면 에러 발생 
// 			{
// 				nSafetyPos_Flag = CTL_NO;
// 
// 				st_msg.mstr_abnormal_msg.Format("[Safety]Safety Pos[%lf] < Clash Pos[%lf - 10mm]", dSafepos_gap[2], d_Safety_Gap);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
// 			}
// 		}
// 		
// 		//각 모토 Y의 속도 정보는 같아야 한다 
// 		if(dCalPos[2] > d_Safety_Gap && nSafetyPos_Flag == CTL_YES) //상호 계산된 갭 값이 일정 값(d_Safety_Gap) 이상으로 안전한 위치에 있어 동작이 가능한 상태이다 
// 		{
// 			//CtlBdFunc.ms_ErrMsg.Format("Axis:%d, Source:%.3f, Reference:%.3f, TS:%.3f, TR:%.3f, TT:%.3f",n_SourceAxis, dCurrentPos[0], dCurrentPos[1], dCalPos[0], dCalPos[1], dCalPos[2]);
// 			//if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			//{
// 			//	st_msg.mstr_abnormal_msg = CtlBdFunc.ms_ErrMsg;
// 			//	st_timer.n_cmd_dialog_list_msg[ABNORMAL_MSG][0] = CTL_YES; st_timer.n_cmd_dialog_list_msg[ABNORMAL_MSG][1] = 0; //2011.0120 변경   // 동작 실패 출력 요청
// 			//}
// 			nRet = CTL_GOOD; //정상 간섭 영역이 아니다 
// 		}
// 		else
// 		{ //정해진 위치보다 더 가까이 있어 안전한 상태가 아니다 동작 중이면 멈추어야 한다 
// 			//000009 0 A "Robot 충돌영역 간섭 문제가 발생-Safety Position Check Error - (Load Robot Y & Unload Robot Y)."
// 			nRet = CTL_ERROR;			
// 			if(n_Mode == 1) //모터가 이동중 안전을 체크하여 에러가 발생하면 해당 이동 모터를 급 정지 한다 
// 			{
// 				cmmSxStopEmg(n_SourceAxis);
// 				CtlBdFunc.ms_ErrMsg.Format("Clash Error Motor E_Stop MotorNum:%d", n_SourceAxis);
// 				CtlBdFunc.Send_Error_Message(MOT_ERR_MSG, n_SourceAxis, alarm.mstr_code, CtlBdFunc.ms_ErrMsg); //2011.0123 
// 			}
// 			
// 			sprintf(cJamcode, "%02d0009", n_SourceAxis);  
// 			alarm.mstr_code = _T(cJamcode); 	alarm.mn_type_mode = CTL_dWARNING; //st_work.m_iRunStatus = CTL_dWARNING;
// 			CtlBdFunc.ms_ErrMsg.Format("Clash_Axis:%d, Mode:%d, Source:%4.3f, Reference:%4.3f, TS:%4.3f, TR:%4.3f, TT:%4.3f",n_SourceAxis, n_Mode, dCurrentPos[0], dCurrentPos[1], dCalPos[0], dCalPos[1], dCalPos[2]);
// 			if (st_handler.cwnd_list != NULL && n_Mode == 1)  // 리스트 바 화면 존재
// 			{
// 				st_msg.mstr_abnormal_msg = CtlBdFunc.ms_ErrMsg;
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
// 			}
// 			CtlBdFunc.Send_Error_Message(MOT_ERR_MSG, n_SourceAxis, alarm.mstr_code, CtlBdFunc.ms_ErrMsg); //2011.0123 
// 		}
// 	}
// 	else if(n_SourceAxis == M_INSERTRBT_Y && n_ReferenceAxis == M_REMOVERBT_Y) //Insert Robot가 BIB Table로 이동하는 동작 
// 	{
// 		//각자 충돌 영역인 DC TEST 사이트를 기준으로 현재의 위치를 빼주어 실제 간섭 영역의 범위를 알수 있게 한다 
// 		dCalPos[0] = (st_motor[n_SourceAxis].md_pos[P_INSERTRBT_BIBTABLE] - dCurrentPos[0]);	//로딩 로보트 
// 		dCalPos[1] = (st_motor[n_ReferenceAxis].md_pos[P_REMOVERBT_BIBTABLE] - dCurrentPos[1]);//언로딩 로보트 
// 
// 		dCalPos[2] = dCalPos[1] + dCalPos[0];
// 
// 		//090915 조건 추가 
// 		if(d_TargetPos > st_motor[n_SourceAxis].md_pos[P_INSERTRBT_SAFETY_POS]) 
// 		{
// 			dCurrMotPos = COMI.Get_MotCurrentPos(M_REMOVERBT_Y);
// 			if(dCurrMotPos > st_motor[M_REMOVERBT_Y].n_interrupt_position)
// 			{
// 				nSafetyPos_Flag = CTL_NO;
// 			}
// 			else
// 			{
// 				nSafetyPos_Flag = CTL_YES;
// 			}
// 		}
// 		else
// 		{
// 			dCalPos[2] = d_Safety_Gap + 1;//090925 어차피 안전 영역으로 읻오하니 조건 체크를 제외한다 
// 			nSafetyPos_Flag = CTL_YES;
// 		}
// 
// 		/////////////////////////////////////////////////////////////
// 		//2011.0123 조건 추가, 잘못된 티칭값 발생시 확인위해 추가 
// 		/////////////////////////////////////////////////////////////
// 		{
// 			dSafepos_gap[0] = st_motor[n_SourceAxis].md_pos[P_INSERTRBT_BIBTABLE] - st_motor[n_SourceAxis].md_pos[P_INSERTRBT_SAFETY_POS];
// 			dSafepos_gap[1] = st_motor[n_ReferenceAxis].md_pos[P_REMOVERBT_BIBTABLE] - st_motor[n_ReferenceAxis].md_pos[P_REMOVERBT_BIBTABLE_SAFETY_POS];
// 			if(dSafepos_gap[0] > dSafepos_gap[1]) //source 쪽 안전 위치 갭이 큰 상태 
// 			{
// 				dSafepos_gap[2] = dSafepos_gap[0];
// 			}
// 			else if(dSafepos_gap[0] < dSafepos_gap[1]) //reference 쪽 안전 위치 갭이 큰 상태 
// 			{
// 				dSafepos_gap[2] = dSafepos_gap[1];
// 			}
// 			else 
// 			{
// 				dSafepos_gap[2] = dSafepos_gap[0];
// 			}
// 
// 			if(dSafepos_gap[2] < (d_Safety_Gap - 10)) //2011.0123 추가 //현재 설정되어 있는 안전위치 값이 clash safety pos - 10mm 값보다 작으면 에러 발생 
// 			{
// 				nSafetyPos_Flag = CTL_NO;
// 
// 				st_msg.mstr_abnormal_msg.Format("[Safety]Safety Pos[%lf] < Clash Pos[%lf - 10mm]", dSafepos_gap[2], d_Safety_Gap);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
// 			}
// 		}
// 
// 
// 		//각 모토 Y의 속도 정보는 같아야 한다 
// 		if(dCalPos[2] > d_Safety_Gap && nSafetyPos_Flag == CTL_YES) //상호 안전한 위치에 있어 동작이 가능한 상태이다 
// 		{
// 			nRet = CTL_GOOD; //정상 간섭 영역이 아니다 
// 		}
// 		else
// 		{ //정해진 위치보다 더 가까이 있어 안전한 상태가 아니다 동작 중이면 멈추어야 한다 
// 			//000009 0 A "Robot 충돌영역 간섭 문제가 발생-Safety Position Check Error - (Load Robot Y & Unload Robot Y)."
// 			nRet = CTL_ERROR;
// 			if(n_Mode == 1) //모터가 이동중 안전을 체크하여 에러가 발생하면 해당 이동 모터를 급 정지 한다 
// 			{
// 				cmmSxStopEmg(n_SourceAxis);
// 				CtlBdFunc.ms_ErrMsg.Format("Clash Error Motor E_Stop MotorNum:%d", n_SourceAxis);
// 				CtlBdFunc.Send_Error_Message(MOT_ERR_MSG, n_SourceAxis, alarm.mstr_code, CtlBdFunc.ms_ErrMsg); //2011.0123 
// 			}	
// 			sprintf(cJamcode, "%02d0009", n_SourceAxis);  
// 			alarm.mstr_code = _T(cJamcode); 	alarm.mn_type_mode = CTL_dWARNING; //st_work.m_iRunStatus = CTL_dWARNING;
// 			CtlBdFunc.ms_ErrMsg.Format("Clash_Axis:%d, Mode:%d, Source:%4.3f, Reference:%4.3f, TS:%4.3f, TR:%4.3f, TT:%4.3f",n_SourceAxis, n_Mode, dCurrentPos[0], dCurrentPos[1], dCalPos[0], dCalPos[1], dCalPos[2]);
// 			if (st_handler.cwnd_list != NULL && n_Mode == 1)  // 리스트 바 화면 존재
// 			{
// 				st_msg.mstr_abnormal_msg = CtlBdFunc.ms_ErrMsg;
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
// 			}
// 			CtlBdFunc.Send_Error_Message(MOT_ERR_MSG, n_SourceAxis, alarm.mstr_code, CtlBdFunc.ms_ErrMsg); //2011.0123 
// 			//CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_SourceAxis, st_work.m_iRunStatus, alarm.mstr_code);  
// 		}
// 	}
// 	else if(n_SourceAxis == M_REMOVERBT_Y && n_ReferenceAxis == M_INSERTRBT_Y) //Remove Robot가 BIB Table로 이동하는 동작 
// 	{
// 		//각자 충돌 영역인 DC TEST 사이트를 기준으로 현재의 위치를 빼주어 실제 간섭 영역의 범위를 알수 있게 한다 
// 		dCalPos[0] = (st_motor[n_SourceAxis].md_pos[P_REMOVERBT_BIBTABLE] - dCurrentPos[0]);	//로딩 로보트 
// 		dCalPos[1] = (st_motor[n_ReferenceAxis].md_pos[P_INSERTRBT_BIBTABLE] - dCurrentPos[1]);//언로딩 로보트 
// 		
// 		dCalPos[2] = dCalPos[1] + dCalPos[0];
// 
// 		//090915 조건 추가 
// 		if(d_TargetPos > st_motor[n_SourceAxis].md_pos[P_REMOVERBT_BIBTABLE_SAFETY_POS]) 
// 		{
// 			dCurrMotPos = COMI.Get_MotCurrentPos(M_INSERTRBT_Y);
// 			if(dCurrMotPos > st_motor[M_INSERTRBT_Y].n_interrupt_position)
// 			{
// 				nSafetyPos_Flag = CTL_NO;
// 			}
// 			else
// 			{
// 				nSafetyPos_Flag = CTL_YES;
// 			}
// 		}
// 		else
// 		{
// 			dCalPos[2] = d_Safety_Gap + 1;	//090925 어차피 안전 영역으로 읻오하니 조건 체크를 제외한다 
// 			nSafetyPos_Flag = CTL_YES;
// 		}
// 
// 		/////////////////////////////////////////////////////////////
// 		//2011.0123 조건 추가, 잘못된 티칭값 발생시 확인위해 추가 
// 		/////////////////////////////////////////////////////////////
// 		if(d_TargetPos != 0) //d_TargetPos == 0 홈체크시는 보지 말자 
// 		{
// 			dSafepos_gap[0] = st_motor[n_SourceAxis].md_pos[P_REMOVERBT_BIBTABLE] - st_motor[n_SourceAxis].md_pos[P_REMOVERBT_BIBTABLE_SAFETY_POS];
// 			dSafepos_gap[1] = st_motor[n_ReferenceAxis].md_pos[P_INSERTRBT_BIBTABLE] - st_motor[n_ReferenceAxis].md_pos[P_INSERTRBT_SAFETY_POS];
// 			if(dSafepos_gap[0] > dSafepos_gap[1]) //source 쪽 안전 위치 갭이 큰 상태 
// 			{
// 				dSafepos_gap[2] = dSafepos_gap[0];
// 			}
// 			else if(dSafepos_gap[0] < dSafepos_gap[1]) //reference 쪽 안전 위치 갭이 큰 상태 
// 			{
// 				dSafepos_gap[2] = dSafepos_gap[1];
// 			}
// 			else 
// 			{
// 				dSafepos_gap[2] = dSafepos_gap[0];
// 			}
// 
// 			if(dSafepos_gap[2] < (d_Safety_Gap - 10)) //2011.0123 추가 //현재 설정되어 있는 안전위치 값이 clash safety pos - 10mm 값보다 작으면 에러 발생 
// 			{
// 				nSafetyPos_Flag = CTL_NO;
// 
// 				st_msg.mstr_abnormal_msg.Format("[Safety]Safety Pos[%lf] < Clash Pos[%lf - 10mm]", dSafepos_gap[2], d_Safety_Gap);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
// 			}
// 		}
// 		
// 		//각 모토 Y의 속도 정보는 같아야 한다 
// 		if(dCalPos[2] > d_Safety_Gap && nSafetyPos_Flag == CTL_YES) //상호 안전한 위치에 있어 동작이 가능한 상태이다 
// 		{
// 			nRet = CTL_GOOD; //정상 간섭 영역이 아니다 
// 		}
// 		else
// 		{ //정해진 위치보다 더 가까이 있어 안전한 상태가 아니다 동작 중이면 멈추어야 한다 
// 			//000009 0 A "Robot 충돌영역 간섭 문제가 발생-Safety Position Check Error - (Load Robot Y & Unload Robot Y)."
// 			nRet = CTL_ERROR;
// 			if(n_Mode == 1) //모터가 이동중 안전을 체크하여 에러가 발생하면 해당 이동 모터를 급 정지 한다 
// 			{
// 				cmmSxStopEmg(n_SourceAxis);
// 				CtlBdFunc.ms_ErrMsg.Format("Clash Error Motor E_Stop MotorNum:%d", n_SourceAxis);
// 				CtlBdFunc.Send_Error_Message(MOT_ERR_MSG, n_SourceAxis, alarm.mstr_code, CtlBdFunc.ms_ErrMsg); //2011.0123 
// 			}	
// 			sprintf(cJamcode, "%02d0009", n_SourceAxis);  
// 			alarm.mstr_code = _T(cJamcode); 	alarm.mn_type_mode = CTL_dWARNING; //st_work.m_iRunStatus = CTL_dWARNING;
// 			CtlBdFunc.ms_ErrMsg.Format("Clash_Axis:%d, Mode:%d, Source:%4.3f, Reference:%4.3f, TS:%4.3f, TR:%4.3f, TT:%4.3f",n_SourceAxis, n_Mode, dCurrentPos[0], dCurrentPos[1], dCalPos[0], dCalPos[1], dCalPos[2]);
// 			if (st_handler.cwnd_list != NULL && n_Mode == 1)  // 리스트 바 화면 존재
// 			{
// 				st_msg.mstr_abnormal_msg = CtlBdFunc.ms_ErrMsg;
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
// 			}
// 			CtlBdFunc.Send_Error_Message(MOT_ERR_MSG, n_SourceAxis, alarm.mstr_code, CtlBdFunc.ms_ErrMsg); //2011.0123 
// 			//CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_SourceAxis, st_work.m_iRunStatus, alarm.mstr_code);  
// 		}
// 	}
// 		
	return nRet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inposition 생략 및 거리별 속도를 관리한다 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CCtlBd_Function::MotorSpeed_Control(int n_Axis, double d_CurrentPos, double d_TargetPos, double * dp_Speed)
{
	double dCurrentPos = 0, dTargetPos = 0, dVal =0, d_abs_val = 0, dAccDecTime=0; 
	int nRetStatus=0;

// 	if(n_Axis == M_DCLDRBT_Z || n_Axis == M_DCULDRBT_Z || n_Axis == M_ULDRBT_Z)
// 	{
// 		dCurrentPos = COMI.Get_MotCurrentPos(n_Axis);		
// 
// 		//목표위치 계산 방법은 ==> 현재위치 - 목표이동 위치 이다 
// 		dVal = dCurrentPos - d_TargetPos;
// 		if(st_motor[n_Axis].md_pos[0] >= d_TargetPos) //P_DCLDRBT_Z_SAFETY == 0 ,목표 지점이 안전위치와 같으면 정해진 속도로 이동 
// 		{
// 			dp_Speed[0] = st_motor[n_Axis].d_spd_vel[0]; dp_Speed[1] = st_motor[n_Axis].d_spd_vel[1]; dp_Speed[2] = st_motor[n_Axis].d_spd_vel[2]; //가속 펄수 값은 9,000,000 이다
// 			//UP할때는 Inposition를 보자  말자 
// 			COMI.Set_Motor_IO_Property(n_Axis, cmINP_EN, cmFALSE); //cmINP_EN=9 //cmFALSE = 0 INP 비활성, cmTRUE = 1 INP 활성 
// 		}
// 		else if(dVal > 0) //먼 위치에서 가까운 위치(홈 위치) 로 이동했을때는 + 값 발생 , 모터가 UP 할 때 발생 
// 		{
// 			//원래 셋팅되어있는 값 사용 
// 			//650,000 13,000,000 13,000,000   ==> 16mm 이동하는데 65ms 소요 
// 			//300,000  9,000000   9,000000    ==> 16mm 이동하는데 80ms 소요 
// 			dp_Speed[0] = st_motor[n_Axis].d_spd_vel[0]; dp_Speed[1] = st_motor[n_Axis].d_spd_vel[1]; dp_Speed[2] = st_motor[n_Axis].d_spd_vel[2]; //가속 펄수 값은 9,000,000 이다
// 
// 			//UP할때 또는 정확한 목표지점으로 가지 않아도 되는 조건이면 Inposition를 보지 말자 
// 			COMI.Set_Motor_IO_Property(n_Axis, cmINP_EN, cmFALSE); //cmINP_EN=9 //cmFALSE = 0 INP 비활성, cmTRUE = 1 INP 활성 
// 		} //12mm 이하로 내려갈 때 
// 		else
// 		{
// 			if(n_Axis == M_DCLDRBT_Z || n_Axis == M_DCULDRBT_Z )
// 			{
// 				if(dVal > -12) //-1 ~ -19 DC로디로봇이 DC 소켓에 놓을때 - 값 발생  //홈 위치에서 증가 방향으로 이동하려면 - 값이 발생한다 
// 				{//dAccPPS = (d_work - st_motor[n_Axis].d_initial_speed) / (d_accel_time / 1000);
// 					//dAccDecTime = (d_work - st_motor[n_Axis].d_initial_speed) / (dAccPPS / 1000);
// 					dAccDecTime = (300000 - st_motor[n_Axis].d_initial_speed) / (5000000 / 1000); //60ms		
// 					dp_Speed[0] = 300000; dp_Speed[1] = dAccDecTime; dp_Speed[2] = dAccDecTime; //가속 펄수 값은 9,000,000 이다 
// 					
// 					COMI.Set_Motor_IO_Property(n_Axis, cmINP_EN, cmTRUE); //cmINP_EN=9 //cmFALSE = 0 INP 비활성, cmTRUE = 1 INP 활성 
// 				} 
// 				//20mm 이하로 내려갈 때 
// 				else if(dVal > -38) //-1 ~ -19 DC로디로봇이 DC 소켓에 놓을때 - 값 발생  //홈 위치에서 증가 방향으로 이동하려면 - 값이 발생한다 
// 				{//dAccPPS = (d_work - st_motor[n_Axis].d_initial_speed) / (d_accel_time / 1000);
// 				 //dAccDecTime = (d_work - st_motor[n_Axis].d_initial_speed) / (dAccPPS / 1000);
// 					dAccDecTime = (300000 - st_motor[n_Axis].d_initial_speed) / (9000000 / 1000); //dAccDecTime = 33 ms			
// 					dp_Speed[0] = 300000; dp_Speed[1] = dAccDecTime; dp_Speed[2] = dAccDecTime; //가속 펄수 값은 9,000,000 이다 
// 
// 					COMI.Set_Motor_IO_Property(n_Axis, cmINP_EN, cmTRUE); //cmINP_EN=9 //cmFALSE = 0 INP 비활성, cmTRUE = 1 INP 활성 
// 				} //20mm이상 내려갈때 
// 				else //if(dVal < -38)  //-20 이하 로딩로봇이 트레이에서 집을때, 언로딩 로봇이 인서트 버퍼 bottom에 놓을때 발생 
// 				{
// 					//dAccDecTime = (650000 - st_motor[n_Axis].d_initial_speed) / (12000000 / 1000); //dAccDecTime = 300 ms			
// 					//dp_Speed[0] = 650000; dp_Speed[1] = dAccDecTime; dp_Speed[2] = dAccDecTime; //가속 펄수 값은 9,000,000 이다 			
// 					dAccDecTime = (st_motor[n_Axis].d_spd_vel[0] - st_motor[n_Axis].d_initial_speed) / (12000000 / 1000); //dAccDecTime = 300 ms			
// 					dp_Speed[0] = st_motor[n_Axis].d_spd_vel[0]; dp_Speed[1] = dAccDecTime; dp_Speed[2] = dAccDecTime; //가속 펄수 값은 9,000,000 이다 		
// 					
// 					COMI.Set_Motor_IO_Property(n_Axis, cmINP_EN, cmTRUE); //cmINP_EN=9 //cmFALSE = 0 INP 비활성, cmTRUE = 1 INP 활성 			
// 				}
// 			}
// 
// 			if(n_Axis == M_DCLDRBT_Z)
// 			{
// 				dp_Speed[0] = st_motor[n_Axis].d_spd_vel[0]; dp_Speed[1] = st_motor[n_Axis].d_spd_vel[1]; dp_Speed[2] = st_motor[n_Axis].d_spd_vel[2]; //가속 펄수 값은 9,000,000 이다
// ///* 2011.0126
// 				if(d_TargetPos == st_motor[M_DCLDRBT_Z].md_pos[P_DCLDRBT_Z_LDPLATE_PICK])
// 				{
// 					//091012 dp_Speed[0] = 650000; dp_Speed[1] = 50; dp_Speed[2] = 100; //  
// 					dp_Speed[0] = 450000; dp_Speed[1] = 100; dp_Speed[2] = 50; //  
// 				}
// 				else if(d_TargetPos == st_motor[M_DCLDRBT_Z].md_pos[P_DCLDRBT_Z_DCBUFF_PICK])
// 				{
// 					dp_Speed[0] = 650000; dp_Speed[1] = 50; dp_Speed[2] = 100; //  
// 				}
// 				else if(d_TargetPos == st_motor[M_DCLDRBT_Z].md_pos[P_DCLDRBT_Z_DCTEST_PICK])
// 				{
// 					dp_Speed[0] = 450000; dp_Speed[1] = 50; dp_Speed[2] = 50; //  
// 				}
// 				else if(d_TargetPos == st_motor[M_DCLDRBT_Z].md_pos[P_DCLDRBT_Z_LDPLATE_PLACE])
// 				{
// 					dp_Speed[0] = 650000; dp_Speed[1] = 50; dp_Speed[2] = 100; //  
// 				}
// 				else if(d_TargetPos == st_motor[M_DCLDRBT_Z].md_pos[P_DCLDRBT_Z_LDBUFF_PLACE])
// 				{
// 					dp_Speed[0] = 650000; dp_Speed[1] = 50; dp_Speed[2] = 100; //  
// 				}
// 				else if(d_TargetPos == st_motor[M_DCLDRBT_Z].md_pos[P_DCLDRBT_Z_DCTEST_PLACE])
// 				{
// 					//dp_Speed[0] = 450000; dp_Speed[1] = 50; dp_Speed[2] = 50; //  
// 					//091012 dp_Speed[0] = 380000; dp_Speed[1] = 50; dp_Speed[2] = 50; //
// 					dp_Speed[0] = 380000; dp_Speed[1] = 50; dp_Speed[2] = 50; //  
// 				}
// 				else
// 				{
// 					dp_Speed[0] = st_motor[n_Axis].d_spd_vel[0]; dp_Speed[1] = st_motor[n_Axis].d_spd_vel[1]; dp_Speed[2] = st_motor[n_Axis].d_spd_vel[2]; //가속 펄수 값은 9,000,000 이다
// 				}
// //*/
// 				COMI.Set_Motor_IO_Property(n_Axis, cmINP_EN, cmTRUE); //cmINP_EN=9 //cmFALSE = 0 INP 비활성, cmTRUE = 1 INP 활성 
// 			}
// 			else if(n_Axis == M_DCULDRBT_Z)
// 			{
// 				dp_Speed[0] = st_motor[n_Axis].d_spd_vel[0]; dp_Speed[1] = st_motor[n_Axis].d_spd_vel[1]; dp_Speed[2] = st_motor[n_Axis].d_spd_vel[2]; //가속 펄수 값은 9,000,000 이다
// 
// ///* 2011.0126 
// 				if(d_TargetPos == st_motor[M_DCULDRBT_Z].md_pos[P_DCULDRBT_Z_DCTEST_PICK])
// 				{
// 					dp_Speed[0] = 550000; dp_Speed[1] = 50; dp_Speed[2] = 100; //  
// 				}
// 				else if(d_TargetPos == st_motor[M_DCULDRBT_Z].md_pos[P_DCULDRBT_Z_DCBUFF_PICK])
// 				{
// 					dp_Speed[0] = 400000; dp_Speed[1] = 50; dp_Speed[2] = 50; //  
// 				}
// 				else if(d_TargetPos == st_motor[M_DCULDRBT_Z].md_pos[P_DCULDRBT_Z_INSERTBUFF_TOP_PICK])
// 				{
// 					dp_Speed[0] = 400000; dp_Speed[1] = 50; dp_Speed[2] = 50; //  
// 				}
// 				else if(d_TargetPos == st_motor[M_DCULDRBT_Z].md_pos[P_DCULDRBT_Z_INSERTBUFF_BOTTOM_PICK])
// 				{
// 					dp_Speed[0] = 550000; dp_Speed[1] = 50; dp_Speed[2] = 100; //  
// 				}
// 				else if(d_TargetPos == st_motor[M_DCULDRBT_Z].md_pos[P_DCULDRBT_Z_DCTEST_PLACE])
// 				{
// 					dp_Speed[0] = 550000; dp_Speed[1] = 50; dp_Speed[2] = 100; //  
// 				}
// 				else if(d_TargetPos == st_motor[M_DCULDRBT_Z].md_pos[P_DCULDRBT_Z_DCBUFF_PLACE])
// 				{
// 					dp_Speed[0] = 400000; dp_Speed[1] = 50; dp_Speed[2] = 50; //  
// 				}
// 				else if(d_TargetPos == st_motor[M_DCULDRBT_Z].md_pos[P_DCULDRBT_Z_INSERTBUFF_TOP_PLACE])
// 				{
// 					//dp_Speed[0] = 400000; dp_Speed[1] = 50; dp_Speed[2] = 50; //  
// 					dp_Speed[0] = 400000; dp_Speed[1] = 50; dp_Speed[2] = 100; //  
// 				}
// 				else if(d_TargetPos == st_motor[M_DCULDRBT_Z].md_pos[P_DCULDRBT_Z_INSERTBUFF_BOTTOM_PLACE])
// 				{
// 					//dp_Speed[0] = 550000; dp_Speed[1] = 50; dp_Speed[2] = 100; //  
// 					dp_Speed[0] = 550000; dp_Speed[1] = 50; dp_Speed[2] = 100; //  
// 				}
// 				else
// 				{
// 					dp_Speed[0] = st_motor[n_Axis].d_spd_vel[0]; dp_Speed[1] = st_motor[n_Axis].d_spd_vel[1]; dp_Speed[2] = st_motor[n_Axis].d_spd_vel[2]; //가속 펄수 값은 9,000,000 이다
// 				}
// //*/
// 				COMI.Set_Motor_IO_Property(n_Axis, cmINP_EN, cmTRUE); //cmINP_EN=9 //cmFALSE = 0 INP 비활성, cmTRUE = 1 INP 활성 
// 			}
// 			else// if(n_Axis == M_ULDRBT_Z)
// 			{//090909 
// 				if(d_TargetPos == st_motor[M_ULDRBT_Z].md_pos[P_ULDRBT_Z_REMOVEBUFF_TOP_PICK])
// 				{
// 					dp_Speed[0] = 400000; dp_Speed[1] = 100; dp_Speed[2] = 150; //  
// 				}
// 				else if(d_TargetPos == st_motor[M_ULDRBT_Z].md_pos[P_ULDRBT_Z_REMOVEBUFF_MIDDLE_PICK])
// 				{
// 					dp_Speed[0] = 500000; dp_Speed[1] = 100; dp_Speed[2] = 150; //  
// 				}
// 				else if(d_TargetPos == st_motor[M_ULDRBT_Z].md_pos[P_ULDRBT_Z_REMOVEBUFF_BOTTOM_PICK])
// 				{
// 					dp_Speed[0] = 400000; dp_Speed[1] = 80; dp_Speed[2] = 150; //  
// 				}
// 				else if(d_TargetPos == st_motor[M_ULDRBT_Z].md_pos[P_ULDRBT_Z_MANUALBUFF_PICK])
// 				{
// 					dp_Speed[0] = 500000; dp_Speed[1] = 100; dp_Speed[2] = 150; //  
// 				}
// 				else if(d_TargetPos == st_motor[M_ULDRBT_Z].md_pos[P_ULDRBT_Z_MANUALBUFF_PLACE])
// 				{
// 					dp_Speed[0] = 400000; dp_Speed[1] = 100; dp_Speed[2] = 150; //  
// 				}
// 				else if(d_TargetPos == st_motor[M_ULDRBT_Z].md_pos[P_ULDRBT_Z_ULDPLATE_PLACE])
// 				{
// 					dp_Speed[0] = 400000; dp_Speed[1] = 80; dp_Speed[2] = 150; //  
// 				}
// 				else
// 				{
// 					dp_Speed[0] = st_motor[n_Axis].d_spd_vel[0]; dp_Speed[1] = st_motor[n_Axis].d_spd_vel[1]; dp_Speed[2] = st_motor[n_Axis].d_spd_vel[2]; //가속 펄수 값은 9,000,000 이다
// 				}
// 				COMI.Set_Motor_IO_Property(n_Axis, cmINP_EN, cmTRUE); //cmINP_EN=9 //cmFALSE = 0 INP 비활성, cmTRUE = 1 INP 활성 			
// 			}
// 		}
// 	}
// 	else if(n_Axis == M_SORTBUFFRBT_Z || n_Axis == M_INSERTRBT_Z || n_Axis == M_REMOVERBT_Z)
// 	{
// 		dCurrentPos = COMI.Get_MotCurrentPos(n_Axis);		
// 
// 		//목표위치 계산 방법은 ==> 현재위치 - 목표이동 위치 이다 
// 		dVal = dCurrentPos - d_TargetPos;
// 		if(dVal > 0) //먼 위치에서 가까운 위치(홈 위치) 로 이동했을때는 + 값 발생 , 모터가 UP 할 때 발생 
// 		{
// 			dp_Speed[0] = st_motor[n_Axis].d_spd_vel[0]; dp_Speed[1] = st_motor[n_Axis].d_spd_vel[1]; dp_Speed[2] = st_motor[n_Axis].d_spd_vel[2]; //가속 펄수 값은 9,000,000 이다
// 			//UP할때 또는 정확한 목표지점으로 가지 않아도 되는 조건이면 Inposition를 보지 말자 
// 
// 			if(n_Axis != M_INSERTRBT_Z) //2011.0302 흔들림 문제로 변경 
// 			{
// 				COMI.Set_Motor_IO_Property(n_Axis, cmINP_EN, cmFALSE); //cmINP_EN=9 //cmFALSE = 0 INP 비활성, cmTRUE = 1 INP 활성 
// 			}
// 		}
// 		else //motor 가 다운 할때 발생 
// 		{
// 			dp_Speed[0] = st_motor[n_Axis].d_spd_vel[0]; dp_Speed[1] = st_motor[n_Axis].d_spd_vel[1]; dp_Speed[2] = st_motor[n_Axis].d_spd_vel[2]; //가속 펄수 값은 9,000,000 이다
// 			COMI.Set_Motor_IO_Property(n_Axis, cmINP_EN, cmTRUE); //cmINP_EN=9 //cmFALSE = 0 INP 비활성, cmTRUE = 1 INP 활성 		
// 		}
// 	}
// 	/*
// 	else if(n_Axis == M_DCLDRBT_X || n_Axis == M_DCULDRBT_X) //아직 사용안함 
// 	{//로보트 X 스크류 모터는 정해진 속도로 게속 이동하여 굳이 필요없음.
// 		dAccDecTime = (st_motor[n_Axis].d_spd_vel[0] - st_motor[n_Axis].d_initial_speed) / (9750000 / 1000); //dAccDecTime = 66.6 ms	//이동구간 시간 : 289ms		
// 		dp_Speed[0] = st_motor[n_Axis].d_spd_vel[0]; dp_Speed[1] = dAccDecTime; dp_Speed[2] = dAccDecTime; //가속 펄수 값은 9,000,000 이다 
// 	}*/
// 	else if(n_Axis == M_DCLDRBT_Y || n_Axis == M_DCULDRBT_Y)// || n_Axis == M_ULDRBT_Y)
// 	{
// 		dCurrentPos = COMI.Get_MotCurrentPos(n_Axis);
// 		d_abs_val = fabs(dCurrentPos - d_TargetPos);
// 		dVal = dCurrentPos - d_TargetPos; //090830 fabs(dCurrentPos - d_TargetPos);
// 		if(d_abs_val < st_size.d_tray_dcld_mm_y + 1) //피치 Y + 2 mm 보다 작으면 정속도를 다운한다 , 진동 문제로 
// 		{
// 			dp_Speed[0] = 200000; dp_Speed[1] = 100; dp_Speed[2] = 100; //가속 펄수 값은 9,000,000 이다
// 		}
// 		else if(n_Axis == M_DCLDRBT_Y && st_motor[M_DCLDRBT_Y].md_pos[P_DCLDRBT_DCSAFETY_POS] == d_TargetPos && dVal < 0) //모터가 증가 방향이면 
// 		{
// 			dp_Speed[0] = 100000; dp_Speed[1] = 100; dp_Speed[2] = 100; //가속 펄수 값은 9,000,000 이다
// 		}
// 		else if(n_Axis == M_DCULDRBT_Y && st_motor[M_DCULDRBT_Y].md_pos[P_DCULDRBT_DCSAFETY_POS] == d_TargetPos && dVal < 0) //모터가 증가 방향이면 
// 		{
// 			dp_Speed[0] = 100000; dp_Speed[1] = 100; dp_Speed[2] = 100; //가속 펄수 값은 9,000,000 이다
// 		}
// 		else
// 		{
// 			dp_Speed[0] = st_motor[n_Axis].d_spd_vel[0]; dp_Speed[1] = st_motor[n_Axis].d_spd_vel[1]; dp_Speed[2] = st_motor[n_Axis].d_spd_vel[2]; 
// 		}
// 	}
// 	else if(n_Axis == M_REMOVERBT_Y) 
// 	{//090907
// 		dCurrentPos = COMI.Get_MotCurrentPos(n_Axis);
// 		d_abs_val = fabs(dCurrentPos - d_TargetPos);
// 		dVal = dCurrentPos - d_TargetPos; //090830 fabs(dCurrentPos - d_TargetPos);
// 		if(n_Axis == M_REMOVERBT_Y && st_motor[M_REMOVERBT_Y].md_pos[P_REMOVERBT_BIBTABLE_SAFETY_POS] == d_TargetPos && dVal < 0) //모터가 증가 방향이면 
// 		{
// 			dp_Speed[0] = 10000; dp_Speed[1] = 100; dp_Speed[2] = 100; //가속 펄수 값은 9,000,000 이다
// 		}		
// 		else
// 		{
// 			dp_Speed[0] = st_motor[n_Axis].d_spd_vel[0]; dp_Speed[1] = st_motor[n_Axis].d_spd_vel[1]; dp_Speed[2] = st_motor[n_Axis].d_spd_vel[2]; 
// 		}
// 	}
// 	else if(n_Axis == M_ULDRBT_Y)//보간 속도 율 
// 	{
// 		dCurrentPos = COMI.Get_MotCurrentPos(n_Axis);
// 		dVal = fabs(dCurrentPos - d_TargetPos);
// 		if(dVal < 50)
// 		{
// 			dp_Speed[0] = 50; dp_Speed[1] = 50; dp_Speed[2] = 50; //speed rate를 50%로 변경 한다
// 		}
// 		else if(dVal < 100) //100mm이하 위치 
// 		{
// 			dp_Speed[0] = 60; dp_Speed[1] = 60; dp_Speed[2] = 60; //speed rate를 50%로 변경 한다
// 		}
// 		else if(dVal < 200) //100mm이하 위치 
// 		{
// 			dp_Speed[0] = 70; dp_Speed[1] = 70; dp_Speed[2] = 70; //speed rate를 50%로 변경 한다
// 		}
// 		else
// 		{
// 			dp_Speed[0] = 100; dp_Speed[1] = 100; dp_Speed[2] = 100; //speed rate를 100%로 변경 한다
// 		}
// 	}
// 	else if(n_Axis == M_ULDRBT_X) //보간 속도 율 
// 	{
// 		dCurrentPos = COMI.Get_MotCurrentPos(n_Axis);
// 		dVal = fabs(dCurrentPos - d_TargetPos);
// 		if(dVal > 400)
// 		{
// 			dp_Speed[0] = 50; dp_Speed[1] = 50; dp_Speed[2] = 50; //speed rate를 50%로 변경 한다
// 		}		
// 		else
// 		{
// 			dp_Speed[0] = 100; dp_Speed[1] = 100; dp_Speed[2] = 100; //speed rate를 100%로 변경 한다
// 		}
// 	}
// 	else
// 	{
// 		dp_Speed[0] = st_motor[n_Axis].d_spd_vel[0]; dp_Speed[1] = st_motor[n_Axis].d_spd_vel[1]; dp_Speed[2] = st_motor[n_Axis].d_spd_vel[2]; 
// 	}
 
	return CTL_GOOD;
}


int CCtlBd_Function::Alarm_Error_Occurrence(int n_Count, int n_Type, int n_Status, CString s_JamCode)
{
      alarm.mn_count_mode = n_Count; alarm.mn_type_mode = n_Type; COMI.mn_run_status = n_Status; 
	  alarm.mstr_code = s_JamCode; 
	  sprintf(alarm.mc_code, s_JamCode);
	  
	  MyJamData.On_Alarm_Info_Set_to_Variable(alarm.mstr_code);
	  
	  if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
	  {
		  if (MOT_DEBUG)
		  {
			  sprintf(st_msg.c_abnormal_msg, "[ALARM]=> [%s] [%s] %d", alarm.mstr_code, st_alarm.mstr_cur_msg, n_Count);	// 20130916 kjh
			  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		  }
		  else
		  {
//			  st_msg.mstr_abnormal_msg.Format("[%s] [%s]", alarm.mstr_code, st_alarm.mstr_cur_msg);
			  sprintf(st_msg.c_abnormal_msg, "[%s] [%s] %d", alarm.mstr_code, st_alarm.mstr_cur_msg, n_Count);	// 20130916 kjh
			  st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		  }
	  }
	  
	return CTL_GOOD;
}

//2011.0123 일부 내용 변경 
//이곳은 알람 메세지 및 일부 내용을 파일로 저장하는 역활을 담당한다 
int CCtlBd_Function::Send_Error_Message(int n_Mode, int n_Axis, CString s_JamCode, CString s_JamMsg)
{
	long lErrorCode;
	short int nErrorParseAxis;
	short int nErrorParseReason;
	char cErrorMsg[200];
	CString sMsg, sTemp;
	
	memset(&cErrorMsg, 0, sizeof(cErrorMsg));  // 데이터 저장 변수 초기화 

	//2011.0123 아래내용 추가 if(MOT_DEBUG) Func.On_LogFile_Add(3, CtlBdFunc.ms_ErrMsg);

	//2011.0123 아래내용 추가 
	if(n_Mode == MOT_ERR_MSG) 
	{//문자 메세지를 받아 리스트 화면에 디스플레이 한다   
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
//			st_msg.mstr_abnormal_msg = s_JamMsg;
			sprintf(st_msg.c_abnormal_msg,s_JamMsg);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}		
	}
	else if(n_Mode == MOT_ERR_CODE)
	{//jamcode  설정 		
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
//			st_msg.mstr_abnormal_msg = s_JamMsg;
			sprintf(st_msg.c_abnormal_msg, s_JamMsg);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}	
	}
	else
	{//jamcode  설정 		
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
//			st_msg.mstr_normal_msg = s_JamMsg;
			sprintf(st_msg.c_normal_msg, s_JamMsg);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
		}	
	}//2011.0123 추가 







/*
	if(n_Mode == MOT_ERR_MSG) 
	{//문자 메세지를 받아 리스트 화면에 디스플레이 한다   
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			st_msg.mstr_abnormal_msg = s_JamMsg;
			st_timer.n_cmd_dialog_list_msg[ABNORMAL_MSG][0] = CTL_YES; st_timer.n_cmd_dialog_list_msg[ABNORMAL_MSG][1] = 0; //2011.0120 변경   // 동작 실패 출력 요청
		}		
	}
	else if(n_Mode == MOT_ERR_CODE)
	{//jamcode  설정 		
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
			st_msg.mstr_abnormal_msg = s_JamMsg;
			st_timer.n_cmd_dialog_list_msg[ABNORMAL_MSG][0] = CTL_YES; st_timer.n_cmd_dialog_list_msg[ABNORMAL_MSG][1] = 0; //2011.0120 변경   // 동작 실패 출력 요청
		}	
	}
*/	
	if(CTLBD_DEBUG_MODE) 
	{
		if(CTLBD_MOTBD_TYPE == 0) //파스텍 보드 
		{

		}
		else if(CTLBD_MOTBD_TYPE == 1) //커미조아 보드 
		{
			cmmErrGetLastCode(&lErrorCode);						//마지막 발생한 에러 코드 확인  
			nErrorParseAxis = cmmErrParseAxis(lErrorCode);     //에러를 발생시킨 축 정보를 가져온다 
			nErrorParseReason = cmmErrParseReason(lErrorCode); //error code를 받는다 
			cmmErrGetString(lErrorCode, cErrorMsg, 200);

			sTemp.Format("Msg=%s", cErrorMsg);
			sMsg.Format("[MSG]MotNum:%d, ECode=%d, EAxis=%d, EReason=%d, Msg[%s]", n_Axis, lErrorCode, nErrorParseAxis, nErrorParseReason, sTemp);
			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
			{
//				st_msg.mstr_abnormal_msg = sMsg;
				sprintf(st_msg.c_abnormal_msg, sMsg);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				//2011.0218 if(MOT_DEBUG) Func.On_LogFile_Add(3, st_msg.mstr_abnormal_msg);
			}	
		}
	} 
	//2011.0123 삭제 alarm.mstr_code = s_JamCode; //alarm.mn_count_mode = 0; alarm.mn_type_mode = CTL_dWARNING;
	
	return CTLBD_YES; 
}


int CCtlBd_Function::OnMotor_SafetyCheck(int n_Mode, int n_Axis, double d_TargetPos)
{
	// n_Mode = 0:Home Check, 1:Start, 2:Check, 3:Jog
	char cJamcode[10] = {NULL};
	int nRet = 0, nRet_1 = 0, nRet_2 = 0, nRet_3=0;
	double d_CurPos[3] = {0,};
	double d_Pos[3] = {0,};
	double d_GapCheck = 0;
	int i = 0;

	// **************************************************************************
	// 모터 알람 상태 검사한다                                                   
	// -> 알람 발생한 경우 알람 해제한다                                         
	// **************************************************************************

	//software limit 값 셋팅 체크 에러 
	if(d_TargetPos < st_motor[n_Axis].md_limit_position[0]) //
	{//000004
 		sprintf(cJamcode, "%02d0004", n_Axis);  
 		alarm.mstr_code = _T(cJamcode); 	alarm.mn_type_mode = CTL_dWARNING; COMI.mn_run_status = CTL_dWARNING;
 		sprintf(alarm.mc_code, "%02d0004", n_Axis);
		CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_Axis, cJamcode, CtlBdFunc.ms_ErrMsg);
		
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
//			st_msg.mstr_abnormal_msg.Format("Motor[%d] Software Limit Error_1[%4.3f] [%4.3f]", n_Axis, st_motor[n_Axis].md_limit_position[0], d_TargetPos);
			sprintf(st_msg.c_abnormal_msg, "Motor[%d] Software Limit Error_1[%4.3f] [%4.3f]", n_Axis, st_motor[n_Axis].md_limit_position[0], d_TargetPos);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}		
		return CTL_ERROR;
	}	

	if(d_TargetPos > st_motor[n_Axis].md_limit_position[1]) //
	{
 		sprintf(cJamcode, "%02d0004", n_Axis);  
 		alarm.mstr_code = _T(cJamcode); 	alarm.mn_type_mode = CTL_dWARNING; COMI.mn_run_status = CTL_dWARNING;
 		sprintf(alarm.mc_code, "%02d0004", n_Axis);

		CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_Axis, cJamcode, CtlBdFunc.ms_ErrMsg);
		
		if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
		{
//			st_msg.mstr_abnormal_msg.Format("Motor[%d] Software Limit Error_2[%4.3f] [%4.3f]", n_Axis, st_motor[n_Axis].md_limit_position[1], d_TargetPos);
			sprintf(st_msg.c_abnormal_msg, "Motor[%d] Software Limit Error_2[%4.3f] [%4.3f]", n_Axis, st_motor[n_Axis].md_limit_position[1], d_TargetPos);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
		}
		return CTL_ERROR;
	}

	// **************************************************************************
	// 모터 파워 상태 검사한다                                                   
	// -> 모터 POWER OFF 시 POWER ON 상태로 만든다                               
	// **************************************************************************
	if (COMI.Get_MotPower(n_Axis) == CTL_ERROR )          // 모터 POWER 상태 검사 함수 
	{
		if (COMI.Set_MotPower(n_Axis, CTL_ON) == CTLBD_RET_GOOD)       // 모터 POWER ON 설정 함수 
		{
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[MotorPowerOn_1] Axis=%d, rcnt=%d", n_Axis, st_motor[n_Axis].mn_retry_cnt);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}

			// 2014.11.29 - Bredmin.
			CString strTemp = "";
			strTemp.Format( "[MotorPowerOn_1] Axis=%d, rcnt=%d", n_Axis, st_motor[n_Axis].mn_retry_cnt );
			Func.On_LogFile_Add( LOG_TOTAL, strTemp );

			if (COMI.Get_MotPower(n_Axis) == CTL_ERROR)  // 모터 POWER 상태 검사 함수 
			{
				if (st_motor[n_Axis].mn_retry_cnt > (MOT_RTY_CNT))
				{
					//sprintf(cJamcode, "%02d0005",  n_Axis); 
					sprintf(cJamcode, "0%02d0005",  n_Axis);		// 2014.12.16 - Bredmin.
					alarm.mstr_code = _T(cJamcode); //091216 	alarm.mn_count_mode = MOT_DEBUG;	
					//sprintf(alarm.mc_code, "%02d0005", n_Axis);
					sprintf(alarm.mc_code, "0%02d0005", n_Axis);		// 2014.12.16 - Bredmin.
					CtlBdFunc.ms_ErrMsg.Format("[Safety] Mot[%d] rcnt[%d] Get Power Check Error", n_Axis, st_motor[n_Axis].mn_retry_cnt);				
					CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_Axis, cJamcode, CtlBdFunc.ms_ErrMsg);

					//2011.0210 st_motor[n_Axis].mn_retry_cnt = 0;
					return CTL_ERROR;
				}
				else
				{
					st_motor[n_Axis].mn_retry_cnt++ ;
					return CTL_RETRY;
				}
			}
		}
		else
		{
			if (st_motor[n_Axis].mn_retry_cnt > MOT_RTY_CNT)
			{
				//sprintf(cJamcode, "%02d0005",  n_Axis);		// 2014.12.16 - Bredmin.
				sprintf(cJamcode, "0%02d0005",  n_Axis); 
				alarm.mstr_code = _T(cJamcode); //091216 	alarm.mn_count_mode = MOT_DEBUG;	
				//sprintf(alarm.mc_code, "%02d0005", n_Axis);
				sprintf(alarm.mc_code, "0%02d0005", n_Axis);		// 2014.12.16 - Bredmin.
				CtlBdFunc.ms_ErrMsg.Format("[Safety] Mot[%d] rcnt[%d] Set Power return Error", n_Axis, st_motor[n_Axis].mn_retry_cnt);				
				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_Axis, cJamcode, CtlBdFunc.ms_ErrMsg);
				//2011.0210 st_motor[n_Axis].mn_retry_cnt = 0;
				return CTL_ERROR;
			}
			else
			{
				st_motor[n_Axis].mn_retry_cnt++ ;
				return CTL_RETRY;
			}
		}
	}

	//모터 알람 발생시 처리 조건 
	if (COMI.Get_MotAlarmStatus(n_Axis) == CTL_ERROR)      // 모터 ALARM 상태 검사 함수 
	{
		if (COMI.Set_MotAlarmClear(n_Axis) == CTL_GOOD)       // 모터 ALARM CLEAR 함수 
		{
			//091119 james			Sleep(1000);  // 일정 시간 후에 상태 확인하기 위해 SLEEP 사용한다 
// 			if (st_handler.cwnd_list != NULL)  // 리스트 바 화면 존재
// 			{
// 				sprintf(st_msg.c_normal_msg, "[Alarm Reset] Axis=%d, rcnt=%d", n_Axis, st_motor[n_Axis].mn_retry_cnt);
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
// 			}
			// 2014.11.29 - Bredmin.
			CString strTemp = "";
			strTemp.Format( "[Alarm Reset] Axis=%d, rcnt=%d", n_Axis, st_motor[n_Axis].mn_retry_cnt );
			Func.On_LogFile_Add( LOG_TOTAL, strTemp );

			if (COMI.Get_MotAlarmStatus(n_Axis) == CTL_ERROR )  // 모터 ALARM 상태 검사 함수 
			{
				if (st_motor[n_Axis].mn_retry_cnt > MOT_RTY_CNT)
				{			
					//sprintf(cJamcode, "%02d0002",  n_Axis);		// 2014.12.16 - Bredmin.
					sprintf(cJamcode, "0%02d0005",  n_Axis); 
					alarm.mstr_code = _T(cJamcode); //091216 	alarm.mn_count_mode = MOT_DEBUG;	
					//sprintf(alarm.mc_code, "%02d0002", n_Axis);
					sprintf(alarm.mc_code, "0%02d0005", n_Axis);		// 2014.12.16 - Bredmin.
					CtlBdFunc.ms_ErrMsg.Format("[Safety] Mot[%d] rcnt[%d] Get Alarm Check Error", n_Axis, st_motor[n_Axis].mn_retry_cnt);				
					CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_Axis, cJamcode, CtlBdFunc.ms_ErrMsg);
					//2011.0210 st_motor[n_Axis].mn_retry_cnt = 0;  // 알람 해제 시도 횟수 
					return CTL_ERROR;
				}
				else
				{
					st_motor[n_Axis].mn_retry_cnt++ ;
					return CTL_RETRY;
				}
			}
		}
		else
		{
			if (st_motor[n_Axis].mn_retry_cnt > MOT_RTY_CNT)
			{
				//sprintf(cJamcode, "%02d0002",  n_Axis);		// 2014.12.16 - Bredmin.
				sprintf(cJamcode, "0%02d0005",  n_Axis); 
				alarm.mstr_code = _T(cJamcode); //091216 	alarm.mn_count_mode = MOT_DEBUG;	
				//sprintf(alarm.mc_code, "%02d0002", n_Axis);
				sprintf(alarm.mc_code, "0%02d0005", n_Axis);		// 2014.12.16 - Bredmin.
				CtlBdFunc.ms_ErrMsg.Format("[Safety] Mot[%d] rcnt[%d] Set Alarm Return Error", n_Axis, st_motor[n_Axis].mn_retry_cnt);				
				CtlBdFunc.Send_Error_Message(MOT_ERR_CODE, n_Axis, cJamcode, CtlBdFunc.ms_ErrMsg);
				
				//2011.0210 st_motor[n_Axis].mn_retry_cnt = 0;  // 알람 해제 시도 횟수 
				return CTL_ERROR;
			}
			else
			{
				st_motor[n_Axis].mn_retry_cnt++ ;
				return CTL_RETRY;
			}
		}
	}
	// **************************************************************************

	// **************************************************************************
	// 모터의 안전 위치를 체크한다                                                 
	//***************************************************************************
	switch(n_Axis)
	{
	case M_EPOXY_TRANSFER_X:
		break;

	}

	//2011.0201 st_motor[n_Axis].mn_retry_cnt = 0;
	st_motor[n_Axis].mn_retry_time_flag = CTL_NO;//2011.0201 추가 
	return CTLBD_RET_GOOD;
}


int CCtlBd_Function::Initialize_FASIO_Board(void) 
{
// 	int m=0, p=0, s=0, i;
// 	int nRet=0;
// 	WORD wMaskMap = 0;
// 
// 	INT nHSSI_speed=0;
// 	INT nMasterNo=0, nPortNo =0, nSlaveNo=0; 
// 
// 	nRet = g_ioMgr.Set_IO_BoardOpen(&nMasterNo, START_NOTHING);  // I/O 보드 오픈 함수 
// 	if (nRet == CTLBD_RET_ERROR)
// 	{
// 		return CTLBD_RET_ERROR; //IO_BOARD_OPEN_FAILED;
// 	}
// 
// 	nRet = g_ioMgr.Search_IO_Master(&nMasterNo);              // PC에 연결된 마스터 보드 갯수 검사 함수 
// 	if (nRet == CTLBD_RET_ERROR)
// 	{
// 		return CTLBD_RET_ERROR;
// 	}
// 	
// 	for(m=0; m<nMasterNo; m++)
// 	{
// 		nRet = g_ioMgr.Search_IO_Port(m, &nPortNo);						// 마스터 보드에 연결된 PORT 갯수 검사 함수 
// 		if (nRet == CTLBD_RET_ERROR) return CTLBD_RET_ERROR;	//PORT_SEARCH_FAILED;
// 		
// 		for(p=0; p<nPortNo; p++)
// 		{
// 			nRet = g_ioMgr.Set_IO_HSSISpeed(m, p, PORT_SPEED_10);		// PORT와 SLAVE I/O 모듈과의 통신 속도 설정 함수 
// 			if (nRet == CTLBD_RET_ERROR) return CTLBD_RET_ERROR; //IO_SPEED_SET_FAILED;
// 			 
// 			nRet = g_ioMgr.Search_IO_Slave(m, p, &nSlaveNo);			// PORT에 연결된 SLAVE 검사 함수 
// 			if (nRet == CTLBD_RET_ERROR) return CTLBD_RET_ERROR; //IO_SLAVE_SEARCH_FAILED;
// 
// 			for(s=0; s<nSlaveNo; s++)
// 			{
// 				nRet = g_ioMgr.Set_IO_SlaveEnable(m, p, s, CTLBD_YES);		// SLAVE I/O 모듈 동작 ENABLE/DISABLE 설정 함수 
// 				if (nRet == CTLBD_RET_ERROR)	return CTLBD_RET_ERROR; //IO_SLAVE_ENABLE_FAILED;
// 
// 				wMaskMap = 0; //clear
// 				for(i=0; i<16; i++) //0~15 까지의 Port 16bit 셋팅, 각각의 I/O 상태를 정의한다 
// 				{			
// 					wMaskMap = wMaskMap | (WORD)pow(2, mw_IOMaskMap[m][p][s][i]);					 
// 				}
// 				g_ioMgr.Set_IO_DefineWord(m, p, s, wMaskMap); // x Master, x포트, x번 슬레이브 , 16bit 셋팅
// 				//g_ioMgr.Set_IO_DefineWord(0, 0, 0, 0x0003); // 0번 Master, 0번 포트, 0번 슬레이브 , 16bit 셋팅
// 				//g_ioMgr.Set_IO_DefineWord(0, 0, 1, 0x0003); // 0번 Master, 0번 포트, 1번 슬레이브 , 16bit 셋팅
// 				//g_ioMgr.Set_IO_DefineWord(0, 1, 0, 0x00ff); // 0번 Master, 1번 포트, 0번 슬레이브 , 16bit 셋팅
// 			}
// 		}
// 	}
// 	
	return CTLBD_RET_GOOD;
}



int CCtlBd_Function::Initialize_InterruptMask_TriggerConfig(void) 
{
	int i, nAxis;
//	st_motor[0].ml_Set_InterruptMask[12] = CTL_YES;
//	COMI.Set_InterruptMask(0, st_motor[0].l_Set_InterruptMask);
	
//	double dRatio[3] = {100,100,100};
	//Set_TriggerSet_OneCompareConfig(int n_Axis, int n_Source, int n_Method, double d_ComparePosition)
//	COMI.Set_TriggerSet_OneCompareConfig(0, 0, 1, 5000);


//	st_motor[].mn_interrupt_flag

/*
	"Normal Stop", // 0
	"Succesive start", // 1
	"", // 2
	"", // 3
	"Start of acceleration", //4
	"End of acceleration", // 5
	"Start of deceleration", // 6
	"End of deceleration", // 7
	"", // 8
	"", // 9
	"Position error tolerance exceed", // 10
	"General Comparator", // 11
	"Compared trigger", // 12
	"CLR signal resets the counter value", // 13
	"LTC input making counter value latched", // 14
	"ORG input signal ON", // 15
	"SD input signal ON", // 16
	"+DR input changes", // 17
	"-DR input changes" // 18

int		n_interrupt_source;  //인터럽트 처리 조건 
	    //n_Mode : 	szCompareSource[4][50] = { 
		//0				"COMMAND Counter",
		//1				"Feedback Counter",
		//2			    "Deviation Counter",
		//3			    "General Counter"

	int		n_interrupt_method; //사용하는 조건 정의  
		//CHAR szCompareMethod[6][50] = {
		//0				"Disable",
		//1				"CMP == COUNTER (No direction)",
		//2				"CMP == COUNTER (Only when counting up)",
		//3				"CMP == COUNTER (Only when counting down)",
		//4				"CMP > COUNTER",
		//5				"CMP < COUNTER"
*/	
	for(i=0; i<5; i++)	//for(nAxis=0; nAxis<MOT_MAXMOTOR; nAxis++)
	{
		if(i == 0)
		{
			nAxis = 0; 
			st_motor[nAxis].mn_interrupt_flag = CTL_YES; //dc load robot Y 무조건 설정 
		}
		else if(i == 1)
		{
			nAxis = 5;
			st_motor[nAxis].mn_interrupt_flag = CTL_YES; //dc unload robot Y 무조건 설정
		}
		else if(i == 2) 
		{
			nAxis = 16;	
			st_motor[nAxis].mn_interrupt_flag = CTL_YES; //insert robot Y 무조건 설정
		}
		else if(i == 3)
		{
			nAxis = 19;		//2009.5.27 james 소켓 푸쉬 추가 
			//st_motor[nAxis].mn_interrupt_flag = CTL_YES; //소켓 푸쉬 무조건 설정
		}
		else if(i == 4)
		{
			nAxis = 20;	
			st_motor[nAxis].mn_interrupt_flag = CTL_YES; //remove robot Y 무조건 설정
		}

		if(st_motor[nAxis].mn_interrupt_flag == CTL_YES) //인터럽트를 사용하는 모터이면 
		{
			st_motor[nAxis].ml_Set_InterruptMask[0] = CTL_NO; //"Normal Stop", // 0
			st_motor[nAxis].ml_Set_InterruptMask[1] = CTL_NO; //"Succesive start", // 1
			st_motor[nAxis].ml_Set_InterruptMask[2] = CTL_NO; //"", // 2
			st_motor[nAxis].ml_Set_InterruptMask[3] = CTL_NO; //"", // 3
			st_motor[nAxis].ml_Set_InterruptMask[4] = CTL_NO; //"Start of acceleration", //4
			st_motor[nAxis].ml_Set_InterruptMask[5] = CTL_NO; //End of acceleration", // 5
			st_motor[nAxis].ml_Set_InterruptMask[6] = CTL_NO; //"Start of deceleration", // 6
			st_motor[nAxis].ml_Set_InterruptMask[7] = CTL_NO; //"End of deceleration", // 7
			st_motor[nAxis].ml_Set_InterruptMask[8] = CTL_NO; //"", // 8
			st_motor[nAxis].ml_Set_InterruptMask[9] = CTL_NO; //"", // 9
			st_motor[nAxis].ml_Set_InterruptMask[10] = CTL_NO; //"Position error tolerance exceed", // 10
			st_motor[nAxis].ml_Set_InterruptMask[11] = CTL_NO; //"General Comparator", // 11

			//이것만 우선 사용한다 
			st_motor[nAxis].ml_Set_InterruptMask[12] = CTL_YES; //"Compared trigger", // 12

			st_motor[nAxis].ml_Set_InterruptMask[13] = CTL_NO; //"CLR signal resets the counter value", // 13
			st_motor[nAxis].ml_Set_InterruptMask[14] = CTL_NO; //"LTC input making counter value latched", // 14
			st_motor[nAxis].ml_Set_InterruptMask[15] = CTL_NO; //"ORG input signal ON", // 15
			st_motor[nAxis].ml_Set_InterruptMask[16] = CTL_NO; //"SD input signal ON", // 16
			st_motor[nAxis].ml_Set_InterruptMask[17] = CTL_NO; //"+DR input changes", // 17
			st_motor[nAxis].ml_Set_InterruptMask[18] = CTL_NO; //"-DR input changes" // 18

			//인터럽트 마스키 정보를 셋팅한다 
			//Set_InterruptMask(int n_Axis, long l_Set_InterruptMask[19])
			COMI.Set_InterruptMask(nAxis, st_motor[nAxis].ml_Set_InterruptMask);

			//인터럽트 발생 조건 및 구간 설정 
			//Set_TriggerSet_OneCompareConfig(int n_Axis, int n_Source, int n_Method, double d_ComparePosition)
			COMI.Set_TriggerSet_OneCompareConfig(nAxis, st_motor[nAxis].mn_interrupt_source, st_motor[nAxis].mn_interrupt_method, st_motor[nAxis].mn_interrupt_position);
		}
	}		
	return CTL_GOOD;
}
