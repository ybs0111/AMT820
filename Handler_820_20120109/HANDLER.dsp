# Microsoft Developer Studio Project File - Name="HANDLER" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=HANDLER - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HANDLER.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HANDLER.mak" CFG="HANDLER - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HANDLER - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "HANDLER - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HANDLER - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 PIODA.LIB TipsWareExt.lib AxtLib.lib MmdSerial.lib HSSI.lib DataAPI.LIB netapi32.lib ijl15l.lib Spr32d70.lib KeyboardDll.lib VkDll.lib wininet.lib libmysql.lib /nologo /stack:0x1999999 /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "HANDLER - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 PIODA.LIB TipsWareExt.lib AxtLib.lib MmdSerial.lib HSSI.lib DataAPI.LIB netapi32.lib ijl15l.lib Spr32d70.lib KeyboardDll.lib VkDll.lib wininet.lib libmysql.lib /nologo /stack:0x1999999 /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "HANDLER - Win32 Release"
# Name "HANDLER - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "SingleApp_Cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SingleInstanceApp.cpp
# End Source File
# End Group
# Begin Group "InitDlgBar_Cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\InitDialogBar.cpp
# End Source File
# End Group
# Begin Group "ToolBar_Cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TextToolBar.cpp
# End Source File
# End Group
# Begin Group "NewLabel_Cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ControlToolTip.cpp
# End Source File
# Begin Source File

SOURCE=.\EXDigitST.cpp
# End Source File
# Begin Source File

SOURCE=.\FloatST.cpp
# End Source File
# Begin Source File

SOURCE=.\GradientStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\NewLabel.cpp
# End Source File
# End Group
# Begin Group "Clock_Cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClockST.cpp
# End Source File
# End Group
# Begin Group "ColorListBox_Cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ColorListBox.cpp
# End Source File
# End Group
# Begin Group "NewButton_Cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\CeXDib.cpp
# End Source File
# Begin Source File

SOURCE=.\LedButton.cpp
# End Source File
# Begin Source File

SOURCE=.\MacButtons.cpp
# End Source File
# Begin Source File

SOURCE=.\UniButtonEx.cpp
# End Source File
# Begin Source File

SOURCE=.\XPButton.cpp
# End Source File
# End Group
# Begin Group "ListBox_Cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ListBoxST.cpp
# End Source File
# End Group
# Begin Group "GIF_Cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PictureEx.cpp
# End Source File
# End Group
# Begin Group "StarWars_Cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\StarWarsCtrl.cpp
# End Source File
# End Group
# Begin Group "GroupBox_Cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XPGroupBox.cpp
# End Source File
# End Group
# Begin Group "Edit_Cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\editex.cpp
# End Source File
# End Group
# Begin Group "ProgressBar_Cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SuperProgressCtrl.cpp
# End Source File
# End Group
# Begin Group "TreeControl_Cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TreeOptionsCtrl.cpp
# End Source File
# End Group
# Begin Group "GridCtrl_Cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GridCell.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCellBase.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCellCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCellCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCellDateTime.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCellNumeric.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GridDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\GridURLCell.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\TitleTip.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Alg_Mysql.cpp
# End Source File
# Begin Source File

SOURCE=.\AlgMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\AMTLotManager.cpp
# End Source File
# Begin Source File

SOURCE=.\AMTRegistry.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapSlider.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpImage.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpImageST.cpp
# End Source File
# Begin Source File

SOURCE=.\CharSetCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckerCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\Cmmsdk.cpp
# End Source File
# Begin Source File

SOURCE=.\COMI_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Comidas.cpp
# End Source File
# Begin Source File

SOURCE=.\ComizoaPublic.cpp
# End Source File
# Begin Source File

SOURCE=.\Comm.cpp
# End Source File
# Begin Source File

SOURCE=.\CtlBd_Function.cpp
# End Source File
# Begin Source File

SOURCE=.\CtlBd_Library.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_About.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Alarm.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Color.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Data_Lot.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Event_Msg.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Exit.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Infor.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Keyboard.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_KeyPad.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_List.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_LotStart.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Manual_Move.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Memo.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Menu.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Message.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Motor_Axis_1.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Motor_Axis_Tab.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Motor_Part.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Part_IO.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Pass_Change.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Pass_Check.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Select.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Select_Retest.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_SelectDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog_Title.cpp
# End Source File
# Begin Source File

SOURCE=.\Digit.cpp
# End Source File
# Begin Source File

SOURCE=.\FastechPublic_IO.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDialogST.cpp
# End Source File
# Begin Source File

SOURCE=.\FPSDatePickerCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FPSDateTimeButtonCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FPSDateTimeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FPSDateTimePopupCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FPSMiniCalendarCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FPSMiniCalendarListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GridControlAlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HANDLER.cpp
# End Source File
# Begin Source File

SOURCE=.\HANDLER.rc
# End Source File
# Begin Source File

SOURCE=.\HANDLERDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\HANDLERView.cpp
# End Source File
# Begin Source File

SOURCE=.\InterfaceBarcode.cpp
# End Source File
# Begin Source File

SOURCE=.\IO_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\IOBoardLibrary.cpp
# End Source File
# Begin Source File

SOURCE=.\LogFromat.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MatrixStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\MyBasicData.cpp
# End Source File
# Begin Source File

SOURCE=.\MyInternetSession.cpp
# End Source File
# Begin Source File

SOURCE=.\MyJamData.cpp
# End Source File
# Begin Source File

SOURCE=.\OleDateTimeEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Picture.cpp
# End Source File
# Begin Source File

SOURCE=.\PPTooltip.cpp
# End Source File
# Begin Source File

SOURCE=.\PreviewRect.cpp
# End Source File
# Begin Source File

SOURCE=.\Public_Function.cpp
# End Source File
# Begin Source File

SOURCE=.\Run_Device_Carrier_Robot.cpp
# End Source File
# Begin Source File

SOURCE=.\Run_DvcLdBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Run_EmptyTrayTransfer.cpp
# End Source File
# Begin Source File

SOURCE=.\Run_Epoxy_Transfer_Robot.cpp
# End Source File
# Begin Source File

SOURCE=.\Run_Handler_Check.cpp
# End Source File
# Begin Source File

SOURCE=.\Run_HeatSinkVision_Transfer_Robot.cpp
# End Source File
# Begin Source File

SOURCE=.\Run_IO_Check.cpp
# End Source File
# Begin Source File

SOURCE=.\Run_LdPicker.cpp
# End Source File
# Begin Source File

SOURCE=.\Run_LdStacker_Elvator.cpp
# End Source File
# Begin Source File

SOURCE=.\Run_LdTrayPlate.cpp
# End Source File
# Begin Source File

SOURCE=.\Run_Motors.cpp
# End Source File
# Begin Source File

SOURCE=.\Run_TowerLamp.cpp
# End Source File
# Begin Source File

SOURCE=.\Run_UldPicker.cpp
# End Source File
# Begin Source File

SOURCE=.\Run_UnPress_Robot.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Administrator.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Basic.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Initial.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_IO_Map.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_List_Alarm.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_List_Error.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_List_Operation.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_List_Step.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_List_Work.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Lock.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Main.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Motor.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Motor_Speed.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Pgm_Info.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Set_Maintenance.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Set_Recipe.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen_Wait_Time.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerItem.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticButton.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\TrueColorToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\VLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\xShadeButton.cpp
# End Source File
# Begin Source File

SOURCE=.\xSkinButton.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "SingleApp_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SingleInstanceApp.h
# End Source File
# End Group
# Begin Group "NewDlg_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\tw_step.h
# End Source File
# End Group
# Begin Group "InitDlgBar_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\InitDialogBar.h
# End Source File
# End Group
# Begin Group "ToolBar_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TextToolBar.h
# End Source File
# End Group
# Begin Group "NewLabel_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ControlToolTip.h
# End Source File
# Begin Source File

SOURCE=.\EXDigitST.h
# End Source File
# Begin Source File

SOURCE=.\FloatST.h
# End Source File
# Begin Source File

SOURCE=.\GradientStatic.h
# End Source File
# Begin Source File

SOURCE=.\NewLabel.h
# End Source File
# End Group
# Begin Group "Clock_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ClockST.h
# End Source File
# End Group
# Begin Group "ColorListBox_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ColorListBox.h
# End Source File
# End Group
# Begin Group "NewButton_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\CeXDib.h
# End Source File
# Begin Source File

SOURCE=.\LedButton.h
# End Source File
# Begin Source File

SOURCE=.\MacButtons.h
# End Source File
# Begin Source File

SOURCE=.\UniButtonEx.h
# End Source File
# Begin Source File

SOURCE=.\XPButton.h
# End Source File
# End Group
# Begin Group "ListBox_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ListBoxST.h
# End Source File
# End Group
# Begin Group "GIF_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PictureEx.h
# End Source File
# End Group
# Begin Group "StarWars_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\StarWarsCtrl.h
# End Source File
# End Group
# Begin Group "GroupBox_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XPGroupBox.h
# End Source File
# End Group
# Begin Group "Edit_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\editex.h
# End Source File
# End Group
# Begin Group "ProgressBar_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SuperProgressCtrl.h
# End Source File
# End Group
# Begin Group "TreeControl_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TreeOptionsCtrl.h
# End Source File
# End Group
# Begin Group "GridCtrl_H"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GridCell.h
# End Source File
# Begin Source File

SOURCE=.\GridCellBase.h
# End Source File
# Begin Source File

SOURCE=.\GridCellCheck.h
# End Source File
# Begin Source File

SOURCE=.\GridCellCombo.h
# End Source File
# Begin Source File

SOURCE=.\GridCellDateTime.h
# End Source File
# Begin Source File

SOURCE=.\GridCellNumeric.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GridDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\GridURLCell.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\TitleTip.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Alg_Mysql.h
# End Source File
# Begin Source File

SOURCE=.\AlgMemory.h
# End Source File
# Begin Source File

SOURCE=.\AMTLotManager.h
# End Source File
# Begin Source File

SOURCE=.\AMTRegistry.h
# End Source File
# Begin Source File

SOURCE=.\BitmapSlider.h
# End Source File
# Begin Source File

SOURCE=.\BmpImage.h
# End Source File
# Begin Source File

SOURCE=.\BmpImageST.h
# End Source File
# Begin Source File

SOURCE=.\CharSetCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CheckerCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ClientSocket.h
# End Source File
# Begin Source File

SOURCE=.\Cmmsdk.h
# End Source File
# Begin Source File

SOURCE=.\COMI_Manager.h
# End Source File
# Begin Source File

SOURCE=.\Comidas.h
# End Source File
# Begin Source File

SOURCE=.\ComidasCommon.h
# End Source File
# Begin Source File

SOURCE=.\ComizoaPublic.h
# End Source File
# Begin Source File

SOURCE=.\Comm.h
# End Source File
# Begin Source File

SOURCE=.\CtlBd_Function.h
# End Source File
# Begin Source File

SOURCE=.\CtlBd_Library.h
# End Source File
# Begin Source File

SOURCE=.\Ctlbd_Variable.h
# End Source File
# Begin Source File

SOURCE=.\DataAPI.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_About.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Alarm.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Color.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Data_Lot.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Event_Msg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Exit.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Infor.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Keyboard.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_KeyPad.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_List.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_LotStart.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Manual_Move.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Memo.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Menu.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Message.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Motor_Axis_1.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Motor_Axis_Tab.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Motor_Part.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Part_IO.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Pass_Change.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Pass_Check.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Select.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Select_Retest.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_SelectDevice.h
# End Source File
# Begin Source File

SOURCE=.\Dialog_Title.h
# End Source File
# Begin Source File

SOURCE=.\Digit.h
# End Source File
# Begin Source File

SOURCE=.\FAS_HSSI.h
# End Source File
# Begin Source File

SOURCE=.\FastechPublic_IO.h
# End Source File
# Begin Source File

SOURCE=.\FileDialogST.h
# End Source File
# Begin Source File

SOURCE=.\FPSDatePickerCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FPSDateTimeButtonCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FPSDateTimeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FPSDateTimePopupCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FPSMiniCalendarCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FPSMiniCalendarListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GridControlAlg.h
# End Source File
# Begin Source File

SOURCE=.\HANDLER.h
# End Source File
# Begin Source File

SOURCE=.\HANDLERDoc.h
# End Source File
# Begin Source File

SOURCE=.\HANDLERView.h
# End Source File
# Begin Source File

SOURCE=.\InterfaceBarcode.h
# End Source File
# Begin Source File

SOURCE=.\IO_Manager.h
# End Source File
# Begin Source File

SOURCE=.\IOBoardLibrary.h
# End Source File
# Begin Source File

SOURCE=.\LogFromat.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MatrixStatic.h
# End Source File
# Begin Source File

SOURCE=.\MenuHead.h
# End Source File
# Begin Source File

SOURCE=.\MyBasicData.h
# End Source File
# Begin Source File

SOURCE=.\MyInternetSession.h
# End Source File
# Begin Source File

SOURCE=.\MyJamData.h
# End Source File
# Begin Source File

SOURCE=.\OleDateTimeEx.h
# End Source File
# Begin Source File

SOURCE=.\Picture.h
# End Source File
# Begin Source File

SOURCE=.\PPTooltip.h
# End Source File
# Begin Source File

SOURCE=.\PreviewRect.h
# End Source File
# Begin Source File

SOURCE=.\Public_Function.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Run_Device_Carrier_Robot.h
# End Source File
# Begin Source File

SOURCE=.\Run_DvcLdBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Run_EmptyTrayTransfer.h
# End Source File
# Begin Source File

SOURCE=.\Run_Epoxy_Transfer_Robot.h
# End Source File
# Begin Source File

SOURCE=.\Run_Handler_Check.h
# End Source File
# Begin Source File

SOURCE=.\Run_HeatSinkVision_Transfer_Robot.h
# End Source File
# Begin Source File

SOURCE=.\Run_IO_Check.h
# End Source File
# Begin Source File

SOURCE=.\Run_LdPicker.h
# End Source File
# Begin Source File

SOURCE=.\Run_LdStacker_Elvator.h
# End Source File
# Begin Source File

SOURCE=.\Run_LdTrayPlate.h
# End Source File
# Begin Source File

SOURCE=.\Run_Motors.h
# End Source File
# Begin Source File

SOURCE=.\Run_TowerLamp.h
# End Source File
# Begin Source File

SOURCE=.\Run_UldPicker.h
# End Source File
# Begin Source File

SOURCE=.\Run_UnPress_Robot.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Administrator.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Basic.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Initial.h
# End Source File
# Begin Source File

SOURCE=.\Screen_IO_Map.h
# End Source File
# Begin Source File

SOURCE=.\Screen_List_Alarm.h
# End Source File
# Begin Source File

SOURCE=.\Screen_List_Error.h
# End Source File
# Begin Source File

SOURCE=.\Screen_List_Operation.h
# End Source File
# Begin Source File

SOURCE=.\Screen_List_Step.h
# End Source File
# Begin Source File

SOURCE=.\Screen_List_Work.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Lock.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Main.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Motor.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Motor_Speed.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Pgm_Info.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Set_Maintenance.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Set_Recipe.h
# End Source File
# Begin Source File

SOURCE=.\Screen_Wait_Time.h
# End Source File
# Begin Source File

SOURCE=.\SerialComm.h
# End Source File
# Begin Source File

SOURCE=.\ServerItem.h
# End Source File
# Begin Source File

SOURCE=.\ServerSocket.h
# End Source File
# Begin Source File

SOURCE=.\StaticButton.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SxLogFont.h
# End Source File
# Begin Source File

SOURCE=.\Thread.h
# End Source File
# Begin Source File

SOURCE=.\TrueColorToolBar.h
# End Source File
# Begin Source File

SOURCE=.\Variable.h
# End Source File
# Begin Source File

SOURCE=.\VLabel.h
# End Source File
# Begin Source File

SOURCE=.\xShadeButton.h
# End Source File
# Begin Source File

SOURCE=.\xSkinButton.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\res\08-HiTech World My Documents.ico"
# End Source File
# Begin Source File

SOURCE=.\res\1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Alarm.gif
# End Source File
# Begin Source File

SOURCE=.\res\alarm_reset.ico
# End Source File
# Begin Source File

SOURCE=.\res\all_apply.ico
# End Source File
# Begin Source File

SOURCE=.\res\AllHomeCheck.bmp
# End Source File
# Begin Source File

SOURCE=.\res\AMT8315.bmp
# End Source File
# Begin Source File

SOURCE=.\res\AMT8315_1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\AMT8315_2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\AMT8315_3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\AMT8315_5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\AMT8315_7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\amt_logo_fast.gif
# End Source File
# Begin Source File

SOURCE=.\res\apply.ico
# End Source File
# Begin Source File

SOURCE=.\res\big5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Big_2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Big_3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Big_4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap10.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap__.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_c.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_e.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_s.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_z.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00011.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00012.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00013.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00014.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00015.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00016.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00017.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00018.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00019.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00020.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00021.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00022.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00023.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buzzer_o.ico
# End Source File
# Begin Source File

SOURCE=.\res\camera_o.ico
# End Source File
# Begin Source File

SOURCE=.\res\chckddwn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\chckdup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\chckdwn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\chckfdwn.bmp
# End Source File
# Begin Source File

SOURCE=.\res\chckfup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\chckup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Choice.gif
# End Source File
# Begin Source File

SOURCE=.\res\ClockST_big.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ClockST_pane.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ClockST_small.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Connect%20Icon 사본.ico"
# End Source File
# Begin Source File

SOURCE=.\res\create.ico
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00003.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00004.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00005.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00006.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00007.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor_z.cur
# End Source File
# Begin Source File

SOURCE=.\res\datepick.bmp
# End Source File
# Begin Source File

SOURCE=.\res\delete.ico
# End Source File
# Begin Source File

SOURCE=.\res\delete1.ico
# End Source File
# Begin Source File

SOURCE=.\res\device.bmp
# End Source File
# Begin Source File

SOURCE=.\res\device.ico
# End Source File
# Begin Source File

SOURCE=.\res\device1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digit_b.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digit_bd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digit_c.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digit_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digit_g.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digit_r.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digit_y.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digitB_b.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digitB_bd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digitB_bl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digitB_c.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digitB_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digitB_g.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digitB_r.bmp
# End Source File
# Begin Source File

SOURCE=.\res\digitB_y.bmp
# End Source File
# Begin Source File

SOURCE=.\res\direction_left.ico
# End Source File
# Begin Source File

SOURCE=.\res\direction_left1.ico
# End Source File
# Begin Source File

SOURCE=".\res\Disconnect%20Icon 사본.ico"
# End Source File
# Begin Source File

SOURCE=.\res\dn.ico
# End Source File
# Begin Source File

SOURCE=.\res\dn_d.ico
# End Source File
# Begin Source File

SOURCE=.\res\dn_d1.ico
# End Source File
# Begin Source File

SOURCE=.\res\dn_d_m1.ico
# End Source File
# Begin Source File

SOURCE=.\res\dn_e.ico
# End Source File
# Begin Source File

SOURCE=.\res\dn_e1.ico
# End Source File
# Begin Source File

SOURCE=.\res\dn_e_m1.ico
# End Source File
# Begin Source File

SOURCE=.\res\download.ico
# End Source File
# Begin Source File

SOURCE=.\res\download_manager.ico
# End Source File
# Begin Source File

SOURCE=.\res\emg.ico
# End Source File
# Begin Source File

SOURCE=.\res\emg_stop.ico
# End Source File
# Begin Source File

SOURCE=.\res\enable1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Exit.gif
# End Source File
# Begin Source File

SOURCE=.\res\exit.ico
# End Source File
# Begin Source File

SOURCE=.\res\exit1.ico
# End Source File
# Begin Source File

SOURCE=.\res\exit_d1.ico
# End Source File
# Begin Source File

SOURCE=.\res\gear12.ico
# End Source File
# Begin Source File

SOURCE=".\res\gnome-mime-application-x-glade.ico"
# End Source File
# Begin Source File

SOURCE=.\res\greenButton.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HANDLER.ico
# End Source File
# Begin Source File

SOURCE=.\res\HANDLER.rc2
# End Source File
# Begin Source File

SOURCE=.\res\HANDLERDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Harrow.cur
# End Source File
# Begin Source File

SOURCE=.\res\home.ico
# End Source File
# Begin Source File

SOURCE=.\res\home_d.ico
# End Source File
# Begin Source File

SOURCE=.\res\home_d1.ico
# End Source File
# Begin Source File

SOURCE=.\res\home_e.ico
# End Source File
# Begin Source File

SOURCE=.\res\home_e1.ico
# End Source File
# Begin Source File

SOURCE=.\res\i_down.ico
# End Source File
# Begin Source File

SOURCE=.\res\i_up.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00007.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00008.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00009.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00010.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00011.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00012.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00013.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00014.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00015.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00016.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00017.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon10.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon11.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon12.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon13.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon14.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon15.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon17.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon18.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon4.ico
# End Source File
# Begin Source File

SOURCE=".\res\initial-1.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\initial-2.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\Initial.gif
# End Source File
# Begin Source File

SOURCE=.\res\io.ico
# End Source File
# Begin Source File

SOURCE=.\res\Keyboard.ico
# End Source File
# Begin Source File

SOURCE=.\res\led_icon.ico
# End Source File
# Begin Source File

SOURCE=.\res\LedGreen.ico
# End Source File
# Begin Source File

SOURCE=.\res\LedNone.ico
# End Source File
# Begin Source File

SOURCE=.\res\LedRed.ico
# End Source File
# Begin Source File

SOURCE=.\res\left.ico
# End Source File
# Begin Source File

SOURCE=.\res\left_d.ico
# End Source File
# Begin Source File

SOURCE=.\res\left_d1.ico
# End Source File
# Begin Source File

SOURCE=.\res\left_d_m.ico
# End Source File
# Begin Source File

SOURCE=.\res\left_e.ico
# End Source File
# Begin Source File

SOURCE=.\res\left_e1.ico
# End Source File
# Begin Source File

SOURCE=.\res\left_e_m.ico
# End Source File
# Begin Source File

SOURCE=.\res\length_1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\length_2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Length_D1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Length_E.gif
# End Source File
# Begin Source File

SOURCE=.\res\Length_E1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\list_arm.ico
# End Source File
# Begin Source File

SOURCE=.\res\list_wor.ico
# End Source File
# Begin Source File

SOURCE=.\res\locking.gif
# End Source File
# Begin Source File

SOURCE=".\res\lot cancel.ico"
# End Source File
# Begin Source File

SOURCE=".\res\lot start.ico"
# End Source File
# Begin Source File

SOURCE=.\res\main.bmp
# End Source File
# Begin Source File

SOURCE=.\res\main2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\main3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\main4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\main_1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\main_2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\main_3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\main_4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\maintena.ico
# End Source File
# Begin Source File

SOURCE=.\res\matrixsetblue.bmp
# End Source File
# Begin Source File

SOURCE=.\res\matrixsetsmallblue.bmp
# End Source File
# Begin Source File

SOURCE=.\res\matrixsettinyblue.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Menu.gif
# End Source File
# Begin Source File

SOURCE=.\res\module.ico
# End Source File
# Begin Source File

SOURCE=.\res\module_b.ico
# End Source File
# Begin Source File

SOURCE=.\res\mot_pos.ico
# End Source File
# Begin Source File

SOURCE=.\res\mot_spd.ico
# End Source File
# Begin Source File

SOURCE=.\res\motor.ico
# End Source File
# Begin Source File

SOURCE=.\res\motor_2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\motor_3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\motor_of.ico
# End Source File
# Begin Source File

SOURCE=.\res\motor_off.ico
# End Source File
# Begin Source File

SOURCE=.\res\motor_on.ico
# End Source File
# Begin Source File

SOURCE=.\res\move_reset.ico
# End Source File
# Begin Source File

SOURCE=.\res\MSG.gif
# End Source File
# Begin Source File

SOURCE=.\res\n_with.ico
# End Source File
# Begin Source File

SOURCE=.\res\ne.cur
# End Source File
# Begin Source File

SOURCE=".\res\Non Safety.ico"
# End Source File
# Begin Source File

SOURCE=.\res\ns.cur
# End Source File
# Begin Source File

SOURCE=.\res\nw.cur
# End Source File
# Begin Source File

SOURCE=.\res\off.ico
# End Source File
# Begin Source File

SOURCE=.\res\off_e1.ico
# End Source File
# Begin Source File

SOURCE=.\res\OffLine.ico
# End Source File
# Begin Source File

SOURCE=".\res\old-edit-undo1.ico"
# End Source File
# Begin Source File

SOURCE=".\res\old-edit-undo2.ico"
# End Source File
# Begin Source File

SOURCE=.\res\on.ico
# End Source File
# Begin Source File

SOURCE=.\res\on_e1.ico
# End Source File
# Begin Source File

SOURCE=.\res\online.ico
# End Source File
# Begin Source File

SOURCE=".\res\Open-Folder-Full128.ico"
# End Source File
# Begin Source File

SOURCE=.\res\open_fol.ico
# End Source File
# Begin Source File

SOURCE=.\res\password.ico
# End Source File
# Begin Source File

SOURCE=.\res\picker_o.ico
# End Source File
# Begin Source File

SOURCE=.\res\PickerOff.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PickerOn.bmp
# End Source File
# Begin Source File

SOURCE=".\res\player-end.ico"
# End Source File
# Begin Source File

SOURCE=".\res\player-up.ico"
# End Source File
# Begin Source File

SOURCE=.\res\player_dn.ico
# End Source File
# Begin Source File

SOURCE=.\res\player_rew.ico
# End Source File
# Begin Source File

SOURCE=.\res\power_of.ico
# End Source File
# Begin Source File

SOURCE=.\res\power_off.ico
# End Source File
# Begin Source File

SOURCE=.\res\power_on.ico
# End Source File
# Begin Source File

SOURCE=.\res\redButton.bmp
# End Source File
# Begin Source File

SOURCE=.\res\reset.ico
# End Source File
# Begin Source File

SOURCE=.\res\right.ico
# End Source File
# Begin Source File

SOURCE=.\res\right_d.ico
# End Source File
# Begin Source File

SOURCE=.\res\right_d_.ico
# End Source File
# Begin Source File

SOURCE=.\res\right_e.ico
# End Source File
# Begin Source File

SOURCE=.\res\right_e1.ico
# End Source File
# Begin Source File

SOURCE=.\res\right_e_.ico
# End Source File
# Begin Source File

SOURCE=.\res\rigth_d.ico
# End Source File
# Begin Source File

SOURCE=.\res\rigth_d1.ico
# End Source File
# Begin Source File

SOURCE=.\res\running_process.ico
# End Source File
# Begin Source File

SOURCE=.\res\Safety.ico
# End Source File
# Begin Source File

SOURCE=.\res\safety1.ico
# End Source File
# Begin Source File

SOURCE=.\res\search.ico
# End Source File
# Begin Source File

SOURCE=.\res\sign11910200.ico
# End Source File
# Begin Source File

SOURCE=.\res\Small_1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\small_2.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Sound Off.ico"
# End Source File
# Begin Source File

SOURCE=".\res\Sound On.ico"
# End Source File
# Begin Source File

SOURCE=.\res\spreadsheet.ico
# End Source File
# Begin Source File

SOURCE=.\res\thickness1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\thickness2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Thickness_D1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Thickness_E1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarDraw.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarDrawDisabled.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolbarDrawHot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Trash1.ico
# End Source File
# Begin Source File

SOURCE=.\res\trashcan_full.ico
# End Source File
# Begin Source File

SOURCE=.\res\Tray.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tray.ico
# End Source File
# Begin Source File

SOURCE=.\res\tree_ctrl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tt_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\up.ico
# End Source File
# Begin Source File

SOURCE=.\res\up1.ico
# End Source File
# Begin Source File

SOURCE=.\res\up_d.ico
# End Source File
# Begin Source File

SOURCE=.\res\up_d1.ico
# End Source File
# Begin Source File

SOURCE=.\res\up_d_3.ico
# End Source File
# Begin Source File

SOURCE=.\res\up_d_m1.ico
# End Source File
# Begin Source File

SOURCE=.\res\up_e.ico
# End Source File
# Begin Source File

SOURCE=.\res\up_e1.ico
# End Source File
# Begin Source File

SOURCE=.\res\up_e_3.ico
# End Source File
# Begin Source File

SOURCE=.\res\up_e_m1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Upload.ico
# End Source File
# Begin Source File

SOURCE=.\res\xls.ico
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\dbghelp.h
# End Source File
# Begin Source File

SOURCE=..\common\ShareMem.cpp
# End Source File
# Begin Source File

SOURCE=..\common\ShareMem.h
# End Source File
# Begin Source File

SOURCE=..\common\StdAfx.h
# End Source File
# End Group
# Begin Group "SrcBase"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SrcBase\ACriticalSection.cpp
# End Source File
# Begin Source File

SOURCE=.\SrcBase\ACriticalSection.h
# End Source File
# Begin Source File

SOURCE=.\SrcBase\ALocalization.cpp
# End Source File
# Begin Source File

SOURCE=.\SrcBase\ALocalization.h
# End Source File
# Begin Source File

SOURCE=.\SrcBase\AMotor.cpp
# End Source File
# Begin Source File

SOURCE=.\SrcBase\AMotor.h
# End Source File
# Begin Source File

SOURCE=.\SrcBase\ANetwork.cpp
# End Source File
# Begin Source File

SOURCE=.\SrcBase\ANetwork.h
# End Source File
# End Group
# Begin Group "SrcPart"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SrcPart\APartDatabase.cpp
# End Source File
# Begin Source File

SOURCE=.\SrcPart\APartDatabase.h
# End Source File
# Begin Source File

SOURCE=.\SrcPart\APartErrReport.cpp
# End Source File
# Begin Source File

SOURCE=.\SrcPart\APartErrReport.h
# End Source File
# Begin Source File

SOURCE=.\SrcPart\APartHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\SrcPart\APartHandler.h
# End Source File
# Begin Source File

SOURCE=.\SrcPart\APartNetworkManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SrcPart\APartNetworkManager.h
# End Source File
# Begin Source File

SOURCE=.\SrcPart\APartShortkey.cpp
# End Source File
# Begin Source File

SOURCE=.\SrcPart\APartShortkey.h
# End Source File
# Begin Source File

SOURCE=.\SrcPart\APartTray.cpp
# End Source File
# Begin Source File

SOURCE=.\SrcPart\APartTray.h
# End Source File
# Begin Source File

SOURCE=.\SrcPart\DataManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SrcPart\DataManager.h
# End Source File
# Begin Source File

SOURCE=.\SrcPart\PartFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\SrcPart\PartFunction.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Error.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section HANDLER : {08BCCF2F-0F1E-4620-B61E-74563DB992F3}
# 	1:21:IDD_SCREEN_MOT_LOADER:111
# 	2:16:Resource Include:resource.h
# 	2:21:IDD_SCREEN_MOT_LOADER:IDD_SCREEN_MOT_LOADER
# 	2:10:ENUM: enum:enum
# 	2:27:CLASS: CScreen_Motor_Loader:CScreen_Motor_Loader
# 	2:23:Screen_Motor_Loader.cpp:Screen_Motor_Loader.cpp
# 	2:21:Screen_Motor_Loader.h:Screen_Motor_Loader.h
# 	2:19:Application Include:handler.h
# End Section
# Section HANDLER : {A45CD778-978C-4F62-85E8-C3D8B39FF674}
# 	1:20:IDD_SCREEN_DIAGNOSIS:116
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:24:CLASS: CScreen_Diagnosis:CScreen_Diagnosis
# 	2:18:Screen_Diagnosis.h:Screen_Diagnosis.h
# 	2:20:IDD_SCREEN_DIAGNOSIS:IDD_SCREEN_DIAGNOSIS
# 	2:19:Application Include:handler.h
# 	2:20:Screen_Diagnosis.cpp:Screen_Diagnosis.cpp
# End Section
# Section HANDLER : {B5882384-1ACF-438A-AFF5-9921F575EF98}
# 	1:23:IDD_SCREEN_MOT_UNLOADER:112
# 	2:16:Resource Include:resource.h
# 	2:23:IDD_SCREEN_MOT_UNLOADER:IDD_SCREEN_MOT_UNLOADER
# 	2:23:Screen_Motor_Unloader.h:Screen_Motor_Unloader.h
# 	2:29:CLASS: CScreen_Motor_Unloader:CScreen_Motor_Unloader
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:handler.h
# 	2:25:Screen_Motor_Unloader.cpp:Screen_Motor_Unloader.cpp
# End Section
# Section HANDLER : {C61BFDB1-4DC7-4DA3-B6CF-8EBB3885084F}
# 	1:17:IDD_DIALOG_SELECT:103
# 	2:16:Resource Include:resource.h
# 	2:17:IDD_DIALOG_SELECT:IDD_DIALOG_SELECT
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:handler.h
# 	2:17:Dialog_Select.cpp:Dialog_Select.cpp
# 	2:15:Dialog_Select.h:Dialog_Select.h
# 	2:21:CLASS: CDialog_Select:CDialog_Select
# End Section
# Section HANDLER : {E26EFCFE-A0CF-4071-B95A-B01BB6CE8052}
# 	1:16:IDD_DIALOG_ALARM:108
# 	2:16:Resource Include:resource.h
# 	2:20:CLASS: CDialog_Alarm:CDialog_Alarm
# 	2:14:Dialog_Alarm.h:Dialog_Alarm.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:handler.h
# 	2:16:IDD_DIALOG_ALARM:IDD_DIALOG_ALARM
# 	2:16:Dialog_Alarm.cpp:Dialog_Alarm.cpp
# End Section
# Section HANDLER : {4921AE74-F35A-4F89-ABED-D13591C80378}
# 	1:28:IDD_SCREEN_MOTOR_LOADER_SITE:107
# 	2:16:Resource Include:resource.h
# 	2:28:IDD_SCREEN_MOTOR_LOADER_SITE:IDD_SCREEN_MOTOR_LOADER_SITE
# 	2:32:CLASS: CScreen_Motor_Loader_Site:CScreen_Motor_Loader_Site
# 	2:10:ENUM: enum:enum
# 	2:28:Screen_Motor_Loader_Site.cpp:Screen_Motor_Loader_Site.cpp
# 	2:26:Screen_Motor_Loader_Site.h:Screen_Motor_Loader_Site.h
# 	2:19:Application Include:handler.h
# End Section
# Section HANDLER : {EA71E92E-6CCA-4B81-AE6B-607D96456237}
# 	1:15:IDD_SCREEN_MAIN:109
# 	2:16:Resource Include:resource.h
# 	2:15:Screen_Main.cpp:Screen_Main.cpp
# 	2:10:ENUM: enum:enum
# 	2:15:IDD_SCREEN_MAIN:IDD_SCREEN_MAIN
# 	2:19:CLASS: CScreen_Main:CScreen_Main
# 	2:19:Application Include:handler.h
# 	2:13:Screen_Main.h:Screen_Main.h
# End Section
# Section HANDLER : {0551BEAA-4766-4D89-BC5E-6F27E0CD3AB9}
# 	1:30:IDD_SCREEN_MOTOR_UNLOADER_SITE:106
# 	2:16:Resource Include:resource.h
# 	2:28:Screen_Motor_Unloader_Site.h:Screen_Motor_Unloader_Site.h
# 	2:30:Screen_Motor_Unloader_Site.cpp:Screen_Motor_Unloader_Site.cpp
# 	2:30:IDD_SCREEN_MOTOR_UNLOADER_SITE:IDD_SCREEN_MOTOR_UNLOADER_SITE
# 	2:34:CLASS: CScreen_Motor_Unloader_Site:CScreen_Motor_Unloader_Site
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:handler.h
# End Section
# Section HANDLER : {C18C84D5-F71C-4CFD-B3E4-5B494F395A85}
# 	1:26:IDD_DIALOG_VISION_POSITION:115
# 	2:16:Resource Include:resource.h
# 	2:30:CLASS: CDialog_Vision_Position:CDialog_Vision_Position
# 	2:26:IDD_DIALOG_VISION_POSITION:IDD_DIALOG_VISION_POSITION
# 	2:24:Dialog_Vision_Position.h:Dialog_Vision_Position.h
# 	2:10:ENUM: enum:enum
# 	2:26:Dialog_Vision_Position.cpp:Dialog_Vision_Position.cpp
# 	2:19:Application Include:handler.h
# End Section
# Section HANDLER : {8EECE9B0-EAD9-48E5-845E-3E242A167545}
# 	1:20:IDD_DIALOG_LOT_START:113
# 	2:16:Resource Include:resource.h
# 	2:24:CLASS: CDialog_Lot_Start:CDialog_Lot_Start
# 	2:20:IDD_DIALOG_LOT_START:IDD_DIALOG_LOT_START
# 	2:20:Dialog_Lot_Start.cpp:Dialog_Lot_Start.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:handler.h
# 	2:18:Dialog_Lot_Start.h:Dialog_Lot_Start.h
# End Section
# Section HANDLER : {42DAE1FB-91DC-4323-B8C4-4E72463BABCB}
# 	1:16:IDD_SCREEN_BASIC:110
# 	2:16:Resource Include:resource.h
# 	2:20:CLASS: CScreen_Basic:CScreen_Basic
# 	2:16:Screen_Basic.cpp:Screen_Basic.cpp
# 	2:14:Screen_Basic.h:Screen_Basic.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:handler.h
# 	2:16:IDD_SCREEN_BASIC:IDD_SCREEN_BASIC
# End Section
# Section HANDLER : {4246EB57-EB45-4662-9F00-B155B1FF545A}
# 	1:15:IDD_DIALOG_MEMO:114
# 	2:16:Resource Include:resource.h
# 	2:15:IDD_DIALOG_MEMO:IDD_DIALOG_MEMO
# 	2:19:CLASS: CDialog_Memo:CDialog_Memo
# 	2:10:ENUM: enum:enum
# 	2:13:Dialog_Memo.h:Dialog_Memo.h
# 	2:19:Application Include:handler.h
# 	2:15:Dialog_Memo.cpp:Dialog_Memo.cpp
# End Section
# Section HANDLER : {BFEC2C57-6718-42FE-A56F-2291C78CE32A}
# 	1:17:IDD_DIALOG_KEYPAD:105
# 	2:16:Resource Include:resource.h
# 	2:15:Dialog_KeyPad.h:Dialog_KeyPad.h
# 	2:17:Dialog_KeyPad.cpp:Dialog_KeyPad.cpp
# 	2:10:ENUM: enum:enum
# 	2:21:CLASS: CDialog_KeyPad:CDialog_KeyPad
# 	2:17:IDD_DIALOG_KEYPAD:IDD_DIALOG_KEYPAD
# 	2:19:Application Include:handler.h
# End Section
# Section HANDLER : {9C119FCB-CE7A-400C-917A-51AEBB96B2D3}
# 	1:13:IDD_SCREEN_IO:102
# 	2:16:Resource Include:resource.h
# 	2:11:Screen_IO.h:Screen_IO.h
# 	2:13:IDD_SCREEN_IO:IDD_SCREEN_IO
# 	2:10:ENUM: enum:enum
# 	2:17:CLASS: CScreen_IO:CScreen_IO
# 	2:13:Screen_IO.cpp:Screen_IO.cpp
# 	2:19:Application Include:handler.h
# End Section
