#ifndef __STDAFX_H_
#define __STDAFX_H_

// Platform Dependent
#ifdef _WIN32
// -- Windows
#include <windows.h>
#include <stdio.h>
#endif

#ifdef linux
// -- Linux
#include <unistd.h>
#endif


// Platform Independent
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <sys/stat.h>

// CURL library
#define HTTP_ONLY
#include <curl/curl.h>

//RAPIDJSON
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

// Fix for TCHAR
#ifndef UNICODE  
typedef std::string String;
#else
typedef std::wstring String;
#endif

#endif