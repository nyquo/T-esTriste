#include "Shader.hpp"

#include <TesTristeLib/Core/Logger.hpp>
#include <TesTristeLib/Core/gl.hpp>

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace TesTriste {

Shader::Shader(const fs::path& vertexPath, const fs::path& fragmentPath) {
    // Create an empty vertex shader handle
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const std::string vertexSrc = parseFile(vertexPath);
    const GLchar* source = vertexSrc.c_str();
    glShaderSource(vertexShader, 1, &source, nullptr);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, infoLog.data());

        // We don't need the shader anymore.
        glDeleteShader(vertexShader);

        // Log error
        Logger::logError("Vertex shader compilation error:");
        Logger::logError(infoLog.data());

        return;
    }
    Logger::logTrace("Vertex Shader compilation success!");

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const std::string fragmentSrc = parseFile(fragmentPath);
    source = fragmentSrc.c_str();
    glShaderSource(fragmentShader, 1, &source, nullptr);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, infoLog.data());

        // We don't need the shader anymore.
        glDeleteShader(fragmentShader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertexShader);

        // Log error
        Logger::logError("Fragment shader compilation error:");
        Logger::logError(infoLog.data());

        return;
    }
    Logger::logTrace("Fragment Shader compilation success!");

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    m_programId = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(m_programId, vertexShader);
    glAttachShader(m_programId, fragmentShader);

    // Link our program
    glLinkProgram(m_programId);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(m_programId, GL_LINK_STATUS, (int*)&isLinked);
    if(isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_programId, maxLength, &maxLength, infoLog.data());

        // We don't need the program anymore.
        glDeleteProgram(m_programId);
        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Use the infoLog as you see fit.
        Logger::logError("Shader linking error:");
        Logger::logError(infoLog.data());

        return;
    }
    Logger::logTrace("Shader linking success!");

    // Always detach shaders after a successful link.
    glDetachShader(m_programId, vertexShader);
    glDetachShader(m_programId, fragmentShader);
}

Shader::~Shader() { glDeleteProgram(m_programId); }

void Shader::bind() const { glUseProgram(m_programId); }

void Shader::unbind() { glUseProgram(0); }

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const {
    glm::vec3 vec(x, y, z);
    setVec3(name, vec);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
}

std::string Shader::parseFile(const fs::path& filePath) {
    Logger::logTrace("FilePath: " + filePath.filename().string());
    std::ifstream file(filePath);
    if(!file.is_open()) {
        Logger::logError("File " + filePath.string() + " could not be opened!");
        return "";
    }
    std::string str;
    std::string content;
    while(std::getline(file, str)) {
        content.append(str + "\n");
    }

    Logger::logTrace("Shader content: " + content);
    return content;
}

}