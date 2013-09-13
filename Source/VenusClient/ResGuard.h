#ifndef __RES_GUARD_H__
#define __RES_GUARD_H__

///////////////////////////////////////////////////////////////////////////////

// Instances of this class will be accessed by multiple threads. So,
// all members of this class (except the constructor and destructor)
// must be thread-safe.
class CResGuard
{
public:
	CResGuard()
	{
		m_lGrdCnt = 0;
		InitializeCriticalSection(&m_cs);
	}
	~CResGuard()
	{
		DeleteCriticalSection(&m_cs);
	}

	// IsGuarded is used for debugging
	BOOL IsGuarded() const
	{
		return(m_lGrdCnt > 0);
	}

public:
	class CGuard
	{
	public:
		CGuard(CResGuard &rg) : m_rg(rg)
		{
			m_rg.Guard();
		};
		~CGuard()
		{
			m_rg.Unguard();
		}

	private:
		CResGuard &m_rg;
	};

private:
	void Guard()
	{
		EnterCriticalSection(&m_cs);
		m_lGrdCnt++;
	}
	void Unguard()
	{
		m_lGrdCnt--;
		LeaveCriticalSection(&m_cs);
	}

	// Guard/Unguard can only be accessed by the nested CGuard class.
	friend class CResGuard::CGuard;

private:
	CRITICAL_SECTION m_cs;
	long m_lGrdCnt;   // # of EnterCriticalSection calls
};

#endif