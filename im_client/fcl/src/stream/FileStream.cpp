/******************************************************************
** 文件名:	FileStream.cpp
** 版  权:	(C)  
** 创建人:	Tony
** 日  期:	2007-5-15
** 描  述:  文件流

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
********************************************************************/


#include "stdafx.h"
#include <sys/stat.h>
#include <io.h>

#include <fcntl.h>
#include "string/string_t.h"
#include "profile/profile.h"
#include "stream/FileStream.h"

#include <errno.h>

BEGIN_NAMESPACE_FCL


CFileStream::CFileStream() : CStream(), m_stream(0)
{
}

CFileStream::CFileStream(const char* path) : CStream(path), m_stream(0)
{
}

CFileStream::~CFileStream()
{
	close();
}

bool CFileStream::open(const char* mode)
{
	if (isOpen())
	{
		//setLastErrorWithFormat(FCLERR_STREAM_IS_OPEN, m_path.c_str());
		return false;
	}

	fclAssert(!m_path.empty() && isValidString(mode));

	if (NULL == (m_stream = fopen(m_path.c_str(), mode)))
	{
		//setLastErrorWithFormat(FCLERR_STREAM_OPEN, m_path.c_str());
		return false;
	}

	return true;
}

bool CFileStream::close()
{
	if (m_stream != NULL)
	{
		if (fclose(m_stream) < 0) // EOS error?
		{
			//setLastErrorWithFormat(FCLERR_STREAM_EOS, m_path.c_str());
			return false;
		}

		m_stream = NULL;
	}

	return true;
}

bool CFileStream::read(IN OUT void* buffer, uint32 toRead)
{
	PROFILE("CFileStream::read");
	fclAssert(m_stream != NULL);

	if (buffer == NULL || toRead == 0) // avoid win32 null read
		return true;

	if (toRead != fread(buffer, 1, toRead, m_stream)) // eof error or read error(bad file, access)
	{
		if (!feof(m_stream))
		{
			if (!ferror(m_stream))
			{
				//setLastErrorWithFormat(FCLERR_STREAM_READ, m_path.c_str());
			}
			return false;
		}
	}

	return true;
}

bool CFileStream::write(const void* buffer, uint32 toWrite)
{
	PROFILE("CFileStream::write");
	fclAssert(m_stream != NULL);

	if (buffer == NULL || toWrite == 0) // avoid win32 null write
		return true;

	if (toWrite != fwrite(buffer, 1, toWrite, m_stream))
	{
		//if (getLastError() == 0)
		//{
		//	//setLastErrorWithFormat(FCLERR_STREAM_WRITE, m_path.c_str());
		//}
		return false;
	}
	return true;
}

bool CFileStream::readString(char* strBuffer, uint32 bufSize)
{
	PROFILE("CFileStream::readString");
	fclAssert(m_stream != NULL && !isNullString(strBuffer) && bufSize > 1);

	char* res = fgets(strBuffer, bufSize, m_stream);
	return res != NULL;
}

bool CFileStream::writeString(const char* str)
{
	PROFILE("CFileStream::writeString");
	fclAssert(m_stream != NULL);

	if (!isValidString(str)) // 
		return true;
	
	return fputs(str, m_stream) != EOF;
}

bool CFileStream::seek(int32 offset, uint32 from)
{
	fclAssert(m_stream != NULL);
	return (fseek(m_stream, offset, from) == 0);
}

bool CFileStream::seekToBegin()
{
	fclAssert(m_stream != NULL);
	return seek(0, SEEK_SET);
}

bool CFileStream::seekToEnd()
{
	fclAssert(m_stream != NULL);
	return seek(0, SEEK_END);
}

bool CFileStream::flush() const
{
	fclAssert(m_stream != NULL);
	return fflush(m_stream) == 0;
}

uint32 CFileStream::getLength() const
{
#if FCL_PLATFORM == FCL_PLATFORM_WIN32
	if (isOpen())
	{/*
		uint32 curPos = getPosition();
		seekToEnd();
		uint32 len = getPosition();
		seek(curPos, SEEK_SET);
		return len;*/
		return filelength(fileno(m_stream));
	}

	if (m_path.empty())
		return 0;

	struct _stat statbuf;
	_stat(m_path.c_str(), &statbuf);
	return statbuf.st_size;
#elif FCL_PLATFORM == FCL_PLATFORM_LINUX
	struct stat statbuf;
	if (isOpen())
	{/*
		uint32 curPos = getPosition();
		seekToEnd();
		uint32 len = getPosition();
		seek(curPos, SEEK_SET);
		return len;*/
		
		fstat(fileno(m_stream), &statbuf);
		return (uint32)statbuf.st_size;
	}

	if (m_path.empty())
		return 0;

	stat(m_path.c_str(), &statbuf);
	return (uint32)statbuf.st_size;
#endif
}

bool CFileStream::setLength(uint32 newLen)
{
	fclAssert(m_stream != NULL);
#if FCL_PLATFORM == FCL_PLATFORM_WIN32
	return _chsize(_fileno(m_stream), newLen) == 0;
#elif FCL_PLATFORM == FCL_PLATFORM_LINUX
	return ftruncate(fileno(m_stream), newLen) == 0;
#endif
}

int32 CFileStream::getPosition() const
{
	fclAssert(m_stream != NULL);

	int32 pos = -1;
	if ((pos = ftell(m_stream)) != -1) // success
		return pos;
	// failed
	//Error("CFileStream::getPosition() catch error: ftell() operater fail!\n");
	return pos;
}


//////////////////////////// other /////////////////////////////////////
bool CFileStream::isExist() const
{
	if (isOpen())
		return true;

	if (m_path.empty())
		return false;

	return _access(m_path.c_str(), 0) != -1;
}

bool CFileStream::isOpen() const
{
	return m_stream != 0;
}

bool CFileStream::remove()
{
	close();
	return ::remove(m_path.c_str()) == 0;
}

bool CFileStream::rename(const char* newName)
{
	fclAssert(!m_path.empty());

	bool isopen = isOpen();
	int32 oldPos = 0;
	if (isopen)
		oldPos = getPosition();

	close();
	
	if (::rename(m_path.c_str(), newName) != 0)
		return false;
	
	m_path = newName;
	if (isopen)
	{
		return (open() && seek(oldPos));
	}

	return true;
}

bool CFileStream::save(const char* newName)
{
	fclAssert(!m_path.empty());

	if (newName == NULL)
		return flush();

	int32 oldPos = 0;
	uint32 size = 0;
	uint8* buffer = 0;

	CFileStream fs(newName);
	bool isopen = isOpen();
	if (!fs.open("wb"))
		goto IOErr;

	if (isopen)
	{
		flush();
		oldPos = getPosition();
	}
	else
	{
		if (!open())
			goto IOErr;
	}

	size = getLength();
	buffer = 0;
	if (size > 0)
	{
		buffer = new uint8[size];
		if (!seekToBegin() || !read(buffer, size) || !fs.write(buffer, size))
			goto IOErr;
	}
	fs.close();

	if (isopen)
		seek(oldPos);
	else
		close();

	delete []buffer;
	buffer = NULL;
	//safeDelete(buffer);
	return true;

IOErr:
	if (!isopen && isOpen())
		close();
	fs.close();
	delete[] buffer;
	buffer = NULL;
	//safeDelete(buffer);
	return false;
}

END_NAMESPACE
