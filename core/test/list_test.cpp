#include <ess.alg/core/list.hpp>

#include <gtest/gtest.h>

//--------------------------------------------------
using namespace ess_alg::core;

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
    
    std::ostream& operator<< (std::ostream &out, const ListControl& obj)
    {
        out << "{";
        auto current = obj.GetBegin();
        while(current != obj.GetEnd())
        {
            out << current->m_data;
            if(current->m_next != obj.GetEnd())
                out << ",";
            current = current->m_next;
        }
        out << "}";
        return out;
    }
    
    bool Equal(const std::vector<int>& left, const ListControl& right)
    {
        auto current = right.GetBegin();
        for(const auto& element: left)
        {
            if(current == right.GetEnd() || element != current->m_data)
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
    ASSERT_EQ(list.GetBegin(), list.GetEnd());
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
    ASSERT_TRUE(Equal({1}, list));
}

TEST_F(ListControl_test, InsertFront)
{
    ListControl list;
    list.InsertBack(1);
    list.InsertFront(2);
    ASSERT_TRUE(Equal({2, 1}, list));
}

TEST_F(ListControl_test, Clear)
{
    ListControl list;
    list.InsertBack(1);
    list.InsertBack(2);
    ASSERT_NE(list.GetBegin(), list.GetEnd());
    list.Clear();
    ASSERT_EQ(list.GetBegin(), list.GetEnd());
}

TEST_F(ListControl_test, InsertBack_Data)
{
    ListControl list;
    list.InsertBack(new ListData(1));
    ASSERT_TRUE(Equal({1}, list));
}

TEST_F(ListControl_test, Initializer_list_Int)
{
    ListControl list({1,2});
    ASSERT_TRUE(Equal({1, 2}, list));
}

TEST_F(ListControl_test, Initializer_list_Data)
{
    ListControl list({new ListData(1), new ListData(2)});
    ASSERT_TRUE(Equal({1, 2}, list));
}

TEST_F(ListControl_test, SortSelect_Empty)
{
    ListControl list;
    ASSERT_EQ(list.GetBegin(), list.GetEnd());
    list.SortSelect();
    ASSERT_EQ(list.GetBegin(), list.GetEnd());
}

TEST_F(ListControl_test, SortSelect_1)
{
    ListControl list({4,3,2,1});
    list.SortSelect();
    ASSERT_TRUE(Equal({1,2,3,4}, list));
}

TEST_F(ListControl_test, SortSelect_2)
{
    ListControl list({0,-1,2,-1});
    list.SortSelect();
    ASSERT_TRUE(Equal({-1,-1,0,2}, list));
}

TEST_F(ListControl_test, SortInsertable_Empty)
{
    ListControl list;
    ASSERT_EQ(list.GetBegin(), list.GetEnd());
    list.SortInsertable();
    ASSERT_EQ(list.GetBegin(), list.GetEnd());
}

TEST_F(ListControl_test, SortInsertable_1)
{
    ListControl list({4,3,2,1});
    list.SortInsertable();
    ASSERT_TRUE(Equal({1,2,3,4}, list));
}

TEST_F(ListControl_test, SortInsertable_2)
{
    ListControl list({0,-1,2,-1});
    list.SortInsertable();
    ASSERT_TRUE(Equal({-1,-1,0,2}, list));
}

TEST_F(ListControl_test, Reverse_Empty)
{
    ListControl list;
    list.Reverse();
    ASSERT_EQ(list.GetBegin(), list.GetEnd());
}

TEST_F(ListControl_test, Reverse)
{
    ListControl list({0,-1,2,-1});
    list.Reverse();
    ASSERT_TRUE(Equal({-1,2,-1,0}, list));
}

TEST_F(ListControl_test, FindMax_Empty)
{
    ListControl list;
    auto max = list.FindMax();
    ASSERT_EQ(max, list.GetEnd());
}

TEST_F(ListControl_test, FindMax_One)
{
    ListControl list({1});
    auto max = list.FindMax();
    ASSERT_EQ(max->m_data, 1);
}

TEST_F(ListControl_test, FindMax_Some)
{
    ListControl list({1,4,3});
    auto max = list.FindMax();
    ASSERT_EQ(max->m_data, 4);
}

TEST_F(ListControl_test, FindMax_OnlyNegative)
{
    ListControl list({-1,-4,-3});
    auto max = list.FindMax();
    ASSERT_EQ(max->m_data, -1);
}

TEST_F(ListControl_test, FindMax_Mixed)
{
    ListControl list({1,-4,3});
    auto max = list.FindMax();
    ASSERT_EQ(max->m_data, 3);
}

TEST_F(ListControl_test, FindMax_First)
{
    ListControl list({55,-4,3});
    auto max = list.FindMax();
    ASSERT_EQ(max->m_data, 55);
}

TEST_F(ListControl_test, FindMax_Second)
{
    ListControl list({1,-4,55});
    auto max = list.FindMax();
    ASSERT_EQ(max->m_data, 55);
}

/* TODO need tests

bool isCycle() const;
bool isCycleR();

void RemoveCycle();
ListData * FindMax() const; //O(N)

*/
