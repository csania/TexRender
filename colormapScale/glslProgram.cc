#include "GL/glew.h"
#include "FileUtils/fileFolderFunctions.h"

#include <QtCore/QByteArray>
#include "glslprogram.h"
#include "errorHandler.h"
#include "fileReadForIncludes.h"

GLSLProgram::GLSLProgram() : programHandle(0), linked(false)
{
	vertexShaderHandle = 0;
	fragmentShaderHandle = 0;
	geometryShaderHandle = 0;
	tessControlShaderHandle = 0;
	tessEvalShaderHandle = 0;
}
GLSLProgram::~GLSLProgram()
{
	glDeleteShader(vertexShaderHandle);
	glDeleteShader(fragmentShaderHandle);
	glDeleteShader(geometryShaderHandle);
	glDeleteShader(tessControlShaderHandle);
	glDeleteShader(tessEvalShaderHandle);
	glDeleteProgram(programHandle);
}

bool GLSLProgram::compileAndLink()
{
	if (!compileUncompressedShaderBuffer(vertexShader().c_str(), GLSLShader::VERTEX)) { ErrorHandler::setError(ErrorHandler::ErrorCode::CompileVertexShaderFailed); return false; }
	if (!compileUncompressedShaderBuffer(tesselationControl().c_str(), GLSLShader::TESS_CONTROL)) { ErrorHandler::setError(ErrorHandler::ErrorCode::CompileTesselationShaderFailed); return false; }
	if (!compileUncompressedShaderBuffer(tesselationShader().c_str(), GLSLShader::TESS_EVALUATION)) { ErrorHandler::setError(ErrorHandler::ErrorCode::CompileTeselationEvaluationShaderFailed); return false; }
	if (!compileUncompressedShaderBuffer(geometryShader().c_str(), GLSLShader::GEOMETRY)) { ErrorHandler::setError(ErrorHandler::ErrorCode::CompileGeometryShaderFailed); return false; }
	if (!compileUncompressedShaderBuffer(fragmentShader().c_str(), GLSLShader::FRAGMENT)) { ErrorHandler::setError(ErrorHandler::ErrorCode::CompileFragmentShaderFailed); return false; }

	if (!link()) { ErrorHandler::setError(ErrorHandler::ErrorCode::CompileLinkFailed); return false; }

	return true;
}

bool GLSLProgram::getShaderCodeBufferFromFile(const std::string &stubFileName, unsigned char **buffer, int &bufferSize)
{
	bool succeeded = true;
	std::string filename = "shader/" + stubFileName;
	std::string filenameAbove = "../" + filename;
	if (!getBufferFromFile(filenameAbove, buffer, bufferSize)) {
		if (!getBufferFromFile(filename, buffer, bufferSize)) {
			succeeded = false;
		}
	}
	return succeeded;
}

bool GLSLProgram::getBufferFromFile(const std::string &fileName, unsigned char **buffer, int &bufferSize)
{
	FileReaderForIncludes fileReader;
	if (!fileExists(fileName)) {
		logString = "File not found.";
		return false;
	}

	bool result = false;
	fileReader.readFile(fileName);

	std::string fileContent = fileReader.getFilerReaderContent();
	bufferSize = fileContent.size();

	*buffer = new unsigned char[bufferSize + 1];
	memcpy(*buffer, fileContent.c_str(), bufferSize + 1);

	if (bufferSize > 0) {
		result = true;
	}

	return result;
}

bool GLSLProgram::uncompressShaderBuffer(unsigned char *compressedBuffer, int size, unsigned char **uncompressedBuffer, int &uncompressedSize)
{
	bool success = false;
	QByteArray uncompressedByteArray = qUncompress(compressedBuffer, size);
	uncompressedSize = uncompressedByteArray.size();

	if (uncompressedSize > 0) {
		*uncompressedBuffer = new unsigned char[uncompressedSize + 1];
		memcpy(*uncompressedBuffer, uncompressedByteArray.data(), uncompressedSize);
		(*uncompressedBuffer)[uncompressedSize] = '\0';
		success = true;
	}

	return success;
}

bool GLSLProgram::compileUncompressedShaderBuffer(const char *source, GLSLShader::GLSLShaderType type)
{
	if (programHandle <= 0) {
		programHandle = glCreateProgram();
		if (programHandle == 0) {
			logString = "Unable to create shader program.";
			return false;
		}
	}
	switch (type) {
	case GLSLShader::VERTEX:
		vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
		return buildShader(vertexShaderHandle, source);
		break;

	case GLSLShader::FRAGMENT:
		fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
		return buildShader(fragmentShaderHandle, source);
		break;

	case GLSLShader::GEOMETRY:
		geometryShaderHandle = glCreateShader(GL_GEOMETRY_SHADER);
		return buildShader(geometryShaderHandle, source);
		break;

	case GLSLShader::TESS_CONTROL:
		tessControlShaderHandle = glCreateShader(GL_TESS_CONTROL_SHADER);
		return buildShader(tessControlShaderHandle, source);
		break;

	case GLSLShader::TESS_EVALUATION:
		tessEvalShaderHandle = glCreateShader(GL_TESS_EVALUATION_SHADER);
		return buildShader(tessEvalShaderHandle, source);
		break;
	default:
		return false;
	}
	return false;
}

bool GLSLProgram::buildShader(unsigned int shaderHandle, const char *source)
{
	glShaderSource(shaderHandle, 1, &source, NULL);

	// Compile the shader
	glCompileShader(shaderHandle);

	// Check for errors
	int result;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result) {
		// Compile failed, store log and return false
		int length = 0;
		logString = "";
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);
		if (length > 0) {
			char * c_log = new char[length];
			int written = 0;
			glGetShaderInfoLog(shaderHandle, length, &written, c_log);
			logString = c_log;
			delete[] c_log;
		}

		return false;
	}
	else {
		// Compile succeeded, attach shader and return true
		glAttachShader(programHandle, shaderHandle);
		return true;
	}
}

bool GLSLProgram::link()
{
	if (linked) return true;
	if (programHandle <= 0) return false;

	glLinkProgram(programHandle);

	int status = 0;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &status);
	if (GL_FALSE == status) {
		// Store log and return false
		int length = 0;
		logString = "";

		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &length);

		if (length > 0) {
			char * c_log = new char[length];
			int written = 0;
			glGetProgramInfoLog(programHandle, length, &written, c_log);
			logString = c_log;
			delete[] c_log;
		}

		return false;
	}
	else {
		linked = true;
		return linked;
	}
}

void GLSLProgram::use()
{
	if (programHandle <= 0 || (!linked)) return;
	//bool success =validate();
	glUseProgram(programHandle);
}

void GLSLProgram::unUse()
{
	// disable any shader program currently actiuve
	glUseProgram(0);
}

std::string GLSLProgram::log()
{
	return logString;
}

int GLSLProgram::getHandle()
{
	return programHandle;
}

bool GLSLProgram::isLinked()
{
	return linked;
}

void GLSLProgram::bindAttribLocation(GLuint location, const char * name)
{
	glBindAttribLocation(programHandle, location, name);
}

void GLSLProgram::bindFragDataLocation(GLuint location, const char * name)
{
	glBindFragDataLocation(programHandle, location, name);
}

bool GLSLProgram::validate()
{
	if (!isLinked()) return false;

	GLint status;
	glValidateProgram(programHandle);
	glGetProgramiv(programHandle, GL_VALIDATE_STATUS, &status);

	if (GL_FALSE == status) {
		// Store log and return false
		int length = 0;
		logString = "";

		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &length);

		if (length > 0) {
			char * c_log = new char[length];
			int written = 0;
			glGetProgramInfoLog(programHandle, length, &written, c_log);
			logString = c_log;
			delete[] c_log;
		}

		return false;
	}
	else {
		return true;
	}
}

int GLSLProgram::getUniformLocation(const char * name)
{
	return glGetUniformLocation(programHandle, name);
}

std::string GLSLProgram::loadShaderFromBuffer(unsigned char *programBuffer, int size)
{
	std::string stringToReturn = "";
	int bufferSize = 0;
	unsigned char *buffer = nullptr;
	bool success = uncompressShaderBuffer(programBuffer, size, &buffer, bufferSize);

	if (bufferSize > 0) {
		stringToReturn = reinterpret_cast<char *>(buffer);

		if (buffer != nullptr) {
			delete[] buffer;
			buffer = nullptr;
		}
	}
	return stringToReturn;
}

std::string GLSLProgram::loadShaderFromFile(const std::string &filename)
{
	std::string stringToReturn = "";
	int bufferSize = 0;
	unsigned char *buffer = nullptr;
	bool success = getShaderCodeBufferFromFile(filename, &buffer, bufferSize);

	if (bufferSize > 0) {
		stringToReturn = reinterpret_cast<char *>(buffer);

		if (buffer != nullptr) {
			delete[] buffer;
			buffer = nullptr;
		}
	}
	return stringToReturn;
}
