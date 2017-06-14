// $Id: Hessian1OutputStream.h 25 2009-03-21 16:09:57Z pukkaone $
#ifndef HESSIAN_HESSIAN1OUTPUTSTREAM_H
#define HESSIAN_HESSIAN1OUTPUTSTREAM_H

#include "hessian/MemoryStreamBuf.h"
#include "hessian/types.h"
#include <vector>
#include <map>

namespace hessian {

/**
 * Encodes values to Hessian protocol format.
 */
class Hessian1OutputStream
{
    MemoryStreamBuf buffer_;

    // Writes byte.
    void write (char byte)
    { buffer_.sputc(byte); }

    // Writes 64-bit integer.
    void write(hessian::Long value);

    // Writes 16-bit length followed by bytes.
    void write(
            std::string::size_type length,
            const char* pBegin,
            const char* pEnd);

    // Writes 16-bit length followed by bytes.  Length equals number of bytes.
    void write (std::string::size_type nBytes, const char* pBegin)
    { write(nBytes, pBegin, pBegin + nBytes); }

public:
    Hessian1OutputStream ():
        buffer_(64)
    { }

    void beginList();
    void endList();

    void beginMap();
    void endMap();

    void beginObject(
            const char* pTypeName, std::string::size_type typeNameLength);
    void endObject();

    void beginCall(
            const char* pMethodName, std::string::size_type methodNameLength);
    void endCall();

    Hessian1OutputStream& operator<<(const hessian::Binary& value);
    Hessian1OutputStream& operator<<(bool value);
    Hessian1OutputStream& operator<<(hessian::Date value);
    Hessian1OutputStream& operator<<(double value);
    Hessian1OutputStream& operator<<(hessian::Int value);
    Hessian1OutputStream& operator<<(hessian::Long value);
    Hessian1OutputStream& operator<<(const std::string& value);

    template <class T>
    inline
    Hessian1OutputStream& operator<<(const std::vector<T>& container)
    {
    	beginList();
    	uint32_t count = container.size();
    	for (uint32_t i = 0; i < count; ++i) {
    	    (*this) << container[i];
    	}
    	endList();
        return *this;
    }

    template <class Key, class T>
    inline
    Hessian1OutputStream& operator<<(const std::map<Key, T>& container)
    {
    	beginMap();
		for (typename std::map<Key, T>::const_iterator iter = container.begin(); iter != container.end(); ++iter) {
			(*this) << (iter->first);
			(*this) << (iter->second);
		}
    	endMap();
    	return *this;
    }

    MemoryStreamBuf* rdbuf ()
    { return &buffer_; }
};

}//namespace hessian
#endif
