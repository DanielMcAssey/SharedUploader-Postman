#include "stdafx.h"
#include "Uploader.h"

String curlResponseBuffer;


Uploader::Uploader(String apiKey)
{
	_API_KEY = apiKey;
}


unsigned int Uploader::UploadFile(String FileLocation)
{
	if (UploadFileExists(FileLocation)) // Redundant check, probably remove
	{
		int expectedResult = 0;
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

		request_headers = curl_slist_append(request_headers, ("X-Authorization: " + _API_KEY).c_str());

		curl_formadd(&form_post_params,
			&form_post_last_ptr,
			CURLFORM_COPYNAME, "uploadFile",
			CURLFORM_FILE, FileLocation.c_str(),
			CURLFORM_END);

		curl_formadd(&form_post_params,
			&form_post_last_ptr,
			CURLFORM_COPYNAME, "submit",
			CURLFORM_COPYCONTENTS, "send",
			CURLFORM_END);

		curl_easy_setopt(curlHandle, CURLOPT_URL, _API_UPLOAD_URL.c_str());
		curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, request_headers);
		curl_easy_setopt(curlHandle, CURLOPT_HTTPPOST, form_post_params);
		curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, Uploader::curl_write);
#if _DEBUG
		curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, 1L);
#endif

		curlResult = curl_easy_perform(curlHandle);
		if (curlResult != CURLE_OK)
			fprintf(stderr, "CURL failed: %s\n", curl_easy_strerror(curlResult));
		else
		{
			long result_http_code = 0;
			curl_easy_getinfo(curlHandle, CURLINFO_RESPONSE_CODE, &result_http_code);
			if (result_http_code != 503)
			{
				rapidjson::Document jsonDocument;
				jsonDocument.Parse(curlResponseBuffer.c_str());

				if (!jsonDocument["ok"].IsNull())
				{
					if (!jsonDocument["ok"]["data"]["clipboard_url"].IsNull())
					{
						rapidjson::Value& cliboardValue = jsonDocument["ok"]["data"]["clipboard_url"];
						String clipboardURL = cliboardValue.GetString();
						CopyToClipboard(clipboardURL);
						printf("Share URL: %s \n", clipboardURL.c_str());
						expectedResult = 1; // OK :)
					}
				}
				else
				{
					if (!jsonDocument["error"]["code"].IsNull())
					{
						rapidjson::Value& errorValue = jsonDocument["error"]["code"];
						String errorCode = errorValue.GetString();
						printf("Server Response [ERROR]: %s \n", errorCode.c_str());
					}
					else
					{
						printf("Server Response [ERROR]: %s \n", "UNKNOWN ERROR");
					}
				}
			}
			else
			{
				String maintenanceMessage = "SharedUploader [SERVER] is in maintenance";
				CopyToClipboard(maintenanceMessage);
				fprintf(stderr, "Server (RESPONSE): %s \n", maintenanceMessage.c_str());
			}
		}

		curl_easy_cleanup(curlHandle);
		curl_formfree(form_post_params);
		curl_slist_free_all(request_headers);
		return expectedResult;
	}

	return 0;
}


void Uploader::CopyToClipboard(String &clipboardData)
{
	OpenClipboard(0);
	EmptyClipboard();
	const size_t stringLength = clipboardData.size() + 1;
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, stringLength);
	if (!hg)
	{
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), clipboardData.c_str(), stringLength);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}


size_t Uploader::curl_write(void *ptr, size_t size, size_t nmemb, void *stream)
{
	curlResponseBuffer.append((char*)ptr, size * nmemb);
	return size*nmemb;
}

