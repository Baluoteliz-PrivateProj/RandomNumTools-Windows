#include "stdafx.h"
#include "Utilc.h"
using namespace CPlusBaluoteli;
using namespace CPlusBaluoteli::util;
using namespace CPlusBaluoteli::control;
#include "ProjDataManager.h"

CRandom::CRandom():
m_nNumMin(0),
m_nNumMax(0),
m_bIsRandomOver(false)
{
}

CRandom::CRandom(int nNumMin, int nNumMax):
m_nNumMin(nNumMin),
m_nNumMax(nNumMax),
m_bIsRandomOver(false)
{
}

CRandom::~CRandom()
{
	m_vecUsed.clear();
	m_nNumMin = 0;
	m_nNumMax = 0;
}

bool CRandom::random(int &nRandomNum)
{
	bool bRes = false;
	if (!m_bIsRandomOver) {
		do	{
			srand(time(NULL));
			nRandomNum = m_nNumMin + (rand() % (m_nNumMax - m_nNumMin + 1));
			vecIt it = find(m_vecUsed.begin(), m_vecUsed.end(), nRandomNum);
			if (m_vecUsed.end() == it) {
				bRes = true;
				m_vecUsed.push_back(nRandomNum);
				if (m_vecUsed.size() == m_nNumMax)
					m_bIsRandomOver = true;
			}
		} while (!bRes);
	}

	return bRes;
}

void CRandom::resetRandom(int nNumMin, int nNumMax)
{
	if (m_nNumMin != nNumMin || m_nNumMax != nNumMax) {

		m_bIsRandomOver = false;
		m_vecUsed.clear();
		m_nNumMin = nNumMin;
		m_nNumMax = nNumMax;
	}
}

void CRandom::muteIndex(const int &nIndex)
{
	m_vecUsed.push_back(nIndex);
}
/*
 * className: CFileData
 * the class main designed for randomNum Tools.Mainly deal with Three Nums fiiles: ***.dat,mute.dat,designation.dat.
 * the class base on FILE,all of this deal with characters
 * Author: baluoteliz@gmail.com.
*/

CFileData::CFileData():
m_pFileDesignation(NULL),
m_pFileMain(nullptr),
m_pFileMute(nullptr),
m_nFileMainLineCount(0),
m_nFileMuteLineCount(0),
m_nFileDesignationLineCount(0),
m_errorType(error_FileData::error_NULL)
{
	m_strMainBuffer.clear();
	m_strMuteBuffer.clear();
	m_strDesignationBuffer.clear();

	m_vecDesignationData.clear();
	m_vecMainData.clear();
	m_vecMuteData.clear();

	m_strMainFilePath.clear();
	m_strMuteFilePath.clear();
	m_strDesignationPath.clear();
}

CFileData::CFileData(const std::string &filepath1, const std::string &filepath2, const std::string &filepath3):
m_pFileDesignation(NULL),
m_pFileMain(nullptr),
m_pFileMute(nullptr),
m_nFileMainLineCount(0),
m_nFileMuteLineCount(0),
m_nFileDesignationLineCount(0),
m_strMainFilePath(filepath1),
m_strMuteFilePath(filepath2),
m_strDesignationPath(filepath3),
m_errorType(error_FileData::error_NULL)
{
	m_strMainBuffer.clear();
	m_strMuteBuffer.clear();
	m_strDesignationBuffer.clear();

	m_vecDesignationData.clear();
	m_vecMainData.clear();
	m_vecMuteData.clear();

	loadFile();
	dataCheckSelf();
	m_randomMain.resetRandom(1,m_nFileMainLineCount);
	m_randomSpecial.resetRandom(1, m_nFileDesignationLineCount);
}

CFileData::~CFileData()
{
	unloadFile();
}

bool CFileData::resetMain()
{
	m_vecMainData.clear();

	fclose(m_pFileMain);
	m_pFileMain = nullptr;
	m_strMainBuffer.clear();
	m_nFileMainLineCount = 0;

	if (NULL != (m_pFileMain = fopen(m_strMainFilePath.data(), "ab+"))) {
		return readAllStrEx(eType_Main);
	}

	return false;
}

bool CFileData::resetMute()
{
	m_vecMuteData.clear();
	fclose(m_pFileMute);
	m_pFileMute = nullptr;
	m_strMuteBuffer.clear();
	m_nFileMuteLineCount = 0;

	if (NULL != (m_pFileMute = fopen(m_strMuteFilePath.data(), "ab+"))) {
		return readAllStrEx(eType_Mute);
	}

	return false;
}

bool CFileData::resetDesignation()
{
	m_vecDesignationData.clear();
	fclose(m_pFileDesignation);
	m_pFileDesignation = nullptr;
	m_strDesignationBuffer.clear();
	m_nFileDesignationLineCount = 0;

	if (NULL != (m_pFileDesignation = fopen(m_strDesignationPath.data(), "ab+"))) {
		return readAllStrEx(eType_Designation);
	}

	return false;
}

bool CFileData::resetAllData()
{
	return resetMain() && resetMute() && resetDesignation();

#if 0
	unloadFile();
	return loadFile();
#endif
}

BOOL CFileData::randomStr(std::string &strNum)
{
	if (m_errorType & error_Ok) {

		int nNum = 0;
		std::string strName;

		{//random special data
			m_randomSpecial.resetRandom(1,m_nFileDesignationLineCount);
			if (m_randomSpecial.random(nNum)) {
				strNum = m_vecDesignationData[nNum - 1];
				vecStrIte itMain = find(m_vecMainData.begin(), m_vecMainData.end(), strNum);
				int nIndexByMain = itMain - m_vecMainData.begin() + 1;
				m_randomMain.muteIndex(nIndexByMain);
				return m_errorType;
			}
		}

		{//random from source file
			int nRandomCount = 0;
			m_randomMain.resetRandom(1, m_nFileMainLineCount);
			do {
				if (m_randomMain.random(nNum))
						strNum = m_vecMainData[nNum - 1];
					else{
						m_errorType |= error_Random_Over;
						break;
					}
			} while (m_vecMuteData.end() != find(m_vecMuteData.begin(), m_vecMuteData.end(), strNum) && (++nRandomCount) <= m_nFileMuteLineCount);
		}
	}

	return m_errorType;
}

bool CFileData::add(const std::string &str, eFileType eType /*= eFileType::eType_Main*/)
{
	switch (eType)
	{
	case eFileType::eType_Main: {
		vecStrIte itMain = find(m_vecMainData.begin(), m_vecMainData.end(), str);
		if (m_vecMainData.end() == itMain) {

			m_nFileMainLineCount++;
			m_vecMainData.push_back(str);
			fseek(m_pFileMain, 0, SEEK_END);
			return fwrite((char*)str.data(), sizeof(char), str.length(), m_pFileMain);
		}
	}
								break;
	case eFileType::eType_Mute:
		break;;
	case eFileType::eType_Designation:
		break;
	default:break;
	}
}

bool CFileData::mute(const std::string &str, eFileType eType /*= eFileType::eType_Mute*/)
{
	switch (eType)
	{
	case eFileType::eType_Main:
		break;
	case eFileType::eType_Mute: {
		vecStrIte itMute = find(m_vecMuteData.begin(), m_vecMuteData.end(), str);
		if (m_vecMuteData.end() != itMute) {

			m_nFileMuteLineCount++;
			m_vecMuteData.push_back(str);
			fseek(m_pFileMute, 0, SEEK_END);
			return fwrite((char*)str.data(), sizeof(char), str.length(), m_pFileMute);
		}
	}
								break;;
	case eFileType::eType_Designation:
		break;
	default:break;
	}
}

bool CFileData::designation(const std::string &str, eFileType eType /*= eFileType::eType_Designation*/)
{
	switch (eType)
	{
	case eFileType::eType_Main:
		break;
	case eFileType::eType_Mute:
		break;
	case eFileType::eType_Designation: {
		vecStrIte itSpecial = find(m_vecDesignationData.begin(), m_vecDesignationData.end(), str);
		if (m_vecDesignationData.end() != itSpecial) {

			m_nFileDesignationLineCount++;
			m_vecDesignationData.push_back(str);
			fseek(m_pFileDesignation, 0, SEEK_END);
			return fwrite((char*)str.data(), sizeof(char), str.length(), m_pFileDesignation);
		}
	}
									   break;
	default:break;
	}
}

bool CFileData::loadFile()
{
	bool bReturn = false;
	char chread[1] = { '\0' };
	int nNumRead = 0; int nNumWritten = 0;
	if (nullptr == m_pFileMain) {
		if (nullptr != (m_pFileMain = fopen(m_strMainFilePath.data(), "ab+"))) {
			readAllStrEx(eType_Main);
		}
		else
			bReturn &= false;
	}

	if (nullptr == m_pFileMute) {
		if (nullptr != (m_pFileMute = fopen(m_strMuteFilePath.data(), "ab+"))) {
			readAllStrEx(eType_Mute);
		}
		else
			bReturn &= false;
	}

	if (nullptr == m_pFileDesignation) {
		if (nullptr != (m_pFileDesignation = fopen(m_strDesignationPath.data(), "ab+"))) {
			readAllStrEx(eType_Designation);
		}
		else
			bReturn &= false;
	}

	return bReturn;
}

void CFileData::unloadFile()
{
	m_vecMainData.clear();
	if (m_pFileMain) {
		fclose(m_pFileMain);
		m_pFileMain = nullptr;
	}

	m_vecMuteData.clear();
	if (m_pFileMute) {
		fclose(m_pFileMute);
		m_pFileMute = nullptr;
	}

	m_vecDesignationData.clear();
	if (m_pFileDesignation){
		fclose(m_pFileDesignation);
		m_pFileDesignation = nullptr;
	}

	m_strMainBuffer.clear();
	m_strMuteBuffer.clear();
	m_strDesignationBuffer.clear();
}

inline int CFileData::readAllStr(eFileType type)
{
	int nNumRead = 0;
	switch (type)
	{
	case eFileType::eType_Main: {
		while (!feof(m_pFileMain)) {
			char szbuf[10] = { '\0' };
			int nReadLen = 10;
			nNumRead = fread(szbuf, sizeof(char), nReadLen, m_pFileMain);
			if (ferror(m_pFileMain)) {
				break;
			}
			m_strMainBuffer.append(szbuf,nNumRead);
			nNumRead = m_strMainBuffer.length();
		}
	}
		break;
	case eFileType::eType_Mute: {
		while (!feof(m_pFileMute)) {
			char szbuf[10] = { '\0' };
			int nReadLen = 10;
			nNumRead = fread(szbuf, sizeof(char), nReadLen, m_pFileMute);
			if (ferror(m_pFileMute)) {
				break;
			}
			m_strMuteBuffer.append(szbuf, nNumRead);
			nNumRead = m_strMuteBuffer.length();
		}
	}
		break;
	case eFileType::eType_Designation: {
		while (!feof(m_pFileDesignation)) {
			char szbuf[10] = { '\0' };
			int nReadLen = 10;
			nNumRead = fread(szbuf, sizeof(char), nReadLen, m_pFileDesignation);
			if (ferror(m_pFileDesignation)) {
				break;
			}
			m_strDesignationBuffer.append(szbuf, nNumRead);
			nNumRead = m_strDesignationBuffer.length();
		}
	}
		break;
	default:break;
	}

	return nNumRead;
}

inline int CFileData::readAllStrEx(eFileType eType)
{
	FILE* pFile = nullptr;
	pFile = (eType==eType_Main ? (m_pFileMain):(eType == eType_Mute ? (m_pFileMute):(eType == eType_Designation?(m_pFileDesignation):(NULL))));

	int nRet = 0;
	std::string strName;
	int nDataLines = 0;
	while (pFile && !feof(pFile)) {
		char chread[2] = { '\0' };
		int nNumRead = fread(chread, sizeof(char), 1, pFile);
		nRet += nNumRead;
		if (ferror(pFile))
			break;
		if ('\r' == chread[0]) {
			nNumRead = fread(chread, sizeof(char), 1, pFile);
			nRet += nNumRead;
			if (ferror(pFile))
				break;
			if ('\n' == chread[0]) {
				if ("" != strName) {
					nDataLines++;
					if (eType_Main == eType)
						m_vecMainData.push_back(strName);
					else if (eType_Mute == eType)
						m_vecMuteData.push_back(strName);
					else if (eType_Designation == eType)
						m_vecDesignationData.push_back(strName);
					strName.clear();
				}
				continue;
			}
		}

		strName += chread;
	}

	if (!strName.empty() && !strName.empty()) {
		nDataLines++;
		if (eType_Main == eType)
			m_vecMainData.push_back(strName);
		else if (eType_Mute == eType)
			m_vecMuteData.push_back(strName);
		else if (eType_Designation == eType)
		m_vecDesignationData.push_back(strName);
		strName.clear();
	}
	if (m_pFileMain == pFile)
		m_nFileMainLineCount = nDataLines;
	else if (m_pFileMute == pFile)
		m_nFileMuteLineCount = nDataLines;
	else if (m_pFileDesignation == pFile)
		m_nFileDesignationLineCount = nDataLines;

	return nRet;
}

int CFileData::dataCheckSelf()
{
	m_errorType = error_NULL;
	//error one:  specialed data is greater than source data.
	if (0 == m_nFileMainLineCount)
		m_errorType |= error_DataFile_Empty;

	bool bFlage = true;
	//Invalid data item in the specified file,the items not int the main files
	if (0 < m_nFileMainLineCount) {
		for (vecStrIte itSpecial = m_vecDesignationData.begin(); m_vecDesignationData.end() != itSpecial; itSpecial++) {

			vecStrIte itMain = find(m_vecMainData.begin(), m_vecMainData.end(), *itSpecial);
			if (m_vecMainData.end() == itMain) {
				bFlage = false;
				break;
			}
		}
		if (!bFlage) {
			m_errorType |= error_InValidItem_InSpecialFiles;
		}
	}

	//all the data is mute in the mute files.
	bFlage = true;
	if (m_nFileMuteLineCount >= m_nFileMainLineCount && 0 < m_nFileMainLineCount) {
		int nCount = 0;
		for (vecStrIte itMain = m_vecMainData.begin(); m_vecMainData.end() != itMain; itMain++) {

			vecStrIte itMute = find(m_vecMuteData.begin(), m_vecMuteData.end(), *itMain);
			nCount++;
			if (m_vecMuteData.end() == itMute)
				break;
		}
		if (nCount == m_nFileMuteLineCount)
			bFlage = false;

		if (!bFlage)
			m_errorType |= error_AllDataItemMuted;
	}

	bFlage = true;
	//error two: the same data appears int the marked data and int the specialed data.
	if (0 < m_nFileMainLineCount) {
		for (vecStrIte itMute = m_vecMuteData.begin(); m_vecMuteData.end() != itMute; itMute++) {

			vecStrIte itSpecial = find(m_vecDesignationData.begin(), m_vecDesignationData.end(), (*itMute));
			if (m_vecDesignationData.end() != itSpecial) {
				bFlage = false;
				break;
			}
		}

		if (!bFlage)
			m_errorType |= error_SameItemConflict_MuteAndSpecial;
	}

	if (0 == m_errorType)
		m_errorType |= error_Ok;

	return m_errorType;
}

inline int CFileData::writeEndStr(eFileType type,const std::string &str)
{
	int nWritten = 0;
	switch (type)
	{
	case eFileType::eType_Main:
		fseek(m_pFileMain, 0, SEEK_END);
		nWritten = fwrite(str.data(), sizeof(char), str.length(), m_pFileMain);
		break;
	case eFileType::eType_Mute:
		fseek(m_pFileMute, 0, SEEK_END);
		nWritten = fwrite(str.data(), sizeof(char), str.length(), m_pFileMute);
		break;
	case eFileType::eType_Designation:
		fseek(m_pFileDesignation, 0, SEEK_END);
		nWritten = fwrite(str.data(), sizeof(char), str.length(), m_pFileDesignation);
		break;
	default:break;
	}

	return nWritten;
}

inline int CFileData::getManiDataLines()
{
	char *pData = (char*)m_strMainBuffer.data();
	char *pKeyStr = "\r\n";
	int nLenKey = strlen(pKeyStr);
	int nTempCount = 0;
	int nRet = 0;
	while ('\0' != *pData) {
		
		if (*pData == *pKeyStr) {
			nTempCount++;
			pKeyStr++;
			if (nTempCount == nLenKey) {
				nRet++;
				pKeyStr -= nTempCount;
				nTempCount = 0;
			}
		}

		pData++;
	}

	return nRet;
}
//////////////////////////////////////////////////////////////////////////
/*
* className: CProjData
* this class is similar to CFileData, but  the difference is that it's more tightly connected to businese.and
* the expansion is more stronger.
* author: baluoteliz@gmail.com
*/

CProjData::CProjData()
{
	m_eDataType = eTypeData::eType_Group_NO;
	m_nGroupCount = 1;
	m_nFileCount = 0;

	loadAllFileData();
}

CProjData::~CProjData()
{
	unloadAllFileData();
}

CProjData::CProjData(const CString &strPrefixPath, const CString &strProjName)
{
	m_eDataType = eTypeData::eType_Group_NO;
	m_nGroupCount = 1;
	m_nFileCount = 0;

	m_strProjPrefixPath = strPrefixPath;
	m_strMainDataName = strProjName;

	loadAllFileData();
}

void CProjData::setDataType(const eTypeData &eType)
{
	CAutoLock al(&m_Lock);

	m_eDataType = eType;
}

eTypeData CProjData::getDataType()
{
	CAutoLock al(&m_Lock);

	return m_eDataType;
}

void CProjData::setGroupCount(int nCount)
{
	CAutoLock al(&m_Lock);
	
	m_nGroupCount = nCount;
}

int CProjData::getGroupCount()
{
	CAutoLock al(&m_Lock);

	return m_nGroupCount;
}

void CProjData::setFileCount(int nCount)
{
	CAutoLock al(&m_Lock);

	m_nFileCount = nCount;
}

int CProjData::getFileCount()
{
	CAutoLock al(&m_Lock);

	return m_nFileCount;
}

void CProjData::setProjPrefixPath(const CString &strPath)
{
	CAutoLock al(&m_Lock);

	m_strProjPrefixPath = strPath;
}

void CProjData::getProjPrefixPath(CString &strPath)
{
	CAutoLock al(&m_Lock);

	strPath = m_strProjPrefixPath;
}

void CProjData::setMainDataName(const CString &strName)
{
	CAutoLock al(&m_Lock);

	m_strMainDataName = strName;
}

void CProjData::getMainDataName(CString &strName)
{
	CAutoLock al(&m_Lock);

	strName = m_strMainDataName;
}

void CProjData::setMuteDataName(const CString &strName)
{
	CAutoLock al(&m_Lock);

	m_strMuteDataName = strName;
}

void CProjData::getMuteDataName(CString &strName)
{
	CAutoLock al(&m_Lock);

	strName = m_strMuteDataName;
}

void CProjData::setDesignationName(const CString &strName)
{
	CAutoLock al(&m_Lock);

	m_strDesignationName = strName;
}

void CProjData::getDesignationName(CString &strName)
{
	CAutoLock al(&m_Lock);

	strName = m_strDesignationName;
}

void CProjData::addData(const CString &strData)
{

}

void CProjData::muteData(const CString &strData)
{

}

void CProjData::designationData(const CString &strData)
{

}

inline int CProjData::loadAllFileData()
{
	int nRet = 0;
	if (_T("") != m_strProjPrefixPath && eType_Group_NO == m_eDataType) {

		//The programer will only select three files.: Main.dat, Mute.dat,Designation.dat;
		DWORD dwAttr = GetFileAttributes(m_strProjPrefixPath);
		if (INVALID_FILE_ATTRIBUTES == dwAttr){
			return nRet;
		}

		CFileFind findHandle;
		CString strDestDir = m_strProjPrefixPath + _T("*.*");
		bool isNotEmpty = findHandle.FindFile(strDestDir);
		while (isNotEmpty) {
			isNotEmpty = findHandle.FindNextFileW();
			CString fileName = findHandle.GetFileName();
			if (!findHandle.IsDirectory() && !findHandle.IsDots()) {
				nRet++;
				if (0 <= fileName.Find(m_strMainDataName)) {
					m_strMainDataName = fileName;
					m_fileIOMain.openReadFile(CommonFun::cs2s(m_strProjPrefixPath + fileName ));
				}
				else if (0 <= fileName.Find(L"mute.dat")) {
					m_strMuteDataName = fileName;
					m_fileIOMute.openReadFile(CommonFun::cs2s(m_strProjPrefixPath + fileName ));
				}
				else if (0 <= fileName.Find(L"designation.dat")) {
					m_strDesignationName = fileName;
					m_fileIODesignation.openReadFile(CommonFun::cs2s(m_strProjPrefixPath + fileName));
				}
			}
		}
	}
	else if (_T("") != m_strProjPrefixPath && eType_Group_Yes == m_eDataType) {

		//The programer will select all files with current dir.; Mute.dat,Designation.dat and others main.dat.
	}
	
	return nRet;
}

inline void CProjData::unloadAllFileData()
{
	m_fileIODesignation.close();
	m_fileIOMain.close();
	m_fileIOMute.close();
}

//////////////////////////////////////////////////////////////////////////
CSingleton<CProjDataInstance>::CGrabo CProjDataInstance::graboInstance;

CProjDataInstance::CProjDataInstance()
{
	std::string strPath = CommonFun::getAbsoluteDir();
	strPath = CommonFun::getPirorDir(strPath);
	strPath = CommonFun::getPirorDirEx(strPath);
	strPath.append("data\\");

	m_strDefPrefixPath = CommonFun::s2cs(strPath);

	LoadProjData();
}

CProjDataInstance::~CProjDataInstance()
{

}


int CProjDataInstance::LoadProjData()
{
	//get All Projects. getAllExplorer
	int nRet = 0;
	DWORD dwAttr = GetFileAttributes(m_strDefPrefixPath);
	if (INVALID_FILE_ATTRIBUTES == dwAttr){
		return 0;
	}

	CFileFind findHandle;
	CString strDestDir = m_strDefPrefixPath + _T("*.*");
	bool isNotEmpty = findHandle.FindFile(strDestDir);
	while (isNotEmpty) {

		isNotEmpty = findHandle.FindNextFileW();
		CString fileName = findHandle.GetFileName();
		if (findHandle.IsDirectory() && !findHandle.IsDots()) {
			nRet++;
			CProjData* pProData = new CProjData(m_strDefPrefixPath + fileName + _T("\\"), fileName);
			m_vecProjData.push_back(pProData);
		}
	}

	return nRet;
}

int CProjDataInstance::LoadProjDataExtra(const CString &strPrefixPath, const CString &strProjName)
{
	return 0;
}

void CProjDataInstance::unInitLoadProdData()
{
	for (std::vector<CProjData*>::iterator it = m_vecProjData.begin(); m_vecProjData.end() != it; it++) {
		delete (*it);
		(*it) = nullptr;
	}
}