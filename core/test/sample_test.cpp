#include <ess.alg/core/sample.hpp>

#include <gtest/gtest.h>

//--------------------------------------------------
using namespace ess_alg::core;

class sample_test
    :public ::testing::Test
{
public:
    void SetUp()
    {}
};

TEST_F(sample_test, SampleFunction_1)
{
    ASSERT_EQ(SampleFunction(1), 1);
}

TEST_F(sample_test, SampleFunction_0)
{
    ASSERT_EQ(SampleFunction(0), 0);
}
