#ifndef __TICK_TIMER__
#define __TICK_TIMER__
#include <Windows.h>
//��ʱ�࣬���ڲ�ʱ���������ȵ�ִ��ʱ��


typedef enum
{
	RES1 = 1,           //��ʱ��λΪ��
	RES2 = (1000 * RES1),  //��ʱ��λΪ����
	RES3 = (1000 * RES2)   //��ʱ�ʵ�λΪ΢��
}TR;

template<class,TR> class CPUTick; //ǰ������
template<class,TR> class SystemTick;

template<template<class,TR> class T = CPUTick ,class R = int64, TR RES = RES3/*Ĭ��Ϊ΢��*/>
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


//ʱ�侫�ȿ�Ϊ�룬���룬΢��
template<class R,TR RES>
class CPUTick:public Tim<R>
{
	typedef R returnType;  //��������
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



//ʱ�侫��Ϊ����
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
////ע�⣺�˴�Ϊƫ�ػ�����֧��RES2�ֱ���
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
////ע�⣺�˴�Ϊƫ�ػ�����֧��RES3�ֱ���
////	typedef R returnType;
////public:
////	returnType getTick()
////	{
////		return (returnType)GetTickCount()/1;
////	}
//
//};




#endif/*__TICK_TIMER__*/