#ifndef fileReadForIncludes_h
#define fileReadForIncludes_h

#include <string>

class FileReaderForIncludes
{
public:
	FileReaderForIncludes();
	~FileReaderForIncludes();
	std::string getFilerReaderContent();
	std::string readFile(const std::string &fullFilePath);
private:
	std::string content;
};

#endif