#include "stdafx.h"
#include "Utilc.h"
using namespace CPlusBaluoteli;
using namespace CPlusBaluoteli::util;
using namespace CPlusBaluoteli::control;
#include "ProjDataManager.h"
#include "RandomNumTools-Windows.h"

CRandom::CRandom():
m_nNumMin(0),
m_nNumMax(0),
m_nItemCount(0),
m_bIsRandomOver(false),
m_nNumRepeatLast(0)
{
}

CRandom::CRandom(int nNumMin, int nNumMax):
m_nNumMin(nNumMin),
m_nNumMax(nNumMax),
m_nItemCount(0),
m_bIsRandomOver(false),
m_nNumRepeatLast(0)
{
	m_nItemCount = m_nNumMax - m_nNumMin + 1;
	for (int nIndex = m_nNumMin; nIndex <= m_nNumMax; nIndex++)
		m_vecUsed.push_back(nIndex);

	if (m_nNumMax < m_nNumMin)
		m_bIsRandomOver = true;//empty_filedata
}

CRandom::~CRandom()
{
	m_vecUsed.clear();
	m_nNumMin = 0;
	m_nNumMax = 0;
	m_nItemCount = 0;
	m_nNumRepeatLast = 0;
}

bool CRandom::random(int &nRandomNum)
{
	bool bRes = false;
	if (!m_bIsRandomOver) {
		do	{
			int nNumMin = 0;
			int nNumMax = m_nItemCount - 1;
			srand(time(NULL));
			nRandomNum = nNumMin + (rand() % (nNumMax - nNumMin + 1));
			nRandomNum = m_vecUsed[nRandomNum];
			vecIntIt it = find(m_vecUsed.begin(), m_vecUsed.end(), nRandomNum);
			if (m_vecUsed.end() != it) {
				bRes = true;
				m_vecUsed.erase(it);
				m_nItemCount--;
				if (0 == m_nItemCount)
					m_bIsRandomOver = true;
			}
		} while (!bRes);
	}

	return bRes;
}

bool CRandom::randomRepeatable(int &nRandomNum)
{
	if (m_nNumMax >= m_nNumMin) {
#if 0
		do{
			srand(time(NULL));
			nRandomNum = m_nNumMin + (rand() % (m_nNumMax - m_nNumMin + 1));
		} while (m_nNumRepeatLast == nRandomNum);
		m_nNumRepeatLast = nRandomNum;
		return true;
#else

		nRandomNum = m_nNumRepeatLast + 1;
		if (nRandomNum > m_nNumMax)
			nRandomNum = m_nNumMin;
		m_nNumRepeatLast = nRandomNum;

		return true;
#endif
	}

	return false;
}

void CRandom::resetRandom(int nNumMin, int nNumMax)
{
	if (m_nNumMin != nNumMin || m_nNumMax != nNumMax) {

		m_bIsRandomOver = false;
		m_vecUsed.clear();
		m_nNumMin = nNumMin;
		m_nNumMax = nNumMax;
		m_nItemCount = m_nNumMax - m_nNumMin + 1;
		m_nNumRepeatLast = 0;

		for (int nIndex = m_nNumMin; nIndex <= m_nNumMax; nIndex++)
			m_vecUsed.push_back(nIndex);

		if (m_nNumMax < m_nNumMin)
			m_bIsRandomOver = true;//empty_filedata
	}
}

bool CRandom::muteNum(const int &nNum)
{
	vecIntIt itUserMute = find(m_vecUsed.begin(), m_vecUsed.end(), nNum);
	if (m_vecUsed.end() != itUserMute) {
		m_vecUsed.erase(itUserMute);
		m_nItemCount--;
		return true;
	}

	return false;
}

bool CRandom::addNum(const int &nNum)
{
	assert(m_nNumMax + 1 == nNum);
	vecIntIt itUserMian = find(m_vecUsed.begin(),m_vecUsed.end(),nNum);
	if (m_vecUsed.end() == itUserMian) {
		m_bIsRandomOver = false;
		m_nNumMax = nNum;
		m_nItemCount++;
		m_vecUsed.push_back(nNum);

		return true;
	}

	return false;
}

void CRandom::resetRandom()
{
	m_bIsRandomOver = false;
	m_vecUsed.clear();
	m_nItemCount = m_nNumMax - m_nNumMin + 1;
	m_nNumRepeatLast = 0;

	for (int nIndex = m_nNumMin; nIndex <= m_nNumMax; nIndex++)
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
m_strProjName(L""),
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

CFileData::CFileData(const std::string &filepath1, const std::string &filepath2, const std::string &filepath3, const CString &strProjName) :
m_pFileDesignation(NULL),
m_pFileMain(nullptr),
m_pFileMute(nullptr),
m_nFileMainLineCount(0),
m_nFileMuteLineCount(0),
m_nFileDesignationLineCount(0),
m_strMainFilePath(filepath1),
m_strMuteFilePath(filepath2),
m_strDesignationPath(filepath3),
m_strProjName(strProjName),
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

	char szbuf[MAX_PATH] = { '\0' };
	sprintf_s(szbuf, "..\\随机结果\\%s.txt", CommonFun::cs2s(strProjName).data());
	m_FileProjResult.openLog(szbuf,OPEN_ALWAYS);
	writeResult("==新一轮随机操作开始.==");
}

CFileData::~CFileData()
{
	m_FileProjResult.close();
	unloadFile();
}

bool CFileData::resetMain()
{
	CAutoLock al(&m_lock);
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
	CAutoLock al(&m_lock);
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
	CAutoLock al(&m_lock);
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
	CAutoLock al(&m_lock);
	if (m_errorType & error_Ok) {

		int nNum = 0;
		std::string strName;

		{//random special data
			if (m_randomSpecial.random(nNum)) {
				strNum = m_vecDesignationData[nNum - 1];
				vecStrIte itMain = find(m_vecMainData.begin(), m_vecMainData.end(), strNum);
				int nIndexByMain = itMain - m_vecMainData.begin() + 1;
				m_randomMain.muteNum(nIndexByMain);
				writeResult(strNum);
				return m_errorType;
			}
		}

		{//random from source file
			int nRandomCount = 0;
			do {
				if (m_randomMain.random(nNum))
						strNum = m_vecMainData[nNum - 1];
					else{
						m_errorType |= error_Random_Over;
						return m_errorType;
					}
			} while (m_vecMuteData.end() != find(m_vecMuteData.begin(), m_vecMuteData.end(), strNum) && (++nRandomCount) <= m_nFileMuteLineCount);
		}
		writeResult(strNum);
	}

	return m_errorType;
}

BOOL CFileData::randomStrRepeatable(std::string &strNum)
{
	if (m_errorType & error_Ok) {

		int nNum = 0;
		if (m_randomMain.randomRepeatable(nNum)) {

			strNum = m_vecMainData[nNum - 1];
		}
	}

	return m_errorType;
}

bool CFileData::add(const std::string &str, eFileType eType /*= eFileType::eType_Main*/)
{
	CAutoLock al(&m_lock);
	switch (eType)
	{
	case eFileType::eType_Main: {
		vecStrIte itMain = find(m_vecMainData.begin(), m_vecMainData.end(), str);
		if (m_vecMainData.end() == itMain) {

			if (m_errorType & error_Random_Over)
				m_errorType &= ~error_Random_Over;
			if (m_errorType & error_DataFile_Empty)
				m_errorType &= ~error_DataFile_Empty;
			m_errorType = error_Ok;
			m_nFileMainLineCount++;
			m_randomMain.addNum(m_nFileMainLineCount);
			m_vecMainData.push_back(str); 
			int nres = fseek(m_pFileMain, 0, SEEK_END);
			std::string strWritten = str + "\r\n";
			nres = fwrite((char*)strWritten.data(), sizeof(char), strWritten.length(), m_pFileMain);
			fflush(m_pFileMain);
			return nres;
		}
		else {
			FormatStr::CFormatStr::Baluoteliz_MessageBox(L"重复项,添加失败.");
			return false;
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
	CAutoLock al(&m_lock);
	switch (eType)
	{
	case eFileType::eType_Main:
		break;
	case eFileType::eType_Mute: {
		vecStrIte itMain = find(m_vecMainData.begin(), m_vecMainData.end(), str);
		if (m_vecMainData.end() == itMain) {

			FormatStr::CFormatStr::Baluoteliz_MessageBox(L"之前没有添加过该数据,请先添加");
			return false;
		}

		vecStrIte itSpecial = find(m_vecDesignationData.begin(), m_vecDesignationData.end(), str);
		if (m_vecDesignationData.end() != itSpecial) {

			//重新导入文件.
			return false;
		}

		vecStrIte itMute = find(m_vecMuteData.begin(), m_vecMuteData.end(), str);
		if (m_vecMuteData.end() == itMute) {

			m_nFileMuteLineCount++;
			m_vecMuteData.push_back(str);
			int nres = fseek(m_pFileMute, 0, SEEK_END);
			std::string strWritten = str + "\r\n";
			nres = fwrite((char*)strWritten.data(), sizeof(char), strWritten.length(), m_pFileMute);
			fflush(m_pFileMain);
			return nres;
		}
		else {
			FormatStr::CFormatStr::Baluoteliz_MessageBox(L"重复项,添加失败");
			return false;
		}
	}
								break;
	case eFileType::eType_Designation:
		break;
	default:break;
	}
}

bool CFileData::designation(const std::string &str, eFileType eType /*= eFileType::eType_Designation*/)
{
	CAutoLock al(&m_lock);
	switch (eType)
	{
	case eFileType::eType_Main:
		break;
	case eFileType::eType_Mute:
		break;
	case eFileType::eType_Designation: {
		vecStrIte itMain = find(m_vecMainData.begin(), m_vecMainData.end(), str);
		if (m_vecMainData.end() == itMain) {

			FormatStr::CFormatStr::Baluoteliz_MessageBox(L"之前没有添加过该数据,请先添加");
			return false;
		}

		vecStrIte itMute = find(m_vecMuteData.begin(), m_vecMuteData.end(), str);
		if (m_vecMuteData.end() != itMute) {

			//重新导入文件.
			return false;
		}

		vecStrIte itSpecial = find(m_vecDesignationData.begin(), m_vecDesignationData.end(), str);
		if (m_vecDesignationData.end() == itSpecial) {

			m_nFileDesignationLineCount++;
			m_randomSpecial.addNum(m_nFileDesignationLineCount);
			m_vecDesignationData.push_back(str);
			int nres = fseek(m_pFileDesignation, 0, SEEK_END); 
			std::string strWritten = str + "\r\n";
			nres = fwrite((char*)strWritten.data(), sizeof(char), strWritten.length(), m_pFileDesignation);
			fflush(m_pFileMain);
			return nres;
		}
		else {
			FormatStr::CFormatStr::Baluoteliz_MessageBox(L"重复项,添加失败");
			return false;
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

	m_strProjName = L"";
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
		if (eType_Main == eType) {
			m_vecMainData.push_back(strName);
			int nres = fwrite("\r\n", 1, 2, m_pFileMain);
			int i = 0;
		}
		else if (eType_Mute == eType) {
			m_vecMuteData.push_back(strName);
			fwrite("\r\n", 1, 2, m_pFileMute);
		}
		else if (eType_Designation == eType) {
			m_vecDesignationData.push_back(strName);
			fwrite("\r\n", 1, 2, m_pFileDesignation);
		}
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

int CFileData::writeResult(const std::string &str)
{
	char szBuffer[1024] = { '\0' };

	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s(szBuffer, "%04d-%02d-%02d %02d:%02d:%02d: %s", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,str.data());

	return m_FileProjResult.write(szBuffer);
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

int CFileData::getFileStatus()
{
	CAutoLock al(&m_lock);

	return m_errorType;
}

CString CFileData::getProjName()
{
	CAutoLock al(&m_lock);

	return m_strProjName;
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

CProjDataInstance::CProjDataInstance():
m_pFileData(nullptr)
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
	if (m_pFileData) {
		delete m_pFileData;
		m_pFileData = nullptr;
	}
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
			vecWStr vecSub;

			CFileFind findHandleSub;
			CString strDestDirSub = m_strDefPrefixPath + fileName + "\\*.*";
			bool isNotEmptySub = findHandleSub.FindFile(strDestDirSub);
			while (isNotEmptySub) {
				isNotEmptySub = findHandleSub.FindNextFileW();
				CString fileNameSub = findHandleSub.GetFileName();
				if (!findHandleSub.IsDirectory() && !findHandleSub.IsDots()){
					vecSub.push_back(fileNameSub);
				}
			}
			m_mapProject.insert(make_pair(fileName, vecSub));
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
	CAutoLock al(&m_Lock);
	if (m_pFileData) {
		delete m_pFileData;
		m_pFileData = nullptr;
	}
}

CString CProjDataInstance::getPrefixPath()
{
	CAutoLock al(&m_Lock);
	return m_strDefPrefixPath;
}

void CProjDataInstance::getProjData(std::map<CString, std::vector<CString>> &mapData)
{
	CAutoLock al(&m_Lock);
	
	mapData = m_mapProject;
}

bool CProjDataInstance::insertProjDataItem(LPRANDOM_NEW_PROJ lpData)
{
	CAutoLock al(&m_Lock);
	if (lpData) {
		
		mapWStrVecIt mapIt = m_mapProject.find(lpData->strProjName);
		if (m_mapProject.end() != mapIt){

			FormatStr::CFormatStr::Baluoteliz_MessageBox(_T("已经存在相同的项目了,请重新命名"));
			return false;
		}
		else{

			m_mapProject.insert(make_pair(lpData->strProjName, lpData->m_vecFileName));
			return true;
		}
	}
}

void CProjDataInstance::deleteItem(const CString strParam)
{
	CAutoLock al(&m_Lock);
	mapWStrVecIt mapit = m_mapProject.find(strParam);
	if (m_mapProject.end() != mapit) {
		
		if (m_pFileData->getProjName() == strParam) {
			delete m_pFileData; 
			m_pFileData = nullptr;

			LPRANDOM_DELETE_PROJ lpData = new RANDOM_DELETE_PROJ;
			if (lpData) {

				lpData->nRes = CFileData::error_Ok;
				lpData->strProjName = strParam;
				::PostMessage(theApp.GetMainWnd()->m_hWnd, RandomMsg_DELETE_PROJ, (WPARAM)lpData, NULL);
			}
		}

		CString strDir;
		CString strDirName;
		int nCount = mapit->second.size();
		strDir.Format(L"%s%s\\", m_strDefPrefixPath, strParam);
		for (int nIndex = 0; nIndex < nCount; nIndex++) {

			strDirName.Format(L"%s%s", strDir,(mapit->second[nIndex]));
			BOOL bRes = DeleteFile(strDirName);
		}

		BOOL bRes =RemoveDirectory(strDir);

		m_mapProject.erase(mapit);
	}
}

void CProjDataInstance::getVectorFiles(const CString strParma,std::vector<CString> &vecTemp)
{
	CAutoLock al(&m_Lock);
	mapWStrVecIt itMap = m_mapProject.find(strParma);
	if (m_mapProject.end() != itMap) {

		vecTemp = itMap->second;
	}
}

int CProjDataInstance::ImportProj(const CString &strProjName)
{
	CAutoLock al(&m_Lock);

	mapWStrVecIt itMpa = m_mapProject.find(strProjName);
	if (m_mapProject.end() != itMpa) {

		if (m_pFileData) {
			if (m_pFileData->getProjName() == strProjName)
				return false;//the same project
			delete m_pFileData;
			m_pFileData = nullptr;
		}

		std::string strProjName1 = CommonFun::cs2s(strProjName);
		if (!m_pFileData) {
			char szbufMain[128] = { '\0' };
			sprintf_s(szbufMain, "..\\data\\%s\\%s.dat", strProjName1.data(), strProjName1.data());
			char szbufMute[128] = { '\0' };
			sprintf_s(szbufMute, "..\\data\\%s\\mute.dat", strProjName1.data());
			char szbufDesignation[128] = { '\0' };
			sprintf_s(szbufDesignation, "..\\data\\%s\\designation.dat", strProjName1.data());
			m_pFileData = new CFileData(szbufMain, szbufMute, szbufDesignation,strProjName);
			return m_pFileData->getFileStatus();
		}
	}
	else
		return CFileData::error_Random_NoValidProj;
}

int CProjDataInstance::resetProj(const CString &strProjName)
{
	CAutoLock al(&m_Lock);

	mapWStrVecIt itMpa = m_mapProject.find(strProjName);
	if (m_mapProject.end() != itMpa) {
		if (m_pFileData) {
			if (m_pFileData->getProjName() == strProjName) {
				delete m_pFileData;
				m_pFileData = nullptr;
			}
		}

		std::string strProjName1 = CommonFun::cs2s(strProjName);
		if (!m_pFileData) {
			char szbufMain[128] = { '\0' };
			sprintf_s(szbufMain, "..\\data\\%s\\%s.dat", strProjName1.data(), strProjName1.data());
			char szbufMute[128] = { '\0' };
			sprintf_s(szbufMute, "..\\data\\%s\\mute.dat", strProjName1.data());
			char szbufDesignation[128] = { '\0' };
			sprintf_s(szbufDesignation, "..\\data\\%s\\designation.dat", strProjName1.data());
			m_pFileData = new CFileData(szbufMain, szbufMute, szbufDesignation, strProjName);
			return m_pFileData->getFileStatus();
		}
	}
	else
		return CFileData::error_Random_NoValidProj;
}

void CProjDataInstance::addStr(const std::string&str)
{
	if (m_pFileData)
		if (m_pFileData->add(str, CFileData::eType_Main))
			FormatStr::CFormatStr::Baluoteliz_MessageBox(L"添加成功");
}

void CProjDataInstance::muteStr(const std::string &str)
{
	if (m_pFileData)
		m_pFileData->mute(str, CFileData::eType_Mute);
}

void CProjDataInstance::designationStr(const std::string &str)
{
	if (m_pFileData)
		m_pFileData->designation(str, CFileData::eType_Designation);
}

bool CProjDataInstance::RandomStr(CString &str)
{
	if (m_pFileData) {

		std::string strNum;
		BOOL bRes = m_pFileData->randomStr(strNum);
		str = CommonFun::s2cs(strNum);

		return !((bRes & CFileData::error_Random_Over) || (bRes & CFileData::error_DataFile_Empty));
	}
}

bool CProjDataInstance::RandomStrRepeatable(CString &str)
{
	if (m_pFileData) {

		std::string strNum;
		BOOL bRes = m_pFileData->randomStrRepeatable(strNum);
		str = CommonFun::s2cs(strNum);

		return !(bRes & CFileData::error_DataFile_Empty);
	}
}