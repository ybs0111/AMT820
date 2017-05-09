#include "stdafx.h"
#include "PartFunction.h"
#include <io.h>
#include <direct.h>

#include "../Dialog_Message.h"
#include "../Dialog_Select.h"

HANDLE DDBToDIB(CBitmap &bitmap, DWORD dwCompression, CPalette *pPal)
{
	BITMAP   bm;
	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER  lpbi;
	DWORD   dwLen;
	HANDLE   hDIB;
	HANDLE   handle;
	HDC    hDC;
	HPALETTE  hPal;


	ASSERT( bitmap.GetSafeHandle() );

	// The function has no arg for bitfields
	if( dwCompression == BI_BITFIELDS )
	 return NULL;

	// If a palette has not been supplied use defaul palette
	hPal = (HPALETTE) pPal->GetSafeHandle();
	if (hPal==NULL)
	 hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

	// Get bitmap information
	bitmap.GetObject(sizeof(bm),(LPSTR)&bm);

	// Initialize the bitmapinfoheader
	bi.biSize  = sizeof(BITMAPINFOHEADER);
	bi.biWidth  = bm.bmWidth;
	bi.biHeight   = bm.bmHeight;
	bi.biPlanes   = 1;
	bi.biBitCount  = bm.bmPlanes * bm.bmBitsPixel;
	bi.biCompression = dwCompression;
	bi.biSizeImage  = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed  = 0;
	bi.biClrImportant = 0;

	// Compute the size of the  infoheader and the color table
	int nColors = (1 << bi.biBitCount);
	if( nColors > 256 )
	 nColors = 0;
	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

	// We need a device context to get the DIB from
	hDC = ::GetDC(NULL);
	hPal = SelectPalette(hDC,hPal,FALSE);
	RealizePalette(hDC);

	// Allocate enough memory to hold bitmapinfoheader and color table
	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);

	if (!hDIB){
	 SelectPalette(hDC,hPal,FALSE);
	 ::ReleaseDC(NULL,hDC);
	 return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	*lpbi = bi;

	// Call GetDIBits with a NULL lpBits param, so the device driver 
	// will calculate the biSizeImage field 
	GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(), 0L, (DWORD)bi.biHeight,
	  (LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	// If the driver did not fill in the biSizeImage field, then compute it
	// Each scan line of the image is aligned on a DWORD (32bit) boundary
	if (bi.biSizeImage == 0){
	 bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8)
		 * bi.biHeight;

	 // If a compression scheme is used the result may infact be larger
	 // Increase the size to account for this.
	 if (dwCompression != BI_RGB)
	  bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}

	// Realloc the buffer so that it can hold all the bits
	dwLen += bi.biSizeImage;
	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
	 hDIB = handle;
	else{
	 GlobalFree(hDIB);

	 // Reselect the original palette
	 SelectPalette(hDC,hPal,FALSE);
	 ::ReleaseDC(NULL,hDC);
	 return NULL;
	}

	// Get the bitmap bits
	lpbi = (LPBITMAPINFOHEADER)hDIB;

	// FINALLY get the DIB
	BOOL bGotBits = GetDIBits( hDC, (HBITMAP)bitmap.GetSafeHandle(),
	   0L,    // Start scan line
	   (DWORD)bi.biHeight,  // # of scan lines
	   (LPBYTE)lpbi    // address for bitmap bits
	   + (bi.biSize + nColors * sizeof(RGBQUAD)),
	   (LPBITMAPINFO)lpbi,  // address of bitmapinfo
	   (DWORD)DIB_RGB_COLORS);  // Use RGB for color table

	if( !bGotBits )
	{
	 GlobalFree(hDIB);

	 SelectPalette(hDC,hPal,FALSE);
	 ::ReleaseDC(NULL,hDC);
	 return NULL;
	}

	SelectPalette(hDC,hPal,FALSE);
	::ReleaseDC(NULL,hDC);
	return hDIB;

}

bool WriteDIB(LPCSTR szFile, HANDLE hDIB)
{
		BITMAPFILEHEADER    hdr;
	  LPBITMAPINFOHEADER  lpbi;

	   if(!hDIB)
		   return FALSE;

	   CFile file;

	   if(!file.Open( szFile, CFile::modeWrite|CFile::modeCreate) )
		   return FALSE;
	   lpbi = (LPBITMAPINFOHEADER)hDIB;
	   int nColors = 1 << lpbi->biBitCount;

	   // Fill in the fields of the file header 
	   hdr.bfType      = ((WORD) ('M' << 8) | 'B');    // is always "BM"
	   hdr.bfSize      = GlobalSize (hDIB) + sizeof(hdr);
	   hdr.bfReserved1 = 0;
	   hdr.bfReserved2 = 0;
	   hdr.bfOffBits   = (DWORD) (sizeof( hdr ) + lpbi->biSize + nColors * sizeof(RGBQUAD));

	   // Write the file header 
	   file.Write(&hdr, sizeof(hdr));

	   // Write the DIB header and the bits 
	   file.Write(lpbi, GlobalSize(hDIB));

	   return TRUE;
}

void CaptureScreenShot()
{
	CClientDC dc( NULL );
	CRect rc(0, 0, GetSystemMetrics(SM_CXMAXTRACK), GetSystemMetrics(SM_CYMAXTRACK) );

	CBitmap captureBmp;
	captureBmp.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height() );

	CDC dcMem;
	dcMem.CreateCompatibleDC( &dc );
	CBitmap* pOldBitmap = dcMem.SelectObject(&captureBmp);

	dcMem.BitBlt(0, 0, rc.Width(), rc.Height(), &dc, 0, 0, SRCCOPY );
	dcMem.SelectObject( pOldBitmap );

	HANDLE hDIB = DDBToDIB(captureBmp, BI_RGB, NULL);
	if( hDIB != NULL )
	{
		SYSTEMTIME	csTime;
		::GetLocalTime(&csTime);
	
		if (_access("..\\ScreenShot",0)==-1)
			_mkdir("..\\ScreenShot");

		CString strPath;
		strPath.Format( "..\\ScreenShot\\%04d%02d%02d_%02d%02d%02d%03d.bmp",
			csTime.wYear, csTime.wMonth, csTime.wDay,
			csTime.wHour, csTime.wMinute, csTime.wSecond, csTime.wMilliseconds );

		WriteDIB((LPCSTR)strPath, hDIB );
//		CloseHandle(hDIB );
	}

	captureBmp.DeleteObject();
}

LONG WINAPI UnHandledExceptionFilter(struct _EXCEPTION_POINTERS *exceptionInfo)
{
	HMODULE	DllHandle		= NULL;

	// Windows 2000 이전에는 따로 DBGHELP를 배포해서 설정해 주어야 한다.
	DllHandle				= LoadLibrary(_T("DBGHELP.DLL"));

	if (DllHandle)
	{
		MINIDUMPWRITEDUMP Dump = (MINIDUMPWRITEDUMP) GetProcAddress(DllHandle, "MiniDumpWriteDump");

		if (Dump)
		{
			CaptureScreenShot();

			TCHAR		DumpPath[MAX_PATH] = {0,};
			SYSTEMTIME	SystemTime;

			GetLocalTime(&SystemTime);
			TCHAR tszBuff[MAX_PATH];
			VERIFY(::GetModuleFileName(AfxGetInstanceHandle(), tszBuff, MAX_PATH));

			CString strTemp, strFilePath;
			
			strTemp.Format(_T("%s"), tszBuff);
			strTemp = strTemp.Left(strTemp.ReverseFind('\\'));
#ifdef _DEBUG
			strTemp = strTemp.Left(strTemp.ReverseFind('\\'));
#endif
			strTemp = strTemp + _T("\\MemoryDump");

			char MBBuffer[1024];
			memset(MBBuffer,0,sizeof(MBBuffer));

#ifdef _UNICODE
			WideCharToMultiByte(CP_ACP, 0, strTemp.GetBuffer(0), -1, MBBuffer, 1024, 0, 0);
#else
			sprintf(MBBuffer, _T("%s"), strTemp);
#endif

			if (_access(MBBuffer,0)==-1)
				_mkdir(MBBuffer);

			MessageBox(NULL, MBBuffer, "", MB_OK );

			_sntprintf(DumpPath, MAX_PATH, _T("%s\\%04d%02d%02d_%02d%02d%02d.dmp"), strTemp,
				SystemTime.wYear,
				SystemTime.wMonth,
				SystemTime.wDay,
				SystemTime.wHour,
				SystemTime.wMinute,
				SystemTime.wSecond);
			
			HANDLE FileHandle = CreateFile(
				DumpPath, 
				GENERIC_WRITE, 
				FILE_SHARE_WRITE, 
				NULL, CREATE_ALWAYS, 
				FILE_ATTRIBUTE_NORMAL, 
				NULL);

			if (FileHandle != INVALID_HANDLE_VALUE)
			{
				_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;
				
				MiniDumpExceptionInfo.ThreadId			= GetCurrentThreadId();
				MiniDumpExceptionInfo.ExceptionPointers	= exceptionInfo;
				MiniDumpExceptionInfo.ClientPointers	= NULL;

				BOOL Success = Dump(
					GetCurrentProcess(), 
					GetCurrentProcessId(), 
					FileHandle, 
					MiniDumpNormal,
					&MiniDumpExceptionInfo, 
					NULL, 
					NULL);

				if (Success)
				{
					CloseHandle(FileHandle);
					return EXCEPTION_CONTINUE_SEARCH;
				}
			}
			CloseHandle(FileHandle);
		}
	}

	return EXCEPTION_CONTINUE_SEARCH;
}



int DoModal_Msg( CString strMsg )
{
	CDialog_Message msg_dlg;
	//st_msg.str_fallacy_msg = strMsg;
	//kwlee 2017.0421
	st_other.str_confirm_msg = strMsg;
				
	return msg_dlg.DoModal();
}

COLORREF GetCokCol( int iIdx )
{
	switch( iIdx )
	{
	case -1: return WHITE_C;
//	case 0:	return COL_COK_0;
//	case 1:	return COL_COK_1;
//	case 2:	return COL_COK_2;
//	case 3:	return COL_COK_3;
//	case 4:	return COL_COK_4;
//	case 5:	return COL_COK_5;
	}

	return BLACK_C;
}


double GetPosCYAtCokStacker( int iIdx )
{
// 	double dFirst = st_motor[MOTOR_COK_ROBOT_Y].md_pos[P_CY_COK_0];
// 	double dLast = st_motor[MOTOR_COK_ROBOT_Y].md_pos[P_CY_COK_5];
//	double dFirst = st_handler.md_common_teaching[CT_COK_Y_START];
//	double dLast = st_handler.md_common_teaching[CT_COK_Y_END];

//	double dRtn = dFirst + ( dLast - dFirst ) / 5 * (double)iIdx;
//	return dRtn;
	return 1;
}

double GetPosCZByCnt( int iCnt )
{
	if( iCnt < 1 )
		iCnt = 1;

// 	double dFirst = st_motor[MOTOR_COK_ROBOT_Z].md_pos[P_CZ_COK_19];
// 	double dLast = st_motor[MOTOR_COK_ROBOT_Z].md_pos[P_CZ_COK_0];

//	double dFirst = st_handler.md_common_teaching[CT_COK_Z_19];
//	double dLast = st_handler.md_common_teaching[CT_COK_Z_0];

//	double dRtn = dLast + ( dFirst - dLast ) / 19 * (double)(iCnt - 1);
//	return dRtn;
	return 1;
}

int GetMdlIdx( int ix, int iy, int iw, int ih, bool bTurn )
{
	/*
	if(st_handler.mn_mirror_type == CTL_YES)
	{
		if( bTurn == false || st_basic.n_degree == EDEG_0 )
		{
			return (iw - ix - 1 + iy * iw) + 1;
		}
		
		if( st_basic.n_degree == EDEG_90 )
		{
			return (iy + ix * ih) + 1;
		}
		
		if( st_basic.n_degree == EDEG_180 )
		{
			return (ix + (ih - iy - 1) * iw) + 1;
		}
		
		if( st_basic.n_degree == EDEG_270 )
		{
			return (ih - iy - 1) + (iw - ix - 1) * ih + 1;
		}
	}
	else
	{
		if( bTurn == false || st_basic.n_degree == EDEG_0 )
		{
			return (ix + (ih - iy - 1) * iw) + 1;
		}
		
		if( st_basic.n_degree == EDEG_90 )
		{
			return (iy + ix * ih) + 1;
		}
		
		if( st_basic.n_degree == EDEG_180 )
		{
			return (iw - ix - 1 + iy * iw) + 1;
		}
		
		if( st_basic.n_degree == EDEG_270 )
		{
			return (ih - iy - 1) + (iw - ix - 1) * ih + 1;
		}
	}*/
	
	return 0;
}

int GetMatrixXMdlIdx()
{
	/*
	int x = st_basic.n_tray_x;
	int y = st_basic.n_tray_y;

	if( (st_basic.n_degree == EDEG_90 || st_basic.n_degree == EDEG_270 ) )
	{
		y = st_basic.n_tray_x;
		x = st_basic.n_tray_y;
	}

	if(st_handler.mn_mirror_type == CTL_YES)
	{
		if( st_basic.n_degree == EDEG_0 )
		{
			return 0;
		}
		
		if( st_basic.n_degree == EDEG_90 )
		{
			return 0;
		}
		
		if( st_basic.n_degree == EDEG_180 )
		{
			return 0;
		}
		
		if( st_basic.n_degree == EDEG_270 )
		{
			return 0;
		}
	}
	else
	{
		if( st_basic.n_degree == EDEG_0 )
		{
			return 0;
		}
		
		if( st_basic.n_degree == EDEG_90 )
		{
			return 0;
		}
		
		if( st_basic.n_degree == EDEG_180 )
		{
			return 0;
		}
		
		if( st_basic.n_degree == EDEG_270 )
		{
			return 0;
		}
	}*/
	
	return 0;
}

int GetMatrixYMdlIdx()
{
	/*
	int x = st_basic.n_tray_x;
	int y = st_basic.n_tray_y;

	if( (st_basic.n_degree == EDEG_90 || st_basic.n_degree == EDEG_270 ) )
	{
		y = st_basic.n_tray_x;
		x = st_basic.n_tray_y;
	}

	if(st_handler.mn_mirror_type == CTL_YES)
	{
		if( st_basic.n_degree == EDEG_0 )
		{
			return 0;
		}
		
		if( st_basic.n_degree == EDEG_90 )
		{
			return 0;
		}
		
		if( st_basic.n_degree == EDEG_180 )
		{
			return y-1;
		}
		
		if( st_basic.n_degree == EDEG_270 )
		{
			return 0;
		}
	}
	else
	{
		if( st_basic.n_degree == EDEG_0 )
		{
			return y-1;
		}
		
		if( st_basic.n_degree == EDEG_90 )
		{
			return 0;
		}
		
		if( st_basic.n_degree == EDEG_180 )
		{
			return y-1;
		}
		
		if( st_basic.n_degree == EDEG_270 )
		{
			return 0;
		}
	}
	*/
	return 0;
}

int DoModal_Select( CString strMsg )
{
	CDialog_Select	select_dlg;
	
	st_msg.mstr_confirm_msg = strMsg;
	
	return select_dlg.DoModal();
}

void ReadBasicData_Int( int& nRead, CString strKey, int nInit, int nMin, int nMax, LPCSTR strFile )
{
	char chr_data[1024];
	CString strInit;
	strInit.Format( "%d", nInit);
	:: GetPrivateProfileString("BasicData", (LPCSTR)strKey, (LPCSTR)strInit, chr_data, 10, strFile);
	int mn_chk = atoi(chr_data);
	if (mn_chk < nMin || mn_chk > nMax)
	{
		nRead = nMin;
		CString mstr_temp;
		mstr_temp.Format("%d", nRead);
		:: WritePrivateProfileString("BasicData", (LPCSTR)strKey, LPCTSTR(mstr_temp), strFile);
	}
	else  nRead = mn_chk;
}

void ReadBasicData_Double( double& dRead, CString strKey, double dInit, double dMin, double dMax , LPCSTR strFile )
{
	char chr_data[1024];
	CString strInit;
	strInit.Format( "%.03f", dInit );
	:: GetPrivateProfileString("BasicData", (LPCSTR)strKey, (LPCSTR)strInit, chr_data, 10, strFile);
	double md_chk = atof(chr_data);
	if (md_chk < dMin || md_chk > dMax)
	{
		dRead = dInit;
		CString mstr_temp;
		mstr_temp.Format("%d", dRead );
		:: WritePrivateProfileString("BasicData", (LPCSTR)strKey, LPCTSTR(mstr_temp), strFile );
	}
	else  dRead = md_chk;
}

/*
COLORREF CalcModuleColor( AModule mdl )
{
	COLORREF col = SKY_C;
	switch( mdl.GetModuleState() )
	{
	case EMS_NONE:				col = WHITE_C;		break;
	case EMS_LOAD:				col = SKY_C;		break;
	case EMS_READY:
	case EMS_READY_RECV:		col = GREEN_L;		break;
	case EMS_START:
	case EMS_TEST:
		{
			if( mdl.GetBin() == "" )			col = YELLOW_C;
			else if( atoi(mdl.GetBin()) > 3 )	col = RED_C;
			else								col = BLUE_L;
		}
		break;
	case EMS_ABORT:
	case EMS_ABORT_RECV:		col = COL_PUPLE;	break;
	case EMS_GOOD:				col = BLUE_L;		break;
	case EMS_FAIL:				col = RED_C;		break;
	case EMS_RELOAD:			col = RED_L;		break;
	}

	return col;
}

COLORREF CalcModuleColor( EMODULE_STATE ems )
{
	COLORREF col = SKY_C;
	switch( ems )
	{
	case EMS_NONE:				col = WHITE_C;		break;
	case EMS_LOAD:				col = SKY_C;		break;
	case EMS_READY:
	case EMS_READY_RECV:		col = GREEN_L;		break;
	case EMS_START:
	case EMS_TEST:				col = YELLOW_C;		break;
	case EMS_ABORT:
	case EMS_ABORT_RECV:		col = COL_PUPLE;	break;
	case EMS_GOOD:				col = BLUE_L;		break;
	case EMS_FAIL:				col = RED_C;		break;
	case EMS_RELOAD:			col = RED_L;		break;
	case EMS_MAXCOUNT:			col = TEST_BC;		break;
	case EMS_COUNTINUE:			col = IN_BC;		break;
	case EMS_OPS:				col = DSERBLE_BC;		break;
	case EMS_REPAIR:			col = ORANGE_L;		break;
	}

	return col;
}
*/
EMODULE_STATE GetStateByStr( CString strState )
{
	if( strState == "LOAD" )				return EMS_LOAD;
	else if( strState == "READY")			return EMS_READY;
	else if( strState == "READY_RECV")		return EMS_READY_RECV;
	else if( strState == "START")			return EMS_START;
	else if( strState == "TEST" )			return EMS_TEST;
	else if( strState == "ABORT")			return EMS_ABORT;
	else if( strState == "ABORT_RECV")		return EMS_ABORT_RECV;
	else if( strState == "GOOD" )			return EMS_GOOD;
	else if( strState == "FAIL" )			return EMS_FAIL;
	else if( strState == "RELOAD" )			return EMS_RELOAD;
	
	return EMS_NONE;
}