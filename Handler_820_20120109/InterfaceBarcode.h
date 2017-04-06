#pragma once

class CComm;


class CInterfaceBarcode
{
public:
	int				m_nBarcodeCount;

	DWORD			m_dwWaitTime[3];

	CString			m_strBarcode[100];

	int				m_nThreadStep;
	int				m_nComPort;			// 통신 포트
	int				m_nBaudRate;		// 통신 속도
	int				m_nDataBit;			// 데이터 비트
	int				m_nStopBit;			// stop 비트
	int				m_nParityBit;		// parity 비트

	BYTE			m_byEnd;

	bool			m_bConnect;			// rs-232c 연결 상태
	bool			m_bThread;

	HANDLE			m_thrHandle;

	CComm			*m_pCom;			// 통신 class 포인터

	bool			OnOpen(int nPort, int nBaudRate, int nParityBit, int nDataBit, int nStopBit, BYTE byEnd);
	void			OnClose();
	void			OnBarcodeReset();
	int				OnBarcodeCount();
	CString			OnBarcodeReadData(int nNum);
	void			OnDataSend(CString strData);
	void			OnDataRevice(CString strData);
public:
	CInterfaceBarcode(void);
	~CInterfaceBarcode(void);
};

extern CInterfaceBarcode clsBarcode;


