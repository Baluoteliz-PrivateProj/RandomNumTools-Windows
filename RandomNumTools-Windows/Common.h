#pragma once
#include <tchar.h>
#include <mmsystem.h>
#pragma comment( lib, "Winmm.lib" )

namespace Random {

	const TCHAR* const KStrImportFile = _T("ѡ����Ŀ");
	const TCHAR* const KStrFullScreen = _T("ȫ��");
	const TCHAR* const KStrConfig = _T("����");

	const TCHAR* const KStrAdd = _T("����");
	const TCHAR* const KStrMute = _T("����");
	const TCHAR* const KStrDesignation = _T("ָ��");
	const TCHAR* const KStrStart = _T("��ʼ");
	const TCHAR* const KStrSure = _T("ȷ��");

	const TCHAR* const KStrAddTip = _T("�����µ�����");
	const TCHAR* const KStrMuteTip = _T("����ָ��������");
	const TCHAR* const KStrDesignationTip = _T("��ǰָ��������");

	const TCHAR* const KStrNew = _T("�½�");
	const TCHAR* const KStrEdit = _T("�༭");
	const TCHAR* const KStrDelete = _T("ɾ��");

	const TCHAR* const KCtrlItemIndex = _T("��Ŀ���");
	const TCHAR* const KCtrlItemName = _T("��Ŀ����");
	const TCHAR* const KCtrlItemCount = _T("��Ŀ�ļ���");
	const TCHAR* const KCtrlItemError = _T("�ļ��쳣");

	const TCHAR* const KListCtrlItemText = _T("ͨ��˫��ѡ��ĳһ����Ŀ");
	const TCHAR* const KERROR_AllDataItemMuted = _T("������Ч���ݶ��Ѿ���������");
	const TCHAR* const KERROR_DataFileEmpty = _T("�����ļ�Ϊ��.");
	const TCHAR* const KERROR_InSpecialFiles = _T("ָ���ļ��г�������Ч����");
	const TCHAR* const KERROR_SameItemConflict_MuteAndSpecial = _T("���κ�ָ���ļ��г�������ͬԪ�ص�����");
	const TCHAR* const KERROR_NoValidProj = _T("�����������Ŀ������");

	const TCHAR* const KOperatorNote = _T("������ʾ �ո�-��ʼ/ȷ��  ESC-�˳�ȫ��");

	const TCHAR* const KFileNameMute = _T("mute.dat");
	const TCHAR* const KFileNameDesignation = _T("designation.dat");

	enum eTypeData{
		eType_NULL = 0x00,
		eType_Group_NO,
		eType_Group_Yes,

		eType_UNKNOW = 0xff,
	};

	typedef struct RANDOM_NEW_PROJ
	{
		eTypeData dataTpye;
		int nGroupCount;
		CString strProjName;

		std::vector<CString /*strFileName*/> m_vecFileName;
	}*PRANDOM_NEW_PROJ,*LPRANDOM_NEW_PROJ;

	typedef struct eTagRANDOM_IMPORT_PROJ{
		CString strProjName;
		int nRes;
	}RANDOM_IMPORT_PROJ,*PRANDOM_IMPORT_PROJ,*LPRANDOM_IMPORT_PROJ;

	typedef struct eTagRANDOM_DELETE_PROJ{
		CString strProjName;
		int nRes;
	}RANDOM_DELETE_PROJ, *PRANDOM_DELETE_PROJ, *LPRANDOM_DELETE_PROJ;

	typedef struct eTagRandom_FULLSCREEN_SPEECK{
		CString strSpeechText;
	}RANDOM_FULLSCREEN_SPEECH,*PRANDOM_FULLSCREEN_SPEECH,*LPRANDOM_FULLSCREEN_SPEECH;

	typedef struct eTagRandom_FULLSCREEN_SHOWRESULT{
		int nIndex;
		CString strData;
		CString strResult;
	}Random_FULLSCREEN_SHOWRESULT, *PRandom_FULLSCREEN_SHOWRESULT, *LPRandom_FULLSCREEN_SHOWRESULT;

	typedef struct eTagRandom_Update_CandidateList
	{
		CString strRandom;
	}RANDOM_UPDATE_CANDIDATELILST, *PRANDOM_UPDATE_CANDIDATELILST, *LPRANDOM_UPDATE_CANDIDATELILST;

	enum eTagRandomMsg
	{
		RandomMsg_NULL = WM_USER,
		RandomMsg_UPDATA_LISTCTRL,
		RandomMsg_IMPORT_PROJ,
		RandomMsg_DELETE_PROJ,
		RandomMsg_FULLSCREEN_SPEECH,
		RandomMsg_UpdateRandom_Interval,
		RandomMsg_UpdateRandom_CandidateList,


		Random_KNOWN = WM_USER +0xff,
	};


	enum eTagEventRender
	{
		Event_Main_Start = 1,
		Event_Main_Choose = 2,
		Event_FulllScreen_Start = 3,
		Event_FullScreen_Choose = 4,

	};

	struct SoundControl{

		static void PASCAL playSoundBk()
		{
			PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			PlaySound(_T("..\\Sounds\\Working.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		}

		static void PASCAL playSoundHit()
		{
			PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			PlaySound(_T("..\\Sounds\\OK.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
	};
}