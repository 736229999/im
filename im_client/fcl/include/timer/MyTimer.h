#ifndef __TICK_TIMER__
#define __TICK_TIMER__
#include <Windows.h>
//测时类，用于测时函数或对象等的执行时间


typedef enum
{
	RES1 = 1,           //计时单位为秒
	RES2 = (1000 * RES1),  //计时单位为毫秒
	RES3 = (1000 * RES2)   //计时率单位为微秒
}TR;

template<class,TR> class CPUTick; //前向声明
template<class,TR> class SystemTick;

template<template<class,TR> class T = CPUTick ,class R = int64, TR RES = RES3/*默认为微秒*/>
class NewTimer
{
public:
	typedef R returnType;
	returnType getTick()
	{
		return (returnType)ti.getTick();
	}
	returnType getFrequency()
	{
		return (returnType)ti.getFrequency();
	}
private:
	T<R,RES> ti;

};

template<class R>
class NewTimer<SystemTick,R,RES2>
{
public:
	typedef R returnType;
	returnType getTick()
	{
		return (returnType)ti.getTick();
	}
	returnType getFrequency()
	{
		return (returnType)ti.getFrequency();
	}
private:
	SystemTick<R,RES2> ti;

};



template<class R>
struct Tim
{
	typedef R returnType;
    virtual returnType  getTick()=0;
};


//时间精度可为秒，毫秒，微妙
template<class R,TR RES>
class CPUTick:public Tim<R>
{
	typedef R returnType;  //返回类型
public:
	CPUTick(){
		::QueryPerformanceFrequency(&fre);
		fre.QuadPart /= RES;
	}
	returnType getTick()
	{
		LARGE_INTEGER tmp;
		::QueryPerformanceCounter(&tmp);
		return (returnType)tmp.QuadPart;
	}
	returnType getFrequency()
	{
		return (returnType) (fre.QuadPart);
	}
private:
	LARGE_INTEGER fre;
};



//时间精度为毫秒
template<class R,TR RES>
class SystemTick:public Tim<R>
{
	typedef R returnType;
public:
	returnType getTick()
	{
		return (returnType)GetTickCount();
	}
};

//template<class R>
//class SystemTick<R,RES1>:public Tim<R>
//{  
//typedef R returnType;
//public:
//	returnType getTick()
//	{
//		
//		return (returnType)GetTickCount()/RES1;
//	}
//
//};


//template<class R>
//class SystemTick<R,RES2>:public Tim<R>
//{
////注意：此处为偏特化，不支持RES2分辩率
////	typedef R returnType;
////public:
////	returnType getTick()
////	{
////		return (returnType)GetTickCount()/1;
////	}
//
//};
//template<class R>
//class SystemTick<R,RES3>:public Tim<R>
//{
////注意：此处为偏特化，不支持RES3分辩率
////	typedef R returnType;
////public:
////	returnType getTick()
////	{
////		return (returnType)GetTickCount()/1;
////	}
//
//};




#endif/*__TICK_TIMER__*/