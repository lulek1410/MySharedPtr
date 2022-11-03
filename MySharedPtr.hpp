#pragma once

#include <cstdint>
#include <iostream>

namespace myptr
{
class SharedPtrCounter
{
public:
    SharedPtrCounter() : ptrCount(0){};
    SharedPtrCounter(const SharedPtrCounter&) = delete;
    SharedPtrCounter& operator=(const SharedPtrCounter&) = delete;

    void operator++() { ptrCount++; };
    void operator--() { ptrCount--; };
    uint16_t get() const { return ptrCount; };
    void reset() { ptrCount = 0; };

private:
    uint16_t ptrCount{};
};

template <typename T>
class MySharedPtr
{
public:
    explicit MySharedPtr(T* ptr = nullptr)
    {
        sharedPtrCounter = new SharedPtrCounter();
        storedValue = ptr;
        ++(*sharedPtrCounter);
    };

    MySharedPtr(MySharedPtr<T>& mySharedPtr)
    {
        sharedPtrCounter = mySharedPtr.sharedPtrCounter;
        storedValue = mySharedPtr.storedValue;
        ++(*sharedPtrCounter);
    };

    ~MySharedPtr()
    {
        printf("Destructor \n");
        --(*sharedPtrCounter);
        if (sharedPtrCounter->get() == 0)
        {
            delete sharedPtrCounter;
            delete storedValue;
        }
    };

    T* operator->() const noexcept { return storedValue; };
    T& operator*() const noexcept { return *storedValue; };
    T* get() const noexcept { return storedValue; }
    uint16_t use_count() const { return sharedPtrCounter->get(); }
    explicit operator bool() const noexcept { return get() != nullptr; }

private:
    SharedPtrCounter* sharedPtrCounter;
    T* storedValue;
};
} // namespace myptr
