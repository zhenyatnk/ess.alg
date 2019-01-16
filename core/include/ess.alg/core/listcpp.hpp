#pragma once

#include <ess.alg/core/BaseExceptions.hpp>
#include <ess.alg/core/export.hpp>

#include <functional>
#include <ostream>
#include <cassert>

namespace ess_alg {
namespace core {
namespace cppstyle{

template<class TypeValue>
class List final
{
public:
    friend std::ostream& operator<< (std::ostream &out, const List<TypeValue>& obj)
    {
        out << "{";
        auto current = obj.m_begin;
        while(!!current)
        {
            out << current->m_data;
            if(!!current->m_next)
                out << ",";
            current = current->m_next;
        }
        out << "}";
        return out;
    }
    
    class ListData final
    {
    private:
        friend class List<TypeValue>;
        
    private:
        ListData() = default;
        ListData(const TypeValue& value)
            :m_data(value)
        {}
        
    public:
        bool m_visited = false;
        TypeValue m_data = {};
        ListData* m_next = nullptr;
    };
    
public:
    List() = default;
    
    List(std::initializer_list<ListData*> initializer)
    {
        for(const auto& element: initializer)
            *this+=element;
    }
    
    List(std::initializer_list<TypeValue> initializer)
    {
        for(const auto& element: initializer)
            *this+=element;
    }
    
    List(List && other)
    {
        m_begin = other.m_begin;
        m_last = other.m_last;
        
        other.m_begin = other.m_last = nullptr;
    }
    
    List& operator=(List&& other)
    {
        Clear();
        
        m_begin = other.m_begin;
        m_last = other.m_last;
        
        other.m_begin = other.m_last = nullptr;
        
        return *this;
    }
    
    ~List()
    {
        Clear();
    }
    
    void Clear()
    {
        while(!!m_begin)
        {
            auto remove = m_begin;
            m_begin = m_begin->m_next;
            delete remove;
        }
    }
    
    bool IsEmpty() const
    {
        return m_begin == nullptr;
    }
    
    ListData* Begin()
    {
        return m_begin;
    }
    
    const ListData* Begin() const
    {
        return m_begin;
    }
    
    ListData* Last()
    {
        return m_last;
    }
    
    const ListData* Last() const
    {
        return m_last;
    }
    
    List<TypeValue>& operator+=(const TypeValue& rhs)
    {
        *this += new ListData(rhs);
        return *this;
    }
    
    List<TypeValue>& operator+=(ListData* rhs)
    {
        if(!m_begin)
            m_begin = m_last = rhs;
        else
        {
            m_last->m_next = rhs;
            m_last = rhs;
        }
        return *this;
    }
    
    void InsertFront(const TypeValue& rhs)
    {
        InsertFront(new ListData(rhs));
    }
    
    void InsertFront(ListData* rhs)
    {
        rhs->m_next = m_begin;
        if(!m_begin)
            m_begin = m_last = rhs;
        else
            m_begin = rhs;
    }
    
    ListData* FindMax() const //O(n)
    {
        auto current = m_begin;
        auto max = m_begin;
        while(!!current)
        {
            if(max->m_data < current->m_data)
                max = current;
            current = current->m_next;
        }
        return max;
    }
    
    void Reverse()
    {
        auto current = m_begin;
        ListData* prev = nullptr;
        while(!!current)
        {
            auto next = current->m_next;
            current->m_next = prev;
            prev = current;
            current = next;
        }
        m_last = m_begin;
        m_begin = prev;
    }
    
    void SortSelect()//O(n*n)
    {
        List<TypeValue> result;
        while(!!m_begin)
        {
            auto beforeMax = FindBeforeMax(m_begin, m_last);
            if(!beforeMax) //first it is max
            {
                auto max = m_begin;
                m_begin = m_begin->m_next;
                result.InsertFront(max);
            }
            else
            {
                auto max = beforeMax->m_next;
                beforeMax->m_next = max->m_next;
                result.InsertFront(max);
            }
        }
        *this = std::move(result);
    }
    
    void SortInsertable()//O(n*n)
    {
        List<TypeValue> result;
        while(!!m_begin)
        {
            auto insertable = m_begin;
            m_begin = m_begin->m_next;
            
            ListData* beforeFind = nullptr;
            auto current = result.Begin();
            while(!!current && current->m_data < insertable->m_data)
            {
                beforeFind = current;
                current = current->m_next;
            }
            if(!beforeFind)
                result.InsertFront(insertable);
            else
            {
                insertable->m_next = beforeFind->m_next;
                beforeFind->m_next = insertable;
            }
        }
        *this = std::move(result);
    }
    
    bool isCycle() const
    {
        if(IsEmpty())
            return false;
        
        auto current = m_begin;
        const auto visited = !current->m_visited;
        while(!!current)
        {
            if(current->m_visited == visited)
                return true;
            current->m_visited = visited;
            current = current->m_next;
        }
        return false;
    }
    
    bool isCycleR()
    {
        if(IsEmpty())
            return false;
        
        Reverse();
        auto rbegin = m_begin;
        Reverse();
        return rbegin == m_begin;
    }
    
    void RemoveCycle()
    {
        if(IsEmpty())
            return;
        
        ListData* prev = nullptr;
        auto current = m_begin;
        const auto visited = !current->m_visited;
        while(!!current)
        {
            if(current->m_visited == visited)
            {
                m_last = prev;
                prev->m_next = nullptr;
                break;
            }
            current->m_visited = visited;
            prev = current;
            current = current->m_next;
        }
    }
    
    bool IsSorted() //O(n)
    {
        auto current = m_begin;
        while(!!current && !!current->m_next)
        {
            if(current->m_data > current->m_next->m_data)
                return false;
            current = current->m_next;
        }
        return true;
    }
    
protected:
    ListData* FindBeforeMax(ListData* begin, ListData* last) const //O(n)
    {
        auto current = begin;
        auto max = begin;
        ListData* beforeMax = nullptr;
        ListData* prev = nullptr;
        while(!!current)
        {
            if(max->m_data < current->m_data)
            {
                beforeMax = prev;
                max = current;
            }
            prev = current;
            current = current->m_next;
        }
        return beforeMax;
    }
    
private:
    ListData* m_begin = nullptr;
    ListData* m_last = nullptr;
};
    
}    
}
}
