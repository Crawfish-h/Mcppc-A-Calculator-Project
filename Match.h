#pragma once
#include <vector>
#include <type_traits>
#include <functional>
#include <tuple>
#include <optional>
#include <iostream>

template<typename... Args>
class Match
{
public:
    Match(Args... values)
    {
        Found_Value = false;
        Values = { values... };
    }

    template<typename F>
    Match<Args...>& Case(Args... values, F block)
    {
        std::tuple<Args...> case_Values(values...);

        if (case_Values == Values)
        {
            block();
            Values = case_Values;
            Found_Value = true;
        }

        return *this;
    }

    template<typename F>
    std::optional<std::tuple<Args...>> Default(F block = [](){})
    {
        if (Found_Value == false)
        {
            block();
            return {};
        }
        
        return Values;
    }

private:
    bool Found_Value;
    std::tuple<Args...> Values;
};