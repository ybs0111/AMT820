#pragma once

template<class T>
class CMultiThreadSync
{
	friend class CThreadSync;
	public:
		class CThreadSync
		{
		public:
			CThreadSync(VOID)
			{
				T::mSync.Enter();
			}

			~CThreadSync(VOID)
			{
				T::mSync.Leave();
			}
		};

		public:
			static CTRCriticalSection mSync;
};

template <class T>
CTRCriticalSection CMultiThreadSync<T>::mSync;