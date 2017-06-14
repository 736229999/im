/******************************************************************
** �ļ���:	Stream.cpp
** ��  Ȩ:	(C)  T���濪����
** ������:	T
** ��  ��:	2005-08-31   14:20
** ��  ��:	���������
**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
********************************************************************/

#include "stdafx.h"
//#include "misc/misc.h"
#include "stream/Stream.h"
#include "string/string_t.h"
#include "stream/Path.h"
#include <assert.h>
#include "trace/Trace.h"
//////////////////////////////////////////////////////////////////////////

BEGIN_NAMESPACE_FCL
// �ļ��������
CStream::CStream(const char* path)
{
	/*m_path.isFile(true);*/
	m_path = path;
}

CStream::~CStream()
{
	close();
}

void CStream::release()
{
	close();
	delete this;
}

bool CStream::setPath(char* path)
{
	assert(isValidString(path));

	if (isOpen())
		close();

	if (path)
		m_path = path;

	return true;
}


//////////////////////////////////////////////////////////////////////////
inline static bool _isDirectoryExist(SPath& absolutePath)
{
	struct _finddata_t fd;
	intptr_t r = (intptr_t)_findfirst(absolutePath.c_str(), &fd); // "c:\"Ҳ�᷵�أ�1����Ŀ¼�����ڣ���Ϊ����������Ŀ¼
	_findclose(r);
	return (r != -1 && fd.attrib == 16);
}

inline static bool _isAbsolutePath(SPath& path)
{
	return (path.length() >= 3 && (((string)path)[0] == '\\' || ((string)path)[1] == ':'));
}

FCL_DECL void toggleFullPath(SPath& path)
{
	if (path.empty())
	{
		path = TtoA(getWorkDir());
		return;
	}

	if (!_isAbsolutePath(path))
	{
		SPath workPath( TtoA(getWorkDir()) );
		workPath.addTailSlash();
		path.insert(0, workPath);
	}
}

FCL_DECL bool checkPath(SString path, bool& isAbsolutePath, uint32& attrib)
{
	SPath mypath( path );
	toggleFullPath(mypath);

	_finddata_t fd;
	intptr_t r = _findfirst(mypath.c_str(), &fd); // "c:\"Ҳ�᷵�أ�1����Ŀ¼�����ڣ���Ϊ����������Ŀ¼
	attrib = fd.attrib;
	_findclose(r);

	return r != -1;
}

FCL_DECL bool isAbsolutePath(SString path)
{
	SPath spath(path);
	return _isAbsolutePath(spath);
}

FCL_DECL bool isFile(SString path)
{
	bool isAbsolutePath;
	uint32 attrib;
	bool ret = checkPath(path, isAbsolutePath, attrib);
	return (ret && attrib != 16);
}

FCL_DECL bool isDirectory(SString path)
{
	bool isAbsolutePath;
	uint32 attrib;
	bool ret = checkPath(path, isAbsolutePath, attrib);
	return (ret && attrib == 16);
}

// �ݹ鴴��Ŀ¼(�����༶,absolutePath�����·����)
// �Զ����Ŀ¼�Ƿ����
FCL_DECL bool createDirectoryRec(SString absolutePath)
{
	assert(isValidString(absolutePath));

	SPath strDir(absolutePath, false);

	if (_isDirectoryExist(strDir))
		return true;

	// ��ȡ��Ŀ¼
	SPath strParent(strDir.getParentDir().c_str(), false);
	if (strParent.empty()) // Ŀ¼���ƴ���
		return false; 

	bool ret = true;
	if (strParent.length() > 3) // �������С��3����ʾΪ���̸�Ŀ¼
		ret = _isDirectoryExist(strParent);// ��鸸Ŀ¼�Ƿ����

	if (!ret) // ��Ŀ¼������,�ݹ���ô�����Ŀ¼
		ret = createDirectoryRec(strParent.c_str()); 

	if (ret) // ��Ŀ¼����,ֱ�Ӵ���Ŀ¼
	{
		int32 r = mkdir(strDir.c_str());
		if (r == -1) // error
			Trace("error,FCLERR_STREAM_CREATE_DIRECTORY");
		ret = r != -1;
	}

	return ret;
}

FCL_DECL bool removeDirectoryRec(SString absolutePath)
{
	assert(isValidString(absolutePath));

	SPath strDir(absolutePath, false);

	// �������ȱ������3��������Ϊ���̸�Ŀ¼��հ�
	if (strDir.empty() || strDir.length() <= 3)
		return false;

	SPath strFiles(strDir, true);
	strFiles += "\\*.*";

	_finddata_t fd;
	intptr_t hFind = _findfirst(strFiles.c_str(), &fd);
	if (hFind == -1 || fd.attrib != 16) // directory not exist, return true
		return true; 

	bool ret = true;
	if (hFind != -1) // directory exist
	{
		bool bFind = true;
		while (bFind)
		{
			SPath strTmp = strDir;
			if (fd.attrib & 16) // remove all sub directory
			{ 
				if (fd.name[0] != '.') // "." ".."
				{
					strTmp.addTailSlash();
					strTmp += fd.name;
					bool rb = true;
					ret = (rb = removeDirectoryRec(strTmp.c_str())) && ret;
				}
			}
			else // remove all files in this directory
			{ 
				strTmp.addTailSlash();
				strTmp +=  fd.name;
				_chmod(strTmp.c_str(), _S_IWRITE); // disable readonly flags
				bool rb = true;
				ret = (rb = (remove(strTmp.c_str()) == 0)) && ret;
				if (!rb)
					Error("removeFile: %s [reason]:\n", strTmp.c_str());
			}

			bFind = _findnext(hFind, &fd) != -1;
		}
		_findclose(hFind);

		if (ret)
		{
			if (_rmdir(strDir.c_str()) != 0)
			{
				Error("removeDirectory: %s [reason]:\n", strDir.c_str());
				return false;
			}
			return true;
		}
	}

	return false;
}

static bool _browseDir(SPath& absoluteDir, SPath& relativeDir, bool (*DIRPROC)(SString, SString, _finddata_t&, uint32), 
					  bool (*FILEPROC)(SString, SString, _finddata_t&, uint32), uint32 userData, uint32 mask)
{
	intptr_t hFile;
	_finddata_t fd;

	SPath strFiles = absoluteDir;
#if FCL_PLATFORM == FCL_PLATFORM_WIN32
		strFiles += "\\*.*";
#elif FCL_PLATFORM == FCL_PLATFORM_LINUX
		strFiles += "/*";
#endif

	if ((hFile = _findfirst(strFiles.c_str(), &fd)) != -1)
	{
		do
		{
			if (fd.attrib & 16) // is directory!
			{
				if (mask & 1)
				{
					if (fd.name[0] != '.') // for ".", ".."
					{
						if (!absoluteDir.empty())
							absoluteDir.addTailSlash();
						if (!relativeDir.empty())
							relativeDir.addTailSlash();

						absoluteDir += fd.name;
						relativeDir += fd.name;

						if (DIRPROC != NULL &&	false == (DIRPROC)(absoluteDir.c_str(), relativeDir.c_str(), fd, userData))
							return false;

						if (!_browseDir(absoluteDir, relativeDir, DIRPROC, FILEPROC, userData, mask))
							return false;

						SPath::_String absDir = absoluteDir.getParentDir();
						absoluteDir = absDir;
						SPath::_String relateDir = relativeDir.getParentDir();
						relativeDir = relateDir;
					}
				}
			}
			else // ���ļ�
			{
				if (mask & 2)
				{
					SPath strFileName = absoluteDir;
					SPath relativeFile = relativeDir;

					if (!strFileName.empty())
						strFileName.addTailSlash();
					if (!relativeFile.empty())
						relativeFile.addTailSlash();

					strFileName += fd.name;
					relativeFile += fd.name;

					if (FILEPROC != NULL &&	false == (FILEPROC)(strFileName.c_str(), relativeFile.c_str(), fd, userData))
						return false;
				}
			}
		} while (_findnext(hFile, &fd) != -1);

		_findclose(hFile);
	}

	return true;
}

FCL_DECL bool browseDir(SString absoluteDir, SString relativeDir, bool (*DIRPROC)(SString, SString, _finddata_t&, uint32), 
					  bool (*FILEPROC)(SString, SString, _finddata_t&, uint32), uint32 userData, uint32 mask)
{
	SPath absolutePath(absoluteDir, false);
	SPath relativePath(relativeDir, false);
	assert(!absolutePath.empty());

	return _browseDir(absolutePath, relativePath, DIRPROC, FILEPROC, userData, mask);
}

END_NAMESPACE

