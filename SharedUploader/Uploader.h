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
	const String _API_UPLOAD_URL = "http://share.glokon.me/api/" + _API_VERSION + "/upload"; // TODO: Self-hosted API endpoints need to be supported
	
	static void CopyToClipboard(String &clipboardData);

	static int curl_progress_func(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded);
	static size_t curl_write(void *ptr, size_t size, size_t nmemb, void *userp);
};

#endif