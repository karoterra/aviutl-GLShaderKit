#include <gtest/gtest.h>

#include <string>

#include "cache.hpp"
#include "move_only.hpp"

// キャッシュを捨てない範囲でPut, Getが機能するか
TEST(MoveOnlyLruCacheTest, PutAndGet) {
    glshaderkit::MoveOnlyLruCache<std::string, MoveOnly> cache(2);

    cache.Put("one", MoveOnly(1));
    cache.Put("two", MoveOnly(2));

    auto one = cache.Get("one");
    ASSERT_TRUE(one.has_value());
    EXPECT_EQ(one->get().x_, 1);

    auto two = cache.Get("two");
    ASSERT_TRUE(two.has_value());
    EXPECT_EQ(two->get().x_, 2);
}

// キャパシティ以上にPutすると捨てられるか
TEST(MoveOnlyLruCacheTest, EvictionWorks) {
    glshaderkit::MoveOnlyLruCache<std::string, MoveOnly> cache(2);

    cache.Put("one", MoveOnly(1));
    cache.Put("two", MoveOnly(2));
    cache.Put("three", MoveOnly(3));    // "one" が捨てられるはず

    auto one = cache.Get("one");
    ASSERT_FALSE(one.has_value());

    auto two = cache.Get("two");
    ASSERT_TRUE(two.has_value());
    EXPECT_EQ(two->get().x_, 2);

    auto three = cache.Get("three");
    ASSERT_TRUE(three.has_value());
    EXPECT_EQ(three->get().x_, 3);
}

// Getしたものがキャッシュの先頭に移動するか
TEST(MoveOnlyLruCacheTest, AccessUpdatesPriority) {
    glshaderkit::MoveOnlyLruCache<std::string, MoveOnly> cache(2);

    cache.Put("one", MoveOnly(1));
    cache.Put("two", MoveOnly(2));

    // "one" が先頭に移動する
    cache.Get("one");

    // "two" が捨てられるはず
    cache.Put("three", MoveOnly(3));

    auto one = cache.Get("one");
    ASSERT_TRUE(one.has_value());
    EXPECT_EQ(one->get().x_, 1);

    auto two = cache.Get("two");
    ASSERT_FALSE(two.has_value());

    auto three = cache.Get("three");
    ASSERT_TRUE(three.has_value());
    EXPECT_EQ(three->get().x_, 3);
}

// 同じキーの値を上書きする
TEST(MoveOnlyLruCacheTest, Overwrite) {
    glshaderkit::MoveOnlyLruCache<std::string, MoveOnly> cache(2);

    cache.Put("one", MoveOnly(1));
    cache.Put("one", MoveOnly(101));

    auto one = cache.Get("one");
    ASSERT_TRUE(one.has_value());
    EXPECT_EQ(one->get().x_, 101);

    EXPECT_EQ(cache.Size(), 1);
}

// クリア
TEST(MoveOnlyLruCacheTest, Clear) {
    glshaderkit::MoveOnlyLruCache<std::string, MoveOnly> cache(2);

    cache.Put("one", MoveOnly(1));
    cache.Put("two", MoveOnly(2));

    cache.Clear();

    auto one = cache.Get("one");
    ASSERT_FALSE(one.has_value());

    auto two = cache.Get("two");
    ASSERT_FALSE(two.has_value());

    EXPECT_EQ(cache.Size(), 0);
}

// 要素削除
TEST(MoveOnlyLruCacheTest, Erase) {
    glshaderkit::MoveOnlyLruCache<std::string, MoveOnly> cache(2);

    cache.Put("one", MoveOnly(1));
    cache.Put("two", MoveOnly(2));

    cache.Erase("one");

    auto one = cache.Get("one");
    ASSERT_FALSE(one.has_value());

    auto two = cache.Get("two");
    ASSERT_TRUE(two.has_value());
    EXPECT_EQ(two->get().x_, 2);

    EXPECT_EQ(cache.Size(), 1);
}
