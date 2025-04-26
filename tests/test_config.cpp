#include <gtest/gtest.h>

#include "config.hpp"

TEST(ConfigTest, DefaultValue) {
    glshaderkit::Config config;
    EXPECT_EQ(config.shaderCacheCapacity, 4);
}

TEST(ConfigTest, Load) {
    glshaderkit::Config config;
    config.Load("data/config_load.ini");
    EXPECT_EQ(config.shaderCacheCapacity, 16);
}

TEST(ConfigTest, LoadNegative) {
    glshaderkit::Config config;
    config.Load("data/config_load_negative.ini");
    EXPECT_EQ(config.shaderCacheCapacity, 1);
}
