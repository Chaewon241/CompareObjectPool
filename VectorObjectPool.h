#pragma once

#include <vector>

template <typename T>
class VectorObjectPool 
{
public:
    using value_type = T;

private:
    std::vector<T*> pool;

public:
    VectorObjectPool(size_t initialSize)
    {
        pool.reserve(initialSize);
        for (size_t i = 0; i < initialSize; ++i) 
        {
            pool.push_back(new T());
        }
    }

    ~VectorObjectPool()
    {
        for (auto obj : pool) 
        {
            delete obj;
        }
    }

    T* acquire() 
    {
        if (pool.empty()) 
        {
            return new T();
        }
        else 
        {
            T* obj = pool.back();
            pool.pop_back();
            return obj;
        }
    }

    void release(T* obj) 
    {
        pool.push_back(obj);
    }
};