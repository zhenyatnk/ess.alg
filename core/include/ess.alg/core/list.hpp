#pragma once

#include <ess.alg/core/BaseExceptions.hpp>
#include <ess.alg/core/export.hpp>

#include <functional>
#include <ostream>

namespace ess_alg {
namespace core {

struct ESS_ALG_CORE_EXPORT ListData final
{
    ListData();
    explicit ListData(int data);
    
    int m_data = 0;
    bool m_visited = false;
    ListData *m_next = nullptr;
};

class ESS_ALG_CORE_EXPORT ListControl final
{
public:
    ListControl();
    ListControl(std::initializer_list<ListData*> initializer);
    ListControl(std::initializer_list<int> initializer);
    ListControl(ListControl && other);
    ListControl& operator=(ListControl&& other);
    
    ~ListControl();
    
    void InsertBack(int a);
    void InsertBack(ListData* data);
    void InsertFront(int a);
    void InsertFront(ListData* data);
    
    ListData* GetBegin();
    ListData* GetEnd();
    
    const ListData* GetBegin() const;
    const ListData* GetEnd() const;
    
    void SortSelect();//O(n^2)
    void SortInsertable();//O(n^2)
    
    void Clear();
    
    bool Empty() const;
    bool isCycle() const;
    bool isCycleR();
    
    void RemoveCycle();
    void Reverse();
    ListData * FindMax() const; //O(N)
    
protected:
    std::pair<ListData*,ListData*> Reverse(ListData *begin, ListData *end);
    ListData* GetBeforeMaxElement(ListData* begin, ListData* end) const;
    
private:
    void FreeMemory();
    
private:
    ListData *m_begin = nullptr;
    ListData *m_end = nullptr;
};  
    
}
}
