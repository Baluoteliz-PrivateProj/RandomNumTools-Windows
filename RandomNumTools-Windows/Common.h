#pragma once
#include <tchar.h>

namespace Random {

	const TCHAR* const KStrImportFile = _T("������Ŀ");
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

	const TCHAR* const KCtrlItemIndex = _T("���");
	const TCHAR* const KCtrlItemName = _T("����");
	const TCHAR* const KCtrlItemCount = _T("�ļ���");

	const TCHAR* const KFileNameMute = _T("Mute.dat");
	const TCHAR* const KFileNameDesignation = _T("Designation.dat");

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