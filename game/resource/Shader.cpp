#include "Shader.hpp"
#include <fstream>
#include <memory>

aik::resource::Shader::Shader()
{
	m_isBound = false;
	m_shaderIDs.reserve(2);
	m_iProgramID = 0;
}

aik::resource::Shader::Shader(const std::string &vertexShader, const std::string &fragmentShader,
                    const std::string &geometryShader) : Shader()
{
    addShader(vertexShader, GL_VERTEX_SHADER);
    addShader(fragmentShader, GL_FRAGMENT_SHADER);
    if(!geometryShader.empty())
    {
        addShader(geometryShader, GL_GEOMETRY_SHADER);
    }
    compileProgram();
}

aik::resource::Shader::~Shader()
{
	m_isBound = false;
	m_shaderIDs.clear();
	m_iProgramID = 0;
}

void aik::resource::Shader::addShader(const std::string& shaderFile, unsigned int shaderType)
{
	GLuint shaderID;
	std::string fileContents, currentLine;
	std::fstream fileStream;
	fileStream.open(shaderFile);
	if (fileStream.is_open())
	{
		while (std::getline(fileStream, currentLine))
		{
			fileContents += currentLine + "\n";
		}
	}
	shaderID = glCreateShader(shaderType);
	const GLchar* const shaderSource = fileContents.c_str();
	glShaderSource(shaderID, 1, &shaderSource, nullptr);
	glCompileShader(shaderID);

    int  success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
		if (shaderType == GL_VERTEX_SHADER)
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		else if (shaderType == GL_FRAGMENT_SHADER)
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		else
			std::cout << "An error occurred in shader compliation with: " << infoLog << std::endl;
    }
	m_shaderIDs.push_back(shaderID);
}

void aik::resource::Shader::compileProgram()
{
	m_iProgramID = glCreateProgram();
	for(const auto& shader : m_shaderIDs)
	{
		glAttachShader(m_iProgramID, shader);
	}
	glLinkProgram(m_iProgramID);

	int  success;
	char infoLog[512];
	glGetProgramiv(m_iProgramID, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(m_iProgramID, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	for(const auto& shader : m_shaderIDs)
	{
		glDeleteShader(shader);
	}
	m_shaderIDs.clear();
}

void aik::resource::Shader::useProgram()
{
	m_isBound = true;
	glUseProgram(m_iProgramID);
}

void aik::resource::Shader::unuseProgram()
{
	m_isBound = false;
    glUseProgram(0);
}

GLint aik::resource::Shader::getUniformLocation(const std::string& uniformName)
{
	// if program is not bound, force a bind
	// if a bind is forced then we unbind it
	bool forcedBind = false;
	if (!m_isBound)
	{
		forcedBind = true;
		useProgram();
	}
	auto location = glGetUniformLocation(m_iProgramID, uniformName.c_str());
	if (forcedBind)
		unuseProgram();
	return location;
}

std::shared_ptr<aik::resource::Shader> aik::resource::Shader::load(const std::string & vertexShader, const std::string & fragmentShader, const std::string & geometryShader) const
{
    return std::make_shared<aik::resource::Shader>(vertexShader, fragmentShader, geometryShader);
}

void aik::resource::Shader::glUniform(GLuint location, GLuint data)
{
	glUniform1ui(location, data);
}

void aik::resource::Shader::glUniform(GLuint location, GLint data)
{
	glUniform1i(location, data);
}

void aik::resource::Shader::glUniform(GLuint location, GLboolean data)
{
	glUniform1i(location, data);
}

void aik::resource::Shader::glUniform(GLuint location, GLfloat data)
{
	glUniform1f(location, data);
}

void aik::resource::Shader::glUniform(GLuint location, GLdouble data)
{
	glUniform1d(location, data);
}

void aik::resource::Shader::glUniform(GLuint location, glm::vec2 data)
{
	glUniform2fv(location, 1, glm::value_ptr(data));
}

void aik::resource::Shader::glUniform(GLuint location, glm::vec3 data)
{
	glUniform3fv(location, 1, glm::value_ptr(data));
}

void aik::resource::Shader::glUniform(GLuint location, glm::vec4 data)
{
	glUniform4fv(location, 1, glm::value_ptr(data));
}

void aik::resource::Shader::glUniformMatrix(GLuint location, GLboolean transpose, glm::mat3 data)
{
	glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(data));
}

void aik::resource::Shader::glUniformMatrix(GLuint location, GLboolean transpose, glm::mat4 data)
{
	glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(data));
}
