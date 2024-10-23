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

    // ��ü�� Ǯ���� ������
    MemoryHeader* acquire() 
    {
        MemoryHeader* entry = (MemoryHeader*)InterlockedPopEntrySList(&poolHead);
        if (entry == nullptr) 
        {
            // Ǯ�� ������ ������ ���� �Ҵ�
            entry = new MemoryHeader();
        }
        return entry;
    }

    // ��ü�� Ǯ�� ��ȯ
    void release(MemoryHeader* entry) 
    {
        InterlockedPushEntrySList(&poolHead, entry);
    }
};