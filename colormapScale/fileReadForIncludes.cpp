#include "fileReadForIncludes.h"
#include <fstream>
#include <iostream>
#include "FileUtils/fileNameUtils.h"

FileReaderForIncludes::FileReaderForIncludes()
{

}

FileReaderForIncludes::~FileReaderForIncludes()
{

}

std::string FileReaderForIncludes::readFile(const std::string &fileToRead)
{
	std::string line;
	std::string nestedFile;
	std::fstream fileStream(fileToRead, std::ios::in);
	std::string nestedFilePath = getPathName(getRealPathToFile(fileToRead));


	if (fileStream.good()) {
		while (getline(fileStream, line)) {
			if (!line.find("#include")) {
				nestedFile = line.substr(line.find_first_of('"') + 1, line.find_last_of('"') - line.find_first_of('"') - 1);
				std::fstream nestedFileStream(nestedFilePath + nestedFile, std::ios::in);
				while (getline(nestedFileStream, line)) {
					content += line + '\n';
				}
			}
			else {
				content += line + '\n';
			}
		}
		fileStream.close();
	}
	else {
		std::cout << "File \"" << fileToRead.c_str() << "\" failed to open" << std::endl;
	}
	return content;
}

std::string FileReaderForIncludes::getFilerReaderContent()
{
	return content;
}