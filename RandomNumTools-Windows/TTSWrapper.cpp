#include "stdafx.h"
#include "Utilc.h"
using namespace CPlusBaluoteli;
using namespace CPlusBaluoteli::util;
using namespace CPlusBaluoteli::FormatStr;
using namespace CPlusBaluoteli::control;
#include "TTSWrapper.h"

CTTSWrapper::CTTSWrapper(void)
{
	m_pIEnumSpObjectTokens = NULL;
	m_pISpObjectToken = NULL;
	m_pISpVoice = NULL;
	m_bComInit = FALSE;
}

CTTSWrapper::~CTTSWrapper(void)
{
}

int CTTSWrapper::Init()
{
	//初始化COM组件  
	if (FAILED(::CoInitializeEx(NULL, 0)))
	{
		//MessageBox("初始化COM组件失败!", "提示", MB_OK|MB_ICONWARNING);  
		return -1;
	}

	m_bComInit = TRUE;
	return 0;
}

int CTTSWrapper::UnInit()
{
	if (m_bComInit)
	{
		::CoUninitialize();
	}

	return 0;
}

int CTTSWrapper::EnumAudioToken(CString arrayVoicePackageName[], int nVoicePackageNameCount)
{
	//枚举所有语音Token  
	if (SUCCEEDED(SpEnumTokens(SPCAT_VOICES, NULL, NULL, &m_pIEnumSpObjectTokens)))
	{
		//得到所有语音Token的个数  
		ULONG ulTokensNumber = 0;
		m_pIEnumSpObjectTokens->GetCount(&ulTokensNumber);

		//检测该机器是否安装有语音包  
		if (ulTokensNumber == 0)
		{
			//MessageBox("该机器没有安装语音包!", "提示", MB_OK|MB_ICONWARNING);  
			return -1;
		}
		if (ulTokensNumber > nVoicePackageNameCount)
		{
			//缓冲区过小  
			return 0;
		}

		//将语音包的名字加入数组中  
		CString strVoicePackageName = _T("");
		CString strTokenPrefixText = _T("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Speech\\Voices\\Tokens\\");
		for (ULONG i = 0; i < ulTokensNumber; i++)
		{
			m_pIEnumSpObjectTokens->Item(i, &m_pISpObjectToken);
			WCHAR* pChar;
			m_pISpObjectToken->GetId(&pChar);
			strVoicePackageName = pChar;
			strVoicePackageName.Delete(0, strTokenPrefixText.GetLength());
			arrayVoicePackageName[i] = strVoicePackageName;
		}

		return ulTokensNumber;
	}

	return -1;
}

//创建SpVoice  
int CTTSWrapper::CreateSpVoice()
{
	//获取ISpVoice接口  
	if (FAILED(CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void**)&m_pISpVoice)))
	{
		//MessageBox("获取ISpVoice接口失败!", "提示", MB_OK|MB_ICONWARNING);  
		return -1;
	}

	return 0;
}
//释放SpVoice  
int CTTSWrapper::DeleteSpVoice()
{
	if (m_pISpVoice != NULL)
	{
		m_pISpVoice->Release();
	}
	m_pISpVoice = NULL;

	return 0;
}
//重置SpVoice  
int CTTSWrapper::ResetSpVoice()
{
	DeleteSpVoice();
	return CreateSpVoice();
}

//设置朗读速度（取值范围：-10到10）  
int  CTTSWrapper::SetRate(long RateAdjust)
{
	if (m_pISpVoice == NULL)
		return -1;

	//设置播放速度  
	m_pISpVoice->SetRate(RateAdjust);
	return 0;
}
//获取朗读速度  
int  CTTSWrapper::GetRate(long *pRateAdjust)
{
	if (m_pISpVoice == NULL)
		return -1;

	m_pISpVoice->GetRate(pRateAdjust);
	return 0;
}

//设置使用的语音库  
int  CTTSWrapper::SetVoice(ISpObjectToken  *pToken)
{
	if (m_pISpVoice == NULL)
		return -1;

	m_pISpVoice->SetVoice(pToken);
	return 0;
}
//获取语音库  
int  CTTSWrapper::GetVoice(unsigned int nIndex, ISpObjectToken** ppToken)
{
	if (m_pIEnumSpObjectTokens == NULL)
		return -1;

	//设置语言  
	m_pIEnumSpObjectTokens->Item(nIndex, ppToken);
	m_pISpObjectToken = *ppToken;
	return 0;
}

//设置音量（取值范围：0到100）  
int  CTTSWrapper::SetVolume(USHORT usVolume)
{
	if (m_pISpVoice == NULL)
		return -1;

	//设置音量大小  
	m_pISpVoice->SetVolume(usVolume);
	return 0;
}
//获取音量  
int  CTTSWrapper::GetVolume(USHORT *pusVolume)
{
	if (m_pISpVoice == NULL)
		return -1;

	//设置音量大小  
	m_pISpVoice->GetVolume(pusVolume);
	return 0;
}

//开始朗读  
int CTTSWrapper::Speak(CString strContent, DWORD dwFlags)
{
	if (m_pISpVoice == NULL)
		return -1;

	//开始进行朗读  
	HRESULT hSucess = m_pISpVoice->Speak(strContent.AllocSysString(), dwFlags, NULL);

	return 0;
}
//朗读生成文件  
int CTTSWrapper::SpeakToWaveFile(CString strContent, char *pFilePathName, DWORD dwFlags)
{
	if (m_pISpVoice == NULL || pFilePathName == NULL)
		return -1;

	//生成WAV文件  
	CComPtr<ISpStream> cpISpStream;
	CComPtr<ISpStreamFormat> cpISpStreamFormat;
	CSpStreamFormat spStreamFormat;
	m_pISpVoice->GetOutputStream(&cpISpStreamFormat);
	spStreamFormat.AssignFormat(cpISpStreamFormat);
	HRESULT hResult = SPBindToFile(pFilePathName, SPFM_CREATE_ALWAYS,
		&cpISpStream, &spStreamFormat.FormatId(), spStreamFormat.WaveFormatExPtr());
	if (SUCCEEDED(hResult))
	{
		m_pISpVoice->SetOutput(cpISpStream, TRUE);
		m_pISpVoice->Speak(strContent.AllocSysString(), dwFlags, NULL);
		return 0;
		//MessageBox("生成WAV文件成功!", "提示", MB_OK);  
	}
	else
	{
		//MessageBox("生成WAV文件失败!", "提示", MB_OK|MB_ICONWARNING);  
		return 1;
	}
}

//暂停朗读  
int CTTSWrapper::Pause()
{
	if (m_pISpVoice != NULL)
	{
		m_pISpVoice->Pause();
	}

	return 0;
}

//继续朗读  
int CTTSWrapper::Resume()
{
	if (m_pISpVoice != NULL)
	{
		m_pISpVoice->Resume();
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

UINT CTTSInstance::ThreadTTSProc1(LPVOID param)
{
	CTTSInstance* pVoid = (CTTSInstance*)param;
	while (!pVoid->m_bResume){

		if (_T("") != pVoid->m_StrHitName) {

			DWORD dwStart = GetTickCount();
			TCHAR szbuf[MAX_PATH] = { '\0' };
			swprintf_s(szbuf, _T("%s,%s,%d\n"), L"ThreadTTSProc1", (pVoid->m_StrHitName), GetTickCount());
			OutputDebugStringW(szbuf);

			pVoid->m_TtsWrapper.Speak(pVoid->m_StrHitName);
			DWORD dwEnd = GetTickCount();
			DWORD dwInterval = dwEnd - dwStart;

			//pVoid->m_StrHitName = _T("");
		}

		//Sleep(500);
		pVoid->m_pTTSThread->SuspendThread();
	}

	CFormatStr::Baluoteliz_WriteLog("ThreadProc Exit m_bResume: %d\n", pVoid->m_bResume);

	return 0;
	return TRUE;
}

CSingleton<CTTSInstance>::CGrabo CTTSInstance::graboInstance;
CTTSInstance::CTTSInstance()
{
}

CTTSInstance::~CTTSInstance()
{
}

inline void CTTSInstance::initTTS()
{
	m_TtsWrapper.Init();

	CString arrayVoicePackageName[50];
	int nVoicePackageCount = 50;

	int nCount = m_TtsWrapper.EnumAudioToken(arrayVoicePackageName, nVoicePackageCount);
	m_TtsWrapper.CreateSpVoice();
	ISpObjectToken* ppToken = nullptr;

	m_TtsWrapper.GetVoice(0, &ppToken);
	m_TtsWrapper.SetVoice(ppToken);
	m_TtsWrapper.SetRate(0);
	m_TtsWrapper.SetVolume(100);

	m_pTTSThread = AfxBeginThread((AFX_THREADPROC)ThreadTTSProc1, this);
}

inline void CTTSInstance::uninitTTS()
{
	DWORD dwExitCode = 0;
	if (0 != GetExitCodeThread(m_pTTSThread->m_hThread, &dwExitCode)) {

		AfxEndThread(dwExitCode);
	}

	m_TtsWrapper.DeleteSpVoice();
	m_TtsWrapper.UnInit();
}

int CTTSInstance::speak(CString strContent, DWORD dwFlags /*= SPF_DEFAULT*/)
{
	return m_pTTSThread->ResumeThread();
}