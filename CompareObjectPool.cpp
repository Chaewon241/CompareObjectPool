#include <iostream>
#include <chrono>

#include "VectorObjectPool.h"
#include "SListObjectPool.h"

template <typename Pool>
void testObjectPool(Pool& pool, size_t iterations) 
{
    std::vector<typename Pool::value_type*> objects;

    auto allocStart = std::chrono::high_resolution_clock::now();

    // 객체를 풀에서 할당
    for (size_t i = 0; i < iterations; ++i) 
    {
        objects.push_back(pool.acquire());
    }

    auto allocEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> allocDuration = allocEnd - allocStart;
    std::cout << "할당 시간: " << allocDuration.count() << " 초" << std::endl;

    auto releaseStart = std::chrono::high_resolution_clock::now();

    // 객체를 풀에 반환
    for (auto obj : objects) 
    {
        pool.release(obj);
    }

    auto releaseEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> releaseDuration = releaseEnd - releaseStart;
    std::cout << "반환 시간: " << releaseDuration.count() << " 초" << std::endl;
}

int main() 
{
    const size_t poolSize = 10000;
    const size_t iterations = 1000000;

    // Vector 풀 성능 테스트
    VectorObjectPool<int> vectorPool(poolSize);
    std::cout << "Vector Object Pool: " << std::endl;
    testObjectPool(vectorPool, iterations);
    std::cout << std::endl;
    // SList 풀 성능 테스트
    SListObjectPool slistPool;
    std::cout << "SList Object Pool: " << std::endl;
    testObjectPool(slistPool, iterations);

    return 0;
}