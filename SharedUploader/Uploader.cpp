#include "stdafx.h"
#include "Uploader.h"


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
	// TODO: Do upload to API
	// REQUIRED: API isnt ready yet.

	return 0;
}