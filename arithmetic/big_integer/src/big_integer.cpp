#include "../include/big_integer.h"

big_integer &big_integer::trivial_multiplication::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier) const
{
    if (first_multiplier.is_zero())
    {
        return first_multiplier;
    }

    if (second_multiplier.is_zero())
    {
        return first_multiplier = big_integer(first_multiplier.get_allocator());
    }

    if (first_multiplier.is_negate())
    {
        first_multiplier.change_sign();
        first_multiplier *= second_multiplier;
        return first_multiplier.change_sign();
    }

    if (second_multiplier.is_negate())
    {
        first_multiplier *= -second_multiplier;
        return first_multiplier.change_sign();
    }

    auto first_count = first_multiplier.get_digits_count();
    auto second_count = second_multiplier.get_digits_count();

    int mult_shift = sizeof(unsigned int) << 2;
    int mult_mask = (1 << mult_shift) - 1;

    auto result = big_integer(first_multiplier.get_allocator());
    for (auto i = 0; i < first_count; i++)
    {
        auto quotient_A = first_multiplier.get_digit(i) >> mult_shift;
        auto remainder_A = first_multiplier.get_digit(i) & mult_mask;

        for (auto j = 0; j < second_count; j++)
        {
            auto double_k = (i + j) << 1; // cuz we split digits to quotient and remainder
            auto quotient_B = second_multiplier.get_digit(j) >> mult_shift;
            auto remainder_B = second_multiplier.get_digit(j) & mult_mask;

            auto op1 = big_integer(remainder_A * remainder_B, first_multiplier.get_allocator());
            op1 <<= (mult_shift * double_k); // zero cuz two low-radix digit parts
            result += op1;

            auto op2 = big_integer(quotient_A * quotient_B, first_multiplier.get_allocator());
            op2 <<= (mult_shift * (double_k + 2)); // two cuz two high-radix digit parts
            result += op2;

            auto op3 = big_integer(quotient_A * remainder_B, first_multiplier.get_allocator());
            op3 <<= (mult_shift * (double_k + 1)); // one cuz one high-radix digit parts
            result += op3;

            auto op4 = big_integer(remainder_A * quotient_B, first_multiplier.get_allocator());
            op4 <<= (mult_shift * (double_k + 1)); // one cuz one high-radix digit parts
            result += op4;
        }
    }

    first_multiplier.deallocate_with_guard(first_multiplier._other_digits);
    first_multiplier = std::move(result);

    return first_multiplier;

    //throw not_implemented("big_integer &big_integer::trivial_multiplication::multiply(big_integer &, big_integer const &)", "your code should be here...");
}

big_integer &big_integer::Karatsuba_multiplication::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier) const
{
    throw not_implemented("big_integer &big_integer::Karatsuba_multiplication::multiply(big_integer &, big_integer const &)", "your code should be here...");
}

big_integer &big_integer::Schonhage_Strassen_multiplication::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier) const
{
    throw not_implemented("big_integer &big_integer::Schonhage_Strassen_multiplication::multiply(big_integer &, big_integer const &)", "your code should be here...");
}

std::pair<big_integer, big_integer> big_integer::trivial_division::divide_internal(
    big_integer const &dividend,
    big_integer const &divisor,
    allocator* allocator,
    big_integer::multiplication_rule multiplication_rule) const
{
    if (dividend.is_zero())
    {
        return std::make_pair(dividend, big_integer());
    }

    if (divisor.is_zero())
    {
        throw std::invalid_argument("divide by zero");
    }

    auto& left = dividend.is_negate() ? -dividend : dividend;
    auto& right = divisor.is_negate() ? -divisor : divisor;

    auto is_negative = dividend.is_negate() ^ divisor.is_negate();
    auto start_range = big_integer(allocator);
    auto end_range = big_integer(allocator);
    auto potential_result = big_integer(allocator);
    auto result = big_integer(allocator);
    auto carry = big_integer(allocator);

    end_range = left;
    do
    {
        potential_result = (start_range + end_range) >> 1;
        result = multiply(potential_result, right, allocator, multiplication_rule);
        carry = left - result;

        if (carry >= 0 && carry < right)
        {
            if (is_negative)
            {
                potential_result.change_sign();
                carry -= right;
                if (carry.is_negate())
                {
                    carry.change_sign();
                }
            }

            if (divisor.is_negate())
            {
                carry.change_sign();
            }

            return { potential_result, carry };
        }

        if (result > left)
        {
            end_range = potential_result;
        }
        else
        {
            start_range = potential_result;
        }

    } while (potential_result != 0);

    throw std::logic_error("error in divide function!");;
}

big_integer &big_integer::trivial_division::divide(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    auto result_pair = divide_internal(dividend, divisor, dividend.get_allocator(), multiplication_rule);
    dividend.deallocate_with_guard(dividend._other_digits);
    return dividend = std::move(result_pair.first);
    //throw not_implemented("big_integer &big_integer::trivial_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::trivial_division::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    auto result_pair = divide_internal(dividend, divisor, dividend.get_allocator(), multiplication_rule);
    dividend.deallocate_with_guard(dividend._other_digits);
    return dividend = std::move(result_pair.second);
    //throw not_implemented("big_integer &big_integer::trivial_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Newton_division::divide(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Newton_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Newton_division::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Newton_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Burnikel_Ziegler_division::divide(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Burnikel_Ziegler_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Burnikel_Ziegler_division::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Burnikel_Ziegler_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer::big_integer(
    allocator *allocator):
    _other_digits(nullptr),
    _allocator(allocator)
{
    _other_digits = static_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), 1));
    _other_digits[0] = 0;
    _oldest_digit = 0;
}

big_integer::big_integer(int digit, allocator *allocator):
    _other_digits(nullptr),
    _allocator(allocator)
{
    _other_digits = static_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), 1));
    _other_digits[0] = 0;
    _oldest_digit = digit;
}

big_integer::big_integer(
    int const *digits,
    const size_t digits_count,
    allocator *allocator):
    _other_digits(nullptr),
    _allocator(allocator)
{
    if (digits == nullptr)
    {
        throw std::logic_error("int const* of digits should not be nullptr");
    }

    if (digits_count == 0)
    {
        throw std::logic_error("int const* of digits should not be empty");
    }

    try
    {
        _other_digits = static_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), digits_count));
    } catch (const std::exception &error)
    {
        deallocate_with_guard(_other_digits);
        _other_digits = nullptr;
        throw;
    }

    _oldest_digit = digits[digits_count - 1];
    _other_digits[0] = digits_count - 1;
    for (auto i = 0; i < digits_count - 1; i++)
    {
        _other_digits[i + 1] = digits[i];
    }

    //throw not_implemented("big_integer::big_integer(int *, size_t, allocator *)", "your code should be here...");
}

big_integer::big_integer(
    std::vector<int> const &digits,
    allocator *allocator):
    _other_digits(nullptr),
    _allocator(allocator)
{
    if (digits.empty() == 0)
    {
        throw std::logic_error("std::vector<int> of digits should not be empty");
    }

    try
    {
        _other_digits = static_cast<unsigned int *>(allocate_with_guard(sizeof(unsigned int), digits.size()));
    } catch (const std::exception &error)
    {
        deallocate_with_guard(_other_digits);
        _other_digits = nullptr;
        throw;
    }

    _oldest_digit = digits[digits.size() - 1];
    _other_digits[0] = digits.size() - 1;
    for (auto i = 0; i < digits.size() - 1; i++)
    {
        _other_digits[i + 1] = digits[i];
    }

    //throw not_implemented("big_integer::big_integer(std::vector<int> const &, allocator *)", "your code should be here...");
}

big_integer::big_integer(
    std::string const &value_as_string,
    size_t base,
    allocator *allocator)
{
    throw not_implemented("big_integer::big_integer(std::string const &, size_t, allocator *)", "your code should be here...");
}

big_integer::~big_integer()
{
    deallocate_with_guard(_other_digits);
    _other_digits = nullptr;

    //throw not_implemented("big_integer::~big_integer()", "your code should be here...");
}

big_integer::big_integer(
big_integer const &other):
    _other_digits(nullptr),
    _allocator(other._allocator)
{
    try
    {
        _other_digits = static_cast<unsigned int *>(
            allocate_with_guard(sizeof(unsigned int),
            other._other_digits[0] + 1));
    } catch (const std::exception &error)
    {
        deallocate_with_guard(_other_digits);
        _other_digits = nullptr;
        throw;
    }

    for (auto i = 0; i < other._other_digits[0] + 1; i++)
    {
        _other_digits[i] = other._other_digits[i];
    }

    //throw not_implemented("big_integer::big_integer(big_integer const &)", "your code should be here...");
}

big_integer &big_integer::operator=(
    big_integer const &other)
{
    if (this != &other)
    {
        if (_other_digits[0] != other._other_digits[0])
        {
            _other_digits[0] = other._other_digits[0];
            try
            {
                _other_digits = static_cast<unsigned int *>(allocate_with_guard(
                    sizeof(unsigned int),
                    other._other_digits[0] + 1));
            } catch (const std::exception &error)
            {
                deallocate_with_guard(_other_digits);
                _other_digits = nullptr;
                throw;
            }
        }

        for (auto i = 0; i < other._other_digits[0] + 1; i++)
        {
            _other_digits[i] = other._other_digits[i];
        }
    }

    return *this;
    //throw not_implemented("big_integer &big_integer::operator=(big_integer const &)", "your code should be here...");
}

big_integer::big_integer(
big_integer &&other) noexcept:
    _oldest_digit(std::move(other._oldest_digit)),
    _other_digits(std::move(other._other_digits)),
    _allocator(other._allocator)
{
    other._other_digits = nullptr;
    other._allocator = nullptr;
    //throw not_implemented("big_integer::big_integer(big_integer &&other) noexcept", "your code should be here...");
}

big_integer &big_integer::operator=(
    big_integer &&other) noexcept
{
    if (this != &other)
    {
        deallocate_with_guard(_other_digits);
        _oldest_digit = std::move(other._oldest_digit);
        _other_digits = std::move(other._other_digits);
        _allocator = other._allocator;

        other._other_digits = nullptr;
        other._allocator = nullptr;
    }

    return *this;
    //throw not_implemented("big_integer &big_integer::operator=(big_integer &&) noexcept", "your code should be here...");
}

bool big_integer::operator==(
    big_integer const &other) const
{
    if (this == &other) return true;

    if (get_digits_count() != other.get_digits_count())
    {
        return false;
    }

    for (auto i = 0; i < get_digits_count(); i++)
    {
        if (_other_digits[i + 1] != other._other_digits[i + 1])
        {
            return false;
        }
    }

    return _oldest_digit == other._oldest_digit;
    //throw not_implemented("bool big_integer::operator==(big_integer const &) const", "your code should be here...");
}

bool big_integer::operator!=(
    big_integer const &other) const
{
    return !(*this == other);

    //throw not_implemented("bool big_integer::operator!=(big_integer const &) const", "your code should be here...");
}

bool big_integer::operator<(
    big_integer const &other) const
{
    if (this == &other) return false;

    if (get_digits_count() < other.get_digits_count())
    {
        return true;
    }

    if (_oldest_digit < other._oldest_digit)
    {
        return true;
    }

    if (_oldest_digit > other._oldest_digit)
    {
        return false;
    }

    for (auto i = get_digits_count(); i > 0; i--)
    {
        if (_other_digits[i] < other._other_digits[i])
        {
            return true;
        }
    }

    return false;

    //throw not_implemented("bool big_integer::operator<(big_integer const &) const", "your code should be here...");
}

bool big_integer::operator>(
    big_integer const &other) const
{
    return !(*this < other || *this == other);

    //throw not_implemented("bool big_integer::operator>(big_integer const &) const", "your code should be here...");
}

bool big_integer::operator<=(
    big_integer const &other) const
{
    return !(*this > other);

    //throw not_implemented("bool big_integer::operator<=(big_integer const &) const", "your code should be here...");
}

bool big_integer::operator>=(
    big_integer const &other) const
{
    return !(*this < other);
    //throw not_implemented("bool big_integer::operator>=(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator-() const
{
    auto result = *this;
    result.change_sign();
    return result;
    //throw not_implemented("big_integer big_integer::operator-() const", "your code should be here...");
}

big_integer &big_integer::operator+=(
    big_integer const &other)
{
    if (other.is_zero())
    {
        return *this;
    }

    if (is_zero())
    {
        return *this = other;
    }

    if (is_negate())
    {
        change_sign();
        *this += -other;
        return change_sign();
    }

    if (other.is_negate())
    {
        return *this -= -other;
    }

    constexpr int sum_shift = sizeof(unsigned int) << 2;
    constexpr int sum_mask = (1 << sum_shift) - 1;

    auto max_count = std::max(get_digits_count(), other.get_digits_count()) + 1;
    unsigned int result = 0;

    auto result_digits = std::vector<int>(max_count);
    for (auto i = 0; i < max_count; i++)
    {
        auto first = get_digit(i);
        auto second = other.get_digit(i);

        result_digits[i] = 0;
        for (auto j = 0; j < 2; j++)
        {
            auto partial_sum = (first & sum_mask) + (second & sum_mask) + result;
            first >>= sum_shift;
            second >>= sum_shift;
            result_digits[i] |= (partial_sum & sum_mask) << sum_shift * j;
            result = partial_sum >> sum_shift;
        }
    }

    if (result_digits[max_count - 1] >> (sizeof(unsigned int) << 3) - 1 != 0)
    {
        result_digits.push_back(0);
    }

    deallocate_with_guard(_other_digits);
    auto temp = big_integer(result_digits, _allocator);
    *this = std::move(temp);

    return *this;
    //throw not_implemented("big_integer &big_integer::operator+=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator+(
    big_integer const &other) const
{
    auto temp = *this;
    temp += other;
    return temp;
    //throw not_implemented("big_integer big_integer::operator+(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator+(
    std::pair<big_integer, allocator *> const &other) const
{
    auto temp = big_integer(other.second);
    temp += *this;
    return temp;
    //throw not_implemented("big_integer big_integer::operator+(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator-=(
    big_integer const &other)
{
    if (is_zero())
    {
        *this += -other;
        return change_sign();
    }

    if (other.is_zero())
    {
        return *this;
    }

    if (*this == other)
    {
        auto temp = big_integer(_allocator);
        deallocate_with_guard(_other_digits);
        return *this = std::move(temp);
    }

    if (is_negate())
    {
        change_sign();
        *this -= -other;
        return change_sign();
    }

    if (other.is_negate())
    {
        return *this += -other;
    }

    if (*this < other)
    {
        auto temp = other;
        temp -= *this;
        deallocate_with_guard(_other_digits);
        return *this = -temp;
    }

    auto max_count = std::max(get_digits_count(), other.get_digits_count());
    unsigned int borrow = 0;

    auto result_digits = std::vector<int>(max_count);
    for (auto i = 0; i < max_count; i++)
    {
        auto first = get_digit(i);
        auto second = other.get_digit(i);

        result_digits[i] = first - second - borrow;
        if (first < second || (first == second && borrow))
        {
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }
    }

    auto k = max_count - 1;
    while (k > 0)
    {
        if (result_digits[k] >> (sizeof(unsigned int) << 3) - 1 != 0)
        {
            break;
        }

        k--;
    }

    while ((max_count - 1 - k) > 1)
    {
        result_digits.pop_back();
    }

    deallocate_with_guard(_other_digits);
    auto temp = big_integer(result_digits, _allocator);
    *this = std::move(temp);

    return *this;

    //throw not_implemented("big_integer &big_integer::operator-=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator-(
    big_integer const &other) const
{
    auto temp = *this;
    temp -= other;
    return temp;
    //throw not_implemented("big_integer big_integer::operator-(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator-(
    std::pair<big_integer, allocator *> const &other) const
{
    auto temp = big_integer(other.second);
    temp -= *this;
    return temp;
    //throw not_implemented("big_integer big_integer::operator-(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator*=(
    big_integer const &other)
{
    const auto multiplication = trivial_multiplication();
    return multiplication.multiply(*this, other);

    //throw not_implemented("big_integer &big_integer::operator*=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator*(
    big_integer const &other) const
{
    auto temp = *this;
    temp *= other;
    return temp;

    //throw not_implemented("big_integer big_integer::operator*(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator*(
    std::pair<big_integer, allocator *> const &other) const
{
    auto temp = big_integer(other.second);
    temp *= *this;
    return temp;

    //throw not_implemented("big_integer big_integer::operator*(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator/=(
    big_integer const &other)
{
    auto division = trivial_division();
    return division.divide(*this, other, multiplication_rule::trivial);
    //throw not_implemented("big_integer &big_integer::operator/=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator/(
    big_integer const &other) const
{
    auto temp = *this;
    temp /= other;
    return temp;
    //throw not_implemented("big_integer big_integer::operator/(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator/(
    std::pair<big_integer, allocator *> const &other) const
{
    auto temp = big_integer(other.second);
    temp /= *this;
    return temp;
    //throw not_implemented("big_integer big_integer::operator/(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator%=(
    big_integer const &other)
{
    auto division = trivial_division();
    return division.modulo(*this, other, multiplication_rule::trivial);
    //throw not_implemented("big_integer &big_integer::operator%=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator%(
    big_integer const &other) const
{
    auto temp = *this;
    temp %= other;
    return temp;
    //throw not_implemented("big_integer big_integer::operator%(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator%(
    std::pair<big_integer, allocator *> const &other) const
{
    auto temp = big_integer(other.second);
    temp %= *this;
    return temp;
    //throw not_implemented("big_integer big_integer::operator%(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer big_integer::operator~() const
{
    auto temp = *this;
    temp._oldest_digit = ~temp._oldest_digit;
    for (auto i = 0; i < temp.get_digits_count(); i++)
    {
        temp._other_digits[i + 1] = ~temp._other_digits[i + 1];
    }

    return temp;
    //throw not_implemented("big_integer big_integer::operator~() const", "your code should be here...");
}

big_integer &big_integer::operator&=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator&=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator&(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator&(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator&(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator&(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator|=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator|=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator|(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator|(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator|(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator|(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator^=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator^=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator^(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator^(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator^(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator^(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator<<=(
    size_t shift)
{
    throw not_implemented("big_integer &big_integer::operator<<=(size_t)", "your code should be here...");
}

big_integer big_integer::operator<<(
    size_t shift) const
{
    throw not_implemented("big_integer big_integer::operator<<(size_t) const", "your code should be here...");
}

big_integer big_integer::operator<<(
    std::pair<size_t, allocator *> const &shift) const
{
    throw not_implemented("big_integer big_integer::operator<<(std::pair<size_t, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator>>=(
    size_t shift)
{
    throw not_implemented("big_integer &big_integer::operator>>=(size_t)", "your code should be here...");
}

big_integer big_integer::operator>>(
    size_t shift) const
{
    throw not_implemented("big_integer big_integer::operator>>(size_t) const", "your code should be here...");
}

big_integer big_integer::operator>>(
    std::pair<size_t, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator>>(std::pair<size_t, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier,
    allocator *allocator,
    big_integer::multiplication_rule multiplication_rule)
{
    multiplication* mult = nullptr;
    switch (multiplication_rule)
    {
        case multiplication_rule::trivial:
            mult = static_cast<multiplication *>(
                allocate_with_guard_static(allocator, sizeof(trivial_multiplication), 1));
            break;
        case multiplication_rule::Karatsuba:
            mult = static_cast<multiplication *>(
                allocate_with_guard_static(allocator, sizeof(Karatsuba_multiplication), 1));
            break;
        case multiplication_rule::SchonhageStrassen:
            mult = static_cast<multiplication *>(
                allocate_with_guard_static(allocator, sizeof(Schonhage_Strassen_multiplication), 1));
            break;
        default:
            throw not_implemented("big_integer &big_integer::multiply(big_integer &first_multiplier, big_integer const &second_multiplier, allocator *allocator, big_integer::multiplication_rule multiplication_rule)", "not implemented multuplication_rule");
    }

    allocator::construct(mult);
    auto& result = mult->multiply(first_multiplier, second_multiplier);
    deallocate_with_guard_static(allocator, mult);
    return result;
    //throw not_implemented("big_integer &big_integer::multiply(big_integer &, big_integer const &, allocator *, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer big_integer::multiply(
    big_integer const &first_multiplier,
    big_integer const &second_multiplier,
    allocator *allocator,
    big_integer::multiplication_rule multiplication_rule)
{
    multiplication* mult = nullptr;
    switch (multiplication_rule)
    {
        case multiplication_rule::trivial:
            mult = static_cast<multiplication *>(
                allocate_with_guard_static(allocator, sizeof(trivial_multiplication), 1));
        break;
        case multiplication_rule::Karatsuba:
            mult = static_cast<multiplication *>(
                allocate_with_guard_static(allocator, sizeof(Karatsuba_multiplication), 1));
        break;
        case multiplication_rule::SchonhageStrassen:
            mult = static_cast<multiplication *>(
                allocate_with_guard_static(allocator, sizeof(Schonhage_Strassen_multiplication), 1));
        break;
        default:
            throw not_implemented("big_integer &big_integer::multiply(big_integer const &first_multiplier, big_integer const &second_multiplier, allocator *allocator, big_integer::multiplication_rule multiplication_rule)", "not implemented multuplication_rule");
    }

    allocator::construct(mult);
    auto temp = first_multiplier;
    auto result = mult->multiply(temp, second_multiplier);
    deallocate_with_guard_static(allocator, mult);
    return result;
    //throw not_implemented("big_integer big_integer::multiply(big_integer const &, big_integer const &, allocator *, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::divide(
    big_integer &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    division* div = nullptr;
    switch (division_rule)
    {
        case division_rule::trivial:
            div = static_cast<division *>(
                allocate_with_guard_static(allocator, sizeof(trivial_division), 1));
        break;
        case division_rule::Newton:
            div = static_cast<division *>(
                allocate_with_guard_static(allocator, sizeof(Newton_division), 1));
        break;
        case division_rule::BurnikelZiegler:
            div = static_cast<division *>(
                allocate_with_guard_static(allocator, sizeof(Burnikel_Ziegler_division), 1));
        break;
        default:
            throw not_implemented("big_integer &big_integer::divide(big_integer &dividend, big_integer const &divisor, allocator *allocator, big_integer::division_rule division_rule, big_integer::multiplication_rule multiplication_rule)", "not implemented division_rule");
    }

    allocator::construct(div);
    auto& result = div->divide(dividend, divisor, multiplication_rule);
    deallocate_with_guard_static(allocator, div);
    return result;

    //throw not_implemented("big_integer &big_integer::divide(big_integer &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer big_integer::divide(
    big_integer const &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    division* div = nullptr;
    switch (division_rule)
    {
        case division_rule::trivial:
            div = static_cast<division *>(
                allocate_with_guard_static(allocator, sizeof(trivial_division), 1));
        break;
        case division_rule::Newton:
            div = static_cast<division *>(
                allocate_with_guard_static(allocator, sizeof(Newton_division), 1));
        break;
        case division_rule::BurnikelZiegler:
            div = static_cast<division *>(
                allocate_with_guard_static(allocator, sizeof(Burnikel_Ziegler_division), 1));
        break;
        default:
            throw not_implemented("big_integer &big_integer::divide(big_integer const &dividend, big_integer const &divisor, allocator *allocator, big_integer::division_rule division_rule, big_integer::multiplication_rule multiplication_rule)", "not implemented division_rule");
    }

    allocator::construct(div);
    auto temp = dividend;
    auto result = div->divide(temp, divisor, multiplication_rule);
    deallocate_with_guard_static(allocator, div);
    return result;

    //throw not_implemented("big_integer big_integer::divide(big_integer const &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    division* div = nullptr;
    switch (division_rule)
    {
        case division_rule::trivial:
            div = static_cast<division *>(
                allocate_with_guard_static(allocator, sizeof(trivial_division), 1));
        break;
        case division_rule::Newton:
            div = static_cast<division *>(
                allocate_with_guard_static(allocator, sizeof(Newton_division), 1));
        break;
        case division_rule::BurnikelZiegler:
            div = static_cast<division *>(
                allocate_with_guard_static(allocator, sizeof(Burnikel_Ziegler_division), 1));
        break;
        default:
            throw not_implemented("big_integer &big_integer::modulo(big_integer &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "not implemented division_rule");
    }

    allocator::construct(div);
    auto& result = div->modulo(dividend, divisor, multiplication_rule);
    deallocate_with_guard_static(allocator, div);
    return result;

    //throw not_implemented("big_integer &big_integer::modulo(big_integer &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer big_integer::modulo(
    big_integer const &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    division* div = nullptr;
    switch (division_rule)
    {
        case division_rule::trivial:
            div = static_cast<division *>(
                allocate_with_guard_static(allocator, sizeof(trivial_division), 1));
        break;
        case division_rule::Newton:
            div = static_cast<division *>(
                allocate_with_guard_static(allocator, sizeof(Newton_division), 1));
        break;
        case division_rule::BurnikelZiegler:
            div = static_cast<division *>(
                allocate_with_guard_static(allocator, sizeof(Burnikel_Ziegler_division), 1));
        break;
        default:
            throw not_implemented("big_integer &big_integer::modulo(big_integer const &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "not implemented division_rule");
    }

    allocator::construct(div);
    auto temp = dividend;
    auto& result = div->modulo(temp, divisor, multiplication_rule);
    deallocate_with_guard_static(allocator, div);
    return result;

    //throw not_implemented("big_integer big_integer::modulo(big_integer const &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

std::ostream &operator<<(
    std::ostream &stream,
    big_integer const &value)
{
    throw not_implemented("std::ostream &operator<<(std::ostream &, big_integer const &)", "your code should be here...");
}

std::istream &operator>>(
    std::istream &stream,
    big_integer &value)
{
    throw not_implemented("std::istream &operator>>(std::istream &, big_integer &)", "your code should be here...");
}

bool big_integer::is_negate() const
{
    return _oldest_digit < 0;
}

bool big_integer::is_zero() const
{
    return _oldest_digit == 0;
}

size_t big_integer::get_digits_count() const
{
    return _other_digits[0];
}

unsigned int big_integer::get_digit(unsigned int index) const
{
    if (index < get_digits_count())
    {
        return _other_digits[index + 1];
    }

    if (index == get_digits_count())
    {
        return _oldest_digit < 0 ? -_oldest_digit : _oldest_digit;
    }

    return 0;
}

big_integer& big_integer::change_sign()
{
    _oldest_digit = -_oldest_digit;
    return *this;
}

[[nodiscard]] allocator *big_integer::get_allocator() const noexcept
{
    throw not_implemented("allocator *big_integer::get_allocator() const noexcept", "your code should be here...");
}
