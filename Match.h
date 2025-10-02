#pragma once
#include <vector>
#include <type_traits>
#include <functional>

/*template<typename T>
class Case
{
public:
    Case(T& value, std::function<void()> block)
    {
        Value = &value;
        Block = block;
    }
    
    T* Value;
    std::function<void()> Block;
};*/



template<typename T>
class Match
{
    Case(T& value)
    {
        
    }

    T Match(T& value)
    {
        std::vector<T> case_Values;
        for (auto acase : { cases... })
        {
            if (*acase.Value == Match_Value)
            {
                acase.Block();
                return *acase.Value;
            }
        }

        return value;
    }
};