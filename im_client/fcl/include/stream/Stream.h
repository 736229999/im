/******************************************************************
** 文件名:	Stream.h
** 版  权:	(C)  
** 创建人:	
** 日  期:	2007-05-11   
** 描  述:  流抽象基类

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
********************************************************************/

#ifndef __STREAM_H__
#define __STREAM_H__

#include "string/string_t.h"
#include "stream/Path.h"

#include <sys/types.h>
#include <sys/stat.h>
#include "Config.h"
#include<io.h>
#include<stdio.h>
#include<direct.h>

//

/** 文件系统压包和解包回调函数,fileCounts为每次回调时处理了的文件数目*/
//typedef void (__attribute__((__stdcall__)) *PACKPROC) (uint32 fileCounts);


BEGIN_NAMESPACE_FCL

#define isNullString(strPtr)		( (strPtr) == NULL )
#define isEmptyString(strPtr)		( (strPtr)[0] == '\0' )
#define isValidString(strPtr)		( (strPtr) != NULL && (strPtr)[0] != '\0' )


typedef const char*		SString;
typedef CPathA	SPath;

enum FclError
{
	// offset
	FCLERR_OFFSET = 'FCL', /// 用3个字符（为了只用低3字节）来标识错误起始编号

	//MemoryError
	FCLERR_MEM_ALLOC = FCLERR_OFFSET,
	FCLERR_MEM_REALLOC,
	FCLERR_MEM_FREE,
	// ...

	// StreamError
	FCLERR_STREAM,
	FCLERR_STREAM_CREATE,
	FCLERR_STREAM_OPEN,
	FCLERR_STREAM_IS_OPEN,
	FCLERR_STREAM_READ,
	FCLERR_STREAM_WRITE,
	FCLERR_STREAM_CLOSE,
	FCLERR_STREAM_EOS,
	FCLERR_STREAM_EXIST,
	FCLERR_STREAM_CREATE_DIRECTORY,
	FCLERR_STREAM_NOT_FIND,
	FCLERR_STREAM_PACKAGE_HEAD_INVALID,
	FCLERR_STREAM_BLOCK_TABLE,
	FCLERR_STREAM_HASH_TABLE,
	// ...

	// Handle
	FCLERR_HANDLE_INVALID,
	// ...

	FCLERR_MAX
};

/// 流（基类）

class FCL_CLASS CStream
{
protected:
	SPath	m_path;		/// 流路径


	// 构造（析构）

public:
	CStream() { /*m_path.isFile(true); */}
	CStream(const char* path);
	virtual ~CStream();

	// 接口方法
public:
	/** 打开流(文件流默认以只读、二进制形式打开，其他流类型不关心打开模式)
	 @param mode 流打开模式
	 @return 成功返回true，否则返回false
	 @note 有关文件流打开模式的信息：<br>

	- "r"	读    ,文件不存在则失败<br>
	- "w"	写    ,文件不存在则创建,存在则清空内容<br>
	- "a"	添加  ,文件不存在则创建<br>
	- "r+"	读写  ,文件不存在则失败<br>
	- "w+"	读写  ,文件不存在则创建,存在则清空内容<br>
	- "a+"	读添加,文件不存在则创建<br>
	- "t"	文本方式<br>
	- "b"	二进制方式<br>
	 */
	virtual bool open(char* mode = "rb") { return false; }

	/// 关闭流（跟open对应）

	virtual bool close() { return false; }


	/** 从流中读取数据（对于包文件中的文件对象拒绝所有越界的读写）

	 @param buffer 缓冲区地址
	 @param toRead 要读取的字节大小
	 @return 成功返回true，否则返回false
	 */
	virtual bool read(IN OUT void* buffer, uint32 toRead) { return false; }
	
	/** 向流中写入数据（对于包文件中的文件对象拒绝所有越界的读写）

	 @param buffer 缓冲区地址
	 @param toWrite 要写入的字节大小
	 @return 成功返回true，否则返回false
	 */
	virtual bool write(const void* bufer, uint32 toWrite) { return false; }


	// 文本文件操作（对于包文件中的文件对象拒绝所有越界的读写）

	// 当到了文件尾后,函数返回值为NULL
	/** 从流中读取一行字符串
	 @param strBuffer 要存储字符串的缓冲区
	 @param bufSize 缓冲区字节大小

	 @return 成功返回true，否则返回false
	 */ 
	virtual bool readString(IN OUT char* strBuffer, uint32 bufSize) { return false; }

	/** 向流写入一行字符串
	 @param str 要写入的字符串

	 @return 成功返回true，否则返回false
	 */ 
	virtual bool writeString(char* str) { return false; }



	/** 文件读写位置定位
	 @param offset 偏移量

	 @param from 从那种指定的位置开始进行偏移，参考: SEEK_SET 文件头; SEEK_CUR 文件当前位置; SEEK_END 文件尾

	 @return 
	 */ 
	virtual bool seek(int32 offset, uint32 from = SEEK_SET) { return false; }

	/// 文件读写位置定位到文件头
	virtual bool seekToBegin() { return false; }

	/// 文件读写位置定位到文件尾
	virtual bool seekToEnd() { return false; }

	/// 获得文件指针的位置

	virtual int32 getPosition() const { return 0; }


	/// 将文件的缓冲区内容写入磁盘(包文件不支持)
	virtual bool flush() { return false; }


	/// 获取文件长度(文件没打开时也能获取长度),如果是包文件,则是指压缩后的长度

	virtual uint32 getLength() const { return 0; }

	/// 设置流的长度
	virtual bool setLength(uint32 newLen) { return false; }

	/// 获得流路径

	virtual const SPath& getPath() const { return m_path; }

	/** 设置流路径

	 @param path 流的路径名

	 @return 成功返回true，否则返回false
	 */ 
	virtual bool setPath(char* path);


	/// 流是否存在(注: 包流不支持,包流时可通过getStream(Handle, name)的返回值来判断是否存在)
	virtual bool isExist() const { return false; }

	/// 流是否处于开启状态

	virtual bool isOpen() const	{ return false; }


	/// 删除流

	virtual bool remove() { return false; }

	/** 重命名流
	 @param newName 新的流名
	 @return 成功返回true，否则返回false
	 */ 
	virtual bool rename(char* newName) { return false; }

	/** 流存储为文件
	 @param newName 要保存的文件名

	 @return 成功返回true，否则返回false
	 */
	virtual bool save(char* newName) { return false; }

	/// 释放流对象

	virtual void release();
};


//////////////////////////////////////////////////////////////////////////
// 一些实用的API


/** 切换到文件的全路径名
 @param path 要转换的文件名（可以是当前目录的相对路径）

 @note 不检查文件或文件所在的各级目录是否存在，检查是否存在使用@c checkPath
 */
 FCL_DECL void toggleFullPath(SPath& path);

/** 检查路径是否存在（包括目录路径和文件路径）
 @param path 路径名

 @param isAbsolutePath 是否是绝对路径

 @param attrib 返回文件（或目录）的属性

 @return 成功返回true，否则返回false
 */
FCL_DECL bool checkPath(IN SString path, IN OUT bool& isAbsolutePath, IN OUT uint32& attrib);

/** 检查路径是否是文件
 @param path 要检查的路径（如果是相对路径，会自动以exe进程所在目录为当前目录进行处理）

 @return 成功返回true，否则如果路径不存在或者是目录则返回false
 */
FCL_DECL bool isFile(SString path);

/** 检查路径是否是目录
 @param path 要检查的路径（如果是相对路径，会自动以exe进程所在目录为当前目录进行处理）

 @return 成功返回true，否则如果路径不存在或者是文件则返回false
 */
FCL_DECL bool isDirectory(SString path);

/** 检查指定的路径是否是绝对路径

 @param path 要检查的路径
 @return 绝对路径返回true，否则返回false
 */
FCL_DECL bool isAbsolutePath(SString path);

/** 递归创建目录
 @param absolutePath 要创建的绝对路径
 @return 成功或者目录已存在返回true，否则返回false，失败后不处理可能创建了的目录

 */
FCL_DECL bool createDirectoryRec(SString absolutePath);

/** 递归删除目录
 @param absolutePath 要删除的绝对路径
 @return 成功或者目录不存在返回true，否则返回false
 */
FCL_DECL bool removeDirectoryRec(SString absolutePath);

/// 目录浏览mask
enum BrowseDirMask{bdm_file=0x1, bdm_dir=0x2};

/** 浏览目录
 @param absoluteDir 绝对目录路径
 @param relativeDir 相对目录，用于记录绝对目录下面所有的子目录的相对位置
 @param DIRPROC 目录回调函数
 @param FILEPROC 文件回调函数
 @param userData 用户提供的私有数据

 @param mask 用于浏览文件和目录的mask，mask＝0x1表示只浏览目录，mask=0x2表示只浏览文件，可进行or操作
 @return 成功返回true，否则返回false
 @note 如：browseDir("e:/mytestdir", "", CPkgOperation::dirProc, CPkgOperation::fileProc, (uint32)this, 3);
 */
FCL_DECL bool browseDir(SString absoluteDir, SString relativeDir, bool (*DIRPROC)(SString, SString, _finddata_t&, uint32), 
					  bool (*FILEPROC)(SString, SString, _finddata_t&, uint32), uint32 userData, uint32 mask = bdm_file|bdm_dir);

END_NAMESPACE

#endif // __STREAM_H__
