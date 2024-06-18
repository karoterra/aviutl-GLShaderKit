#pragma once

#include <iostream>
#include <string>

#include <Windows.h>

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

    // 最後のエラーメッセージを取得
    static std::string GetLastErrorMessage() {
        DWORD error = GetLastError();
        return Log::GetLastErrorMessage(error);
    }

    // エラーコードを指定してエラーメッセージを取得
    static std::string GetLastErrorMessage(DWORD error) {
        LPSTR buf;
        DWORD length = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&buf),
            0,
            nullptr
        );
        if (length > 0) {
            std::string message(buf, length);
            LocalFree(buf);
            return message;
        }
        else {
            return "Unknown error";
        }
    }
};

} // namespace glshaderkit
