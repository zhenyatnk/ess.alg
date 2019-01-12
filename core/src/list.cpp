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
    m_begin = m_end = new ListData();
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
    
    other.m_begin = other.m_end = new ListData();
}
        
ListControl& ListControl::operator=(ListControl&& other)
{
    if(this != &other)
    {
        this->FreeMemory();
        
        m_begin = other.m_begin;
        m_end = other.m_end;
        
        other.m_begin = other.m_end = new ListData();
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
    if(Empty())
    {
        m_begin = data;
        m_begin->m_next = m_end;
    }
    else
    {
        auto last = m_begin;
        while(last->m_next != m_end)
            last = last->m_next;
        
        last->m_next = data;
        data->m_next = m_end;
    }

}

void ListControl::InsertFront(int a)
{
    InsertFront(new ListData(a));
}

void ListControl::InsertFront(ListData* data)
{
    data->m_next = m_begin;
    m_begin = data;
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
    
    while(m_begin != m_end)
    {
        auto beforMaxElement = GetBeforeMaxElement(m_begin, m_end);
        if(!!beforMaxElement)
        {
            auto maxElement = beforMaxElement->m_next;
            
            beforMaxElement->m_next = maxElement->m_next;
            maxElement->m_next = nullptr;
            
            resultList.InsertFront(maxElement);
        }
        else //max it is begin
        {
            auto maxElement = m_begin;
            m_begin = m_begin->m_next;
            maxElement->m_next = nullptr;
            
            resultList.InsertFront(maxElement);
        }
            
    }
    *this = std::move(resultList);
}

void ListControl::SortInsertable()//O(n^2)
{
    ListControl resultList;
    
    while(m_begin != m_end)
    {
        auto insertable = m_begin;
        m_begin = insertable->m_next;
        
        ListData* prev = nullptr;
        auto currentRes = resultList.GetBegin();
        while(currentRes != resultList.GetEnd() && currentRes->m_data < insertable->m_data)
        {
            prev = currentRes;
            currentRes = currentRes->m_next;
        }
        
        if(!!prev)
        {
            insertable->m_next = prev->m_next;
            prev->m_next = insertable;
        }
        else
            resultList.InsertFront(insertable);
    }
    *this = std::move(resultList);
}


void ListControl::Clear()
{
    while(m_begin != m_end)
    {
        auto delete_current = m_begin;
        m_begin = delete_current->m_next;
        delete delete_current;
    }
}

bool ListControl::Empty() const
{
    return m_begin == m_end;
}

bool ListControl::isCycle() const
{
    if(Empty())
        return false;
    
    auto current = m_begin;
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
    
    auto reverse = Reverse(m_begin, m_end);
    Reverse(reverse.first, reverse.second);
    return (m_begin == reverse.first);
}

void ListControl::RemoveCycle()
{
    if(Empty())
        return ;
    ListData* prev = nullptr;
    auto current = m_begin;
    const auto visited = !current->m_visited;
    while(current != m_end)
    {
        if(current->m_visited == visited)
            break;
        current->m_visited = visited;
        prev = current;
        current = current->m_next;
    }
    if(current != m_end)
        prev->m_next = m_end;
}

void ListControl::Reverse()
{
    auto result = Reverse(m_begin, m_end);
    m_begin = result.first;
    m_end = result.second;
}

ListData * ListControl::FindMax() const //O(N)
{
    ListData *maxElement = m_begin;
    auto current = m_begin;
    while(current != m_end)
    {
        if(maxElement->m_data < current->m_data)
            maxElement = current;
        current = current->m_next;
    }
    return maxElement;
}

std::pair<ListData*,ListData*> ListControl::Reverse(ListData *begin, ListData *end)
{
    if(Empty())
        return std::pair<ListData*,ListData*>(begin, end);
    
    ListData* prev = end;
    ListData* current = begin;
    while(current != end)
    {
        auto next = current->m_next;
        current->m_next = prev;
        
        prev = current;
        current = next;
    }
    return std::pair<ListData*,ListData*>(prev, end);
}

ListData* ListControl::GetBeforeMaxElement(ListData* begin, ListData* end) const
{
    ListData* prev = nullptr;
    ListData* beforMaxElement = nullptr;
    ListData *maxElement = begin;
    auto current = begin;
    while(current != m_end)
    {
        if(maxElement->m_data < current->m_data)
        {
            beforMaxElement = prev;
            maxElement = current;
        }
        prev = current;
        current = current->m_next;
    }
    return beforMaxElement;
}


void ListControl::FreeMemory()
{
    Clear();
    delete m_end;
}

}
}
