#ifndef __FCL_SINGLE__
#define __FCL_SINGLE__

//#include <assert.h>

template<class T, bool mustDelete = true>
class CSingleton
{
public:

	static T* instance()
	{
		if (!_instance)
		{
			_instance = new T;
			assert(_instance);
			if (mustDelete)
				atexit(releaseInstance);
		}

		return _instance;
	}

protected:

	/// ʹ�ñ���������Ϊ���û���������һ��ʵ��
	CSingleton()
	{
	}
	static void  releaseInstance()
	{
		if (_instance && mustDelete)
		{
			delete _instance;
			_instance = NULL;
		}
	}

	static T*	_instance;		/// ʵ����ָ̬��
};

template<class T,bool mustDelete>
 T* CSingleton<T,mustDelete>::_instance = NULL;


#endif /*__FCL_SINGLE__*/