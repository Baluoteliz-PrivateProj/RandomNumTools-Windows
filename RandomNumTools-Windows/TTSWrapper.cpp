#include "stdafx.h"
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