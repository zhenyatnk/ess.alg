#include <ess.alg/core/list.hpp>

#include <gtest/gtest.h>

//--------------------------------------------------
using namespace ess_alg::core;

namespace
{
    std::string to_string(const std::vector<int>& lf)
    {
        std::string result("{");
        for(auto i = 0; i < lf.size(); ++i)
        {
            result += std::to_string(lf[i]);
            if(i + 1 < lf.size())
                result += ",";
        }
        result +="}";
        return result;
    }
    
    std::string to_string(const ListControl& list)
    {
        std::string result("{");
        auto current = list.GetBegin()->m_next;
        while(current != list.GetEnd())
        {
            result += std::to_string(current->m_data);
            if(current->m_next != list.GetEnd())
                result += ",";
            current = current->m_next;
        }
        result +="}";
        return result;
    }
    
    bool ListEq(const std::vector<int>& lf, const ListControl& list)
    {
        auto current = list.GetBegin()->m_next;
        for(const auto& element: lf)
        {
            if(current == list.GetEnd() || element != current->m_data)
                return false;
            current = current->m_next;
        }
        return true;
    }
    
    bool ListEqEx(const std::vector<int>& lf, const ListControl& list)
    {
        if(ListEq(lf, list))
            return true;
        std::cout<< to_string(lf) << " not equal " << to_string(list) << std::endl;
        return false;
    }
}

class ListControl_test
    :public ::testing::Test
{
public:
    void SetUp()
    {}
};

TEST_F(ListControl_test, Ctor_Empty)
{
    ListControl list;
    ASSERT_EQ(list.GetBegin()->m_next, list.GetEnd());
}

TEST_F(ListControl_test, Empty)
{
    ListControl list;
    ASSERT_TRUE(list.Empty());
}

TEST_F(ListControl_test, Not_Empty)
{
    ListControl list;
    list.InsertBack(1);
    ASSERT_TRUE(!list.Empty());
}

TEST_F(ListControl_test, InsertBack)
{
    ListControl list;
    list.InsertBack(1);
    ASSERT_TRUE(ListEqEx({1}, list));
}

TEST_F(ListControl_test, InsertFront)
{
    ListControl list;
    list.InsertBack(1);
    list.InsertFront(2);
    ASSERT_TRUE(ListEqEx({2, 1}, list));
}

TEST_F(ListControl_test, Clear)
{
    ListControl list;
    list.InsertBack(1);
    list.InsertBack(2);
    ASSERT_NE(list.GetBegin()->m_next, list.GetEnd());
    list.Clear();
    ASSERT_EQ(list.GetBegin()->m_next, list.GetEnd());
}

TEST_F(ListControl_test, InsertBack_Data)
{
    ListControl list;
    list.InsertBack(new ListData(1));
    ASSERT_TRUE(ListEqEx({1}, list));
}

TEST_F(ListControl_test, Initializer_list_Int)
{
    ListControl list({1,2});
    ASSERT_TRUE(ListEqEx({1, 2}, list));
}

TEST_F(ListControl_test, Initializer_list_Data)
{
    ListControl list({new ListData(1), new ListData(2)});
    ASSERT_TRUE(ListEqEx({1, 2}, list));
}

TEST_F(ListControl_test, SortSelect_Empty)
{
    ListControl list;
    ASSERT_EQ(list.GetBegin()->m_next, list.GetEnd());
    list.SortSelect();
    ASSERT_EQ(list.GetBegin()->m_next, list.GetEnd());
}

TEST_F(ListControl_test, SortSelect_1)
{
    ListControl list({4,3,2,1});
    list.SortSelect();
    ASSERT_TRUE(ListEqEx({1,2,3,4}, list));
}

TEST_F(ListControl_test, SortSelect_2)
{
    ListControl list({0,-1,2,-1});
    list.SortSelect();
    ASSERT_TRUE(ListEqEx({-1,-1,0,2}, list));
}

TEST_F(ListControl_test, SortInsertable_Empty)
{
    ListControl list;
    ASSERT_EQ(list.GetBegin()->m_next, list.GetEnd());
    list.SortInsertable();
    ASSERT_EQ(list.GetBegin()->m_next, list.GetEnd());
}

TEST_F(ListControl_test, SortInsertable_1)
{
    ListControl list({4,3,2,1});
    list.SortInsertable();
    ASSERT_TRUE(ListEqEx({1,2,3,4}, list));
}

TEST_F(ListControl_test, SortInsertable_2)
{
    ListControl list({0,-1,2,-1});
    list.SortInsertable();
    ASSERT_TRUE(ListEqEx({-1,-1,0,2}, list));
}
/* TODO need tests

bool isCycle() const;
bool isCycleR();

void RemoveCycle();
void Reverse();
ListData * FindMax() const; //O(N)

*/
