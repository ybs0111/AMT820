// APartErrReport.h: interface for the APartErrReport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APARTERRREPORT_H__3BBD87E7_CCB4_4DB2_B7F2_8550C6F76947__INCLUDED_)
#define AFX_APARTERRREPORT_H__3BBD87E7_CCB4_4DB2_B7F2_8550C6F76947__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMiniDump
{
public:
	static BOOL Begin(VOID);
	static BOOL End(VOID);
};

class APartErrReport  
{
public:
	APartErrReport();
	virtual ~APartErrReport();

};

#endif // !defined(AFX_APARTERRREPORT_H__3BBD87E7_CCB4_4DB2_B7F2_8550C6F76947__INCLUDED_)
