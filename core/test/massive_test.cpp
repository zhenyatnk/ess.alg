#include <ess.alg/core/massive.h>

#include <gtest/gtest.h>

//--------------------------------------------------
using namespace ess_alg::core;

class Massive_test
    :public ::testing::Test
{
public:
    void SetUp()
    {}
};

TEST_F(Massive_test, median_empty)
{
    ASSERT_EQ(0, FindMedian(std::vector<int>{}));
}
TEST_F(Massive_test, median_duplicate)
{
    ASSERT_EQ(0, FindMedian(std::vector<int>{5,5,5,5})); // problem alghoritm
}
TEST_F(Massive_test, median)
{
    ASSERT_EQ(7, FindMedian(std::vector<int>{1,3,4,7,8,8,9}));
}
