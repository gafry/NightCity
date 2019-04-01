#include "Shader.h"

Shader::Shader(const char * vShaderCode, const char * fShaderCode)
{
	unsigned int vertex, fragment;

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	//checkCompileErrors(vertex, "VERTEX");

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	//checkCompileErrors(fragment, "FRAGMENT");

	// shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	//checkCompileErrors(ID, "PROGRAM");

	// smazani shaderu po nalinkovani do programu
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(ID);
}

// neni potreba ve finalni verzi
//void Shader::checkCompileErrors(GLuint shader, std::string type)
//{
//	GLint success;
//	GLchar infoLog[1024];
//	if (type != "PROGRAM")
//	{
//		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//		if (!success)
//		{
//			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
//			std::wstring s(L"ERROR::SHADER_COMPILATION_ERROR of type: ");
//			wprintf(L"%s\n", s.c_str());
//		}
//	}
//	else
//	{
//		glGetProgramiv(shader, GL_LINK_STATUS, &success);
//		if (!success)
//		{
//			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
//			std::wstring s(L"ERROR::PROGRAM_LINKING_ERROR of type:");
//			wprintf(L"%s\n", s.c_str());
//		}
//	}
//}
