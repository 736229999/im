#pragma once
#include <windows.h>
#include < Dbghelp.h>
#include <iostream>  
#include <vector>  
#include "curl.h"

using namespace std; 


#pragma comment(lib, "Dbghelp.lib")


namespace NSDumpFile
{ 
	void CreateDumpFile(LPCTSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)  
	{  
		// 创建Dump文件  
		//  
		HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  


		// Dump信息  
		//  
		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;  
		dumpInfo.ExceptionPointers = pException;  
		dumpInfo.ThreadId = GetCurrentThreadId();  
		dumpInfo.ClientPointers = TRUE;  


		// 写入Dump文件内容  
		//  
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);  


		CloseHandle(hDumpFile);  
	}  


	LPTOP_LEVEL_EXCEPTION_FILTER WINAPI MyDummySetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
	{
		return NULL;
	}


	BOOL PreventSetUnhandledExceptionFilter()
	{
		HMODULE hKernel32 = LoadLibrary("kernel32.dll");
		if (hKernel32 ==   NULL)
			return FALSE;


		void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
		if(pOrgEntry == NULL)
			return FALSE;


		unsigned char newJump[ 100 ];
		DWORD dwOrgEntryAddr = (DWORD) pOrgEntry;
		dwOrgEntryAddr += 5; // add 5 for 5 op-codes for jmp far


		void *pNewFunc = &MyDummySetUnhandledExceptionFilter;
		DWORD dwNewEntryAddr = (DWORD) pNewFunc;
		DWORD dwRelativeAddr = dwNewEntryAddr -  dwOrgEntryAddr;


		newJump[ 0 ] = 0xE9;  // JMP absolute
		memcpy(&newJump[ 1 ], &dwRelativeAddr, sizeof(pNewFunc));
		SIZE_T bytesWritten;
		BOOL bRet = WriteProcessMemory(GetCurrentProcess(),    pOrgEntry, newJump, sizeof(pNewFunc) + 1, &bytesWritten);
		return bRet;
	}

	static long curl_writer(void *data, int size, int nmemb, string *content)
	{
		long sizes = size * nmemb;
		*content += string((char*)data, sizes);
		return sizes;
	}

	BOOL upLoadDump(const char* szDump)
	{
		CURL *curl;
		CURLcode res;
		string strRet;
		string strError;

		curl = curl_easy_init();
		if(curl) 
		{
			struct curl_httppost *formpost = 0;
			struct curl_httppost *lastptr  = 0;
			curl_formadd(&formpost,  
				&lastptr,  
				CURLFORM_COPYNAME, "file",  
				CURLFORM_FILE, szDump,  
				CURLFORM_END);  
#ifdef _VER_PREVIEW_		
			curl_easy_setopt(curl, CURLOPT_PROXY, PROXY_URL); 
#endif
			string strHttp = UPLOAD_DUMP_URL;
			curl_easy_setopt(curl, CURLOPT_URL,strHttp.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strRet); 
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);   // 30s超时
			/* Perform the request, res will get the return code */ 
			res = curl_easy_perform(curl);
			/* Check for errors */ 
			if(res != CURLE_OK)
			{
				strError = curl_easy_strerror(res);
			}

			/* always cleanup */ 
			curl_easy_cleanup(curl);

			if(res == CURLE_OK)
			{
				//解析json字符创
				Json::Reader reader;
				Json::Value recv;
				if (reader.parse(strRet, recv))
				{
					//获取result字段
					bool result = recv["success"].asBool();
					if(result)
					{
						return TRUE;
					}
				}
			}
		}
		return FALSE;
	}

	LONG WINAPI UnhandledExceptionFilterEx(struct _EXCEPTION_POINTERS *pException)
	{
		CString strPath;
		TCHAR szUserPath[MAX_PATH];
		SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, szUserPath);
		if( strlen(szUserPath) > 0 )
			strPath = CString(szUserPath)+"\\";
#ifdef _VER_INTERNAL_
		strPath = strPath + "MouMou_Internal\\";
#elif defined _CS_PLAT_
		strPath = strPath + "MouMou_Cs\\";
#elif defined _WORK_
		strPath = strPath + "MouMou_Work\\";
#elif defined _BUSINESS_VESION
		strPath = strPath + "MouMou_Business\\";
#else
		strPath = strPath + "MouMou\\";
#endif
		uint32 uiSendTime = time(NULL);
		CTime  tm(uiSendTime);
		CString strTime = tm.Format("%Y-%m-%d %H.%M.%S");
		CString strDump = strPath  + strTime + ".dmp";

		OSVERSIONINFOEX osinfo;
		osinfo.dwOSVersionInfoSize=sizeof(OSVERSIONINFOEX);
		CString strSystem;
		if(GetVersionEx((OSVERSIONINFO*)&osinfo))
		{
			if(osinfo.dwMajorVersion == 6 && osinfo.dwMinorVersion == 0)
				strSystem = "vista";
			else if(osinfo.dwMajorVersion == 6 && osinfo.dwMinorVersion == 1)
				strSystem = "win7";
			else if((osinfo.dwMajorVersion > 6 && osinfo.dwMinorVersion == 0) || (osinfo.dwMajorVersion == 6 && osinfo.dwMinorVersion == 2))
				strSystem = "win8";
			else
				strSystem = "xp";
		}
		CString strMoumouType;
#ifdef _CS_PLAT_EX_
		strMoumouType = "CS";
#else
		strMoumouType = "BUSINESS";
#endif
		//dump文件添加系统版本和ID
		strDump.Format("%s%s_%s_%s_%d.dmp", strPath, strTime, strSystem, strMoumouType, g_ImUserInfo.GetId());
		CreateDumpFile(strDump, pException);

		//上传dump文件;
		BOOL bUp = upLoadDump(strDump);

		XT_LOGOUT req;
		req.id = g_ImUserInfo.GetId();
		req.authData = 0;
		g_pProtocol->SendReq(req);

		if(CMessageBox::Model(GetDesktopWindow(),"程序出错，是否发送错误报告并重启？","",MB_YESNO/*|MB_DEFBUTTON2*/)==IDYES)
		{
			//得到当前程序的路径；
			char strPath[100]; 
			GetModuleFileName(NULL,strPath,100);

			//创建守护进程，在启用新进程成功后发送WM_QUIT消息结束原来的进程；
			STARTUPINFO startInfo;
			PROCESS_INFORMATION processInfo;
			ZeroMemory(&startInfo, sizeof(STARTUPINFO));
			startInfo.cb = sizeof(STARTUPINFO);
			if(CreateProcess(NULL,(LPTSTR)(LPCTSTR)strPath, NULL, NULL,FALSE, 0, NULL, NULL, &startInfo, &processInfo))
			{
				CloseHandle( processInfo.hProcess );
				CloseHandle( processInfo.hThread );

				//强行关闭所有线程
				HANDLE hself = GetCurrentProcess();
				TerminateProcess(hself, 0);

				//PostQuitMessage(WM_CLOSE);
			}
		}
		// TODO: MiniDumpWriteDump
		//FatalAppExit(-1,  "程序错误");
		return /*EXCEPTION_CONTINUE_SEARCH*/EXCEPTION_EXECUTE_HANDLER;
	}


	void RunCrashHandler()
	{
		SetUnhandledExceptionFilter(UnhandledExceptionFilterEx);
		PreventSetUnhandledExceptionFilter();
	}
};


#define DeclareDumpFile() NSDumpFile::RunCrashHandler();