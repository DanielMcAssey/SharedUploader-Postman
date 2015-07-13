// @name: SharedUploader Postman
// @version: 1.2.0

#include "stdafx.h"
#include "Uploader.h"

using namespace std;

int main(int argc, char* argv[])
{
	// Header
	cout << "=========================================================" << endl;
	cout << " GLOKON SharedUploader Postman (Server: share.glokon.me)" << endl;
	cout << "=========================================================" << endl;

	const int expectedArgCount = 3;
	int appResult = 1;
	if (argc == expectedArgCount) // Make sure we have 2 args: api key and file location
	{
		String apiKey = argv[1];
		String fileLocation = argv[2];

		Uploader* UploadTool = new Uploader(apiKey);
		cout << "UPLOADING FILE: " + fileLocation << endl;

		unsigned int uploadResult = UploadTool->UploadFile(fileLocation);
		if (uploadResult > 0) // Upload was OK
		{
			cout << "SUCCESS: UPLOAD COMPLETE" << endl;
			appResult = 0;
		}
		else
		{
			cout << "ERROR: UPLOAD FAILED" << endl;
		}
	}
	else
	{
		cout << "ERROR: TOO FEW ARGUMENTS" << endl;
	}

#if _DEBUG
	// Stop console from exiting when debugging
	std::getchar();
#endif
	
	return appResult;
}