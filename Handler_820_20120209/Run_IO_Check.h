// Run_IO_Check.h: interface for the CRun_IO_Check class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUN_IO_CHECK_H__6686C693_3A31_4325_AB22_1011FD3AF1EF__INCLUDED_)
#define AFX_RUN_IO_CHECK_H__6686C693_3A31_4325_AB22_1011FD3AF1EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRun_IO_Check  
{
public:
	void Thread_Run();
	CRun_IO_Check();
	virtual ~CRun_IO_Check();

};

extern CRun_IO_Check Run_IO_Check;
#endif // !defined(AFX_RUN_IO_CHECK_H__6686C693_3A31_4325_AB22_1011FD3AF1EF__INCLUDED_)
