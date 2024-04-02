#include <iostream>
#include <vector>
#include "adapter.h"

int main() {
  std::vector<int> v = {1, 2, 3, 4, 5, 6};

  auto filtered = v | Filter([](int i){ return i % 2; });
  auto transformed = filtered | Transform([](int i){ return i * i; });

  for (int i : transformed) {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  std::vector<std::vector<int>> nested = {{1, 2}, {3, 4, 5}, {6}};
  auto flattened = nested | flatten<std::vector<int>>();
  for (int i : flattened) {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  std::vector<int> v1 = {1, 2, 3};
  std::vector<char> v2 = {'a', 'b', 'c'};
  auto zipped = v1 | zip(v1, v2);
  for (const auto& pair : zipped) {
    std::cout << "{" << pair.first << ", " << pair.second << "} ";
  }
  std::cout << std::endl;

  std::vector<int> t = {1, 2, 3};
  auto cycled = t | cycle<std::vector<int>>(3);
  for (int i : cycled) {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  auto maxElem = v | max_element(std::less<int>());
  std::cout << "Max element: " << maxElem << std::endl;

  return 0;

}