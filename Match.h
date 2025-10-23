#pragma once
#include <vector>
#include <type_traits>
#include <tuple>
#include <optional>
#include "Util.h"

struct Match_Break_Type {};
extern Match_Break_Type Match_Break;

namespace Match_Detail
{
    template<typename... Args>
    struct Case_Struct
    {
        Case_Struct(std::tuple<Args...>& values) : Values(values) {}

        template<typename F>
        Case_Struct& Case(Args... values, F block)
        {
            if (Match_Broken == false)
            {
                std::tuple<Args...> case_Values(values...);

                if (case_Values == Values)
                {
                    // Getting the address of the return type is always safe here
                    // because it is just being used to set the type of block_val.
                    auto block_val = Func_Rem_Void(block);
                    if (typeid(block_val) == typeid(Match_Break_Type))
                    {
                        Match_Broken = true;
                    }

                    Found_Value = true;
                }
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
        std::tuple<Args...> Values;
        bool Found_Value = false;
        bool Match_Broken = false;
    };
}

template<typename... Args>
auto Match(Args... args)
{
    std::tuple<Args...> values(args...);

    return Match_Detail::Case_Struct<Args...>(values);
}

/*
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
*/