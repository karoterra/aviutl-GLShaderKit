#pragma once

#include <list>
#include <unordered_map>
#include <optional>
#include <utility>
#include <functional>

namespace glshaderkit {

// ムーブのみ(コピー不可)なValue用のLRUキャッシュ
template<typename Key, typename Value>
class MoveOnlyLruCache {
public:
    using Item = std::pair<Key, Value>;
    using Iterator = typename std::list<Item>::iterator;

    explicit MoveOnlyLruCache(size_t capacity) : capacity_(capacity) {}

    void Put(const Key& key, Value&& value) {
        auto it = cacheItemMap_.find(key);

        if (it != cacheItemMap_.end()) {
            // 既に存在する場合、リストを更新して先頭へ
            cacheItemList_.erase(it->second);
            cacheItemMap_.erase(it);
        } else if (cacheItemList_.size() >= capacity_) {
            // キャパ超えたら末尾を削除
            const auto& last = cacheItemList_.back();
            cacheItemMap_.erase(last.first);
            cacheItemList_.pop_back();
        }

        cacheItemList_.emplace_front(key, std::move(value));
        cacheItemMap_[key] = cacheItemList_.begin();
    }

    std::optional<std::reference_wrapper<Value>> Get(const Key& key) {
        auto it = cacheItemMap_.find(key);
        if (it == cacheItemMap_.end()) {
            return std::nullopt;
        }
        cacheItemList_.splice(cacheItemList_.begin(), cacheItemList_, it->second);
        return std::ref(it->second->second);
    }

    void Erase(const Key& key) {
        auto it = cacheItemMap_.find(key);
        if (it != cacheItemMap_.end()) {
            cacheItemList_.erase(it->second);
            cacheItemMap_.erase(it);
        }
    }

    void Clear() {
        cacheItemMap_.clear();
        cacheItemList_.clear();
    }

    size_t Size() const {
        return cacheItemList_.size();
    }

    void SetCapacity(size_t capacity) {
        capacity_ = capacity;
    }

private:
    size_t capacity_;
    std::list<Item> cacheItemList_;
    std::unordered_map<Key, Iterator> cacheItemMap_;
};

} // namespace glshaderkit
