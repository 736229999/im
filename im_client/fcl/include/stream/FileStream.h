/******************************************************************
** �ļ���:	FileStream.h
** ��  Ȩ:	(C)  
** ������:	Tony
** ��  ��:	2007-5-15
** ��  ��:  �ļ���
**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
********************************************************************/

#ifndef __FILESTREAM_H__
#define __FILESTREAM_H__

#include "stream/Stream.h"

BEGIN_NAMESPACE_FCL


/// ��׼�ļ�������������ʹ�òο�CStream�Ķ�Ӧ������
class FCL_CLASS CFileStream : public CStream
{
protected:
	FILE* m_stream;		/// �ļ������

public:
	CFileStream();
	CFileStream(const char* path);
	virtual ~CFileStream();

public:
	/// ����ļ������
	FILE* getHandle() const		{ return m_stream; }

	/// �ļ���ת�����ļ����
	operator FILE*() const		{ return m_stream; }

	// CStream�ӿ�
public:
	/** ���ļ���(Ĭ����ֻ������������ʽ��)
	 @param mode �ļ���ģʽ
	 @return �ɹ�����true�����򷵻�false
	 @note �й��ļ���ģʽ����Ϣ��<br>

	- "r"	��    ,�ļ���������ʧ��<br>
	- "w"	д    ,�ļ��������򴴽�,�������������<br>
	- "a"	���  ,�ļ��������򴴽�<br>
	- "r+"	��д  ,�ļ���������ʧ��<br>
	- "w+"	��д  ,�ļ��������򴴽�,�������������<br>
	- "a+"	�����,�ļ��������򴴽�<br>
	- "t"	�ı���ʽ<br>
	- "b"	�����Ʒ�ʽ<br>
	 */
	virtual bool open(const char* mode = "rb");

	/// �ر��ļ���
	virtual bool close();

	/** ���ļ����ж�ȡ���ݣ����ڰ��ļ��е��ļ�����ܾ�����Խ��Ķ�д��
	 @param buffer ��������ַ
	 @param toRead Ҫ��ȡ���ֽڴ�С
	 @return �ɹ�����true�����򷵻�false
	 @note ����д�ļ�ʱ,����EOF��������Ҳ����true����Ϊ��Ӱ�����ݵ���ȷ�ԣ�<br>
		ֻ�е�bad file,access֮��Ĵ���ŷ���false
	 */
	virtual bool read(IN OUT void* buffer, uint32 toRead);

	/** ���ļ�����д�����ݣ����ڰ��ļ��е��ļ�����ܾ�����Խ��Ķ�д��
	 @param buffer ��������ַ
	 @param toWrite Ҫд����ֽڴ�С
	 @return �ɹ�����true�����򷵻�false
	 @note ����д�ļ�ʱ,����EOF��������Ҳ����true����Ϊ��Ӱ�����ݵ���ȷ�ԣ�<br>
		ֻ�е�bad file,access֮��Ĵ���ŷ���false
	 */
	virtual bool write(const void* bufer, uint32 toWrite);

	/** ���ļ����ж�ȡһ���ַ���
	 @param strBuffer Ҫ�洢�ַ����Ļ�����
	 @param bufSize �������ֽڴ�С
	 @return �ɹ�����true�����򷵻�false
	 @note ���뱣֤�ļ��򿪷�ʽ���ı�ģʽ
	*/ 
	virtual bool readString(IN OUT char* strBuffer, uint32 bufSize);

	/** ���ļ���д��һ���ַ���
	 @param str Ҫд����ַ���
	 @return �ɹ�����true�����򷵻�false
	 @note ���뱣֤�ļ��򿪷�ʽ���ı�ģʽ
	*/ 
	virtual bool writeString(const char* str);

	/// �÷�ͬCStream
	virtual bool seek(int32 offset, uint32 from = SEEK_SET);

	/// �÷�ͬCStream
	virtual bool seekToBegin();

	/// �÷�ͬCStream
	virtual bool seekToEnd();

	/// �÷�ͬCStream
	virtual int32 getPosition() const;

	/// �÷�ͬCStream
	virtual bool flush() const;

	/// �÷�ͬCStream
	virtual uint32 getLength() const;

	/// �÷�ͬCStream
	virtual bool setLength(uint32 newLen);

	/// �÷�ͬCStream
	virtual bool isExist() const;

	/// �÷�ͬCStream
	virtual bool isOpen() const;

	/// ɾ���ļ���
	virtual bool remove();

	/** �������ļ���
	 @param newName �µ��ļ�������
	 @return �ɹ�����true�����򷵻�false
	 @note ��ǰ���ļ�����Ȼ�������������ļ�����״̬���䣬���ļ���δ�رգ���ô����Ҳû�йرգ��ļ�ָ���ȡλ��Ҳ���䡣
	 */ 
	virtual bool rename(const char* newName);

	/** �ļ����洢Ϊ�ļ�
	 @param newName Ҫ������ļ���
	 @return �ɹ�����true�����򷵻�false
	 @note ��newNameΪNULLʱ���ú����൱��flush�������൱�ڽ��ļ�������
		������copyһ�����Ϊһ���ļ���save��ǰ���ļ���״̬���䡣
	 */
	virtual bool save(const char* newName);
};

END_NAMESPACE

#endif // __FILESTREAM_H__
