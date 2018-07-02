#pragma once


#include <sapi.h> //����TTS��������ͷ�ļ��Ϳ��ļ�  
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

	//ö����������Token  
	int EnumAudioToken(CString arrayVoicePackageName[], int nVoicePackageNameCount);

	//����SpVoice  
	int CreateSpVoice();
	//�ͷ�SpVoice  
	int DeleteSpVoice();
	//����SpVoice(������ʱ����ʶ�����)  
	int ResetSpVoice();

	//�����ʶ��ٶȣ�ȡֵ��Χ��-10��10��  
	int  SetRate(long RateAdjust);
	//��ȡ�ʶ��ٶ�  
	int  GetRate(long *pRateAdjust);

	//����ʹ�õ�������  
	int  SetVoice(ISpObjectToken   *pToken);
	//��ȡ������  
	int  GetVoice(unsigned int nIndex, ISpObjectToken** ppToken);

	//����������ȡֵ��Χ��0��100��  
	int  SetVolume(USHORT usVolume);
	//��ȡ����  
	int  GetVolume(USHORT *pusVolume);

	//�ʶ�  
	int Speak(CString strContent, DWORD dwFlags = SPF_DEFAULT);
	//�ʶ������ļ�  
	int SpeakToWaveFile(CString strContent, char *pFilePathName, DWORD dwFlags = SPF_DEFAULT);
	//��ͣ�ʶ�  
	int Pause();
	//�����ʶ�  
	int Resume();
	//���������ʶ�  
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