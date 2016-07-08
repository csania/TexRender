#include "GL/glew.h"
#include "TextureLoader.h"

#include <fstream>
#include <sstream>

TextureLoader *TextureLoader::instance = nullptr;

TextureLoader *TextureLoader::Instance()
{
	if (!instance) {
		instance = new TextureLoader();
	}
	return instance;
}

unsigned int TextureLoader::setupPointDrawShader(std::string vsSourceLocation, std::string fsSourceLocation)
{
	bool shadersSupported = (glCreateShader != nullptr);
	GLuint shaderProgId = 0;

	if (shadersSupported) {

		const char *shaderSrc[1];

		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		std::string vsSource = readShaderSource(vsSourceLocation);
		std::string fsSource = readShaderSource(fsSourceLocation);

		/*std::string vsSource = readShaderSource("C:\\Users\\alex.cicenkov\\Documents\\Visual Studio 2015\\Projects\\colormapScale\\Shaders\\colorScale_vs.glsl");
		std::string fsSource = readShaderSource("C:\\Users\\alex.cicenkov\\Documents\\Visual Studio 2015\\Projects\\colormapScale\\Shaders\\colorScale_fs.glsl");*/

		shaderSrc[0] = vsSource.c_str();
		glShaderSource(vertexShaderID, 1, shaderSrc, 0);
		glCompileShader(vertexShaderID);

		shaderSrc[0] = fsSource.c_str();
		glShaderSource(fragmentShaderID, 1, shaderSrc, 0);
		glCompileShader(fragmentShaderID);

		if (checkShaderCompileError(vertexShaderID) && checkShaderCompileError(fragmentShaderID)) {

			shaderProgId = glCreateProgram();

			glAttachShader(shaderProgId, vertexShaderID);
			glAttachShader(shaderProgId, fragmentShaderID);

			glLinkProgram(shaderProgId);


			int status = 0;
			glGetProgramiv(shaderProgId, GL_LINK_STATUS, &status);
			if (GL_FALSE == status) {
				// Store log and return false
				int length = 0;
				std::string logString = "";

				glGetProgramiv(shaderProgId, GL_INFO_LOG_LENGTH, &length);

				if (length > 0) {
					char * c_log = new char[length];
					int written = 0;
					glGetProgramInfoLog(shaderProgId, length, &written, c_log);
					logString = c_log;
					delete[] c_log;
				}

				return false;
			}

			GLint linkedVS, linkedFS = 0;
			glGetProgramiv(shaderProgId, GL_LINK_STATUS, &linkedVS);
			glGetProgramiv(shaderProgId, GL_LINK_STATUS, &linkedFS);

			if (!linkedVS || !linkedFS) {
				shaderProgId = 0;
			}
		}
	}
	else {
		std::ostringstream hardwareInfo;
		if (glGetString(GL_VERSION)) {
			hardwareInfo << "\tGL version   = " << glGetString(GL_VERSION) << "\n";
		}
		if (glGetString(GL_VENDOR)) {
			hardwareInfo << "\tGL vendor    = " << glGetString(GL_VENDOR) << "\n";
		}
		if (glGetString(GL_RENDERER)) {
			hardwareInfo << "\tGL renderer  = " << glGetString(GL_RENDERER) << "\n";
		}
		if (glGetString(GL_EXTENSIONS)) {
			hardwareInfo << "\tGL extensions= " << glGetString(GL_EXTENSIONS) << "\n";
		}

	}
	return shaderProgId;
}

std::string TextureLoader::readShaderSource(std::string location)
{
	std::string content;
	std::ifstream fileIn(location);

	if (fileIn.good()) {
		content.assign((std::istreambuf_iterator<char>(fileIn)),
			std::istreambuf_iterator<char>());
	}

	return content;
}

bool TextureLoader::checkShaderCompileError(GLuint shaderID)
{
	GLint compiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		// Compile failed, store log and return false
		int length = 0;
		//logString = "";
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
		if (length > 0) {
			char * c_log = new char[length];
			int written = 0;
			glGetShaderInfoLog(shaderID, length, &written, c_log);
			auto logString = c_log;
			delete[] c_log;
		}
		return false;
	}
	return true;
}

bool TextureLoader::loadDDsTex(std::string name)
{
	loadedTexture = new gli::texture2D(loadTexFromHD(name.c_str()));
	if (loadedTexture) {
		return true;
	}

	return false;
}

gli::texture2D* TextureLoader::getLoadedTexture()
{
	return loadedTexture;
}

TextureLoader::TextureLoader()
{
}

TextureLoader::~TextureLoader()
{
	if (loadedTexture) {
		delete loadedTexture;
		loadedTexture = nullptr;
	}
}

gli::storage TextureLoader::loadTexFromHD(const char * Filename)
{
	std::ifstream FileIn(Filename, std::ios::in | std::ios::binary);
	assert(!FileIn.fail());

	if (FileIn.fail())
		return gli::storage();

	gli::detail::ddsHeader HeaderDesc;
	gli::detail::ddsHeader10 HeaderDesc10;
	char Magic[4];

	//* Read magic number and check if valid .dds file 
	FileIn.read((char*)&Magic, sizeof(Magic));

	assert(strncmp(Magic, "DDS ", 4) == 0);

	// Get the surface descriptor 
	FileIn.read((char*)&HeaderDesc, sizeof(HeaderDesc));
	if (HeaderDesc.format.flags & gli::detail::DDPF_FOURCC && HeaderDesc.format.fourCC == gli::detail::D3DFMT_DX10)
		FileIn.read((char*)&HeaderDesc10, sizeof(HeaderDesc10));

	gli::format Format(gli::FORMAT_NULL);
	if (HeaderDesc.format.fourCC == gli::detail::D3DFMT_DX10)
		Format = gli::detail::format_dds2gli_cast(HeaderDesc10.Format);
	else if (HeaderDesc.format.flags & gli::detail::DDPF_FOURCC)
		Format = gli::detail::format_fourcc2gli_cast(HeaderDesc.format.flags, HeaderDesc.format.fourCC);
	else if (HeaderDesc.format.flags & gli::detail::DDPF_RGB)
	{
		switch (HeaderDesc.format.bpp)
		{
		case 8:
			Format = gli::R8_UNORM;
			break;
		case 16:
			Format = gli::RG8_UNORM;
			break;
		case 24:
			Format = gli::RGB8_UNORM;
			break;
		case 32:
			Format = gli::RGBA8_UNORM;
			break;
		}
	}
	else
		assert(0);

	std::streamoff Curr = FileIn.tellg();
	FileIn.seekg(0, std::ios_base::end);
	std::streamoff End = FileIn.tellg();
	FileIn.seekg(Curr, std::ios_base::beg);

	gli::storage::size_type const MipMapCount = (HeaderDesc.flags & gli::detail::DDSD_MIPMAPCOUNT) ?
		HeaderDesc.mipMapLevels : 1;

	gli::storage::size_type FaceCount(1);
	if (HeaderDesc.cubemapFlags & gli::detail::DDSCAPS2_CUBEMAP)
		FaceCount = int(glm::bitCount(HeaderDesc.cubemapFlags & gli::detail::DDSCAPS2_CUBEMAP_ALLFACES));

	gli::storage::size_type DepthCount = 1;
	if (HeaderDesc.cubemapFlags & gli::detail::DDSCAPS2_VOLUME)
		DepthCount = HeaderDesc.depth;

	gli::storage Storage(
		HeaderDesc10.arraySize,
		FaceCount,
		MipMapCount,
		Format,
		gli::storage::dimensions_type(HeaderDesc.width, HeaderDesc.height, DepthCount));

	FileIn.read((char*)Storage.data(), std::size_t(End - Curr));

	gli::storage bloatedStorage(
		HeaderDesc10.arraySize,
		FaceCount,
		MipMapCount,
		gli::RGBA8_UNORM,
		gli::storage::dimensions_type(HeaderDesc.width, HeaderDesc.height, DepthCount));

	unsigned int* spFit = (unsigned int *)bloatedStorage.data();
	unsigned char *tex = (unsigned char *)Storage.data();

	for (int ix = 0; ix < HeaderDesc.width*HeaderDesc.height; ix++) {
		spFit[ix] = *(tex + 2) + *(tex + 1) * 256 + *(tex) * 65536;
		//std::cout << spFit[i] << std::endl;
		tex += 3;
	}

	return bloatedStorage;
}
