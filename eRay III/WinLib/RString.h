
#ifndef _RSTRING_H_
#define _RSTRING_H_

#include <Windows.h>
#include <tchar.h>

#pragma warning( disable : 4995)
#include <string>


namespace RClasses {

#ifdef UNICODE
	typedef std::wstring RString;
#else
	typedef std::string RString;
#endif

typedef std::size_t len_t;


} // namespace RClasses

#endif