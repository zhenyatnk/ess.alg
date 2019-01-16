#include <ess.alg/core/listcpp.hpp>

#include <gtest/gtest.h>

//--------------------------------------------------
using namespace ess_alg::core::cppstyle;

namespace
{
    std::ostream& operator<< (std::ostream &out, const std::vector<int>& obj)
    {
        out << "{";
        for(auto i = 0; i < obj.size(); ++i)
        {
            out << obj[i];
            if(i + 1 < obj.size())
                out << ",";
        }
        out << "}";
        return out;
    }
    
    template<typename TypeValue>
    bool Equal(const std::vector<TypeValue>& left, const List<TypeValue>& right)
    {
        auto current = right.Begin();
        for(const auto& element: left)
        {
            if(!current || element != current->m_data)
            {
                std::cout << "Equal:"  << std::endl
                << " Left: " << left << std::endl
                << "Right: " << right << std::endl;
                return false;
            }
            current = current->m_next;
        }
        return true;
    }
}

class ListCpp_test
    :public ::testing::Test
{
public:
    void SetUp()
    {}
};

TEST_F(ListCpp_test, Ctor_Empty)
{
    List<int> list;
    ASSERT_EQ(list.Begin(), nullptr);
}

TEST_F(ListCpp_test, Empty)
{
    List<int> list;
    ASSERT_TRUE(list.IsEmpty());
}

TEST_F(ListCpp_test, Not_Empty)
{
    List<int> list;
    list+=1;
    ASSERT_TRUE(!list.IsEmpty());
}

TEST_F(ListCpp_test, InsertBack)
{
    List<int> list;
    list+=1;
    ASSERT_TRUE(Equal({1}, list));
}

TEST_F(ListCpp_test, Clear)
{
    List<int> list;
    list+=1;
    list+=2;
    ASSERT_NE(list.Begin(), nullptr);
    list.Clear();
    ASSERT_EQ(list.Begin(), nullptr);
}

TEST_F(ListCpp_test, Initializer_list_Int)
{
    List<int> list({1,2});
    ASSERT_TRUE(Equal({1, 2}, list));
}

TEST_F(ListCpp_test, FindMax_Empty)
{
    List<int> list;
    auto max = list.FindMax();
    ASSERT_EQ(max, nullptr);
}

TEST_F(ListCpp_test, FindMax_One)
{
    List<int> list({1});
    auto max = list.FindMax();
    ASSERT_EQ(max->m_data, 1);
}

TEST_F(ListCpp_test, FindMax_Some)
{
    List<int> list({1,4,3});
    auto max = list.FindMax();
    ASSERT_EQ(max->m_data, 4);
}

TEST_F(ListCpp_test, FindMax_OnlyNegative)
{
    List<int> list({-1,-4,-3});
    auto max = list.FindMax();
    ASSERT_EQ(max->m_data, -1);
}

TEST_F(ListCpp_test, FindMax_Mixed)
{
    List<int> list({1,-4,3});
    auto max = list.FindMax();
    ASSERT_EQ(max->m_data, 3);
}

TEST_F(ListCpp_test, FindMax_First)
{
    List<int> list({55,-4,3});
    auto max = list.FindMax();
    ASSERT_EQ(max->m_data, 55);
}

TEST_F(ListCpp_test, FindMax_Second)
{
    List<int> list({1,-4,55});
    auto max = list.FindMax();
    ASSERT_EQ(max->m_data, 55);
}

TEST_F(ListCpp_test, Reverse_Empty)
{
    List<int> list;
    list.Reverse();
    ASSERT_EQ(list.Begin(), nullptr);
}

TEST_F(ListCpp_test, Reverse)
{
    List<int> list({0,-1,2,-1});
    list.Reverse();
    ASSERT_TRUE(Equal({-1,2,-1,0}, list));
}

TEST_F(ListCpp_test, SortSelect_Empty)
{
    List<int> list;
    ASSERT_EQ(list.Begin(), nullptr);
    list.SortSelect();
    ASSERT_EQ(list.Begin(), nullptr);
}

TEST_F(ListCpp_test, SortSelect_Once)
{
    List<int> list({1});
    list.SortSelect();
    ASSERT_TRUE(Equal({1}, list));
}

TEST_F(ListCpp_test, SortSelect_1)
{
    List<int> list({4,3,2,1});
    list.SortSelect();
    ASSERT_TRUE(Equal({1,2,3,4}, list));
}

TEST_F(ListCpp_test, SortSelect_2)
{
    List<int> list({0,-1,2,-1});
    list.SortSelect();
    ASSERT_TRUE(Equal({-1,-1,0,2}, list));
}

TEST_F(ListCpp_test, SortInsertable_Empty)
{
    List<int> list;
    ASSERT_EQ(list.Begin(), nullptr);
    list.SortInsertable();
    ASSERT_EQ(list.Begin(), nullptr);
}

TEST_F(ListCpp_test, SortInsertable_Once)
{
    List<int> list({1});
    list.SortInsertable();
    ASSERT_TRUE(Equal({1}, list));
}

TEST_F(ListCpp_test, SortInsertable_1)
{
    List<int> list({4,3,2,1});
    list.SortInsertable();
    ASSERT_TRUE(Equal({1,2,3,4}, list));
}

TEST_F(ListCpp_test, SortInsertable_2)
{
    List<int> list({0,-1,2,-1});
    list.SortInsertable();
    ASSERT_TRUE(Equal({-1,-1,0,2}, list));
}

TEST_F(ListCpp_test, IsCycle_Empty)
{
    List<int> list;
    ASSERT_FALSE(list.isCycle());
}

TEST_F(ListCpp_test, IsCycle_SimpleNonCycle)
{
    List<int> list({1,2,3});
    ASSERT_FALSE(list.isCycle());
}

TEST_F(ListCpp_test, IsCycle_OneElement)
{
    List<int> list({1});
    list.Begin()->m_next = list.Begin();
    EXPECT_TRUE(list.isCycle());
    list.Begin()->m_next = nullptr;
}

TEST_F(ListCpp_test, IsCycle_Simple)
{
    List<int> list({1,2});
    list.Begin()->m_next->m_next = list.Begin();
    EXPECT_TRUE(list.isCycle());
    list.Begin()->m_next->m_next = nullptr;
}

TEST_F(ListCpp_test, IsCycleR_Empty)
{
    List<int> list;
    ASSERT_FALSE(list.isCycleR());
}

TEST_F(ListCpp_test, IsCycleR_SimpleNonCycle)
{
    List<int> list({1,2,3});
    ASSERT_FALSE(list.isCycleR());
}

TEST_F(ListCpp_test, IsCycleR_OneElement)
{
    List<int> list({1});
    list.Begin()->m_next = list.Begin();
    EXPECT_TRUE(list.isCycleR());
    list.Begin()->m_next = nullptr;
}

TEST_F(ListCpp_test, IsCycleR_Simple)
{
    List<int> list({1,2});
    list.Begin()->m_next->m_next = list.Begin();
    EXPECT_TRUE(list.isCycleR());
    list.Begin()->m_next->m_next = nullptr;
}


TEST_F(ListCpp_test, RemoveCycle_Empty)
{
    List<int> list;
    list.RemoveCycle();
}
TEST_F(ListCpp_test, RemoveCycle_NotCycle)
{
    List<int> list({1,2});
    list.RemoveCycle();
}

TEST_F(ListCpp_test, RemoveCycle_Simple)
{
    List<int> list({1,2});
    list.Begin()->m_next->m_next = list.Begin();
    list.RemoveCycle();
    ASSERT_NE(list.Begin()->m_next->m_next, list.Begin());
}
