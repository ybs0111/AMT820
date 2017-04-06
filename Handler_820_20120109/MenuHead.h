// Copyright (C) 2004 AMT Corporation
// All rights reserved.
//
// DIVISION    : A.M.T. Coporation
// MODIFIED BY : ViboX Kim
// REVISION    :
//     2004. 05. 01 : Modified by ViboX Kim
//

#ifndef _STRUCT_H_
#define _STRUCT_H_

/* ***************************************************************************** */
/* 화면 뷰 클래스 추가 부분                                                      */
/* ***************************************************************************** */
#include "Screen_Main.h"				// 메인 화면 클래스 추가 
#include "Screen_Lock.h"				// System Locking 화면 클래스 추가 
#include "Screen_Initial.h"				// 초기화 진행 화면 클래스 추가 
#include "Screen_List_Alarm.h"			// 알람 텍스트 리스트 출력 화면 클래스 추가 
#include "Screen_List_Work.h"			// 디바이스 테스트 결과 텍스트 리스트 출력 화면 클래스 추가 
#include "Screen_List_Operation.h"		// Operation 정보 출력 화면 클래스 추가 
#include "Screen_List_Step.h"			// 쓰레드 스텝 정보 출력 화면 클래스 추가 

//#include "Screen_IO.h"					// I/O 상태 정보 출력 화면 클래스 추가 

#include "Screen_Administrator.h"		// Administrator 출력 화면 클래스 추가 
/* ***************************************************************************** */

/* ***************************************************************************** */
/* 화면 뷰 클래스 ID 설정 부분                                                   */
/* ***************************************************************************** */
#define IDW_SCREEN_MAIN					101
#define IDW_SCREEN_LOCK					102
#define IDW_SCREEN_INIT					103
	
#define IDW_SCREEN_BASIC		 		201

#define IDW_SCREEN_WORK_INFO			301
#define IDW_SCREEN_SET_MAINTENANCE		302
#define IDW_SCREEN_SET_INTERFACE		303
#define IDW_SCREEN_SET_MEASURE			304
#define IDW_SCREEN_WAIT_TIME			401

#define IDW_SCREEN_MOTOR				501
#define IDW_SCREEN_MOTOR_SPEED			502


#define IDW_SCREEN_LIST_OPERATION		601
#define IDW_SCREEN_LIST_WORK			602
#define IDW_SCREEN_LIST_ALARM			603
#define IDW_SCREEN_LIST_STEP			605
#define IDW_SCREEN_LIST_ERROR			606
#define IDW_SCREEN_LIST_DATA_VIEW		607
#define IDW_SCREEN_LIST_WAIT			608 //kwlee 2017.0406


#define IDW_SCREEN_IO					701

#define IDW_SCREEN_ADMINISTRATOR		801

#define IDW_SCREEN_CAMERA				901

#define IDW_SCREEN_SETTING				1500
#define IDW_SCREEN_IO_MAP				1601

#define IDW_SCREEN_PGM_INFO				1700

/* ***************************************************************************** */

#endif