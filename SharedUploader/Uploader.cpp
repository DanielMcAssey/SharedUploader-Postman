#include "stdafx.h"
#include "Uploader.h"
#include "UploadFileTypes.h"


Uploader::Uploader(String apiKey)
{
	_API_KEY = apiKey;
	PopulateFileTypeMap();
}


void Uploader::PopulateFileTypeMap(void)
{
	__UPLOADER_FILETYPE_MAP.insert(std::pair<String, Uploader_FileTypes>("generic", Uploader_FileTypes::UPLOADER_GENERIC_FILE));
	__UPLOADER_FILETYPE_MAP.insert(std::pair<String, Uploader_FileTypes>("image", Uploader_FileTypes::UPLOADER_IMAGE));
	__UPLOADER_FILETYPE_MAP.insert(std::pair<String, Uploader_FileTypes>("pdf", Uploader_FileTypes::UPLOADER_PDF));
}

enum Uploader_FileTypes Uploader::GetFileTypeByString(String strFileType)
{
	String lowerFileType = strFileType; // To make Lowercase
	for (unsigned int i = 0; i < lowerFileType.length(); ++i)
		lowerFileType[i] = tolower(lowerFileType[i]); // Lowercase each of the char in string

	std::unordered_map<String, enum Uploader_FileTypes>::const_iterator findIterator = __UPLOADER_FILETYPE_MAP.find(lowerFileType); // Iterator to check item was found
	if (findIterator != __UPLOADER_FILETYPE_MAP.end())
		return findIterator->second; // Return second element in map (ENUM)

	return Uploader_FileTypes::UPLOADER_ERROR; // Return generic error, context can be gained from function
}


unsigned int Uploader::UploadFile(enum Uploader_FileTypes FileType, String FileLocation)
{
	// REQUIRED: API isnt ready yet.
	if (UploadFileExists(FileLocation)) // Redundant check, probably remove
	{
		struct stat uploadFileInfo;
		curl_off_t uploadFileSize;
		stat(FileLocation.c_str(), &uploadFileInfo);
		uploadFileSize = uploadFileInfo.st_size;

		CURL* curlHandle = curl_easy_init();
		CURLcode curlResult;
		if (!curlHandle)
			return 0;

		struct curl_httppost *form_post_params = NULL;
		struct curl_httppost *form_post_last_ptr = NULL;
		struct curl_slist *request_headers = NULL;

		request_headers = curl_slist_append(request_headers, ("X-API-KEY: " + _API_KEY).c_str());

		curl_formadd(&form_post_params,
			&form_post_last_ptr,
			CURLFORM_COPYNAME, "uploadFile",
			CURLFORM_FILE, FileLocation,
			CURLFORM_END);

		curl_formadd(&form_post_params,
			&form_post_last_ptr,
			CURLFORM_COPYNAME, "submit",
			CURLFORM_COPYCONTENTS, "send",
			CURLFORM_END);

		curl_easy_setopt(curlHandle, CURLOPT_URL, _API_UPLOAD_URL);
		curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, request_headers);
		curl_easy_setopt(curlHandle, CURLOPT_HTTPPOST, form_post_params);

		curlResult = curl_easy_perform(curlHandle);
		if (curlResult != CURLE_OK)
			return 0;

		curl_easy_cleanup(curlHandle);
		curl_formfree(form_post_params);
		curl_slist_free_all(request_headers);
	}

	return 0;
}