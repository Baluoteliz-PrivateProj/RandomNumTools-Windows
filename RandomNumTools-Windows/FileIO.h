#ifndef __FILEIO_H__
#define __FILEIO_H__

#include <vector>
#include <map>

namespace CPlusBaluoteli
{
	class CFileIO
	{
	public:
		CFileIO();
		~CFileIO();

		void openLog(const std::string &filepath, int fileFlage = OPEN_ALWAYS);
		void openMedia(const std::string &filepath, int fileFlage = CREATE_ALWAYS);
		void openReadFile(const std::string &filePath);
		void close();
		int write(std::string bufferStr);
		int read(char *bufferOut, int bufferLen);
		std::string readLine();
		std::string read();

		void resetFile(DWORD dwMoveMethod  = FILE_BEGIN);

	public:
		static bool generatorFile(const std::string &path);

	protected:
		int write(char *bufferIn, int bufferLen);

	private:
		HANDLE fileHandle_;
		int filelimitLine_;
		bool isLog_;
	};

	class CFileIni
	{
	public:
		CFileIni();
		CFileIni(const std::string &filePath);
		~CFileIni();

		bool openFile(const std::string &IniFile);
		bool write(const std::string &section, const std::string &key, const std::string &Value);
		std::string read(const std::string &section, const std::string &key);
		bool getSectionName(std::vector<std::string> &vecSection);
		bool getSection(const std::string &section, std::map<std::string, std::string> &mapKeyValue);
		std::string getFilePah();

	private:
		std::string iniFile_;
		bool isValid_;
	};

	class CIniBase
	{
	public:
		CIniBase(const std::string &filePath);
		virtual ~CIniBase();
		std::string getFilePah();

#define __DECLARE_INICONFIG_FUN(CMDID)\
		std::string get##CMDID();\
		int set##CMDID(const std::string &v);

#define __IMPLEMENT_INICONFIG_FUN(CLASSNAME,CMDID,FIELD,KEY)\
		std::string CLASSNAME::get##CMDID()	{\
		return pIniInstance_->read(FIELD,KEY);}\
		int CLASSNAME::set##CMDID(const std::string &v)	{\
		return pIniInstance_->write(FIELD,KEY,v);}

#define __DECLARE_INICONFIG_SIMILAR_FUN(CMDID)\
		std::string get##CMDID(const std::string &filed);\
		int set##CMDID(const std::string &filed,const std::string &v);

#define __IMPLEMENT_INICONFIG_SIMILAR_FUN(CLASSNAME,CMDID,KEY)\
		std::string CLASSNAME::get##CMDID(const std::string &filed){\
		return pIniInstance_->read(filed,KEY);}\
		int CLASSNAME::set##CMDID(const std::string &filed,const std::string &v){\
		return pIniInstance_->write(filed,KEY,v);}

	protected:
		CFileIni *pIniInstance_;
	};


#define INI_BaseInfo													"BaseInfo"
#define INI_BaseInfo_VendorName								"VendorName"
#define INI_BaseInfo_TitleName									"TitleName"
#define INI_BaseInfo_TagName									"TagName"
#define INI_BaseInfo_ImportFileDirPrefix						"ImportDirPrefix"

	class CConfig :public CIniBase
	{
	public:
		CConfig();
		CConfig(const std::string &path);
		~CConfig();

	public:
		__DECLARE_INICONFIG_FUN(VendorName)
		__DECLARE_INICONFIG_FUN(TitleName)
		__DECLARE_INICONFIG_FUN(TagName)
		__DECLARE_INICONFIG_FUN(ImportDirPrefix)

	private:

	};

}
#endif
