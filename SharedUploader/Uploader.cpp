#include "stdafx.h"
#include "Uploader.h"

Uploader::Uploader(String apiKey)
{
	_API_KEY = apiKey;
}


unsigned int Uploader::UploadFile(String FileLocation)
{
	if (UploadFileExists(FileLocation))
	{
		int expectedResult = 0;
		struct stat uploadFileInfo;
		curl_off_t uploadFileSize;
		stat(FileLocation.c_str(), &uploadFileInfo);
		uploadFileSize = uploadFileInfo.st_size;

		String readBuffer;

		CURL* curlHandle = curl_easy_init();
		CURLcode curlResult;
		if (!curlHandle)
			return 0;

		struct curl_httppost *form_post_params = NULL;
		struct curl_httppost *form_post_last_ptr = NULL;
		struct curl_slist *request_headers = NULL;

		request_headers = curl_slist_append(request_headers, ("X-Authorization: " + _API_KEY).c_str());
		request_headers = curl_slist_append(request_headers, "Expect:");

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
		curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &readBuffer);
		curl_easy_setopt(curlHandle, CURLOPT_FRESH_CONNECT, TRUE);
		curl_easy_setopt(curlHandle, CURLOPT_NOPROGRESS, FALSE);
		curl_easy_setopt(curlHandle, CURLOPT_PROGRESSFUNCTION, Uploader::curl_progress_func);
		curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDSIZE, uploadFileSize);
#if _DEBUG
		curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, 1L);
#endif
		curlResult = curl_easy_perform(curlHandle);
		curl_easy_cleanup(curlHandle);
		curl_formfree(form_post_params);
		curl_slist_free_all(request_headers);

		printf("\n");
		if (curlResult != CURLE_OK)
			fprintf(stderr, "CURL failed: %s\n", curl_easy_strerror(curlResult));
		else
		{
			long result_http_code = 0;
			curl_easy_getinfo(curlHandle, CURLINFO_RESPONSE_CODE, &result_http_code);
			if (result_http_code != 503) //503 Means maintenance
			{
				rapidjson::Document jsonDocument;
				jsonDocument.Parse(readBuffer.c_str());

				if (jsonDocument.HasMember("ok"))
				{
					if (jsonDocument["ok"].HasMember("data") && jsonDocument["ok"]["data"].HasMember("clipboard_url"))
					{
						rapidjson::Value& cliboardValue = jsonDocument["ok"]["data"]["clipboard_url"];
						String clipboardURL = cliboardValue.GetString();
						CopyToClipboard(clipboardURL);
						printf("Share URL: %s \n", clipboardURL.c_str());
						expectedResult = 1; // OK :)
					}
				}
				else if (jsonDocument.HasMember("error"))
				{
					if (jsonDocument["error"].HasMember("code"))
					{
						rapidjson::Value& errorValue = jsonDocument["error"]["code"];
						String errorCode = errorValue.GetString();
						printf("Server Response [ERROR]: %s \n", errorCode.c_str());
					}
					else
					{
						printf("Server Response [ERROR]: %s \n", "UNKNOWN ERROR");
					}

					if (jsonDocument["error"].HasMember("data") && jsonDocument["error"]["data"].HasMember("mime-type"))
					{
						rapidjson::Value& errorValue = jsonDocument["error"]["data"]["mime-type"];
						String errorData = errorValue.GetString();
						printf("Server Response [ERROR-DATA]: %s \n", errorData.c_str());
					}
				}
			}
			else
			{
				String maintenanceMessage = "SharedUploader [SERVER] is in maintenance";
				CopyToClipboard(maintenanceMessage);
				fprintf(stderr, "Server Response [ERROR]: %s \n", maintenanceMessage.c_str());
				printf("Server Response [ERROR]: %s \n", maintenanceMessage.c_str());
			}
		}
		return expectedResult;
	}

	printf("FILE NOT FOUND - %s \n", (String)FileLocation);
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


int Uploader::curl_progress_func(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded)
{
	if (TotalToUpload <= 0.0)
	{
		return 0;
	}

	const int progressBarWidth = 40;
	double uploadedFraction = NowUploaded / TotalToUpload;
	int progressBarDots = round(uploadedFraction * progressBarWidth);

	int progressBarDot = 0;
	printf("UPLOADING: %3.0f%% [", uploadedFraction * 100);
	for (; progressBarDot < progressBarDots; progressBarDot++) {
		if (progressBarDot == (progressBarDots - 1) && (uploadedFraction * 100) < 100.0)
		{
			printf(">");
		}
		else
		{
			printf("=");
		}
	}
	for (; progressBarDot < progressBarWidth; progressBarDot++) {
		printf(" ");
	}
	printf("]\r");
	fflush(stdout);

	return 0;
}


size_t Uploader::curl_write(void *ptr, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)ptr, size * nmemb);
	return size * nmemb;
}