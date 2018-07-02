#pragma once


#include <sapi.h> //包含TTS语音引擎头文件和库文件  
#include <sphelper.h>  
#include <string.h>  
#pragma comment(lib, "sapi.lib")  
#include "Singleton.h"

class CTTSWrapper
{
public:
	CTTSWrapper(void);
	virtual ~CTTSWrapper(void);

	int Init();
	int UnInit();

	//枚举所有语音Token  
	int EnumAudioToken(CString arrayVoicePackageName[], int nVoicePackageNameCount);

	//创建SpVoice  
	int CreateSpVoice();
	//释放SpVoice  
	int DeleteSpVoice();
	//重置SpVoice(用于临时清除朗读数据)  
	int ResetSpVoice();

	//设置朗读速度（取值范围：-10到10）  
	int  SetRate(long RateAdjust);
	//获取朗读速度  
	int  GetRate(long *pRateAdjust);

	//设置使用的语音库  
	int  SetVoice(ISpObjectToken   *pToken);
	//获取语音库  
	int  GetVoice(unsigned int nIndex, ISpObjectToken** ppToken);

	//设置音量（取值范围：0到100）  
	int  SetVolume(USHORT usVolume);
	//获取音量  
	int  GetVolume(USHORT *pusVolume);

	//朗读  
	int Speak(CString strContent, DWORD dwFlags = SPF_DEFAULT);
	//朗读生成文件  
	int SpeakToWaveFile(CString strContent, char *pFilePathName, DWORD dwFlags = SPF_DEFAULT);
	//暂停朗读  
	int Pause();
	//继续朗读  
	int Resume();
	//跳过部分朗读  
	int Skip(CString strItemType = L"Sentence", long lNumItems = 65535, ULONG *pulNumSkipped = NULL);

protected:
	IEnumSpObjectTokens * m_pIEnumSpObjectTokens;
	ISpObjectToken * m_pISpObjectToken;
	ISpVoice * m_pISpVoice;
	BOOL m_bComInit;
};


class CTTSInstance :public CSingleton<CTTSInstance>
{
public:
	friend CSingleton;
	~CTTSInstance();

	int speak(CString strContent, DWORD dwFlags = SPF_DEFAULT);
	
protected:
	static UINT ThreadTTSProc1(LPVOID param);
	inline void initTTS();
	inline void uninitTTS();

private:
	CTTSInstance();
	static CSingleton<CTTSInstance>::CGrabo graboInstance;
	CLock m_lock;

	CTTSWrapper m_TtsWrapper;
	bool m_bResume;
	CWinThread* m_pTTSThread;
	CString m_StrHitName;
};