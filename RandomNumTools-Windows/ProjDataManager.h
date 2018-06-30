#pragma once

#include "Singleton.h"

class CRandom{
public:
	CRandom();
	CRandom(int nNumMin,int nNumMax);
	~CRandom();

	bool random(int &nRandomNum);
	void resetRandom(int nNumMin,int nNumMax);
	void muteIndex(const int &nIndex);

private:
	int m_nNumMin;
	int m_nNumMax;

	bool m_bIsRandomOver;
	typedef std::vector<int>::iterator vecIt;
	std::vector<int> m_vecUsed;
};

class CFileData
{
public:
	CFileData();
	CFileData(const std::string &filepath1,const std::string &filepath2,const std::string &filepath3);
	~CFileData();

	enum eFileType{
		eType_NULL = -1,
		eType_Main,
		eType_Mute,
		eType_Designation,

		eType_UNKOWN = 0xff
	};

	enum error_FileData{
		error_NULL = 0,
		error_Ok = 1,
		error_AllDataItemMuted = 2,//all data belong to mute data
		error_SameItemConflict_MuteAndSpecial = 4,//the same item between mute data and designation data.
		error_DataFile_Empty = 8,
		error_InValidItem_InSpecialFiles = 16, //inivalid dataitme in designation.dat.
		error_Random_Over = 32,

		error_UnKnown = 0xff,
	};

	bool resetMain();
	bool resetMute();
	bool resetDesignation();
	bool resetAllData();

	int dataCheckSelf(); //data check self
	
	BOOL randomStr(std::string &strNum);
	bool add(const std::string &str, eFileType eType = eFileType::eType_Main);
	bool mute(const std::string &str, eFileType eType = eFileType::eType_Mute);
	bool designation(const std::string &str,eFileType eType = eFileType::eType_Designation);

protected:
	inline bool loadFile();
	inline void unloadFile();

	inline int readAllStr(eFileType type);
	inline int writeEndStr(eFileType type, const std::string &str);
	inline int getManiDataLines();
	
	typedef struct etagStatus{
		etagStatus(int nIndex, bool bstatus):
			m_bstatus(bstatus),
			m_nIndex(nIndex){
		}
		etagStatus():
		m_bstatus(false),
		m_nIndex(0){
		}
		bool m_bstatus;
		int m_nIndex;
	}STATUS, *PSTATUS, *LPSTATUS;

	typedef struct etagLineInfo{
		etagLineInfo() :
			m_strText(""),
			m_bStatus(false) {
		}
		etagLineInfo(const std::string &strText,const bool &bstatus):
		m_strText(strText),
		m_bStatus(bstatus) {
		}
		bool operator <(const etagLineInfo& lineinfo) const{
			if (this != &lineinfo)
				return m_strText < lineinfo.m_strText;
		}
		std::string m_strText;
		int m_bStatus;
	}LINEINFO,*PLINEINFO,*LPLINEINFO;
	
	typedef std::map<int,LINEINFO>::iterator iteratorMain;
	typedef std::map<std::string, STATUS>::iterator iteratorSpecial;

	inline int readAllStrEx(eFileType eType);

private:

	FILE* m_pFileMain;
	FILE* m_pFileMute;
	FILE* m_pFileDesignation;
	std::string m_strMainFilePath;
	std::string m_strMuteFilePath;
	std::string m_strDesignationPath;
	int m_nFileMainLineCount;
	int m_nFileMuteLineCount;
	int m_nFileDesignationLineCount;

	std::string m_strMainBuffer;
	std::string m_strMuteBuffer;
	std::string m_strDesignationBuffer;

// 	std::map<int /*nIndex*/, LINEINFO/*status*/> m_mapMainData;
// 	std::map<std::string /*strName*/, STATUS /*status*/> m_mapMuteData;
// 	std::map<std::string /*strName*/, STATUS /*status*/> m_mapDesignationData;

	std::vector<std::string> m_vecMainData;
	std::vector<std::string> m_vecMuteData;
	std::vector<std::string> m_vecDesignationData;
	typedef std::vector<std::string>::iterator vecStrIte;

	CRandom m_randomSpecial;
	CRandom m_randomMain;
	int m_errorType;
};

class CProjData
{
public:
	CProjData();
	CProjData(const CString &strPrefixPath,const CString &strProjName);
	~CProjData();

	void setDataType(const eTypeData &eType);
	eTypeData getDataType();

	void setGroupCount(int nCount);
	int getGroupCount();

	void setFileCount(int nCount);
	int getFileCount();

	void setProjPrefixPath(const CString &strPath);
	void getProjPrefixPath(CString &strPath);

	void setMainDataName(const CString &strName);
	void getMainDataName(CString &strName);

	void setMuteDataName(const CString &strName);
	void getMuteDataName(CString &strName);

	void setDesignationName(const CString &strName);
	void getDesignationName(CString &strName);
	
public:

	void addData(const CString &strData);
	void muteData(const CString &strData);
	void designationData(const CString &strData);

protected:
	inline int loadAllFileData();
	inline void unloadAllFileData();

private:

	eTypeData m_eDataType;
	int m_nGroupCount;
	int m_nFileCount;
	CString m_strProjPrefixPath;
	CString m_strMainDataName;
	CString m_strMuteDataName;
	CString m_strDesignationName;

	int m_nAllProjCount;
	std::vector<std::string > m_vecMuteName;
	std::vector<std::string> m_vecDesignationName;

	CFileIO m_fileIOMain;
	CFileIO m_fileIOMute;
	CFileIO m_fileIODesignation;

	CLock m_Lock;
};

class CProjDataInstance :public CSingleton<CProjDataInstance>
{
public:
	friend CSingleton;
	~CProjDataInstance();

	int LoadProjData();
	int LoadProjDataExtra(const CString &strPrefixPath,const CString &strProjName);

	void unInitLoadProdData();

private:
	CProjDataInstance();
	static CSingleton<CProjDataInstance>::CGrabo graboInstance;
	
	CString m_strDefPrefixPath;
	std::vector<CProjData*> m_vecProjData;
	CLock m_Lock;
};
