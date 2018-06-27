#pragma once
#include <tchar.h>

namespace Random {

	const TCHAR* const KStrImportFile = _T("导入项目");
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

		std::vector<CString /*strFileName*/> m_vecFileName;
	}*PRANDOM_NEW_PROJ,*LPRANDOM_NEW_PROJ;

	enum eTagRandomMsg
	{
		RandomMsg_NULL = WM_USER,


		Random_KNOWN = WM_USER +0xff,
	};
}