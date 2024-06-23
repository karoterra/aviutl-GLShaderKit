#pragma once

#include <algorithm>

#include <Windows.h>

namespace glshaderkit {

// DLLの設定
struct Config {
    // シェーダーのキャッシュ数
    int shaderCacheCapacity = 4;

    // iniファイルから設定を読み込む
    void Load(const char* path) {
        shaderCacheCapacity = GetPrivateProfileIntA("config", "shaderCacheCapacity", 4, path);
        shaderCacheCapacity = std::max(1, shaderCacheCapacity);
    }
};

} // namespace glshaderkit
