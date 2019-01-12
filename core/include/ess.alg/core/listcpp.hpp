#pragma once

#include <ess.alg/core/BaseExceptions.hpp>
#include <ess.alg/core/export.hpp>

#include <functional>
#include <ostream>

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
        while(current != obj.m_end)
        {
            out << current->m_data;
            if(current->m_next != obj.m_end)
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
        TypeValue m_data = {};
        ListData* m_next = nullptr;
    };
    
public:
    List()
    {
        m_begin = m_end = new ListData();
    }
    
    List(std::initializer_list<ListData*> initializer)
        :List()
    {
        for(const auto& element: initializer)
            *this+=element;
    }
    
    List(std::initializer_list<TypeValue> initializer)
        :List()
    {
        for(const auto& element: initializer)
            *this+=element;
    }
    
    List(List && other)
    {
        m_begin = other.m_begin;
        m_end = other.m_end;
        
        other.m_begin = other.m_end = new ListData();
    }
    
    List& operator=(List&& other)
    {
        Clear();
        delete m_end;
        
        m_begin = other.m_begin;
        m_end = other.m_end;
        
        other.m_begin = other.m_end = new ListData();
        
        return *this;
    }
    
    ~List()
    {
        Clear();
        delete m_end;
    }
    
    void Clear()
    {
        while(m_begin != m_end)
        {
            auto remove = m_begin;
            m_begin = m_begin->m_next;
            delete remove;
        }
    }
    
    bool IsEmpty() const
    {
        return m_begin == m_end;
    }
    
    ListData* Begin()
    {
        return m_begin;
    }
    
    const ListData* Begin() const
    {
        return m_begin;
    }
    
    const ListData* End() const
    {
        return m_end;
    }
    
    List<TypeValue>& operator+=(const TypeValue& rhs)
    {
        *this += new ListData(rhs);
        return *this;
    }
    
    List<TypeValue>& operator+=(ListData* rhs)
    {
        auto last = GetLast();
        if (!last)
        {
            rhs->m_next = m_end;
            m_begin = rhs;
        }
        else
        {
            last->m_next = rhs;
            rhs->m_next = m_end;
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
        m_begin = rhs;
    }
    
    ListData* FindMax() const //O(n)
    {
        auto current = m_begin;
        auto max = m_end;
        while(current != m_end)
        {
            if(max == m_end || max->m_data < current->m_data)
                max = current;
            current = current->m_next;
        }
        return max;
    }
    
    void Reverse()
    {
        auto current = m_begin;
        auto prev = m_end;
        while(current != m_end)
        {
            auto next = current->m_next;
            current->m_next = prev;
            prev = current;
            current = next;
        }
        m_begin = prev;
    }
    
    void SortSelect()//O(n*n)
    {
        List<TypeValue> result;
        while(m_begin != m_end)
        {
            auto beforeMax = FindBeforeMax(m_begin, m_end);
            assert(beforeMax != m_end);
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
        while(m_begin != m_end)
        {
            auto insertable = m_begin;
            m_begin = m_begin->m_next;
            
            ListData* beforeFind = nullptr;
            auto current = result.Begin();
            while(current != result.End() && current->m_data < insertable->m_data)
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
    
protected:
    ListData* GetLast()
    {
        auto current = m_begin;
        ListData* prev = nullptr;
        while(current != m_end)
        {
            prev = current;
            current = current->m_next;
        }
        return prev;
    }
    
    ListData* FindBeforeMax(ListData* begin, ListData* end) const //O(n)
    {
        auto current = begin;
        auto max = end;
        auto beforeMax = end;
        ListData* prev = nullptr;
        while(current != end)
        {
            if(max == end || max->m_data < current->m_data)
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
    ListData* m_begin;
    ListData* m_end;
};
    
}    
}
}
