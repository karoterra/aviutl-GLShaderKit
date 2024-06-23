#pragma once

#include <string>
#include <unordered_map>
#include <list>

#include "gl_shader.hpp"

namespace glshaderkit {

class GLShaderManager {
public:
    struct Item {
        std::string basePath;
        GLShader shader;
    };

    using Iterator = typename std::list<Item>::iterator;

    GLShaderManager(size_t capacity) : capacity_(capacity), current_(nullptr) {}

    GLShader& Get(const std::string& basePath, bool forceReload) {
        auto it = cacheMap_.find(basePath);
        current_ = nullptr;

        // キャッシュされているとき
        if (it != cacheMap_.end()) {
            // 再ロード
            if (forceReload) {
                it->second->shader = GLShader(basePath);
            }
            // キャッシュの先頭に移動
            cacheItems_.splice(cacheItems_.begin(), cacheItems_, it->second);
            current_ = &(*cacheItems_.begin());
            return current_->shader;
        }

        // キャッシュされていないとき
        // シェーダーをロードしてキャッシュに追加
        cacheItems_.emplace_front(basePath, GLShader(basePath));
        current_ = &(*cacheItems_.begin());
        cacheMap_[basePath] = cacheItems_.begin();

        // キャッシュ数がcapacityを超えたときは最後尾のキャッシュを削除
        if (cacheItems_.size() > capacity_) {
            auto last = cacheItems_.end();
            last--;
            cacheMap_.erase(last->basePath);
            cacheItems_.pop_back();
        }

        return current_->shader;
    }

    Item* Current() const {
        return current_;
    }

    void SetCapacity(size_t capacity) {
        capacity_ = capacity;
    }

    void Release() {
        current_ = nullptr;
        cacheMap_.clear();
        cacheItems_.clear();
    }

private:
    // キャッシュサイズ
    size_t capacity_;

    std::list<Item> cacheItems_;
    std::unordered_map<std::string, Iterator> cacheMap_;
    Item* current_;
};

} // namespace glshaderkit
