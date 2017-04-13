#ifndef __AXT_LIBRARY_DEFINE_H__
#define __AXT_LIBRARY_DEFINE_H__

//	Console application 프로그램을 위해서 아래 부분을 정의함
typedef unsigned long int				DWORD;
typedef unsigned short int				WORD;
typedef unsigned char					BYTE;
typedef int								BOOL;
typedef void							*HANDLE;

#ifndef PASCAL
	#define PASCAL						__stdcall
#endif	// PASCAL

#ifndef EXPORT
	#define EXPORT
#endif	// EXPORT

//typedef int							BOOL;			// 0(FALSE), 1(TRUE)
typedef signed char						INT8;			// -128 .. 127
typedef unsigned char					UINT8;			// 0 .. 255
typedef signed short int				INT16;			// -32768 .. 32767
typedef unsigned short int				UINT16;			// 0 .. 65535
typedef int								INT32;			// -2,147,483,648 .. 2,147,483,647
typedef unsigned int					UINT32;			// 0 .. 4,294,967,295


// Bus type
#ifndef BUSTYPE_UNKNOWN
#define BUSTYPE_UNKNOWN					-1					// Unknown
#define BUSTYPE_ISA						0					// ISA(Industrial Standard Architecture)
#define BUSTYPE_PCI						1					// PCI
#define BUSTYPE_VME						2					// VME
#define BUSTYPE_CPCI					3					// Compact PCI
#define BUSTYPE_MIN						BUSTYPE_ISA			// 0
#define BUSTYPE_MAX						BUSTYPE_CPCI		// 3
#define BUSTYPE_NUM						4					// 4
#endif	// BUSTYPE_UNKNOWN

// 베이스 보드 정의
#define AXT_UNKNOWN						0x00				// Unknown Baseboard
#define AXT_BIHR						0x01				// ISA bus, Half size
#define AXT_BIFR						0x02				// ISA bus, Full size
#define AXT_BPHR						0x03				// PCI bus, Half size
#define AXT_BPFR						0x04				// PCI bus, Full size
#define AXT_BV3R						0x05				// VME bus, 3U size
#define AXT_BV6R						0x06				// VME bus, 6U size
#define AXT_BC3R						0x07				// cPCI bus, 3U size
#define AXT_BC6R						0x08				// cPCI bus, 6U size

// 모듈 정의
#define AXT_SMC_2V01					0x01				// CAMC-5M, 2 Axis
#define AXT_SMC_2V02					0x02				// CAMC-FS, 2 Axis
#define AXT_SMC_1V01					0x03				// CAMC-5M, 1 Axis
#define AXT_SMC_1V02					0x04				// CAMC-FS, 1 Axis
#define AXT_SMC_4V51					0x33				// MCX314,  4 Axis
#define AXT_SMC_2V53					0x35				// PMD, 2 Axis
#define AXT_SMC_2V54					0x36				// MCX312,  2 Axis
#define AXT_SIO_DI32					0x97				// Digital IN  32점
#define AXT_SIO_DO32P					0x98				// Digital OUT 32점
#define AXT_SIO_DB32P					0x99				// Digital IN 16점 / OUT 16점
#define AXT_SIO_DO32T					0x9E				// Digital OUT 16점, Power TR 출력
#define AXT_SIO_DB32T					0x9F				// Digital IN 16점 / OUT 16점, Power TR 출력
#define AXT_SIO_AI4R					0xA1				// A1h(161) : AI 4Ch, 12 bit
#define AXT_SIO_AO4R					0xA2				// A2h(162) : AO 4Ch, 12 bit
#define AXT_SIO_AI16H					0xA3				// A3h(163) : AI 4Ch, 16 bit
#define AXT_SIO_AO8H					0xA4				// A4h(164) : AO 4Ch, 16 bit
#define AXT_COM_234R					0xD3				// COM-234R
#define AXT_COM_484R					0xD4				// COM-484R

// Module header info.
#define REG_PREAMBLE					0x00				// Preamble		: B6h
#define REG_ID							0x02				// Module ID	: 97h, 98h, 99h
#define REG_VERSION						0x04				// Version		: 0.0
#define REG_SOFTWARE_RESET				0x06				// bit 0 : 1(hi)로 Write시 Software reset

// -------------------------------------------------------------------------------------
#define AXT_MODULE						4					// 베이스보드의 모듈 갯수 
#define MAX_AXT_BOARD					21					// 장착할 수 있는 보드의 갯수
#define MAX_AXT_MODULE					(MAX_AXT_BOARD*4)	// 장착할 수 있는 모듈의 갯수 
#define MAX_AXIS						(MAX_AXT_MODULE*2)	// 최대 모션 축의 갯수

#define DIO_MODULE_ALL					0					//$$
#define AIO_MODULE_ALL					0					//$$

// Sync 및 Trigger 관련 Register
#define AXT_SYNC_OFFSET					0x1800
#define AXT_BASE_EEPROM					0x1802				// <+> 2002/03/07
#define AXT_INTR_MASK					0x1804
#define AXT_INTR_FLAG					0x1806

// 모듈의 어드레스 - 오프셋
#define SUBMODULE0						0x0000				// Module 0 offset
#define SUBMODULE1						0x0400				// Module 1 offset
#define SUBMODULE2						0x0800				// Module 2 offset
#define SUBMODULE3						0x0C00				// Module 3 offset

//#define MODULE_NUM						4					// 베이스보드의 모듈 갯수

class CEzBasic;		//##

// 베이스보드 정보 : SMC/DIO Module
typedef struct {
	BOOL		fOpenDevice;								// Is Open device ?
	CEzBasic	*pEzBasic;									// EzBasic
	INT16		nBusType;									// 버스 종류
	INT16		nCardNo;									// 카드의 순서 : Only PCI, cPCI bus
	INT16		nIrqNo;										// IRQ no : Only ISA, VME bus
	BOOL		fEnableInterrupt;							// Is Enable interrupt ?
	UINT32		dwBaseAddr;									//
	INT16		nNumberOfAxis;								// Number of axis	=> only SMC
	INT16		BoardID;									// 베이스보드 ID : BIFR, ..., 
	INT16		BoardVersion;								// 베이스보드 Version	//<+> 2002.11.15 JNS
	UINT8		ModuleID[AXT_MODULE];						// Module ID
	UINT8		ModuleVersion[AXT_MODULE];					// Module Version		//<+> 2002.11.15 JNS
} BoardInfoStruct;

// only MCX312/MCX314 Module
typedef struct {
	BOOL   fOpenModule;
	INT16  nBoardNo;
	INT16  nModuleNo;
	INT16  ModuleID;
	UINT16 wModuleAddr;
	INT16  nNumberOfAxis;
} McxModuleInfoStruct;


// VALUE가 MIN과 MAX 사이의 값인가 ?
#ifndef InBound
#define InBound(MIN, MAX, VALUE)		((MIN) <= (VALUE) && (VALUE) <= (MAX) ? TRUE : FALSE)
#endif	// InBound

// VALUE가 MIN보다 작은 값이면 MIN, MAX보다 큰 값이면 MAX, MIN과 MAX사이의 값이면 VALUE를 리턴
#ifndef Bound
#define Bound(MIN, MAX, VALUE)			((MIN) > (VALUE) ? (MIN) : ((MAX) < (VALUE) ? (MAX) : (VALUE)))
#endif

// 반올림
#ifndef round
#define round(x)						(x >= 0)?(INT32)(x + 0.5):(INT32)(x -0.5)
#endif	// round

typedef struct _ERROR_MESSAGE_STRUCT {
		INT16  ErrorCode;									// Error code
		char  ErrorMsg[255];								// Error message
} ERROR_MESSAGE_STRUCT;

#ifndef AJIN_PREAMBLE
#define AJIN_PREAMBLE					0xB6				// Preamble : B6h
#endif	// AJIN_PREAMBLE


#ifndef WM_USER
#define WM_USER							0x0400
#endif	// WM_USER

// CAMC-5M Module
#define WM_CAMC5M_INTERRUPT				(WM_USER + 2001)
typedef void (*AXT_CAMC5M_INTERRUPT_PROC)(INT16 nBoardNo, INT16 nModulePos, UINT8 byFlag);
//Example : void C5MInterruptProc(INT16 nBoardNo, INT16 nModulePos, UINT8 byFlag);

// CAMC-FS Module
#define WM_CAMCFS_INTERRUPT				(WM_USER + 2002)
typedef void (*AXT_CAMCFS_INTERRUPT_PROC)(INT16 nBoardNo, INT16 nModulePos, UINT8 byFlag);
//Example : void CFSInterruptProc(INT16 nBoardNo, INT16 nModulePos, UINT8 byFlag);

// MCX-312 Module
#define WM_MCX312_INTERRUPT				(WM_USER + 2003)
typedef void (*AXT_MCX312_INTERRUPT_PROC)(INT16 nBoardNo, INT16 nModulePos, UINT16 dwFlag);
//Example : void M312InterruptProc(INT16 nBoardNo, INT16 nModulePos, UINT16 wFlag);

// MCX-314 Module
#define WM_MCX314_INTERRUPT				(WM_USER + 2004)
typedef void (*AXT_MCX314_INTERRUPT_PROC)(INT16 nBoardNo, INT16 nModulePos, UINT32 dwFlag);
//Example : void M314InterruptProc(INT16 nBoardNo, INT16 nModulePos, UINT32 dwFlag);

/* Undefine
// PMD Module
#define WM_PMD_INTERRUPT				(WM_USER + 2005)
typedef void (*AXT_PMD_INTERRUPT_PROC)(INT16 nBoardNo, INT16 nModulePos, UINT32 dwFlag);
//Example : void PmdInterruptProc(INT16 nChannelNo);
*/

// Comm Module
#define WM_COMM_INTERRUPT				(WM_USER + 2006)
typedef void (*AXT_COMM_INTERRUPT_PROC)(INT16 nChannelNo);
//Example : void CommInterruptProc(INT16 nChannelNo);

// DIO Module
#define WM_DIO_INTERRUPT				(WM_USER + 2007)
typedef void (*AXT_DIO_INTERRUPT_PROC)(INT16 nBoardNo, INT16 nModulePos, UINT32 dwFlag);
//Example : void DioInterruptProc(INT16 nBoardNo, INT16 nModulePos, UINT32 dwFlag);

// AIO Module
#define WM_AIO_INTERRUPT				(WM_USER + 2008)
typedef void (*AXT_AIO_INTERRUPT_PROC)(INT16 nChannelNo, INT16 nStatus);
//Example : void AioInterruptProc(INT16 nChannelNo, INT16 nStatus);


/* Type 설정	*/
#ifndef OFF
#define	OFF								0					// OFF=0
#define	ON								1					// ON=1
#endif	// OFF

//#ifndef BOOL
//#define BOOL    int						//INT16
//#endif

#ifndef FALSE
#define FALSE							0
#define TRUE							1
#endif	// TRUE

#ifndef LOW
#define LOW								0
#define HIGH							1
#endif	// HIGH

#ifndef DISABLE
#define DISABLE							0
#define ENABLE							1
#endif	// ENABLE


#endif	// __AXT_LIBRARY_DEFINE_H__
