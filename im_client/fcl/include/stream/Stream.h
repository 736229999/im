/******************************************************************
** �ļ���:	Stream.h
** ��  Ȩ:	(C)  
** ������:	
** ��  ��:	2007-05-11   
** ��  ��:  ���������

**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
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

/** �ļ�ϵͳѹ���ͽ���ص�����,fileCountsΪÿ�λص�ʱ�����˵��ļ���Ŀ*/
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
	FCLERR_OFFSET = 'FCL', /// ��3���ַ���Ϊ��ֻ�õ�3�ֽڣ�����ʶ������ʼ���

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

/// �������ࣩ

class FCL_CLASS CStream
{
protected:
	SPath	m_path;		/// ��·��


	// ���죨������

public:
	CStream() { /*m_path.isFile(true); */}
	CStream(const char* path);
	virtual ~CStream();

	// �ӿڷ���
public:
	/** ����(�ļ���Ĭ����ֻ������������ʽ�򿪣����������Ͳ����Ĵ�ģʽ)
	 @param mode ����ģʽ
	 @return �ɹ�����true�����򷵻�false
	 @note �й��ļ�����ģʽ����Ϣ��<br>

	- "r"	��    ,�ļ���������ʧ��<br>
	- "w"	д    ,�ļ��������򴴽�,�������������<br>
	- "a"	���  ,�ļ��������򴴽�<br>
	- "r+"	��д  ,�ļ���������ʧ��<br>
	- "w+"	��д  ,�ļ��������򴴽�,�������������<br>
	- "a+"	�����,�ļ��������򴴽�<br>
	- "t"	�ı���ʽ<br>
	- "b"	�����Ʒ�ʽ<br>
	 */
	virtual bool open(char* mode = "rb") { return false; }

	/// �ر�������open��Ӧ��

	virtual bool close() { return false; }


	/** �����ж�ȡ���ݣ����ڰ��ļ��е��ļ�����ܾ�����Խ��Ķ�д��

	 @param buffer ��������ַ
	 @param toRead Ҫ��ȡ���ֽڴ�С
	 @return �ɹ�����true�����򷵻�false
	 */
	virtual bool read(IN OUT void* buffer, uint32 toRead) { return false; }
	
	/** ������д�����ݣ����ڰ��ļ��е��ļ�����ܾ�����Խ��Ķ�д��

	 @param buffer ��������ַ
	 @param toWrite Ҫд����ֽڴ�С
	 @return �ɹ�����true�����򷵻�false
	 */
	virtual bool write(const void* bufer, uint32 toWrite) { return false; }


	// �ı��ļ����������ڰ��ļ��е��ļ�����ܾ�����Խ��Ķ�д��

	// �������ļ�β��,��������ֵΪNULL
	/** �����ж�ȡһ���ַ���
	 @param strBuffer Ҫ�洢�ַ����Ļ�����
	 @param bufSize �������ֽڴ�С

	 @return �ɹ�����true�����򷵻�false
	 */ 
	virtual bool readString(IN OUT char* strBuffer, uint32 bufSize) { return false; }

	/** ����д��һ���ַ���
	 @param str Ҫд����ַ���

	 @return �ɹ�����true�����򷵻�false
	 */ 
	virtual bool writeString(char* str) { return false; }



	/** �ļ���дλ�ö�λ
	 @param offset ƫ����

	 @param from ������ָ����λ�ÿ�ʼ����ƫ�ƣ��ο�: SEEK_SET �ļ�ͷ; SEEK_CUR �ļ���ǰλ��; SEEK_END �ļ�β

	 @return 
	 */ 
	virtual bool seek(int32 offset, uint32 from = SEEK_SET) { return false; }

	/// �ļ���дλ�ö�λ���ļ�ͷ
	virtual bool seekToBegin() { return false; }

	/// �ļ���дλ�ö�λ���ļ�β
	virtual bool seekToEnd() { return false; }

	/// ����ļ�ָ���λ��

	virtual int32 getPosition() const { return 0; }


	/// ���ļ��Ļ���������д�����(���ļ���֧��)
	virtual bool flush() { return false; }


	/// ��ȡ�ļ�����(�ļ�û��ʱҲ�ܻ�ȡ����),����ǰ��ļ�,����ָѹ����ĳ���

	virtual uint32 getLength() const { return 0; }

	/// �������ĳ���
	virtual bool setLength(uint32 newLen) { return false; }

	/// �����·��

	virtual const SPath& getPath() const { return m_path; }

	/** ������·��

	 @param path ����·����

	 @return �ɹ�����true�����򷵻�false
	 */ 
	virtual bool setPath(char* path);


	/// ���Ƿ����(ע: ������֧��,����ʱ��ͨ��getStream(Handle, name)�ķ���ֵ���ж��Ƿ����)
	virtual bool isExist() const { return false; }

	/// ���Ƿ��ڿ���״̬

	virtual bool isOpen() const	{ return false; }


	/// ɾ����

	virtual bool remove() { return false; }

	/** ��������
	 @param newName �µ�����
	 @return �ɹ�����true�����򷵻�false
	 */ 
	virtual bool rename(char* newName) { return false; }

	/** ���洢Ϊ�ļ�
	 @param newName Ҫ������ļ���

	 @return �ɹ�����true�����򷵻�false
	 */
	virtual bool save(char* newName) { return false; }

	/// �ͷ�������

	virtual void release();
};


//////////////////////////////////////////////////////////////////////////
// һЩʵ�õ�API


/** �л����ļ���ȫ·����
 @param path Ҫת�����ļ����������ǵ�ǰĿ¼�����·����

 @note ������ļ����ļ����ڵĸ���Ŀ¼�Ƿ���ڣ�����Ƿ����ʹ��@c checkPath
 */
 FCL_DECL void toggleFullPath(SPath& path);

/** ���·���Ƿ���ڣ�����Ŀ¼·�����ļ�·����
 @param path ·����

 @param isAbsolutePath �Ƿ��Ǿ���·��

 @param attrib �����ļ�����Ŀ¼��������

 @return �ɹ�����true�����򷵻�false
 */
FCL_DECL bool checkPath(IN SString path, IN OUT bool& isAbsolutePath, IN OUT uint32& attrib);

/** ���·���Ƿ����ļ�
 @param path Ҫ����·������������·�������Զ���exe��������Ŀ¼Ϊ��ǰĿ¼���д���

 @return �ɹ�����true���������·�������ڻ�����Ŀ¼�򷵻�false
 */
FCL_DECL bool isFile(SString path);

/** ���·���Ƿ���Ŀ¼
 @param path Ҫ����·������������·�������Զ���exe��������Ŀ¼Ϊ��ǰĿ¼���д���

 @return �ɹ�����true���������·�������ڻ������ļ��򷵻�false
 */
FCL_DECL bool isDirectory(SString path);

/** ���ָ����·���Ƿ��Ǿ���·��

 @param path Ҫ����·��
 @return ����·������true�����򷵻�false
 */
FCL_DECL bool isAbsolutePath(SString path);

/** �ݹ鴴��Ŀ¼
 @param absolutePath Ҫ�����ľ���·��
 @return �ɹ�����Ŀ¼�Ѵ��ڷ���true�����򷵻�false��ʧ�ܺ󲻴�����ܴ����˵�Ŀ¼

 */
FCL_DECL bool createDirectoryRec(SString absolutePath);

/** �ݹ�ɾ��Ŀ¼
 @param absolutePath Ҫɾ���ľ���·��
 @return �ɹ�����Ŀ¼�����ڷ���true�����򷵻�false
 */
FCL_DECL bool removeDirectoryRec(SString absolutePath);

/// Ŀ¼���mask
enum BrowseDirMask{bdm_file=0x1, bdm_dir=0x2};

/** ���Ŀ¼
 @param absoluteDir ����Ŀ¼·��
 @param relativeDir ���Ŀ¼�����ڼ�¼����Ŀ¼�������е���Ŀ¼�����λ��
 @param DIRPROC Ŀ¼�ص�����
 @param FILEPROC �ļ��ص�����
 @param userData �û��ṩ��˽������

 @param mask ��������ļ���Ŀ¼��mask��mask��0x1��ʾֻ���Ŀ¼��mask=0x2��ʾֻ����ļ����ɽ���or����
 @return �ɹ�����true�����򷵻�false
 @note �磺browseDir("e:/mytestdir", "", CPkgOperation::dirProc, CPkgOperation::fileProc, (uint32)this, 3);
 */
FCL_DECL bool browseDir(SString absoluteDir, SString relativeDir, bool (*DIRPROC)(SString, SString, _finddata_t&, uint32), 
					  bool (*FILEPROC)(SString, SString, _finddata_t&, uint32), uint32 userData, uint32 mask = bdm_file|bdm_dir);

END_NAMESPACE

#endif // __STREAM_H__
