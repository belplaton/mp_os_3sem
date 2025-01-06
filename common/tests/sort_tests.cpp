#include <functional>
#include <iostream>
#include "../include/mpos_sort.h"
#include <vector>

template<typename T>
void test_sort(const std::string& name, const sort_strategy<T>& strategy, const std::vector<T>& data)
{
	std::cout << "\nTesting " << name << "...\nOriginal data: ";
	for (const auto& val : data)
	{
		std::cout << val << " ";
	}

	auto sorted_data = strategy.sort(data, [](const T& a, const T& b) { return a < b; });

	std::cout << "\nSorted data: ";
	for (const auto& val : sorted_data)
	{
		std::cout << val << " ";
	}

	std::cout << "\n";
}
int main(int argc, char *argv[])
{
	std::vector<int> test_data = { 34, 7, 23, 32, 5, 62, 137, 64 };

	counting_sort<int> counting;
	insertion_sort<int> insertion;
	selection_sort<int> selection;
	shell_sort<int> shell;
	quick_sort<int> quick;

	test_sort("Counting Sort", counting, test_data);
	test_sort("Insertion Sort", insertion, test_data);
	test_sort("Selection Sort", selection, test_data);
	test_sort("Shell Sort", shell, test_data);
	test_sort("Quick Sort", quick, test_data);

	return 0;
}
