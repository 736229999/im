/******************************************************************
** 文件名:	csv.h
** 版  权:	(C)  
** 创建人:	
** 日  期:	2007-5-11
** 描  述:	一个读写csv的类
**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
********************************************************************/

#ifndef __CSV_H__
#define __CSV_H__

#include "Config.h"
#include "stream/Stream.h"
#include <vector>
#include <string>

#	pragma warning(push)
#	pragma warning(disable : 4996)
#	include <hash_map>
	using namespace std;
/* 文件格式示例
// 宏定义文件(example.def)
#version = 1.0.0.2
#define yo1 = 30
#define yo2 = 24
#define yo3 = 60
#define yo4 = 18

//csv文件(example.csv)
#version = 1.0.0.2
#include example.def
名字,性别,年龄,婚否,财产,身份证
string,macro,int,bool,float,int64
张三,男,yo1,true,532467.85,123456789456213452
李四,女,yo2,0,23685.88,123789789456213452
王五,男,yo3,1,501478.00,321456789456213452
赵六,女,yo4,false,11328.99,123444789456213452

格式要点：
1、基本以Excel的存储格式为准，并进行了扩充：
a、支持注释；
b、支持文件的include；
c、支持宏定义define；

2、无效行：
a、以#号开头的行(#号前可以包括任意的空白符和制表符)，用于注释，解析时会忽略掉，如果是中文＃号在调试版下会
	输出警告并返回错误；
b、空行或者只有空白符和制表符组合成的行，也会忽略掉；

3、有效行：
a、字段描述行，有意义，但不属于通过记录索引能够访问的内容，本格式强制要求有字段描述行，它总是第一个有效行；
b、字段类型行，有意义，但不属于通过记录索引能够访问的内容，本格式强制要求有字段类型行，它总是第二个有效行，
	字段数以之为准，调试版会进行一些辅助的类型校验，目前支持bool,int,int64,float,string,marco几种类型；
c、记录行，有意义，保存了每条记录，能通过记录索引进行访问，从第三个有效行开始的有效行都是记录行；

4、宏定义：
a、以#define(注意，是英文#，单词define建议全为小写字母)开头的为宏定义，宏定义的值默认都为字符串，不要加引号！
b、如果采用了宏定义，应该将需要使用宏的整列全部用宏(否则将忽略宏替换，但发生类型转换时不保证没有错误的发生)，
	并且将字段的类型置为macro；

5、包含定义：
a、我们扩充了格式，使之支持包含命令，如：
	#include res/config/item.def
b、包含指令的写法仿照上面示例，注意路径是以当前工作目录为准（不是文件所在目录），路径不要加引号；
c、该功能主要用于将宏定义写在另一个文件中，这样csv文件比较干净，而且可以几个csv包含同一个宏定义文件；

6、其他：
a、bool类型支持true,false表达也支持1,0来表达；
b、一条记录占一行；
c、每个字段之间用分隔符进行分隔，分隔符可以自定义，默认用英文逗号','，不支持中文逗号；
d、每行字节长度不超过2048字节(不会变态到写那么长吧？！)；
e、单元格支持包含分隔符；
f、单元格可以包含回车换行符；

 */

BEGIN_NAMESPACE_FCL

/** 一个Excel的CSV(我们加入了部分扩充特性)文件读取类，暂不支持保存 */
class FCL_CLASS Csv
{
protected:
	typedef std::vector<std::string>		StringList;
	typedef hash_map<string, std::string>	MacroMap;
	typedef std::vector<int32>				FieldTypeList;
	typedef std::vector<StringList*>		RecordList;

	RecordList		m_recordList;	/// 记录列表(不保存注释行和空行)
	StringList		m_fieldDesc;	/// 字段描述
	FieldTypeList	m_fieldType;	/// 字段类型
	char			m_separator;	/// 字段间的分隔符
	//int32				m_curRow;		/// 当前操作的行(用于加速访问下一个字段)
	//int32				m_curCol;		/// 当前操作的列(用于加速访问下一个字段)
	//int32				m_curPos;		/// 当前字段的结束位置(用于加速访问下一个字段)
	MacroMap		m_macroMap;		/// 宏映射表

public:
	Csv(char separator = ',');
	virtual ~Csv();

	enum FieldType{ftUnknow = 0,ftBool,ftInt,ftInt64,ftFloat,ftString,ftMacro};

public:
	/** 从文件中打开一个CSV文件
	 @param filename 标准文件名
	 @return 成功返回true，否则返回false
	 */
	bool open(const char* filename);

	/** 从流中打开一个CSV文件
	 @param stream 流对象
	 @return 成功返回true，否则返回false
	 */
	virtual bool open(CStream* stream);

	/** 保存为CSV文件
	 @param filename 要保存的文件名
	 @return 成功返回true，否则返回false
	 */
	virtual bool save(char* filename);

	/// 关闭CSV操作
	void close();

	/** 得到某一行(已经去掉了末尾可能的回车换行符)
	 @param row 行索引，从0开始，包括各种无意义的行在内
	 @return 成功返回对应字符串，当索引越界返回NULL
	 */
	//char* getLine(uint32 row);

	/** 得到bool值
	 @param row 有效行索引，从0开始
	 @param col 列索引，从0开始
	 @param def 默认参数，当发生错误或者定位的值未设定时，直接返回默认值
	 @retval 返回相应的bool值
	 */
	bool getBool(int32 row, int32 col, bool def = false) const;

	/** 得到int32值
	 @param row 有效行索引，从0开始
	 @param col 列索引，从0开始
	 @param def 默认参数，当发生错误或者定位的值未设定时，直接返回默认值
	 @retval 返回相应的int32值
	 */
	int32 getInt(int32 row, int32 col, int32 def = 0) const;

	/** 得到int64值
	 @param row 有效行索引，从0开始
	 @param col 列索引，从0开始
	 @param def 默认参数，当发生错误或者定位的值未设定时，直接返回默认值
	 @retval 返回相应的int64值
	 */
	int64 getInt64(int32 row, int32 col, int64 def = 0) const;

	/** 得到float值
	 @param row 有效行索引，从0开始
	 @param col 列索引，从0开始
	 @param def 默认参数，当发生错误或者定位的值未设定时，直接返回默认值
	 @retval 返回相应的float值
	 */
	float getFloat(int32 row, int32 col, float def = 0.0f) const;

	/** 得到string值
	 @param row 有效行索引，从0开始
	 @param col 列索引，从0开始
	 @param def 默认参数，当发生错误或者定位的值未设定时，直接返回默认值
	 @retval 返回相应的string值
	 */
	const std::string& getString(int32 row, int32 col, const std::string& def = "") const;

public:
	/** 设置分隔符
	 @param separator 分隔符号
	 @retval 返回先前的分隔符
	 */
	void setSeparator(char separator)	{ m_separator = separator; }

	/// 获得分隔符
	char getSeparator() const	{ return m_separator; }

	/// 获得记录数
	uint32 getRecordCount() const	{ return (uint32)m_recordList.size(); }

	/// 获得字段的数目
	uint32 getFieldCount() const	{ return (uint32)m_fieldType.size(); }

	/** 通过字段描述获得对应的列索引
	 @param fieldDesc 字段描述字符串
	 @retval 返回相应的列索引，如果失败，则返回-1；
	 */
	int32 getFieldIndex(char* fieldDesc) const;

	/** 通过列索引获取对应的字段描述
	 @param col 列索引
	 @retval 返回相应的字段描述字符串，如果列索引越界，则返回NULL；
	 */
	const char* getFieldDesc(int32 col) const;

	/** 通过列索引获得对应的字段类型
	 @param col 列索引
	 @retval 返回相应的列索引，如果失败，则返回ftUnknow；
	 */
	uint32 getFieldType(int32 col) const;

	/** 获取指定的宏的值
	 @param macroName 要获取的宏的名称
	 @retval 对应宏的值，如果不存在返回NULL
	 */
	const char* getMacro(char* macroName) const;

private:
	void parseFieldDesc(std::string& line);
	bool parseFieldType(std::string& line);
	bool parseMacro(std::string& exp);
	bool parseInclude(std::string& filename);
	bool parseRecord(std::string& line);
	bool parseField(std::string& field, StringList* record, int32 row);
public:
	void trimLeft(std::string& str, const std::string& space);
	void trimRight(std::string& str, const std::string& space);
	void trim(std::string& str, const std::string& space);
	int32 countChar(char* str, char c);
};


#	pragma warning(pop)

END_NAMESPACE
#endif // __CSV_H__
