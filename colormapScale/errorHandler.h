#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <string>

class ErrorHandler
{

public:
	enum ErrorCode {
		CompileVertexShaderFailed = 0,
		CompileTesselationShaderFailed = 1,
		CompileTeselationEvaluationShaderFailed = 2,
		CompileGeometryShaderFailed = 3,
		CompileFragmentShaderFailed = 4,
		CompileLinkFailed = 5,
		FailedToLoadProject = 6,
		AllTexturesFailedToLoad = 7,
		DeadTilesFailedToLoad = 8,
		LfpFilesNotFound = 9,

		NONCRITICAL = 0x8000,
		NoError = 0x8001,
		ScanFailedToLoad = 0x8002,
		SomeTexturesFailedToLoad = 0x8003
	};

	static bool isCritical(ErrorCode code);
	static std::string translate(ErrorCode code);
	static ErrorCode lastError();
	static void setError(ErrorCode error);
	static void clearLastError();

private:
	static ErrorCode lastErrorCode;
};
#endif