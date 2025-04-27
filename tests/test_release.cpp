#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "release.hpp"

using namespace glshaderkit;

class MockRelease : public IRelease {
public:
    MOCK_METHOD(void, Release, (), (override));
};

TEST(ReleaseContainerTest, AddAndContains) {
    ReleaseContainer container;
    MockRelease resource1;
    MockRelease resource2;

    EXPECT_FALSE(container.Contains(&resource1));
    EXPECT_FALSE(container.Contains(&resource2));

    container.Add(&resource1);
    EXPECT_TRUE(container.Contains(&resource1));
    EXPECT_FALSE(container.Contains(&resource2));

    container.Add(&resource2);
    EXPECT_TRUE(container.Contains(&resource1));
    EXPECT_TRUE(container.Contains(&resource2));
}

TEST(ReleaseContainerTest, Size) {
    ReleaseContainer container;
    MockRelease resource1;
    MockRelease resource2;

    EXPECT_EQ(container.Size(), 0);

    container.Add(&resource1);
    EXPECT_EQ(container.Size(), 1);

    container.Add(&resource2);
    EXPECT_EQ(container.Size(), 2);

    container.Remove(&resource1);
    EXPECT_EQ(container.Size(), 1);
}

TEST(ReleaseContainerTest, Remove) {
    ReleaseContainer container;
    MockRelease resource1;
    MockRelease resource2;

    container.Add(&resource1);
    container.Add(&resource2);

    container.Remove(&resource1);
    EXPECT_FALSE(container.Contains(&resource1));
    EXPECT_TRUE(container.Contains(&resource2));

    container.Remove(&resource1);
    EXPECT_FALSE(container.Contains(&resource1));
    EXPECT_TRUE(container.Contains(&resource2));
}

TEST(ReleaseContainerTest, ReleaseAll) {
    ReleaseContainer container;
    testing::StrictMock<MockRelease> resource1;
    testing::StrictMock<MockRelease> resource2;
    testing::StrictMock<MockRelease> resource3;

    container.Add(&resource1);
    container.Add(&resource2);
    container.Add(&resource3);

    container.Remove(&resource2);

    EXPECT_CALL(resource1, Release()).Times(1);
    EXPECT_CALL(resource2, Release()).Times(0);
    EXPECT_CALL(resource3, Release()).Times(1);

    container.ReleaseAll();

    EXPECT_EQ(container.Size(), 0);
}
