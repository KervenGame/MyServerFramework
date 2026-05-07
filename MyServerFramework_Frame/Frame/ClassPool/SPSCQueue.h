#pragma once

#include "UsingSTD.h"

// SPSC 无锁环形队列（用于对象回收）
// 线程1（创建者）从队列取出空闲对象
// 线程2（销毁者）将对象推入队列等待复用
template<typename T, int Capacity>
class SPSCQueue
{
    static_assert((Capacity& (Capacity - 1)) == 0, "Capacity must be power of 2");
    // 避免 false sharing
    struct alignas(64) AlignedAtomic 
    {
        atomic<size_t> value{ 0 };
    };
    AlignedAtomic mHead;  // 线程1读（消费）
    AlignedAtomic mTail;  // 线程2写（生产）
    Array<Capacity, T*> mSlots{};
    static constexpr size_t MASK = Capacity - 1;
public:
    // 线程2调用：将销毁的对象归还到队列
    bool push(T* obj) noexcept 
    {
        size_t tail = mTail.value.load(memory_order_relaxed);
        size_t next = (tail + 1) & MASK;
        if (next == mHead.value.load(memory_order_acquire))
        {
            return false;  // 队列满
        }
        mSlots[(int)tail] = obj;
        mTail.value.store(next, memory_order_release);
        return true;
    }
    // 线程1调用：取出可复用的对象
    T* pop() noexcept 
    {
        size_t head = mHead.value.load(memory_order_relaxed);
        if (head == mTail.value.load(memory_order_acquire))
        {
            return nullptr;  // 队列空
        }
        T* obj = mSlots[(int)head];
        mHead.value.store((head + 1) & MASK, memory_order_release);
        return obj;
    }
    bool empty() const noexcept 
    {
        return mHead.value.load(memory_order_acquire) == mTail.value.load(memory_order_acquire);
    }
};