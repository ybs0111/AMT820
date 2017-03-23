// stdafx.cpp : source file that includes just the standard includes
//	HANDLER.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

void DoEvents()
{
	MSG msg;
	
	// window message         
	while (PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))         
	{            
		TranslateMessage(&msg);            
		DispatchMessage(&msg);         
	}      
}

/* ****************************************************************************** */
/* ��ȭ ���ڿ� ���� OK ��ư ���� �Լ�                                             */
/* ****************************************************************************** */
CButton *CreateOkButton(int parm_resource_id, CWnd *parm_parent)
{
	CButton *p_button_object = CreateSubClassingTipsButtonObject(parm_resource_id, parm_parent);

	SetTipsButtonOneProperty(p_button_object, TNB_SET_TEXT_COLOR, NULL, RGB(0, 0, 128), RGB(0, 0, 0));
	SetTipsButtonOneProperty(p_button_object, TNB_SET_BORDER_COLOR, NULL, RGB(0, 64, 192), RGB(0, 0, 0));
    //SetTipsButtonOneProperty(p_button_object, TNB_SET_BITMAP, ".\\image\\����.bmp", 1, RGB(221, 221, 221));
	SetTipsButtonOneProperty(p_button_object, TNB_SET_IMAGE_REVISION, NULL, 5, -1);
	SetTipsButtonOneProperty(p_button_object, TNB_SET_TEXT_REVISION, NULL, -6, 0);

	/*
	CButton *p_button_object = CreateSubClassingTipsButtonObject(parm_resource_id, parm_parent);

    //SetTipsButtonOneProperty(p_button_object, TNB_SET_BITMAP, ".\\image\\�˻�.bmp", 0, NULL);
	SetTipsButtonOneProperty(p_button_object, TNB_SET_IMAGE_REVISION, NULL, 4, -1);
	*/

	return p_button_object;
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* ��ȭ ���ڿ� ���� Cancel ��ư ���� �Լ�                                         */
/* ****************************************************************************** */
CButton *CreateCancelButton(int parm_resource_id, CWnd *parm_parent)
{
	CButton *p_button_object = CreateSubClassingTipsButtonObject(parm_resource_id, parm_parent);

	SetTipsButtonOneProperty(p_button_object, TNB_SET_TEXT_COLOR, NULL, RGB(0, 0, 128), RGB(0, 0, 0));
	SetTipsButtonOneProperty(p_button_object, TNB_SET_BORDER_COLOR, NULL, RGB(0, 64, 192), RGB(0, 0, 0));
    //SetTipsButtonOneProperty(p_button_object, TNB_SET_BITMAP, ".\\image\\����.bmp", 1, RGB(221, 221, 221));
	SetTipsButtonOneProperty(p_button_object, TNB_SET_IMAGE_REVISION, NULL, 5, -1);
	SetTipsButtonOneProperty(p_button_object, TNB_SET_TEXT_REVISION, NULL, -6, 0);

	/*
	CButton *p_button_object = CreateSubClassingTipsButtonObject(parm_resource_id, parm_parent);

    //SetTipsButtonOneProperty(p_button_object, TNB_SET_BITMAP, ".\\image\\����.bmp", 0, NULL);
	SetTipsButtonOneProperty(p_button_object, TNB_SET_IMAGE_REVISION, NULL, 4, -1);
	*/

	return p_button_object;
}
/* ****************************************************************************** */

/* ****************************************************************************** */
/* ��ȭ ���ڿ� ���� SAVE OK ��ư ���� �Լ�                                        */
/* ****************************************************************************** */
CButton *CreateSaveOkButton(int parm_resource_id, CWnd *parm_parent)
{
	CButton *p_button_object = CreateSubClassingTipsButtonObject(parm_resource_id, parm_parent);

	SetTipsButtonOneProperty(p_button_object, TNB_SET_TEXT_COLOR, NULL, RGB(0, 0, 128), RGB(0, 0, 0));
	SetTipsButtonOneProperty(p_button_object, TNB_SET_BORDER_COLOR, NULL, RGB(0, 64, 192), RGB(0, 0, 0));
	SetTipsButtonOneProperty(p_button_object, TNB_SET_IMAGE_REVISION, NULL, 5, -1);
	SetTipsButtonOneProperty(p_button_object, TNB_SET_TEXT_REVISION, NULL, -6, 0);

	return p_button_object;
}
/* ****************************************************************************** */


