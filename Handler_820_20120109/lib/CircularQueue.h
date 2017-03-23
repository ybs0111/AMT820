#pragma once

#define MAX_QUEUE_LENGTH	50
#define MAX_BUFFER_LENGTH	4096
/////////////////////////
// 사용할 큐 선언
typedef struct _QUEUE_DATA
{
	VOID	*Object;
	BYTE	Data[MAX_BUFFER_LENGTH];
	DWORD	DataLength;

	CHAR	RemoteAddress[32];
	USHORT	RemotePort;

	DWORD	Protocol;
} QUEUE_DATA;

//template<class T>
class CCircularQueue : public CMultiThreadSync<CCircularQueue>
{
public:
	CCircularQueue(VOID);
	~CCircularQueue(VOID);

private:
	QUEUE_DATA	mQueue[MAX_QUEUE_LENGTH];
	DWORD		mQueueHead;
	DWORD		mQueueTail;

public:
	BOOL		Begin(VOID);
	BOOL		End(VOID);

	BYTE*		Push(VOID *object, BYTE *data, DWORD dataLength);
	BYTE*		Push(VOID *object, BYTE *data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort);
	BYTE*		Push(VOID *object, DWORD protocol, BYTE *data, DWORD dataLength);
	BYTE*		Push(VOID *object, DWORD protocol, BYTE *data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort);

	BOOL		Pop(VOID **object, BYTE *data, DWORD &dataLength);
	BOOL		Pop(VOID **object, BYTE *data, DWORD &dataLength, LPSTR remoteAddress, USHORT &remotePort);
	BOOL		Pop(VOID **object, DWORD &protocol, BYTE *data, DWORD &dataLength);
	BOOL		Pop(VOID **object, DWORD &protocol, BYTE *data, DWORD &dataLength, LPSTR remoteAddress, USHORT &remotePort);
	BOOL		Pop(VOID);

	BOOL		IsEmpty(VOID);
};


//CCircularQueue<QUEUE_DATA>cQueue;

