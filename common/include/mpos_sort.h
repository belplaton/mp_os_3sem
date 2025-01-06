#ifndef SORT_H
#define SORT_H
#include <functional>
#include <vector>
#include <algorithm>
#include <cmath>

template<typename T>
class sort_strategy 

{
public:

    virtual ~sort_strategy() = default;

    virtual std::vector<T> sort(std::vector<T> const&, std::function<bool(T const&, T const&)> const&) const = 0;

};

template<typename T>
class counting_sort final : public sort_strategy<T>
{

public:

    counting_sort() = default;

    ~counting_sort() override = default;

    std::vector<T> sort(std::vector<T> const& data, std::function<bool(T const&, T const&)> const& cmp) const override
    {
        std::vector<T> res(data.size(), 0);
        std::vector<int> count(data.size(), 0);
        for (int i = data.size() - 1; i >= 1; i--)
        {
            for (int j = i - 1; j >= 0; j--)
            {
                if (cmp(data[i], data[j]))
                {
                    count[j]++;
                }
                else
                {
                    count[i]++;
                }
            }
        }

        for (auto i = 0; i < data.size(); i++)
        {
            res[count[i]] = data[i];
        }

        return res;
    }
};

template<typename T>
class insertion_sort final : public sort_strategy<T>
{

public:

    insertion_sort() = default;

    ~insertion_sort() override = default;

    std::vector<T> sort(std::vector<T> const& data, std::function<bool(T const&, T const&)> const& cmp) const override
    {
        std::vector<T> res(data);
        for (int i = 1; i < data.size(); i++)
        {
            auto temp = res[i];
            auto j = i - 1;
            while (cmp(temp, res[j]) && j >= 0)
            {
                res[j + 1] = res[j];
                j--;
            }

            res[j + 1] = temp;
        }

        return res;
    }
};

template<typename T>
class selection_sort final : public sort_strategy<T>
{

public:

    selection_sort() = default;

    ~selection_sort() override = default;

    std::vector<T> sort(std::vector<T> const& data, std::function<bool(T const&, T const&)> const& cmp) const override
    {
        std::vector<T> res(data);
        for (auto i = 0; i < data.size() - 1; i++)
        {
            auto min_index = i;
            for (auto j = i + 1; j < data.size(); j++)
            {
                if (cmp(res[j], res[min_index]))
                {
                    min_index = j;
                }
            }

            if (min_index != i)
            {
                auto temp = res[i];
                res[i] = res[min_index];
                res[min_index] = temp;
            }
        }

        return res;
    }
};

template<typename T>
class shell_sort final : public sort_strategy<T>
{

public:

    shell_sort() = default;

    ~shell_sort() override = default;

    std::vector<T> sort(std::vector<T> const& data, std::function<bool(T const&, T const&)> const& cmp) const override
    {
        std::vector<T> res(data);
        for (auto gap = data.size() / 2; gap > 0; gap /= 2)
        {
            for (auto i = gap; i < data.size(); i++)
            {
                auto temp = res[i];
                auto j = i;
                while (cmp(temp, res[j - gap]) && j >= gap)
                {
                    res[j] = res[j - gap];
                    j -= gap;
                }

                res[j] = temp;
            }
        }

        return res;
    }
};

template<typename T>
class quick_sort final : public sort_strategy<T>
{

private:

    void qsort(std::vector<T>& data, int low, int high, std::function<bool(T const&, T const&)> const& cmp) const
    {
        if (low < high)
        {
            auto pivot = data[(low + high) / 2];
            auto i = low;
            auto j = high;
            while (i <= j)
            {
                while (cmp(data[i], pivot)) i++;
                while (cmp(pivot, data[j])) j--;
                if (i <= j)
                {
                    std::swap(data[i], data[j]);
                    i++;
                    j--;
                }
            }

            qsort(data, low, j, cmp);
            qsort(data, i, high, cmp);
        }
    }

public:

    quick_sort() = default;

    ~quick_sort() override = default;

    std::vector<T> sort(std::vector<T> const& data, std::function<bool(T const&, T const&)> const& cmp) const override
    {
        std::vector<T> res(data);
        qsort(res, 0, res.size() - 1, cmp);
        return res;
    }
};

template<typename T>
class heap_sort final : public sort_strategy<T>
{

private:

    void heapify(std::vector<T>& data, int n, int i, std::function<bool(T const&, T const&)> const& cmp) const
    {
        auto root = i;
        auto left = 2 * i + 1;
        auto right = 2 * i + 2;

        if (left < n && cmp(data[root], data[left]))
        {
            root = left;
        }

        if (right < n && cmp(data[root], data[right]))
        {
            root = right;
        }

        if (root != i)
        {
            std::swap(data[i], data[root]);
            heapify(data, n, root, cmp);
        }
    }

public:

    heap_sort() = default;

    ~heap_sort() override = default;

    std::vector<T> sort(std::vector<T> const& data, std::function<bool(T const&, T const&)> const& cmp) const override
    {
        std::vector<T> res(data);
        for (auto i = data.size() / 2 - 1; i >= 0; --i)
        {
            heapify(res, data.size(), i, cmp);
        }

        for (auto i = data.size() - 1; i > 0; --i)
        {
            std::swap(res[0], res[i]);
            heapify(res, i, 0, cmp);
        }

        return res;
    }
};

class radix_sort final : public sort_strategy<int>
{

private:

    std::vector<int> sort_by_digit(const std::vector<int>& data, int exp, bool ascending) const
    {
        std::vector<int> output(data.size());
        std::vector<int> count(10, 0);

        for (auto i = 0; i < data.size(); i++)
        {
            auto digit = (data[i] / exp) % 10;
            count[digit]++;
        }

        if (ascending)
        {
            for (auto i = 1; i < 10; i++)
            {
                count[i] += count[i - 1];
            }
        }
        else 
        {
            for (auto i = 8; i >= 0; i--)
            {
                count[i] += count[i + 1];
            }
        }

        for (int i = data.size() - 1; i >= 0; i--)
        {
            auto digit = (data[i] / exp) % 10;
            output[count[digit] - 1] = data[i];
            count[digit]--;
        }

        return output;
    }

public:

    radix_sort() = default;

    ~radix_sort() override = default;

    std::vector<int> sort(std::vector<int> const& data, std::function<bool(int const&, int const&)> const& cmp) const override
    {
        std::vector<int> res(data);
        if (res.size() == 0)
        {
            return res;
        }

        auto ascending = cmp(1, 2);
        auto max_elem = *std::max_element(res.begin(), res.end());
        auto exp = 1;
        while (max_elem / exp > 0)
        {
            res = sort_by_digit(res, exp, ascending);
            exp *= 10;
        }

        return res;
    }
};

#endif //SORT_H