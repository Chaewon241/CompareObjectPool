#pragma once

#include <windows.h>
#include <iostream>

struct MemoryHeader : public SLIST_ENTRY 
{
    int data;
};

class SListObjectPool
{
public:
    using value_type = MemoryHeader;

private:
    SLIST_HEADER poolHead;

public:
    SListObjectPool() 
    {
        InitializeSListHead(&poolHead);
    }

    ~SListObjectPool() 
    {
        while (QueryDepthSList(&poolHead) > 0) 
        {
            MemoryHeader* entry = (MemoryHeader*)InterlockedPopEntrySList(&poolHead);
            delete entry;
        }
    }

    // 객체를 풀에서 가져옴
    MemoryHeader* acquire() 
    {
        MemoryHeader* entry = (MemoryHeader*)InterlockedPopEntrySList(&poolHead);
        if (entry == nullptr) 
        {
            // 풀에 여유가 없으면 새로 할당
            entry = new MemoryHeader();
        }
        return entry;
    }

    // 객체를 풀에 반환
    void release(MemoryHeader* entry) 
    {
        InterlockedPushEntrySList(&poolHead, entry);
    }
};