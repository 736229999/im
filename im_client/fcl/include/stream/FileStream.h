/******************************************************************
** 文件名:	FileStream.h
** 版  权:	(C)  
** 创建人:	Tony
** 日  期:	2007-5-15
** 描  述:  文件流
**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
********************************************************************/

#ifndef __FILESTREAM_H__
#define __FILESTREAM_H__

#include "stream/Stream.h"

BEGIN_NAMESPACE_FCL


/// 标准文件流（各方法的使用参考CStream的对应方法）
class FCL_CLASS CFileStream : public CStream
{
protected:
	FILE* m_stream;		/// 文件流句柄

public:
	CFileStream();
	CFileStream(const char* path);
	virtual ~CFileStream();

public:
	/// 获得文件流句柄
	FILE* getHandle() const		{ return m_stream; }

	/// 文件流转换成文件句柄
	operator FILE*() const		{ return m_stream; }

	// CStream接口
public:
	/** 打开文件流(默认以只读、二进制形式打开)
	 @param mode 文件打开模式
	 @return 成功返回true，否则返回false
	 @note 有关文件打开模式的信息：<br>

	- "r"	读    ,文件不存在则失败<br>
	- "w"	写    ,文件不存在则创建,存在则清空内容<br>
	- "a"	添加  ,文件不存在则创建<br>
	- "r+"	读写  ,文件不存在则失败<br>
	- "w+"	读写  ,文件不存在则创建,存在则清空内容<br>
	- "a+"	读添加,文件不存在则创建<br>
	- "t"	文本方式<br>
	- "b"	二进制方式<br>
	 */
	virtual bool open(const char* mode = "rb");

	/// 关闭文件流
	virtual bool close();

	/** 从文件流中读取数据（对于包文件中的文件对象拒绝所有越界的读写）
	 @param buffer 缓冲区地址
	 @param toRead 要读取的字节大小
	 @return 成功返回true，否则返回false
	 @note 当读写文件时,发生EOF错误，这里也返回true，因为不影响数据的正确性，<br>
		只有当bad file,access之类的错误才返回false
	 */
	virtual bool read(IN OUT void* buffer, uint32 toRead);

	/** 向文件流中写入数据（对于包文件中的文件对象拒绝所有越界的读写）
	 @param buffer 缓冲区地址
	 @param toWrite 要写入的字节大小
	 @return 成功返回true，否则返回false
	 @note 当读写文件时,发生EOF错误，这里也返回true，因为不影响数据的正确性，<br>
		只有当bad file,access之类的错误才返回false
	 */
	virtual bool write(const void* bufer, uint32 toWrite);

	/** 从文件流中读取一行字符串
	 @param strBuffer 要存储字符串的缓冲区
	 @param bufSize 缓冲区字节大小
	 @return 成功返回true，否则返回false
	 @note 必须保证文件打开方式是文本模式
	*/ 
	virtual bool readString(IN OUT char* strBuffer, uint32 bufSize);

	/** 向文件流写入一行字符串
	 @param str 要写入的字符串
	 @return 成功返回true，否则返回false
	 @note 必须保证文件打开方式是文本模式
	*/ 
	virtual bool writeString(const char* str);

	/// 用法同CStream
	virtual bool seek(int32 offset, uint32 from = SEEK_SET);

	/// 用法同CStream
	virtual bool seekToBegin();

	/// 用法同CStream
	virtual bool seekToEnd();

	/// 用法同CStream
	virtual int32 getPosition() const;

	/// 用法同CStream
	virtual bool flush() const;

	/// 用法同CStream
	virtual uint32 getLength() const;

	/// 用法同CStream
	virtual bool setLength(uint32 newLen);

	/// 用法同CStream
	virtual bool isExist() const;

	/// 用法同CStream
	virtual bool isOpen() const;

	/// 删除文件流
	virtual bool remove();

	/** 重命名文件流
	 @param newName 新的文件流名字
	 @return 成功返回true，否则返回false
	 @note 当前的文件流虽然改了名，但是文件流的状态不变，如文件流未关闭，那么现在也没有关闭，文件指针存取位置也不变。
	 */ 
	virtual bool rename(const char* newName);

	/** 文件流存储为文件
	 @param newName 要保存的文件名
	 @return 成功返回true，否则返回false
	 @note 当newName为NULL时，该函数相当于flush，否则，相当于将文件流现在
		的内容copy一份另存为一个文件，save后当前的文件流状态不变。
	 */
	virtual bool save(const char* newName);
};

END_NAMESPACE

#endif // __FILESTREAM_H__
