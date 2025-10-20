#pragma once
#include <concepts>
#include <type_traits>
#include <optional>
#include <any>
#include <memory>

enum Comp { And, Or };

template<Comp comp = And, typename T, typename ...Args>
bool Eq_Multi(T&& value, Args&&... args)
{
    if (comp == And)
        return ((value == args) && ...);
    else
        return ((value == args) || ...);
}

// Returns true if any trailing zeros were removed.
inline bool Remove_Trailing_Zeros(std::string& number_String)
{
    bool removed_Zero = false;
    for (ssize_t i = number_String.size() - 1; i > -1; i--)
    {
        if (number_String[i] == '0')
        {
            number_String.erase(number_String.begin() + i);
            removed_Zero = true;
        }else
        {
            break;
        }
    }

    return removed_Zero;
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

template<Callable C>
class Logic
{
    C Function_;
    bool Cond_ = false;

    template<class T> 
    struct Implicit_Any
    {
        Implicit_Any(T& t) : Any(t) {}
        std::any Any;
        operator T() const 
        {
            return std::any_cast<T>(Any);
        }
    };

public:
    Logic(bool expr, C function) {}
        //{ Cond_ = expr; Function_ = function; }

    /*template<typename ...Args>
    std::optional<Implicit_Any> If(bool expr, Args... args)
    {
        if (Cond_ != true)
        {
            Cond_ = expr;
        }else
        {
            return Implicit_Any(*Function_(args...));
        }

        return {};
    }*/
};