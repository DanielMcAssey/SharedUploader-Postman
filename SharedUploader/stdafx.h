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

#endif


// Platform Independent
#include <iostream>
#include <string>
#include <unordered_map>
#include <sys/stat.h>
#include <curl/curl.h>

// Fix for TCHAR
#ifndef UNICODE  
typedef std::string String;
#else
typedef std::wstring String;
#endif

#include "UploadFileTypes.h"
#include "Uploader.h"

#endif