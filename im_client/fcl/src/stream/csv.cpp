/******************************************************************
** 文件名:	csv.cpp
** 版  权:	(C)  T引擎开发组
** 创建人:	T
** 日  期:	2006-03-01   8:49
** 描  述:	一个读写csv的类
**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
********************************************************************/

#include "stdafx.h"
#include "profile/profile.h"
#include "trace/Trace.h"
#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable : 4996)
#endif

#include "stream/csv.h"
#include "stream/FileStream.h"

BEGIN_NAMESPACE_FCL

Csv::Csv(char separator) 
{
	m_separator = separator;
}

Csv::~Csv()
{
}

void Csv::close()
{
	m_recordList.clear();
	m_fieldDesc.clear();
	m_fieldType.clear();
}

bool Csv::getBool(int32 row, int32 col, bool def) const
{
	assert(getFieldType(col) == ftBool || getFieldType(col) == ftMacro);
	if (row < 0 || row >= (int32)m_recordList.size()
		|| col < 0 || col >= (int32)m_fieldType.size())
		return def;

	std::string& cell = (*m_recordList[row])[col];
	if (stricmp(cell.c_str(), "false") == 0 || stricmp(cell.c_str(), "0") == 0)
		return false;

	assert(stricmp(cell.c_str(), "true") == 0 || stricmp(cell.c_str(), "1") == 0);

	return true;
}

int32 Csv::getInt(int32 row, int32 col, int32 def) const
{
	assert(getFieldType(col) == ftInt || getFieldType(col) == ftMacro);
	if (row < 0 || row >= (int32)m_recordList.size()
		|| col < 0 || col >= (int32)m_fieldType.size())
		return def;

	std::string& cell = (*m_recordList[row])[col];

	return atoi(cell.c_str());
}

int64 Csv::getInt64(int32 row, int32 col, int64 def) const
{
	assert(getFieldType(col) == ftInt64 || getFieldType(col) == ftMacro);
	if (row < 0 || row >= (int32)m_recordList.size()
		|| col < 0 || col >= (int32)m_fieldType.size())
		return def;

	std::string& cell = (*m_recordList[row])[col];

	return _atoi64(cell.c_str());
}

float Csv::getFloat(int32 row, int32 col, float def) const
{
	assert(getFieldType(col) == ftFloat || getFieldType(col) == ftMacro);
	if (row < 0 || row >= (int32)m_recordList.size()
		|| col < 0 || col >= (int32)m_fieldType.size())
		return def;

	std::string& cell = (*m_recordList[row])[col];

	return (float)atof(cell.c_str());
}

const std::string& Csv::getString(int32 row, int32 col, const std::string& def) const
{
	assert(getFieldType(col) == ftString || getFieldType(col) == ftMacro);
	if (row < 0 || row >= (int32)m_recordList.size()
		|| col < 0 || col >= (int32)m_fieldType.size())
		return def;

	std::string& cell = (*m_recordList[row])[col];

	return cell;
}

int32 Csv::getFieldIndex(char* fieldDesc) const
{
	assert(isValidString(fieldDesc));

	for (uint32 i = 0; i < m_fieldDesc.size(); i++)
		if (stricmp(m_fieldDesc[i].c_str(), fieldDesc) == 0)
			return i;

	return -1;
}

const char* Csv::getFieldDesc(int32 col) const
{
	if (col >= 0 && col < (int32)m_fieldDesc.size())
		return m_fieldDesc[col].c_str();
	return NULL;
}

uint32 Csv::getFieldType(int32 col) const
{
	if (col >= 0 && col < (int32)m_fieldType.size())
		return m_fieldType[col];
	return (uint32)ftUnknow;
}

const char* Csv::getMacro(char* macroName) const
{
	MacroMap::const_iterator it = m_macroMap.find(macroName);
	if (it != m_macroMap.end())
		return ((*it).second).c_str();
	return NULL;
}

bool Csv::open(const char* filename)
{
	assert(isValidString(filename));
	CFileStream fs(filename);
	return open(&fs);
}

bool Csv::open(CStream* stream)
{
	PROFILE("LoadCsv");
	assert(stream != 0);
	bool isopen = stream->isOpen();
	if (!isopen)
		stream->open("r");

	if (stream->isOpen())
	{
#ifdef FCL_DEBUG
		Trace("Parsing csv file: %s\n", stream->getPath().c_str());
#endif
		const int32 MAX_LINE_LEN = 2048;
		std::string strBuffer;
		uint32 validLineCount = 0;
		char buffer[MAX_LINE_LEN];
		bool ret = stream->readString(buffer, MAX_LINE_LEN);
		while (ret)
		{
			int32 quoteCount = countChar(buffer, '"');
			strBuffer = buffer;
			while (quoteCount & 1) // 说明本行跟下一行要合并
			{
				ret = stream->readString(buffer, MAX_LINE_LEN);
				if (!ret)
				{
					Error("Csv::open(), csv file format error, quote count is odd!\n");
					goto parse_err;
				}
				quoteCount += countChar(buffer, '"');
				strBuffer += buffer;
			}

			// 检查注释行或者空行
			trim(strBuffer, " \t\r\n");

			if (!strBuffer.empty())
			{
#ifdef FCL_DEBUG
				if (*(Uint16*)&strBuffer[0] == '＃') // 中文＃号开头
				{
					Warning("Csv::open() error, you use chinese '＃'!\n");
				}
#endif
				if (strBuffer[0] == '#') // 扩展功能
				{
					if (6 < strBuffer.length() && strnicmp(&strBuffer[1], "define", 6) == 0) // #define
					{
						strBuffer.erase(0, 7);
						parseMacro(strBuffer); // 如果错误，忽略...
					}
					else if (7 < strBuffer.length() && strnicmp(&strBuffer[1], "include", 7) == 0) // #include
					{
						strBuffer.erase(0, 8);
						parseInclude(strBuffer); // 如果错误，忽略...
					}
				}
				else
				{
					// 添加有意义的行
					if (validLineCount == 0) // 添加字段描述行
					{
						if (m_fieldDesc.size() > 0)
						{
							Error("Csv::open(), the field description had been parse, maybe '#include' error!\n");
							goto parse_err;
						}
						parseFieldDesc(strBuffer);
						validLineCount++;
					}
					else if (validLineCount == 1) // 添加字段类型行
					{
						if (m_fieldType.size() > 0)
						{
							Error("Csv::open(), the field type had been parse, maybe '#include' error!\n");
							goto parse_err;
						}
						if (!parseFieldType(strBuffer))
							goto parse_err;
						validLineCount++;
					}
					else // 添加实际的数据记录
					{
						if (!parseRecord(strBuffer))
							goto parse_err;
					}
				}
			}


			// 读下一条记录
			ret = stream->readString(buffer, MAX_LINE_LEN);
		}
	}
	else
		return false;

	if (!isopen)
		stream->close();

	return true;

parse_err:
	if (!isopen)
		stream->close();
	return false;
}

bool Csv::save(char* filename)
{
	Error("Csv::save() does not implement!\n");
	return true;
}

void Csv::parseFieldDesc(std::string& line)
{
	size_t start_pos = std::string::npos;
	size_t end_pos = line.find_first_of(m_separator);
	do
	{
		std::string str = line.substr(start_pos + 1, end_pos - start_pos - 1);
#ifdef FCL_DEBUG
		if (str.length() == 0)
		{
			Warning("Csv::parseFieldDesc(), empty field desciption!\n");
		}
#endif
		m_fieldDesc.push_back(str);
		start_pos = end_pos;
		end_pos = line.find_first_of(m_separator, start_pos + 1);
		if (end_pos == std::string::npos && start_pos < line.length())
			end_pos = line.length();
	} while (end_pos != std::string::npos); // find
}

bool Csv::parseFieldType(std::string& line)
{
	size_t start_pos = std::string::npos;
	size_t end_pos = line.find_first_of(m_separator);
	do
	{
		std::string str = line.substr(start_pos + 1, end_pos - start_pos - 1);
		if (stricmp(str.c_str(), "bool") == 0)
			m_fieldType.push_back((int32)ftBool);
		else if (stricmp(str.c_str(), "int") == 0)
			m_fieldType.push_back((int32)ftInt);
		else if (stricmp(str.c_str(), "int64") == 0)
			m_fieldType.push_back((int32)ftInt64);
		else if (stricmp(str.c_str(), "float") == 0)
			m_fieldType.push_back((int32)ftFloat);
		else if (stricmp(str.c_str(), "string") == 0)
			m_fieldType.push_back((int32)ftString);
		else if (stricmp(str.c_str(), "macro") == 0)
			m_fieldType.push_back((int32)ftMacro);
		else
		{
			Error("Csv::parseFieldType(), invalid field type: %s\n", str.c_str());
			//m_fieldType.push_back((int32)ftUnknow);
			return false;
		}

		start_pos = end_pos;
		end_pos = line.find_first_of(m_separator, start_pos + 1);
		if (end_pos == std::string::npos && start_pos < line.length())
			end_pos = line.length();
	} while (end_pos != std::string::npos); // find
	return true;
}

// 已经去掉了前面的"#define"
bool Csv::parseMacro(std::string& exp)
{
	std::string space = " \t";
	space += m_separator;
	trim(exp, space);
	size_t pos = exp.find_first_of('=');
	size_t len = exp.length();
	if (pos > 0 && pos < len)
	{
		std::string name = exp.substr(0, pos);
		std::string value = exp.substr(pos + 1);
		trim(name, " \t");
		trim(value, " \t");
		m_macroMap[name] = value;
		return true;
	}

	Warning("Csv::parseMacro(), invalid macro '#define %s'\n", exp.c_str());
	return false;
}

// 已经去掉了前面的"#include"
bool Csv::parseInclude(std::string& filename)
{
	std::string space = " \t";
	space += m_separator;
	trim(filename, space);
	return open(filename.c_str());
}

bool Csv::parseRecord(std::string& line)
{
	if (line.empty())
		return true;

	const char* str = line.c_str();
	int32 len = (int32)line.length();
	int32 pos = -1;
	int32 quoteCount = 0;		// 引号的数目
	int32 separatorCount = 0;	// 分隔符数目（一个单元格里面的分隔符不统计在内）
	int32 last_separator_pos = -1; // 上一个分隔符位置
	int32 maxFieldCount = (int32)m_fieldType.size(); // 我们约定以字段类型定义的那行的字段数为字段的最大数目
	int32 row = (int32)m_recordList.size();
	int32 fieldCount = 0;
	StringList* record = new StringList;

	while (str[++pos])
	{
		if (str[pos] == m_separator) // 分隔符
		{
			if ((quoteCount & 1) == 0) // 如果引号数目为奇数，说明该分隔符不是字段的分隔符，是单元格内部的分隔符，否则为字段分隔符
			{
				if (++fieldCount < maxFieldCount)
				{
					std::string field = line.substr(last_separator_pos + 1, pos - last_separator_pos - 1);
					//Trace("fieldIdx=%d, text=%s\n", separatorCount, field.c_str());
					parseField(field, record, row);
					separatorCount++;
					last_separator_pos = pos;
				}
				else
				{
					Warning("format error: field count too more!\n");
				}
			}
		}
		else if (str[pos] == '"') // 引号
		{
			quoteCount++;
		}
	}

	// 最后一个记录后面没有分隔符
	if (last_separator_pos <= len - 1 && (quoteCount & 1) == 0 && fieldCount + 1 <= maxFieldCount)
	{
		std::string field = line.substr(last_separator_pos + 1, pos - last_separator_pos - 1);
		//Trace("fieldIdx=%d, text=%s\n", separatorCount, field.c_str());
		parseField(field, record, row);
	}

	int32 delta = maxFieldCount - (int32)record->size();
	if (delta > 0)
	{
		for (int32 i = 0; i < delta; i++)
			record->push_back("");
	}

	m_recordList.push_back(record);

	return true;
}

// 传入的字符串可能带引号格式
bool Csv::parseField(std::string& field, StringList* record, int32 row)
{
	const char* str = field.c_str();

	// 看看是否前后有引号
	if (str[0] == '"')
	{
		if (str[field.length() - 1] == '"')
		{
			field.erase(field.length() - 1);
			field.erase(0, 1);
		}
		else
		{
			Error("format error!\n");
			return false;
		}
	}

	str = field.c_str();
	int32 pos = -1;
	int32 len = (int32)field.length();
	while (str[++pos])
	{
		if (str[pos] == '"')
		{
			if (pos + 1 < len && str[pos + 1] == '"') // 有2个连续的引号，当一个引号用
			{
				field.erase(pos + 1, 1);
			}
			else
			{
				Error("format error!\n");
				return false;
			}
		}
	}

	if (field.empty())
	{
		record->push_back("");
		return true;
	}

	// 是否是宏替换？
	if (m_fieldType[record->size()] == ftMacro)
	{
		MacroMap::iterator it = m_macroMap.find(field);
		if (it != m_macroMap.end())
		{
			field = (*it).second;
		}
		else
		{
			Warning("Csv::parseField(), macro[%s] had not been defined in the unit[%d, %d], will keep original string!\n", field.c_str(), row, record->size());
		}
	}
	record->push_back(field);
	return true;
}

void Csv::trimLeft(std::string& str, const std::string& space)
{
	if (str.empty() || space.empty())
		return;

	int32 pos = -1;
	while (str[++pos])
	{
		int32 inpos = -1;
		while (space[++inpos] && space[inpos] != str[pos]);
		if (inpos >= (int32)space.length()) // not found
			break;
	}

	if (pos > 0)
		str.erase(0, pos);
}

void Csv::trimRight(std::string& str, const std::string& space)
{
	if (str.empty() || space.empty())
		return;

	int32 pos = (int32)str.length();
	while (--pos >= 0)
	{
		int32 inpos = -1;
		while (space[++inpos] && space[inpos] != str[pos]);
		if (inpos >= (int32)space.length()) // not found
			break;
	}

	if (pos < (int32)str.length() - 1)
		str.erase(pos + 1);
}

void Csv::trim(std::string& str, const std::string& space)
{
	trimLeft(str, space);
	trimRight(str, space);
}

int32 Csv::countChar(char* str, char c)
{
	int32 inpos = -1;
	int32 count = 0;
	while (str[++inpos])
	{
		if (str[inpos] == c)
			count++;
	}
	
	return count;
}

END_NAMESPACE

#pragma warning(pop)
