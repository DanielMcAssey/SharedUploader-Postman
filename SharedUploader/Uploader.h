#ifndef __UPLOADER_H_
#define __UPLOADER_H_


class Uploader
{
public:
	Uploader(String apiKey);

	unsigned int UploadFile(String FileLocation);

	static bool UploadFileExists(const String& name) {
		struct stat buffer;
		return (stat(name.c_str(), &buffer) == 0);
	}
protected:
	String _API_KEY = "";
	const String _API_VERSION = "v1";
#if _DEBUG
	const String _API_UPLOAD_URL = "http://shareduploader.mcassey.net/api/" + _API_VERSION + "/upload";
#else
	const String _API_UPLOAD_URL = "http://share.glokon.me/api/" + _API_VERSION + "/upload";
#endif
	
	void CopyToClipboard(String &clipboardData);

	static size_t curl_write(void *ptr, size_t size, size_t nmemb, void *stream);
};

#endif