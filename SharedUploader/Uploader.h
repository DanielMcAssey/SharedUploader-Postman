#ifndef __UPLOADER_H_
#define __UPLOADER_H_


class Uploader
{
public:
	Uploader(String apiKey);

	unsigned int UploadFile(enum Uploader_FileTypes FileType, String FileLocation);
	enum Uploader_FileTypes GetFileTypeByString(String strFileType);

	static bool UploadFileExists(const String& name) {
		struct stat buffer;
		return (stat(name.c_str(), &buffer) == 0);
	}
protected:
	String _API_KEY = "";
	const String _API_VERSION = "1.0";
	const String _API_UPLOAD_URL = "http://shared.glokon.com/api/" + _API_VERSION + "/upload";
	std::unordered_map<String, enum Uploader_FileTypes> __UPLOADER_FILETYPE_MAP;

	void PopulateFileTypeMap(void);
};

#endif