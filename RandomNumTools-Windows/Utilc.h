#pragma once

#include <tlhelp32.h>
#include <WinSock2.h>
#pragma comment(lib,"Ws2_32.lib")
#include <vector>
#include <time.h>

namespace CPlusBaluoteli
{
	namespace util{

		struct CommonFun
		{

			static std::string PASCAL getAbsoluteDir()
			{
				TCHAR path[MAX_PATH] = { 0 };
				GetModuleFileName(nullptr, path, MAX_PATH);

				std::string filePath = CStringA(path).GetBuffer();
				return filePath.substr(0, filePath.rfind("\\") + 1);
			}

			static std::string PASCAL getFilePath()
			{
				TCHAR path[MAX_PATH] = { 0 };
				GetModuleFileName(nullptr, path, MAX_PATH);
				return CStringA(path).GetBuffer();
			}

			static std::string  PASCAL getCurRunningExeName()
			{
				TCHAR path[MAX_PATH] = { 0 };
				GetModuleFileName(nullptr, path, MAX_PATH);

				std::string filePath = CStringA(path).GetBuffer();
				return filePath.substr(filePath.rfind("\\") + 1, filePath.length() - filePath.rfind("\\"));
			}

			static std::string  PASCAL getFileAbsolutePath(const std::string &file)
			{
				HMODULE hModule = GetModuleHandle(CString(file.c_str()));
				TCHAR path[MAX_PATH] = { 0 };
				GetModuleFileName(hModule, path, MAX_PATH);
				return CStringA(path).GetBuffer();
			}

			static std::string PASCAL getPirorDir(const std::string &file)
			{
				HMODULE hModule = GetModuleHandle(CString(file.c_str()));
				TCHAR path[MAX_PATH] = { 0 };
				GetModuleFileName(hModule, path, MAX_PATH);
				std::string fullpath = CStringA(path).GetBuffer();
				return fullpath.substr(0, fullpath.rfind("\\"));
			}

			static std::string  PASCAL getPirorDirEx(const std::string &file) 
			{
				return file.substr(0, file.rfind("\\") + 1);
			}

			static std::string PASCAL getRootDir(const std::string &file)
			{
				std::string FileDir = getFileAbsolutePath(file);
				return FileDir.substr(0, FileDir.find("\\") + 1);
			}

			static std::string PASCAL int2str(int nNum)
			{
				char str[MAX_PATH] = { 0 };
				_itoa_s(nNum, str, 10);
				return str;
			}

			static CString PASCAL int2CS(int nNum)
			{
				char str[MAX_PATH] = { 0 };
				_itoa_s(nNum, str, 10);
				return CString(str);
			}

			static std::string PASCAL float2str(float fValue)
			{
				char str[MAX_PATH] = { 0 };
				sprintf_s(str, "%f", fValue);
				return str;
			}

			static int PASCAL str2int(const std::string &str)
			{
				return atoi(str.c_str());
			}

			static int PASCAL str2long(const std::string &str)
			{
				return atoll(str.data());
			}

			static float PASCAL str2float(const std::string &str)
			{
				return (float)atof(str.c_str());
			}

			static CString PASCAL CommonFun::s2cs(const std::string &str)
			{
				return CString(str.c_str());
			}

			static std::string PASCAL CommonFun::cs2s(const CString &str)
			{
				CString sTemp(str);
				return CStringA(sTemp.GetBuffer()).GetBuffer();
			}

			static std::string PASCAL utf82gbk(const char *utf8)
			{
				std::string str;

				if (utf8 != NULL)
				{
					int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
					std::wstring strW;

					strW.resize(len);

					MultiByteToWideChar(CP_UTF8, 0, utf8, -1, (LPWSTR)strW.data(), len);

					len = WideCharToMultiByte(936, 0, strW.data(), len - 1, NULL, 0, NULL, NULL);

					str.resize(len);

					WideCharToMultiByte(936, 0, strW.data(), -1, (LPSTR)str.data(), len, NULL, NULL);
				}

				return str;
			}

			static std::string PASCAL gbk2utf8(const char *gbk)
			{
				std::string str;

				if (gbk != NULL)
				{
					int len = MultiByteToWideChar(936, 0, gbk, -1, NULL, 0);
					std::wstring strW;

					strW.resize(len);

					MultiByteToWideChar(936, 0, gbk, -1, (LPWSTR)strW.data(), len);

					len = WideCharToMultiByte(CP_UTF8, 0, strW.data(), len - 1, NULL, 0, NULL, NULL);

					str.resize(len);

					WideCharToMultiByte(CP_UTF8, 0, strW.data(), -1, (LPSTR)str.data(), len, NULL, NULL);
				}

				return str;
			}

			static std::string  PASCAL gbk2utf8(const std::string &gbk)
			{
				return gbk2utf8(gbk.c_str());
			}

			static std::string  PASCAL utf82gbk(const std::string &utf8)
			{
				return utf82gbk(utf8.c_str());
			}

			static std::string PASCAL getTime()
			{
				SYSTEMTIME st = { 0 };
				GetLocalTime(&st);
				CString timeStr;
				timeStr.Format(_T("%d%02d%02d-%02d%02d%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
				return CommonFun::cs2s(timeStr);
			}

			static DWORD PASCAL getProcessID(const std::string &processName)
			{
				HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
				PROCESSENTRY32 pe32;
				pe32.dwSize = sizeof(PROCESSENTRY32);
				hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
				if (INVALID_HANDLE_VALUE == hProcessSnap)
				{
					CloseHandle(hProcessSnap);
					return -1;
				}

				if (!Process32First(hProcessSnap, &pe32))
				{
					CloseHandle(hProcessSnap);
					return -1;
				}
				do
				{
					std::string processNameEnum = CStringA(pe32.szExeFile).GetBuffer();
					if (processNameEnum == processName)
					{
						CloseHandle(hProcessSnap);
						hProcessSnap = INVALID_HANDLE_VALUE;
						return pe32.th32ProcessID;
					}
				} while (Process32Next(hProcessSnap, &pe32));

				CloseHandle(hProcessSnap);
				return -1;
			}

			static bool  PASCAL closeProcess(const std::string &processName, int &num)
			{
				DWORD processId = getProcessID(processName);
				HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, false, processId);
				if (INVALID_HANDLE_VALUE != processHandle && processHandle)
				{
					num++;
					if (TerminateProcess(processHandle, 0))
					{
					}
					else
					{
						WaitForSingleObject(processHandle, 2000);
					}

					CloseHandle(processHandle);
				}
				else
				{
					return true;
				}
				return closeProcess(processName, num);
			}

			static bool PASCAL closeProcess(DWORD dwProcess){

				HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, false, dwProcess);
				if (INVALID_HANDLE_VALUE != processHandle){
					if (TerminateProcess(processHandle, 0)){}
					else
						WaitForSingleObject(processHandle, 2000);

					return CloseHandle(processHandle);
				}

				return false;
			}

			static void PASCAL closeCurrentProcess()
			{
				DWORD processId = GetCurrentProcessId();
				HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, false, processId);
				if (INVALID_HANDLE_VALUE != processHandle)
				{
					if (TerminateProcess(processHandle, 0))
					{
						CloseHandle(processHandle);
						return;
					}
					else
					{
						WaitForSingleObject(processHandle, 2000);
					}
				}

				CloseHandle(processHandle);
				return;
			}

			static int PASCAL getProcessIdMutil(const std::string &processName)
			{
				std::vector<DWORD> vecProcessid;
				HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
				PROCESSENTRY32 pe32;
				pe32.dwSize = sizeof(PROCESSENTRY32);
				hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
				if (INVALID_HANDLE_VALUE == hProcessSnap)
				{
					return vecProcessid.size();
				}
				if (!Process32First(hProcessSnap, &pe32))
				{
					CloseHandle(hProcessSnap);     // Must clean up the snapshot object!
					return vecProcessid.size();
				}

				do
				{
					if (processName == CommonFun::cs2s(pe32.szExeFile)){

						vecProcessid.push_back(pe32.th32ProcessID);
						printf("processName: %s, processId: %d\n", CStringA(pe32.szExeFile).GetBuffer(), pe32.th32ProcessID);
					}

				} while (Process32Next(hProcessSnap, &pe32));

				CloseHandle(hProcessSnap);
				return vecProcessid.size();
			}

			static std::vector<DWORD>  PASCAL getProcessMutilVec(const std::string processName)
			{
				std::vector<DWORD> vecProcessid;
				HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
				PROCESSENTRY32 pe32;
				pe32.dwSize = sizeof(PROCESSENTRY32);
				hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
				if (INVALID_HANDLE_VALUE == hProcessSnap)
				{
					return vecProcessid;
				}
				if (!Process32First(hProcessSnap, &pe32))
				{
					CloseHandle(hProcessSnap);     // Must clean up the snapshot object!
					return vecProcessid;
				}

				do
				{
					if (processName == CommonFun::cs2s(pe32.szExeFile)){

						vecProcessid.push_back(pe32.th32ProcessID);
						printf("processName: %s, processId: %d\n", CStringA(pe32.szExeFile).GetBuffer(), pe32.th32ProcessID);
					}

				} while (Process32Next(hProcessSnap, &pe32));

				CloseHandle(hProcessSnap);
				return vecProcessid;
			}

			static bool PASCAL registerStartUp()
			{
				HKEY hKey;
				DWORD dwDisposition;

				long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_ALL_ACCESS, &hKey);

				if (lRet == ERROR_SUCCESS)
				{
					CString currRunPath = CommonFun::s2cs((getFilePath()));
					lRet = RegSetValueEx(hKey, _T("RandomNumTools"), 0, REG_SZ, (const unsigned char*)currRunPath.GetBuffer(), (DWORD)(currRunPath.GetLength() * 2));
					currRunPath.ReleaseBuffer();

					RegCloseKey(hKey);
					return TRUE;
				}

				return FALSE;
			}

			static DWORD  PASCAL openProcess(const std::string &processName, const std::string &cmdLine)
			{
				DWORD processId;

				STARTUPINFO si;
				ZeroMemory(&si, sizeof(STARTUPINFO));
				si.cb = sizeof(STARTUPINFO);
				si.dwFlags = STARTF_USESHOWWINDOW;
				si.wShowWindow = SW_HIDE;
				PROCESS_INFORMATION pi;
				ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

				CString CmdLine;
				std::string fullpath = getAbsoluteDir() + processName;
				CmdLine.Format(_T("%s -%s"), CommonFun::s2cs(fullpath), CommonFun::s2cs(cmdLine));
				BOOL res = CreateProcess(NULL, (CmdLine).GetBuffer(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
				return pi.dwProcessId;
			}

			static std::string PASCAL getRandomAppLogPath(const std::string &strAttribute)
			{
				CString strRet;
				std::string strTime;
				std::string exeName;
				std::string pirorDir;

				pirorDir = getPirorDir(getFilePath());
				pirorDir = getPirorDirEx(pirorDir);
				strTime = getTime();

				exeName.append("RandomNumTools_app_");
				exeName.append(strAttribute);
				exeName.append(".log");

				strRet.Format(_T("%slogger\\%s_%s"), CommonFun::s2cs(pirorDir), CommonFun::s2cs(strTime), CommonFun::s2cs(exeName));
				CString logPirorDir = CommonFun::s2cs(getPirorDirEx(CommonFun::cs2s(strRet)));
				BOOL res = CreateDirectory(logPirorDir, NULL);

				return CommonFun::cs2s(strRet);
			}

#if 0

			std::string  getRandomUid()
			{
				int nNumMin = 1000000000;
				int nNumMax = 9999999999;
				int nNum = 0;

				if (TRUE){

					srand(time(NULL));
					nNum = nNumMin + MAKELONG(rand(), rand()) % (nNumMax - nNumMin);
					char logMsg[48] = { '\0' };
					sprintf_s(logMsg, "Monitor LoginUID: %d\n", nNum);
					CFormatStr::AgoraOutDebugStr(CommonFun::s2cs(logMsg));
				}

				return (nNum > 0) ? int2str(nNum) : "";
			}

#endif
		};
	}

	namespace FormatStr{

		class CFormatStr{
		public:

			static void PASCAL Baluoteliz_MessageBox(LPCTSTR lpFormat, ...){

				TCHAR szBuffer[1024] = { _T("\0") };
				va_list args;
				_tcsnccat_s(szBuffer, util::CommonFun::s2cs(gStrInstance + ": "), gStrInstance.length() + 2);
				va_start(args, lpFormat);
				_vsnwprintf(szBuffer + _tcslen(szBuffer), sizeof(szBuffer) / sizeof(TCHAR) - _tcslen(szBuffer), lpFormat, args);
				va_end(args);

				AfxMessageBox(szBuffer);
			}

			static void PASCAL Baluoteliz_OutDebugStr(LPCTSTR lpFormat, ...){

				TCHAR szBuffer[1024] = { _T("\0") };
				va_list args;
				_tcsnccat(szBuffer, _T("\n"), 1);
				va_start(args, lpFormat);
				_vsnwprintf(szBuffer + _tcslen(szBuffer), sizeof(szBuffer) / sizeof(TCHAR) - _tcslen(szBuffer), lpFormat, args);
				va_end(args);

				OutputDebugString(szBuffer);
			}

			static void  PASCAL Baluoteliz_WriteLog(LPSTR lpFormat, ...) {

				char szBuffer[1024] = { '\0' };
				va_list args;
				va_start(args, lpFormat);

				SYSTEMTIME st;
				GetLocalTime(&st);
				sprintf_s(szBuffer, "%04d%02d%02d:%02d%02d%02d%03d [%s] ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,st.wMilliseconds, gStrInstance.data());

				_vsnprintf(szBuffer + strlen(szBuffer), sizeof(szBuffer) / sizeof(char) - strlen(szBuffer), lpFormat, args);
				va_end(args);

				gLogRandom.write(szBuffer);
			}
		};
	}

	namespace control {

		class CLock
		{
		public:
			CLock(){
				m_pCriticalSection = new CRITICAL_SECTION;
				assert(m_pCriticalSection);
				InitializeCriticalSection(m_pCriticalSection);
			}
			~CLock(){
				if (m_pCriticalSection){
					DeleteCriticalSection(m_pCriticalSection);
					delete m_pCriticalSection;
					m_pCriticalSection = nullptr;
				}
			}

			void lock(){
				EnterCriticalSection(m_pCriticalSection);
			}

			void unlock(){
				LeaveCriticalSection(m_pCriticalSection);
			}

		private:
			LPCRITICAL_SECTION m_pCriticalSection;
		};

		class CAutoLock
		{
		public:
			CAutoLock(CLock *pLock) :m_pLock(pLock){
				assert(m_pLock);
				m_pLock->lock();
			}
			~CAutoLock(){
				m_pLock->unlock();
			}

		private:
			CLock *m_pLock;
		};

		class CHighResoluteFrameCtrl
		{//Get frame rate in real time. More accurate.
		public:
			CHighResoluteFrameCtrl(){
				tickInterval_ = 0;
				frameCount_ = 0;
				counterInterval_.QuadPart = 0;
				lfrequency_.QuadPart = 0;
				counterBegin_.QuadPart = 0;
				counterPiror_.QuadPart = 0;
				counterNext_.QuadPart = 0;
			}

			~CHighResoluteFrameCtrl(){
				tickInterval_ = 0;
				frameCount_ = 0;
				counterInterval_.QuadPart = 0;
				lfrequency_.QuadPart = 0;
				counterBegin_.QuadPart = 0;
				counterPiror_.QuadPart = 0;
				counterNext_.QuadPart = 0;
			}

			void setInterval(unsigned int interval){

				assert(interval && 0 < interval);
				if (tickInterval_ == interval)
				{
					return;
				}
				if (tickInterval_)
				{
					tickInterval_ = 0;
					frameCount_ = 0;
					counterInterval_.QuadPart = 0;
					lfrequency_.QuadPart = 0;
					counterBegin_.QuadPart = 0;
					counterPiror_.QuadPart = 0;
					counterNext_.QuadPart = 0;
				}
				tickInterval_ = interval;

				QueryPerformanceFrequency(&lfrequency_);
				QueryPerformanceCounter(&counterBegin_);
				counterPiror_ = counterBegin_;
				counterInterval_.QuadPart = lfrequency_.QuadPart * tickInterval_ / 1000;
			}

			float rate(){
				LARGE_INTEGER counterCurrent;
				QueryPerformanceCounter(&counterCurrent);
				return float(frameCount_ / ((counterCurrent.QuadPart - counterBegin_.QuadPart) / lfrequency_.QuadPart));
			}

			int wait(){

				if (!counterInterval_.QuadPart)
				{
					printf("please setInterval first!!!!!!\n");
					return false;
				}

				frameCount_++;
				LARGE_INTEGER counterCurrent;
				QueryPerformanceCounter(&counterCurrent);
				LONGLONG counterEscape = counterInterval_.QuadPart * frameCount_ - (counterCurrent.QuadPart - counterBegin_.QuadPart);
				LONGLONG res = counterEscape;

				//TO DO
				while (0 < counterEscape)
				{
					Sleep(1);
					QueryPerformanceCounter(&counterCurrent);
					counterEscape = counterInterval_.QuadPart * frameCount_ - (counterCurrent.QuadPart - counterBegin_.QuadPart);
				}

				return (int)res;
			}

			int getFrameCount(){

				return frameCount_;
			}

		private:
			unsigned int tickInterval_;
			int frameCount_;
			LARGE_INTEGER lfrequency_;
			LARGE_INTEGER counterInterval_;
			LARGE_INTEGER counterBegin_;
			LARGE_INTEGER counterPiror_;
			LARGE_INTEGER counterNext_;
		};

	}

}