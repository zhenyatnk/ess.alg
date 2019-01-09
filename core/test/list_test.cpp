#include <ess.alg/core/list.hpp>

#include <gtest/gtest.h>

//--------------------------------------------------
using namespace ess_alg::core;

namespace
{
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
    ASSERT_TRUE(ListEq({1}, list));
}

TEST_F(ListControl_test, InsertFront)
{
    ListControl list;
    list.InsertBack(1);
    list.InsertFront(2);
    ASSERT_TRUE(ListEq({2, 1}, list));
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

/* TODO need tests
void InsertBack(int a);
void InsertBack(ListData* data);
void InsertFront(int a);
void InsertFront(ListData* data);

void SortSelect();//O(n^2)
void SortInsertable();//O(n^2)

bool isCycle() const;
bool isCycleR();

void RemoveCycle();
void Reverse();
ListData * FindMax() const; //O(N)

*/
