#pragma once
#include <tchar.h>
#include <mmsystem.h>
#pragma comment( lib, "Winmm.lib" )

namespace Random {

	const TCHAR* const KStrImportFile = _T("新建");
	const TCHAR* const KStrFullScreen = _T("全屏");
	const TCHAR* const KStrConfig = _T("配置");

	const TCHAR* const KStrAdd = _T("增加");
	const TCHAR* const KStrMute = _T("屏蔽");
	const TCHAR* const KStrDesignation = _T("指定");
	const TCHAR* const KStrStart = _T("开始");
	const TCHAR* const KStrSure = _T("确认");

	const TCHAR* const KStrAddTip = _T("增加新的名称");
	const TCHAR* const KStrMuteTip = _T("屏蔽指定的名称");
	const TCHAR* const KStrDesignationTip = _T("提前指定的名称");

	const TCHAR* const KStrNew = _T("新建");
	const TCHAR* const KStrEdit = _T("编辑");
	const TCHAR* const KStrDelete = _T("删除");

	const TCHAR* const KCtrlItemIndex = _T("项目序号");
	const TCHAR* const KCtrlItemName = _T("项目名称");
	const TCHAR* const KCtrlItemCount = _T("项目文件数");
	const TCHAR* const KCtrlItemError = _T("文件异常");

	const TCHAR* const KListCtrlItemText = _T("通过双击选中某一个项目");
	const TCHAR* const KERROR_AllDataItemMuted = _T("所有有效数据都已经被屏蔽了");
	const TCHAR* const KERROR_DataFileEmpty = _T("数据文件为空.");
	const TCHAR* const KERROR_InSpecialFiles = _T("指定文件中出现了无效数据");
	const TCHAR* const KERROR_SameItemConflict_MuteAndSpecial = _T("屏蔽和指定文件中出现了相同元素的数据");
	const TCHAR* const KERROR_NoValidProj = _T("不存在这个项目的数据");

	const TCHAR* const KOperatorNote = _T("操作提示 空格-开始/确定  ESC-退出全屏");

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

	enum eTagRandomMsg
	{
		RandomMsg_NULL = WM_USER,
		RandomMsg_UPDATA_LISTCTRL,
		RandomMsg_IMPORT_PROJ,
		RandomMsg_DELETE_PROJ,
		RandomMsg_FULLSCREEN_SPEECH,
		RandomMsg_UpdateRandom_Interval,


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