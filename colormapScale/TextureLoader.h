#pragma once

#include <gli/gli.hpp>
#include <string>

class gli::storage;
class gli::texture2D;

class TextureLoader
{
public :
	static TextureLoader *Instance();

	unsigned int setupPointDrawShader(std::string vsSource, std::string fsSource);
	std::string readShaderSource(std::string location);
	bool checkShaderCompileError(GLuint shaderID);
	bool loadDDsTex(std::string name);

	gli::texture2D * getLoadedTexture();


private:
	TextureLoader();
	virtual ~TextureLoader();
	static TextureLoader *instance;

	gli::storage loadTexFromHD(const char* texPath);

	gli::texture2D *loadedTexture;
};
