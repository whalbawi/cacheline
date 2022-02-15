#include "cacheline.h"

#include "gtest/gtest.h"

class CachelineTest : public ::testing::Test {
  protected:
    cacheline::Cacheline cacheline{};
};

TEST_F(CachelineTest, Version) {
    ASSERT_NE(cacheline::Cacheline::get_version(), "");
}
