#pragma once

#include <iostream>
#include <string>

namespace glshaderkit {

class Log {
public:
    static void Info(const std::string& message) {
        std::cout << "[GLShaderKit][INFO]" << message << std::endl;
    }

    // コンソールに黄色で出力
    static void Warning(const std::string& message) {
        std::cout << "\033[93m[GLShaderKit][WARNING]" << message << "\033[m" << std::endl;
    }

    // コンソールに赤色で出力
    static void Error(const std::string& message) {
        std::cout << "\033[91m[GLShaderKit][ERROR]" << message << "\033[m" << std::endl;
    }
};

} // namespace glshaderkit
