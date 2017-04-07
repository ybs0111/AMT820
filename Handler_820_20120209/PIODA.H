#include "Windows.h"

#ifndef EXPORTS
#ifdef  __cplusplus
	#define EXPORTS extern "C" __declspec (dllimport)
#else
	#define EXPORTS __declspec (dllimport)
#endif 
#endif

#ifndef __PIO_DA_HEADER__
#define __PIO_DA_HEADER__

// return code
#define PIODA_NoError 			        0
#define PIODA_DriverOpenError	  	    1
#define PIODA_DriverNoOpen		        2
#define PIODA_GetDriverVersionError	    3
#define PIODA_InstallIrqError	    	4
#define PIODA_ClearIntCountError    	5
#define PIODA_GetIntCountError    	    6
#define PIODA_RegisterApcError	    	7
#define PIODA_RemoveIrqError	    	8
#define PIODA_FindBoardError        	9
#define PIODA_ExceedBoardNumber     	10
#define PIODA_ResetError            	11

#define PIODA_EEPROMDataError           12
#define PIODA_EEPROMWriteError          13


// to trigger a interrupt when high -> low
#define PIODA_ActiveLow                 0
// to trigger a interrupt when low -> high
#define PIODA_ActiveHigh                1

// ID
#define PIO_DA                        0x800400   

// Test functions
EXPORTS float  CALLBACK PIODA_FloatSub(float fA, float fB);
EXPORTS short  CALLBACK PIODA_ShortSub(short nA, short nB);
EXPORTS WORD   CALLBACK PIODA_GetDllVersion(void);

// Driver functions
EXPORTS WORD   CALLBACK PIODA_DriverInit(void);
EXPORTS void   CALLBACK PIODA_DriverClose(void);
EXPORTS WORD   CALLBACK PIODA_SearchCard
               (WORD *wBoards, DWORD dwPIOCardID);
EXPORTS WORD   CALLBACK PIODA_GetDriverVersion(WORD *wDriverVersion);
EXPORTS WORD   CALLBACK PIODA_GetConfigAddressSpace
               (WORD wBoardNo,    DWORD *wAddrBase,  WORD *wIrqNo,
                WORD *wSubVendor, WORD  *wSubDevice, WORD *wSubAux,
                WORD *wSlotBus,   WORD  *wSlotDevice );
EXPORTS WORD   CALLBACK PIODA_ActiveBoard( WORD wBoardNo );
EXPORTS WORD   CALLBACK PIODA_WhichBoardActive(void);
EXPORTS WORD   CALLBACK PIODA_SetCounter(WORD wBoardNo, 
               WORD wWhichCounter, WORD bConfig, DWORD wValue);
EXPORTS DWORD  CALLBACK PIODA_GetBaseAddress(WORD wBoardNo);

// EEPROM functions
EXPORTS WORD   CALLBACK PIODA_EEP_READ
               (WORD wBoardNo, WORD wOffset, WORD *bHi, WORD *bLo);
EXPORTS WORD   CALLBACK PIODA_EEP_WR_EN(WORD wBoardNo);
EXPORTS WORD   CALLBACK PIODA_EEP_WR_DIS(WORD wBoardNo);
EXPORTS WORD   CALLBACK PIODA_EEP_WRITE
               (WORD wBoardNo, WORD wOffset, WORD HI, WORD LO);

// DA functions
//The D/A output for the PIODA_Voltage and PIODA_Current function is none calibration function
//EXPORTS WORD   CALLBACK PIODA_Voltage
//               (WORD wBoardNo, WORD wChannel, float fValue);
//EXPORTS WORD   CALLBACK PIODA_Current
//               (WORD wBoardNo, WORD wChannel, float fValue);
EXPORTS WORD   CALLBACK PIODA_CalVoltage
               (WORD wBoardNo, WORD wChannel, float fValue);
EXPORTS WORD   CALLBACK PIODA_CalCurrent
               (WORD wBoardNo, WORD wChannel, float fValue);

// DIO functions
EXPORTS void   CALLBACK PIODA_OutputWord
               (DWORD wBaseAddress, DWORD wOutData);
EXPORTS void   CALLBACK PIODA_OutputByte
               (DWORD wBaseAddress, WORD bOutputValue);
EXPORTS DWORD  CALLBACK PIODA_InputWord(DWORD wBaseAddress);
EXPORTS WORD   CALLBACK PIODA_InputByte(DWORD wBaseAddress);
EXPORTS WORD   CALLBACK PIODA_DI(WORD wBoardNo, DWORD *wVal);
EXPORTS WORD   CALLBACK PIODA_DO(WORD wBoardNo, DWORD wDO);

// Interrupt functions
EXPORTS WORD   CALLBACK PIODA_IntInstall
               (WORD wBoardNo, HANDLE *hEvent, 
			    WORD wInterruptSource, WORD wActiveMode);
EXPORTS WORD   CALLBACK PIODA_IntRemove(void);
EXPORTS WORD   CALLBACK PIODA_IntResetCount(void);
EXPORTS WORD   CALLBACK PIODA_IntGetCount(DWORD *dwIntCount);

#endif // end define __PIO_DA_HEADER__

