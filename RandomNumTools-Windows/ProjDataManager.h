#pragma once

#include "Singleton.h"

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

	bool resetMain();
	bool resetMute();
	bool resetDesignation();
	bool resetAllData();

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
	
	inline int readAllStrEx(FILE* pFile,std::map<int,std::string> &m_mapData);

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

	std::map<int /*nIndex*/, std::string /*strName*/> m_mapMainData;
	std::map<int /*nIndex*/, std::string /*strName*/> m_mapMuteData;
	std::map<int /*nIndex*/, std::string /*strName*/> m_mapDesignationData;
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
