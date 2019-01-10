#include <ess.alg/core/list.hpp>

namespace ess_alg {
namespace core {
  
ListData::ListData()
{}

ListData::ListData(int data)
:m_data(data)
{}
    
////////////////////////////////////////////
ListControl::ListControl()
{
    m_begin = new ListData();
    m_begin->m_next = m_end = new ListData();
}
    
ListControl::ListControl(std::initializer_list<ListData*> initializer)
    :ListControl()
{
    for(const auto& element: initializer)
        InsertBack(element);
}
    
ListControl::ListControl(std::initializer_list<int> initializer)
    :ListControl()
{
    for(const auto& element: initializer)
        InsertBack(element);
}
    
ListControl::ListControl(ListControl && other)
{
    m_begin = other.m_begin;
    m_end = other.m_end;
    
    other.m_begin = new ListData();
    other.m_begin->m_next = other.m_end = new ListData();
}
        
ListControl& ListControl::operator=(ListControl&& other)
{
    if(this != &other)
    {
        this->FreeMemory();
        
        m_begin = other.m_begin;
        m_end = other.m_end;
        
        other.m_begin = new ListData();
        other.m_begin->m_next = other.m_end = new ListData();
    }
    return *this;
}
    
ListControl::~ListControl()
{
    FreeMemory();
}
    
void ListControl::InsertBack(int a)
{
    InsertBack(new ListData(a));
}
        
void ListControl::InsertBack(ListData* data)
{
    auto current = m_begin;
    while(current->m_next != m_end)
        current = current->m_next;
    
    data->m_next = current->m_next;
    current->m_next = data;
}

void ListControl::InsertFront(int a)
{
    InsertFront(new ListData(a));
}

void ListControl::InsertFront(ListData* data)
{
    data->m_next = m_begin->m_next;
    m_begin->m_next = data;
}

ListData * ListControl::GetBegin()
{
    return m_begin;
}

ListData * ListControl::GetEnd()
{
    return m_end;
}
    
const ListData* ListControl::GetBegin() const
{
    return m_begin;
}

const ListData* ListControl::GetEnd() const
{
    return m_end;
}

void ListControl::SortSelect()//O(n^2)
{
    ListControl resultList;
    
    if(m_begin->m_next == m_end)
        return;
    
    while(m_begin->m_next != m_end)
    {
        auto beforMaxElement = GetBeforeMaxElement(m_begin, m_end);
        if(!!beforMaxElement)//throw
        {
            auto maxElement = beforMaxElement->m_next;
            
            beforMaxElement->m_next = maxElement->m_next;
            maxElement->m_next = nullptr;
            
            resultList.InsertFront(maxElement);
        }
    }
    *this = std::move(resultList);
}

void ListControl::SortInsertable()//O(n^2)
{
    ListControl resultList;
    
    if(m_begin->m_next == m_end)
        return;
    
    while(m_begin->m_next != m_end)
    {
        auto insertable = m_begin->m_next;
        m_begin->m_next = insertable->m_next;
        
        auto currentRes = resultList.GetBegin();
        while(currentRes->m_next != resultList.GetEnd() && currentRes->m_next->m_data < insertable->m_data)
            currentRes = currentRes->m_next;
        
        insertable->m_next = currentRes->m_next;
        currentRes->m_next = insertable;
    }
    *this = std::move(resultList);
}


void ListControl::Clear()
{
    while(m_begin->m_next != m_end)
    {
        auto delete_current = m_begin->m_next;
        m_begin->m_next = delete_current->m_next;
        delete delete_current;
    }
}

bool ListControl::Empty() const
{
    return m_begin->m_next == m_end;
}

bool ListControl::isCycle() const
{
    if(Empty())
        return false;
    
    auto current = m_begin->m_next;
    const auto visited = !current->m_visited;
    while(current != m_end)
    {
        if(current->m_visited == visited)
            return true;
        current->m_visited = visited;
        current = current->m_next;
    }
    return false;
}

bool ListControl::isCycleR()
{
    if(Empty())
        return false;
    
    auto reverse = Reverse(m_begin);
    Reverse(reverse);
    return (m_begin == reverse);
}

void ListControl::RemoveCycle()
{
    if(Empty())
        return ;
    
    auto current = m_begin;
    const auto visited = !current->m_next->m_visited;
    while(current->m_next != m_end)
    {
        if(current->m_next->m_visited == visited)
            break;
        current->m_next->m_visited = visited;
        current = current->m_next;
    }
    if(current->m_next != m_end)
        current->m_next = m_end;
}

void ListControl::Reverse()
{
    m_begin = Reverse(m_begin);
}

ListData * ListControl::FindMax() const //O(N)
{
    ListData *maxElement = m_begin;
    auto current = m_begin->m_next;
    while(current != m_end)
    {
        if(maxElement->m_data < current->m_data)
            maxElement = current;
        current = current->m_next;
    }
    return maxElement;
}

ListData* ListControl::Reverse(ListData *begin)
{
    ListData* prev = nullptr;
    ListData* current = begin;
    while(current != nullptr)
    {
        auto next = current->m_next;
        current->m_next = prev;
        
        prev = current;
        current = next;
    }
    return prev;
}

ListData* ListControl::GetBeforeMaxElement(ListData* begin, ListData* end) const
{
    if(begin->m_next == end)
        return nullptr;
    
    auto current = begin;
    auto beforMaxElement = current;
    while(current->m_next != end)
    {
        if(beforMaxElement->m_next->m_data < current->m_next->m_data)
            beforMaxElement = current;
        current = current->m_next;
    }
    return beforMaxElement;
}


void ListControl::FreeMemory()
{
    Clear();
    delete m_begin;
    delete m_end;
}

}
}
