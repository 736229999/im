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

	/// 使用保护构造是为了用户不能声明一个实例
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

	static T*	_instance;		/// 实例静态指针
};

template<class T,bool mustDelete>
 T* CSingleton<T,mustDelete>::_instance = NULL;


#endif /*__FCL_SINGLE__*/