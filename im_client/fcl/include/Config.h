#ifndef __FCL_CONFIG__
#define __FCL_CONFIG__

#include <assert.h>

#ifdef FCL_EXPORTS
#define FCL_CLASS  __declspec(dllexport)
#define FCL_DECL   FCL_CLASS
#define FCL_API    FCL_CLASS
#else
#define FCL_CLASS  __declspec(dllimport)
#define FCL_DECL   FCL_CLASS
#define FCL_API    FCL_CLASS
#endif

///命名空间管理
#define BEGIN_NAMESPACE(x)    namespace x{
#define BEGIN_NAMESPACE_FCL   namespace fcl{
#define END_NAMESPACE           };
#define USE_NAMESPACE(x)		using namespace x;
#define USE_NAMESPACE_FCL		using namespace FCL;
#define BEGIN_API      extern "C" {
#define END_API        };
///宏定义
#ifdef _DEBUG
#define fclAssert(exp)			assert(exp)
#define fclVerify(exp)			fclAssert(exp)
#else
#define fclAssert(exp)			((void)0)
#define fclVerify(exp)			(exp);
#endif

//支持性能测试
//#define SUPPORT_PROFILE    
//支持VC调试器内输出Trace信息
#define TRACE_SINK_VC
//类型定义

//有符号数据定义
typedef  char           int8;
typedef  short int      int16;
typedef  int            int32;
typedef  __int64        int64;
//无符号数据定义
typedef  unsigned char       uint8;
typedef  unsigned short int  uint16;
typedef  unsigned int        uint32;
typedef  unsigned __int64    uint64;




#endif /*__FCL_CONFIG*/