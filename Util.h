#pragma once
#include <concepts>
#include <type_traits>
#include <optional>
#include <any>
#include <memory>
#include <vector>

enum Comp { And, Or };

template<Comp comp = And, typename T, typename ...Args>
inline bool Eq_Multi(T&& value, Args&&... args)
{
    if (comp == And)
        return ((value == args) && ...);
    else
        return ((value == args) || ...);
}

// Returns true if any trailing zeros were removed.
inline ssize_t Remove_Trailing_Zeros(std::string& number_String)
{
    size_t num_Removed = 0;
    for (ssize_t i = number_String.size() - 1; i > -1; i--)
    {
        if (number_String[i] == '0')
        {
            number_String.erase(number_String.begin() + i);
            num_Removed++;
        }else
        {
            break;
        }
    }

    if (number_String[number_String.size() - 1] == '.')
    {
        number_String.erase(number_String.begin() + number_String.size() - 1);
    }

    return num_Removed;
}

// If 'string' is a function convert it do a digit and then return true.
// if 'string' is a digit return true. 
inline bool To_Digit(const std::string& string)
{
    for (size_t i = 0; i < string.size(); i++)
    {
        if (i == 0 && string[i] == '-')
            continue;

        if (isdigit(string[i]) == false && string[i] != '.')
        {
            return false;
        }
    }

    return true;
}

template<typename C, typename T>
inline size_t Remove_All_Of(C& container, T value)
{
    size_t num_Removed = 0;
    size_t value_Num = 0;

    for (auto& element : container)
    {
        if (element == value)
        {
            value_Num++;
        }
    }
    
    while (value_Num > 0)
    {
        for (size_t i = 0; i < container.size(); i++)
        {
            if (container[i] == value)
            {
                container.erase(container.begin() + i);
                value_Num--;
                num_Removed++;
            }
        }
    }

    return num_Removed;
}

template <typename F>
concept Callable = std::is_invocable<F>::value;

template <typename F, typename R, typename ...Args>
concept Callable_Ts = std::is_invocable_r_v<R, F, Args...>;

template<typename F>
concept Returns_Void = requires(F f)
{
    { f() } -> std::convertible_to<void>;
};

template<Returns_Void F>
void* Func_Rem_Void(F block)
{
    block();
    return nullptr;
}

template<typename F>
auto Func_Rem_Void(F block)
{
    return block();
}

template<typename T>
concept Iterable = requires(T t)
{
    requires (!std::is_same<T, std::string>::value);
    t.begin();
    t.end();
} && !std::same_as<std::remove_cvref_t<T>, std::string>;

