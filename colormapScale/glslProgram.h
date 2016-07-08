#pragma once

#include <string>

namespace GLSLShader {
	enum GLSLShaderType {
		VERTEX, FRAGMENT, GEOMETRY,
		TESS_CONTROL, TESS_EVALUATION
	};
};

class GLSLProgram
{
private:
	unsigned int programHandle;
	unsigned int vertexShaderHandle;
	unsigned int fragmentShaderHandle;
	unsigned int geometryShaderHandle;
	unsigned int tessControlShaderHandle;
	unsigned int tessEvalShaderHandle;
	bool linked;
	std::string logString;

	int getUniformLocation(const char * name);

	bool getBufferFromFile(const std::string &fileName, unsigned char **buffer, int &bufferSize);

protected:
	bool compileUncompressedShaderBuffer(const char *source, GLSLShader::GLSLShaderType type);
	std::string loadShaderFromBuffer(unsigned char *programBuffer, int size);
	std::string loadShaderFromFile(const std::string &filename);

	virtual std::string tesselationShader() = 0;
	virtual std::string fragmentShader() = 0;
	virtual std::string geometryShader() = 0;
	virtual std::string tesselationControl() = 0;
	virtual std::string vertexShader() = 0;

	bool getShaderCodeBufferFromFile(const std::string &stubFileName, unsigned char **buffer, int &bufferSize);
	bool uncompressShaderBuffer(unsigned char *compressedBuffer, int size, unsigned char **uncompressedBuffer, int &uncompressedSize);
	bool buildShader(unsigned int shaderHandle, const char *source);
public:
	GLSLProgram();
	virtual ~GLSLProgram();
	virtual bool compileAndLink();


	bool link();
	bool validate();
	void use();
	void unUse();

	std::string log();

	int getHandle();
	bool isLinked();

	void bindAttribLocation(GLuint location, const char * name);
	void bindFragDataLocation(GLuint location, const char * name);

};