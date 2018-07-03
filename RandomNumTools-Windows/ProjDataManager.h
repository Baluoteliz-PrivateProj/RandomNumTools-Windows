#pragma once

#include "Singleton.h"

typedef std::vector<int>::iterator vecIntIt;
typedef std::vector<std::string>::iterator vecStrIt;
typedef std::vector<int> vecInt;
typedef std::vector<std::string> vecStr;
typedef std::vector<CString>::iterator vecWStrIt;
typedef std::vector<CString> vecWStr;
typedef std::map<CString, std::vector<CString>>::iterator mapWStrVecIt;
typedef std::map<CString, std::vector<CString>> mapWStrVec;

class CRandom{
public:
	CRandom();
	CRandom(int nNumMin,int nNumMax);
	~CRandom();

	bool random(int &nRandomNum);
	bool randomRepeatable(int &nRandomNum);
	void resetRandom(int nNumMin,int nNumMax);
	bool muteNum(const int &nNum);
	bool addNum(const int &nNum);
	void resetRandom();

private:
	int m_nNumMin;
	int m_nNumMax;
	int m_nItemCount;

	bool m_bIsRandomOver;
	std::vector<int> m_vecUsed;

	int m_nNumRepeatLast;
};

class CFileData
{
public:
	CFileData();
	CFileData(const std::string &filepath1,const std::string &filepath2,const std::string &filepath3,const CString &strProjName);
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
		error_Random_NoValidProj = 64,

		error_UnKnown = 0xff,
	};

	bool resetMain();
	bool resetMute();
	bool resetDesignation();
	bool resetAllData();

	int dataCheckSelf(); //data check self
	int getFileStatus();
	CString getProjName();
	
	BOOL randomStr(std::string &strNum);
	BOOL randomStrRepeatable(std::string &strNum);
	bool add(const std::string &str, eFileType eType = eFileType::eType_Main);
	bool mute(const std::string &str, eFileType eType = eFileType::eType_Mute);
	bool designation(const std::string &str,eFileType eType = eFileType::eType_Designation);

protected:
	inline bool loadFile();
	inline void unloadFile();

	inline int readAllStr(eFileType type);
	inline int writeEndStr(eFileType type, const std::string &str);
	inline int getManiDataLines();
	
	inline int readAllStrEx(eFileType eType);
	inline int writeResult(const std::string &str);

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
	CString m_strProjName;

	std::vector<std::string> m_vecMainData;
	std::vector<std::string> m_vecMuteData;
	std::vector<std::string> m_vecDesignationData;
	typedef std::vector<std::string>::iterator vecStrIte;

	CRandom m_randomSpecial;
	CRandom m_randomMain;
	int m_errorType;
	CLock m_lock;

	CFileIO m_FileProjResult;
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

	CString getPrefixPath();
	void getProjData(std::map<CString,std::vector<CString>> &mapData);
	bool insertProjDataItem(LPRANDOM_NEW_PROJ lpData);
	void deleteItem(const CString strParam);
	void getVectorFiles(const CString strParma,std::vector<CString> &vecTemp);
	int ImportProj(const CString &strProjName);
	int resetProj(const CString &strProjName);

	void addStr(const std::string &str);
	void muteStr(const std::string &str);
	void designationStr(const std::string &str);
	bool RandomStr(CString &str);
	bool RandomStrRepeatable(CString &str);

private:
	int LoadProjData();
	int LoadProjDataExtra(const CString &strPrefixPath, const CString &strProjName);
	void unInitLoadProdData();

	CProjDataInstance();
	static CSingleton<CProjDataInstance>::CGrabo graboInstance;
	
	std::map<CString,std::vector<CString>> m_mapProject;
	CString m_strDefPrefixPath;
	CFileData* m_pFileData;
	CLock m_Lock;
};
