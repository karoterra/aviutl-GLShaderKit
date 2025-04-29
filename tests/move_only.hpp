#pragma once

class MoveOnly {
public:
    int x_;

    explicit MoveOnly(int x) : x_(x) {}

    // コピー禁止
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    // ムーブ
    MoveOnly(MoveOnly&& other) : x_(other.x_) {
        other.x_ = 0;
    }

    // ムーブ代入
    MoveOnly& operator=(MoveOnly&& other) {
        if (this != &other) {
            x_ = other.x_;
            other.x_ = 0;
        }
        return *this;
    }
};
