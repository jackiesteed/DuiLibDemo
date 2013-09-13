#ifndef __SINGLETON_H__
#define __SINGLETON_H__


#include <memory>
#include "ResGuard.h"
using namespace std;



template <class T>
class Singleton
{
public:
	static inline T *Instance();

private:
	Singleton(void) {}
	~Singleton(void) {}
	Singleton(const Singleton &) {}
	Singleton &operator= (const Singleton &) {}

	static auto_ptr<T> _instance;
	static CResGuard _rs;
};

//Class that will implement the singleton mode,
//must use the macro in it's delare file
#define DECLARE_SINGLETON_CLASS( type ) \
	friend class auto_ptr< type >;\
	friend class Singleton< type >;


template <class T>
auto_ptr<T> Singleton<T>::_instance;

template <class T>
CResGuard Singleton<T>::_rs;

template <class T>
inline T *Singleton<T>::Instance()
{
	if( 0 == _instance.get() )
	{
		CResGuard::CGuard gd(_rs);
		if( 0 == _instance.get())
		{
			_instance.reset ( new T);
		}
	}
	return _instance.get();
}

#endif