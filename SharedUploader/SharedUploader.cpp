// @name: SharedUploader Postman
// @version: 1.1.0

#include "stdafx.h"

#include "Uploader.h"

using namespace std;

const int expectedArgCount = 3;

int main(int argc, char* argv[])
{
	// Header
	cout << "=========================================================" << endl;
	cout << " GLOKON SharedUploader Postman (Server: share.glokon.me)" << endl;
	cout << "=========================================================" << endl;

	if (argc == expectedArgCount || _DEBUG) // Make sure we have 2 args: api key and file location
	{
		String apiKey = argv[1];
		String fileLocation = argv[2];

		Uploader* UploadTool = new Uploader(apiKey);
		cout << "Uploading File: " + fileLocation << endl;

		unsigned int uploadResult = UploadTool->UploadFile(fileLocation);
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
	else
	{
		cout << "TOO FEW ARGUMENTS" << endl;
	}

	cout << "UPLOAD FAILED" << endl;

#if _DEBUG
	// Stop console from exiting when debugging
	std::getchar();
#endif
	
	return 1;
}