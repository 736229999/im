/******************************************************************
** �ļ���:	csv.h
** ��  Ȩ:	(C)  
** ������:	
** ��  ��:	2007-5-11
** ��  ��:	һ����дcsv����
**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
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
/* �ļ���ʽʾ��
// �궨���ļ�(example.def)
#version = 1.0.0.2
#define yo1 = 30
#define yo2 = 24
#define yo3 = 60
#define yo4 = 18

//csv�ļ�(example.csv)
#version = 1.0.0.2
#include example.def
����,�Ա�,����,���,�Ʋ�,���֤
string,macro,int,bool,float,int64
����,��,yo1,true,532467.85,123456789456213452
����,Ů,yo2,0,23685.88,123789789456213452
����,��,yo3,1,501478.00,321456789456213452
����,Ů,yo4,false,11328.99,123444789456213452

��ʽҪ�㣺
1��������Excel�Ĵ洢��ʽΪ׼�������������䣺
a��֧��ע�ͣ�
b��֧���ļ���include��
c��֧�ֺ궨��define��

2����Ч�У�
a����#�ſ�ͷ����(#��ǰ���԰�������Ŀհ׷����Ʊ��)������ע�ͣ�����ʱ����Ե�����������ģ����ڵ��԰��»�
	������沢���ش���
b�����л���ֻ�пհ׷����Ʊ����ϳɵ��У�Ҳ����Ե���

3����Ч�У�
a���ֶ������У������壬��������ͨ����¼�����ܹ����ʵ����ݣ�����ʽǿ��Ҫ�����ֶ������У������ǵ�һ����Ч�У�
b���ֶ������У������壬��������ͨ����¼�����ܹ����ʵ����ݣ�����ʽǿ��Ҫ�����ֶ������У������ǵڶ�����Ч�У�
	�ֶ�����֮Ϊ׼�����԰�����һЩ����������У�飬Ŀǰ֧��bool,int,int64,float,string,marco�������ͣ�
c����¼�У������壬������ÿ����¼����ͨ����¼�������з��ʣ��ӵ�������Ч�п�ʼ����Ч�ж��Ǽ�¼�У�

4���궨�壺
a����#define(ע�⣬��Ӣ��#������define����ȫΪСд��ĸ)��ͷ��Ϊ�궨�壬�궨���ֵĬ�϶�Ϊ�ַ�������Ҫ�����ţ�
b����������˺궨�壬Ӧ�ý���Ҫʹ�ú������ȫ���ú�(���򽫺��Ժ��滻������������ת��ʱ����֤û�д���ķ���)��
	���ҽ��ֶε�������Ϊmacro��

5���������壺
a�����������˸�ʽ��ʹ֧֮�ְ�������磺
	#include res/config/item.def
b������ָ���д����������ʾ����ע��·�����Ե�ǰ����Ŀ¼Ϊ׼�������ļ�����Ŀ¼����·����Ҫ�����ţ�
c���ù�����Ҫ���ڽ��궨��д����һ���ļ��У�����csv�ļ��Ƚϸɾ������ҿ��Լ���csv����ͬһ���궨���ļ���

6��������
a��bool����֧��true,false���Ҳ֧��1,0����
b��һ����¼ռһ�У�
c��ÿ���ֶ�֮���÷ָ������зָ����ָ��������Զ��壬Ĭ����Ӣ�Ķ���','����֧�����Ķ��ţ�
d��ÿ���ֽڳ��Ȳ�����2048�ֽ�(�����̬��д��ô���ɣ���)��
e����Ԫ��֧�ְ����ָ�����
f����Ԫ����԰����س����з���

 */

BEGIN_NAMESPACE_FCL

/** һ��Excel��CSV(���Ǽ����˲�����������)�ļ���ȡ�࣬�ݲ�֧�ֱ��� */
class FCL_CLASS Csv
{
protected:
	typedef std::vector<std::string>		StringList;
	typedef hash_map<string, std::string>	MacroMap;
	typedef std::vector<int32>				FieldTypeList;
	typedef std::vector<StringList*>		RecordList;

	RecordList		m_recordList;	/// ��¼�б�(������ע���кͿ���)
	StringList		m_fieldDesc;	/// �ֶ�����
	FieldTypeList	m_fieldType;	/// �ֶ�����
	char			m_separator;	/// �ֶμ�ķָ���
	//int32				m_curRow;		/// ��ǰ��������(���ڼ��ٷ�����һ���ֶ�)
	//int32				m_curCol;		/// ��ǰ��������(���ڼ��ٷ�����һ���ֶ�)
	//int32				m_curPos;		/// ��ǰ�ֶεĽ���λ��(���ڼ��ٷ�����һ���ֶ�)
	MacroMap		m_macroMap;		/// ��ӳ���

public:
	Csv(char separator = ',');
	virtual ~Csv();

	enum FieldType{ftUnknow = 0,ftBool,ftInt,ftInt64,ftFloat,ftString,ftMacro};

public:
	/** ���ļ��д�һ��CSV�ļ�
	 @param filename ��׼�ļ���
	 @return �ɹ�����true�����򷵻�false
	 */
	bool open(const char* filename);

	/** �����д�һ��CSV�ļ�
	 @param stream ������
	 @return �ɹ�����true�����򷵻�false
	 */
	virtual bool open(CStream* stream);

	/** ����ΪCSV�ļ�
	 @param filename Ҫ������ļ���
	 @return �ɹ�����true�����򷵻�false
	 */
	virtual bool save(char* filename);

	/// �ر�CSV����
	void close();

	/** �õ�ĳһ��(�Ѿ�ȥ����ĩβ���ܵĻس����з�)
	 @param row ����������0��ʼ�����������������������
	 @return �ɹ����ض�Ӧ�ַ�����������Խ�緵��NULL
	 */
	//char* getLine(uint32 row);

	/** �õ�boolֵ
	 @param row ��Ч����������0��ʼ
	 @param col ����������0��ʼ
	 @param def Ĭ�ϲ�����������������߶�λ��ֵδ�趨ʱ��ֱ�ӷ���Ĭ��ֵ
	 @retval ������Ӧ��boolֵ
	 */
	bool getBool(int32 row, int32 col, bool def = false) const;

	/** �õ�int32ֵ
	 @param row ��Ч����������0��ʼ
	 @param col ����������0��ʼ
	 @param def Ĭ�ϲ�����������������߶�λ��ֵδ�趨ʱ��ֱ�ӷ���Ĭ��ֵ
	 @retval ������Ӧ��int32ֵ
	 */
	int32 getInt(int32 row, int32 col, int32 def = 0) const;

	/** �õ�int64ֵ
	 @param row ��Ч����������0��ʼ
	 @param col ����������0��ʼ
	 @param def Ĭ�ϲ�����������������߶�λ��ֵδ�趨ʱ��ֱ�ӷ���Ĭ��ֵ
	 @retval ������Ӧ��int64ֵ
	 */
	int64 getInt64(int32 row, int32 col, int64 def = 0) const;

	/** �õ�floatֵ
	 @param row ��Ч����������0��ʼ
	 @param col ����������0��ʼ
	 @param def Ĭ�ϲ�����������������߶�λ��ֵδ�趨ʱ��ֱ�ӷ���Ĭ��ֵ
	 @retval ������Ӧ��floatֵ
	 */
	float getFloat(int32 row, int32 col, float def = 0.0f) const;

	/** �õ�stringֵ
	 @param row ��Ч����������0��ʼ
	 @param col ����������0��ʼ
	 @param def Ĭ�ϲ�����������������߶�λ��ֵδ�趨ʱ��ֱ�ӷ���Ĭ��ֵ
	 @retval ������Ӧ��stringֵ
	 */
	const std::string& getString(int32 row, int32 col, const std::string& def = "") const;

public:
	/** ���÷ָ���
	 @param separator �ָ�����
	 @retval ������ǰ�ķָ���
	 */
	void setSeparator(char separator)	{ m_separator = separator; }

	/// ��÷ָ���
	char getSeparator() const	{ return m_separator; }

	/// ��ü�¼��
	uint32 getRecordCount() const	{ return (uint32)m_recordList.size(); }

	/// ����ֶε���Ŀ
	uint32 getFieldCount() const	{ return (uint32)m_fieldType.size(); }

	/** ͨ���ֶ�������ö�Ӧ��������
	 @param fieldDesc �ֶ������ַ���
	 @retval ������Ӧ�������������ʧ�ܣ��򷵻�-1��
	 */
	int32 getFieldIndex(char* fieldDesc) const;

	/** ͨ����������ȡ��Ӧ���ֶ�����
	 @param col ������
	 @retval ������Ӧ���ֶ������ַ��������������Խ�磬�򷵻�NULL��
	 */
	const char* getFieldDesc(int32 col) const;

	/** ͨ����������ö�Ӧ���ֶ�����
	 @param col ������
	 @retval ������Ӧ�������������ʧ�ܣ��򷵻�ftUnknow��
	 */
	uint32 getFieldType(int32 col) const;

	/** ��ȡָ���ĺ��ֵ
	 @param macroName Ҫ��ȡ�ĺ������
	 @retval ��Ӧ���ֵ����������ڷ���NULL
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
