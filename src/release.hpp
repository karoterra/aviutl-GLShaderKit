#pragma once

#include <vector>

namespace glshaderkit {

// リソースの解放処理をもつクラス用のインターフェース
class IRelease {
public:
    virtual ~IRelease() = default;
    virtual void Release() = 0;
};

// リリース対象をまとめるコンテナ
class ReleaseContainer {
public:
    // 登録済みリリース対象の個数を取得する
    size_t Size() const;

    // 登録済みか確認する
    bool Contains(IRelease* x) const;

    // リリース対象として登録する
    void Add(IRelease* x);

    // リリース対象から登録解除する
    void Remove(IRelease* x);

    // 登録済みのリリース対象をすべてリリースする
    void ReleaseAll();
private:
    // リリース対象
    std::vector<IRelease*> targets_;
};

} // namespace glshaderkit
