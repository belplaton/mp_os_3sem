#include "../include/big_integer.h"

big_integer &big_integer::trivial_multiplication::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier) const
{
    throw not_implemented("big_integer &big_integer::trivial_multiplication::multiply(big_integer &, big_integer const &)", "your code should be here...");
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

big_integer &big_integer::trivial_division::divide(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::trivial_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::trivial_division::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::trivial_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
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
        deallocate_with_guard(_allocator);
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
                deallocate_with_guard(_allocator);
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
    throw not_implemented("big_integer big_integer::operator+(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator+(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator+(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator-=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator-=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator-(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator-(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator-(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator-(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator*=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator*=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator*(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator*(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator*(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator*(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator/=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator/=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator/(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator/(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator/(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator/(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator%=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator%=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator%(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator%(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator%(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator%(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer big_integer::operator~() const
{
    throw not_implemented("big_integer big_integer::operator~() const", "your code should be here...");
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
    throw not_implemented("big_integer &big_integer::multiply(big_integer &, big_integer const &, allocator *, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer big_integer::multiply(
    big_integer const &first_multiplier,
    big_integer const &second_multiplier,
    allocator *allocator,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer big_integer::multiply(big_integer const &, big_integer const &, allocator *, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::divide(
    big_integer &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer &big_integer::divide(big_integer &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer big_integer::divide(
    big_integer const &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer big_integer::divide(big_integer const &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer &big_integer::modulo(big_integer &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer big_integer::modulo(
    big_integer const &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer big_integer::modulo(big_integer const &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
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
