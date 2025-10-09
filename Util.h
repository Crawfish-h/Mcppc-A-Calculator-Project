#pragma once
#include <array>

enum Comp { And, Or };

template<Comp comp = And, typename T, typename... Args>
bool Eq_Multi(T&& value, Args&&... args)
{
    if (comp == And)
        return ((value == args) && ...);
    else
        return ((value == args) || ...);
}

// Returns true if any trailing zeros were removed.
bool Remove_Trailing_Zeros(std::string& number_String)
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

bool Is_Digit(const std::string& string)
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