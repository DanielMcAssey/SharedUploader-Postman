// @name: SharedUploader Postman
// @version: 1.0.0

#include "stdafx.h"

#include "UploadFileTypes.h"
#include "Uploader.h"

using namespace std;

#if _DEBUG
const int expectedArgCount = 1;
#else
const int expectedArgCount = 4;
#endif

int main(int argc, char* argv[])
{
	// Header
	cout << "===========================================================" << endl;
	cout << " GLOKON SharedUploader Postman (Server: shared.glokon.com)" << endl;
	cout << "===========================================================" << endl;

	if (argc == expectedArgCount) // Make sure we have 3 args: api key, file type and file location
	{
#if _DEBUG
		String apiKey = "";
		String fileType = "";
		String fileLocation = "";
#else
		String apiKey = argv[1];
		String fileType = argv[2];
		String fileLocation = argv[3];
#endif
		

		Uploader* UploadTool = new Uploader(apiKey);
		enum Uploader_FileTypes enumFileType = UploadTool->GetFileTypeByString(fileType); // Get enum from string
		if (enumFileType == Uploader_FileTypes::UPLOADER_ERROR)
			return -1;
		
		if (!Uploader::UploadFileExists(fileLocation))
			return -1;

		cout << "Uploading File: " + fileLocation << endl;

		// TODO: Make upload async
		unsigned int uploadResult = UploadTool->UploadFile(enumFileType, fileLocation);
		if (uploadResult > 0) // Upload was OK
		{
			cout << "UPLOAD COMPLETE" << endl;
#if _DEBUG
			// Stop console from exiting when debugging
			std::getchar();
#endif
			return 0;
		}
	}

	cout << "UPLOAD FAILED :(" << endl;

#if _DEBUG
	// Stop console from exiting when debugging
	std::getchar();
#endif
	
	return -1;
}