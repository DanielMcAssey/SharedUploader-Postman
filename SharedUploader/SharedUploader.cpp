// @name: Shared Uploader for GLOKON
// @version: 0.1.0

#include "stdafx.h"

using namespace std;

int main(int argc, char* argv[])
{
	// Header
	cout << "====================================================" << endl;
	cout << " GLOKON Shared Uploader (Server: shared.glokon.com)" << endl;
	cout << "====================================================" << endl;

	if (argc == 3) // Make sure we have 3 args: api key, file type and file location
	{
		String apiKey = argv[0];
		Uploader* UploadTool = new Uploader(apiKey);

		enum Uploader_FileTypes fileType = UploadTool->GetFileTypeByString(argv[1]); // Get enum from string
		if (fileType == Uploader_FileTypes::UPLOADER_ERROR)
			return -1;

		String fileLocation = argv[2];
		if (!Uploader::UploadFileExists(fileLocation))
			return -1;

		// TODO: Make upload async
		unsigned int uploadResult = UploadTool->UploadFile(fileType, fileLocation);
		if (uploadResult > 0) // Upload was OK
		{
			cout << "UPLOAD COMPLETE" << endl;
			return 0;
		}
		else // Upload was not OK :(
		{
			cout << "UPLOAD FAILED :(" << endl;
			return -1;
		}
	}

#if _DEBUG
	// Stop console from exiting when debugging
	std::getchar();
#endif
	
	return 0;
}