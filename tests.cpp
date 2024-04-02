#include <gtest/gtest.h>
#include <vector>
#include "adapter.h"

TEST(TransformTest, MultiplyByTwo) {
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto result = vec | Transform([](int x) { return x * 2; });
  EXPECT_EQ(result, (std::vector<int>{2, 4, 6, 8, 10}));
}

TEST(TransformTest, AddOne) {
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto result = vec | Transform([](int x) { return x + 1; });
  EXPECT_EQ(result, (std::vector<int>{2, 3, 4, 5, 6}));
}

TEST(TransformTest, Square) {
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto result = vec | Transform([](int x) { return x * x; });
  EXPECT_EQ(result, (std::vector<int>{1, 4, 9, 16, 25}));
}

TEST(FilterTest, EvenNumbers) {
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto result = vec | Filter([](int x) { return x % 2 == 0; });
  EXPECT_EQ(result, (std::vector<int>{2, 4}));
}

TEST(FilterTest, GreaterThanThree) {
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto result = vec | Filter([](int x) { return x > 3; });
  EXPECT_EQ(result, (std::vector<int>{4, 5}));
}

TEST(FilterTest, LessOrEqualThree) {
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto result = vec | Filter([](int x) { return x <= 3; });
  EXPECT_EQ(result, (std::vector<int>{1, 2, 3}));
}

TEST(TakeTest, TakeThree) {
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto result = vec | Take(3);
  EXPECT_EQ(result, (std::vector<int>{1, 2, 3}));
}

TEST(TakeTest, TakeZero) {
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto result = vec | Take(0);
  EXPECT_TRUE(result.empty());
}

TEST(TakeTest, TakeMoreThanSize) {
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto result = vec | Take(10);
  EXPECT_EQ(result, vec);
}

TEST(DropTest, DropThree) {
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto result = vec | Drop(3);
  EXPECT_EQ(result, (std::vector<int>{4, 5}));
}

TEST(DropTest, DropZero) {
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto result = vec | Drop(0);
  EXPECT_EQ(result, vec);
}

TEST(DropTest, DropMoreThanSize) {
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto result = vec | Drop(10);
  EXPECT_TRUE(result.empty());
}

TEST(ReverseTest, ReverseVector) {
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto result = vec | Reverse();
  EXPECT_EQ(result, (std::vector<int>{5, 4, 3, 2, 1}));
}

TEST(ReverseTest, ReverseEmpty) {
  std::vector<int> vec = {};
  auto result = vec | Reverse();
  EXPECT_TRUE(result.empty());
}

TEST(KeysValuesTest, Keys) {
  std::unordered_map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}};
  auto result = map | Keys();
  EXPECT_EQ(result, (std::vector<int>{1, 2, 3}));
}

TEST(KeysValuesTest, Values) {
  std::unordered_map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}};
  auto result = map | Values();
  EXPECT_EQ(result, (std::vector<std::string>{"one", "two", "three"}));
}

TEST(AdapterChainTest, TransformAndFilter) {
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto result = vec
      | Transform([](int x) { return x * 2; })
      | Filter([](int x) { return x % 4 == 0; });
  EXPECT_EQ(result, (std::vector<int>{4, 8}));
}

TEST(AdapterChainTest, TransformFilterTake) {
  std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto result = vec
      | Transform([](int x) { return x * 2; })
      | Filter([](int x) { return x % 3 == 0; })
      | Take(3);
  EXPECT_EQ(result, (std::vector<int>{6, 12, 18}));
}

TEST(AdapterChainTest, TransformDropReverse) {
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto result = vec
      | Transform([](int x) { return x * 3; })
      | Drop(2)
      | Reverse();
  EXPECT_EQ(result, (std::vector<int>{15, 12, 9}));
}

TEST(AdapterChainTest, FilterTakeDrop) {
  std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto result = vec
      | Filter([](int x) { return x % 2 == 0; })
      | Take(4)
      | Drop(1);
  EXPECT_EQ(result, (std::vector<int>{4, 6, 8}));
}

TEST(AdapterChainTest, FilterReverseTransform) {
  std::vector<int> vec = {10, 20, 30, 40, 50};
  auto result = vec
      | Filter([](int x) { return x > 20; })
      | Reverse()
      | Transform([](int x) { return x / 10; });
  EXPECT_EQ(result, (std::vector<int>{5, 4, 3}));
}

TEST(AdapterChainTest, TransformFilterReverse) {
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto result = vec
      | Transform([](int x) { return x + 5; })
      | Filter([](int x) { return x % 2 == 0; })
      | Reverse();
  EXPECT_EQ(result, (std::vector<int>{10, 8, 6}));
}

TEST(AdapterChainTest, DropTakeTransform) {
  std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto result = vec
      | Drop(3)
      | Take(4)
      | Transform([](int x) { return x * 2; });
  EXPECT_EQ(result, (std::vector<int>{8, 10, 12, 14}));
}

TEST(AdapterChainTest, ReverseTakeFilter) {
  std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto result = vec
      | Reverse()
      | Take(5)
      | Filter([](int x) { return x > 6; });
  EXPECT_EQ(result, (std::vector<int>{10, 9, 8, 7}));
}

TEST(AdapterChainTest, KeysValuesTransform) {
  std::unordered_map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}};
  auto result = map
      | Keys()
      | Transform([](int x) { return x * 10; });
  EXPECT_EQ(result, (std::vector<int>{10, 20, 30}));
}

TEST(AdapterChainTest, ValuesReverseFilter) {
  std::unordered_map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}};
  auto result = map
      | Values()
      | Reverse()
      | Filter([](const std::string& s) { return s.size() == 3; });
  EXPECT_EQ(result, (std::vector<std::string>{"two", "one"}));
}

TEST(AdapterChainTest, TransformTakeDropKeys) {
  std::unordered_map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"}};
  auto result = map
      | Keys()
      | Transform([](int x) { return x + 1; })
      | Take(4)
      | Drop(2);
  EXPECT_EQ(result, (std::vector<int>{4, 5}));
}

TEST(AdapterChainTest, DropReverseTakeTransform) {
  std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto result = vec
      | Drop(3)
      | Reverse()
      | Take(4)
      | Transform([](int x) { return x - 2; });
  EXPECT_EQ(result, (std::vector<int>{8, 7, 6, 5}));
}

TEST(AdapterChainTest, ComplexChain) {
  std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto result = vec
      | Transform([](int x) { return x * 2; })
      | Filter([](int x) { return x > 10; })
      | Drop(1)
      | Take(2)
      | Reverse();
  EXPECT_EQ(result, (std::vector<int>{16, 14}));
}

TEST(AdapterChainTest, MultipleTransforms) {
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto result = vec
      | Transform([](int x) { return x * 2; })
      | Transform([](int x) { return x + 3; });
  EXPECT_EQ(result, (std::vector<int>{5, 7, 9, 11, 13}));
}

TEST(AdapterChainTest, MultipleFilters) {
  std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto result = vec
      | Filter([](int x) { return x % 2 == 0; })
      | Filter([](int x) { return x > 5; });
  EXPECT_EQ(result, (std::vector<int>{6, 8, 10}));
}

TEST(AdapterChainTest, TransformFilterDropReverse) {
  std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto result = vec
      | Transform([](int x) { return x * 2; })
      | Filter([](int x) { return x > 10; })
      | Drop(2)
      | Reverse();
  EXPECT_EQ(result, (std::vector<int>{20, 18, 16}));
}

TEST(AdapterChainTest, DropTakeFilterTransform) {
  std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto result = vec
      | Drop(2)
      | Take(5)
      | Filter([](int x) { return x % 2 == 0; })
      | Transform([](int x) { return x * 3; });
  EXPECT_EQ(result, (std::vector<int>{12, 18}));
}

TEST(AdapterChainTest, ReverseKeysValues) {
  std::unordered_map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}};
  auto result = map
      | Keys()
      | Reverse()
      | Transform([](int x) { return x * 5; });
  EXPECT_EQ(result, (std::vector<int>{15, 10, 5}));
}

TEST(AdapterChainTest, ComplexChainWithValues) {
  std::unordered_map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}};
  auto result = map
      | Values()
      | Transform([](const std::string& s) { return s + "!"; })
      | Filter([](const std::string& s) { return s.size() > 4; });
  EXPECT_EQ(result, (std::vector<std::string>{"three!", "four!"}));
}

TEST(AdapterChainTest, TransformFilterReverseEmptyVector) {
  std::vector<int> vec = {};
  auto result = vec
      | Transform([](int x) { return x * 2; })
      | Filter([](int x) { return x % 3 == 0; })
      | Reverse();
  EXPECT_TRUE(result.empty());
}

TEST(AdapterChainTest, FilterDropTakeReverse) {
  std::vector<int> vec = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
  auto result = vec
      | Filter([](int x) { return x > 30; })
      | Drop(1)
      | Take(4)
      | Reverse();
  EXPECT_EQ(result, (std::vector<int>{80, 70, 60, 50}));
}

TEST(AdapterChainTest, DropReverseFilterTransform) {
  std::vector<int> vec = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50};
  auto result = vec
      | Drop(4)
      | Reverse()
      | Filter([](int x) { return x % 3 == 0; })
      | Transform([](int x) { return x / 3; });
  EXPECT_EQ(result, (std::vector<int>{15, 10}));
}

TEST(AdapterChainTest, MultipleTakes) {
  std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto result = vec
      | Take(8)
      | Take(5);
  EXPECT_EQ(result, (std::vector<int>{1, 2, 3, 4, 5}));
}

TEST(AdapterChainTest, MultipleDrops) {
  std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto result = vec
      | Drop(2)
      | Drop(3);
  EXPECT_EQ(result, (std::vector<int>{6, 7, 8, 9, 10}));
}

TEST(AdapterChainTest, FilterAllFalse) {
  std::vector<int> vec = {1, 2, 3, 4, 5};
  auto result = vec | Filter([](int x) { return x > 10; });
  EXPECT_TRUE(result.empty());
}

TEST(AdapterChainTest, TransformNegativeNumbers) {
  std::vector<int> vec = {-1, -2, -3, -4, -5};
  auto result = vec | Transform([](int x) { return x * x; });
  EXPECT_EQ(result, (std::vector<int>{1, 4, 9, 16, 25}));
}

TEST(AdapterChainTest, FilterNegativeNumbers) {
  std::vector<int> vec = {-1, -2, -3, -4, -5};
  auto result = vec | Filter([](int x) { return x < -3; });
  EXPECT_EQ(result, (std::vector<int>{-4, -5}));
}

TEST(AdapterChainTest, DropAndReverse) {
  std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto result = vec
      | Drop(5)
      | Reverse();
  EXPECT_EQ(result, (std::vector<int>{10, 9, 8, 7, 6}));
}

TEST(ZipTest, BasicZip) {
  std::vector<int> v1 = {1, 2, 3};
  std::vector<char> v2 = {'a', 'b', 'c'};
  auto zipped = v1 | zip(v1, v2);
  std::vector<std::pair<int, char>> expected = {{1, 'a'}, {2, 'b'}, {3, 'c'}};
  EXPECT_EQ(zipped, expected);
}

TEST(ZipTest, DifferentSizes) {
  std::vector<int> v1 = {1, 2};
  std::vector<char> v2 = {'a', 'b', 'c'};
  auto zipped = v1 | zip(v1, v2);
  std::vector<std::pair<int, char>> expected = {{1, 'a'}, {2, 'b'}};
  EXPECT_EQ(zipped, expected);
}

TEST(ZipTest, EmptyVectors) {
  std::vector<int> v1 = {};
  std::vector<char> v2 = {};
  auto zipped = v1 | zip(v1, v2);
  std::vector<std::pair<int, char>> expected = {};
  EXPECT_EQ(zipped, expected);
}

TEST(CycleTest, BasicCycle) {
  std::vector<int> v = {1, 2, 3};
  auto cycled = v | cycle<std::vector<int>>(2);
  std::vector<int> expected = {1, 2, 3, 1, 2, 3};
  EXPECT_EQ(cycled, expected);
}

TEST(CycleTest, CycleZeroTimes) {
  std::vector<int> v = {1, 2, 3};
  auto cycled = v | cycle<std::vector<int>>(0);
  std::vector<int> expected = {};
  EXPECT_EQ(cycled, expected);
}

TEST(CycleTest, EmptyVector) {
  std::vector<int> v = {};
  auto cycled = v | cycle<std::vector<int>>(3);
  std::vector<int> expected = {};
  EXPECT_EQ(cycled, expected);
}

TEST(FlattenTest, BasicFlatten) {
  std::vector<std::vector<int>> nested = {{1, 2}, {3, 4, 5}, {6}};
  auto flattened = nested | flatten<std::vector<int>>();
  std::vector<int> expected = {1, 2, 3, 4, 5, 6};
  EXPECT_EQ(flattened, expected);
}

TEST(FlattenTest, EmptyInnerVectors) {
  std::vector<std::vector<int>> nested = {{}, {}, {}};
  auto flattened = nested | flatten<std::vector<int>>();
  std::vector<int> expected = {};
  EXPECT_EQ(flattened, expected);
}

TEST(FlattenTest, MixedEmptyAndNonEmpty) {
  std::vector<std::vector<int>> nested = {{}, {1, 2, 3}, {}, {4, 5}};
  auto flattened = nested | flatten<std::vector<int>>();
  std::vector<int> expected = {1, 2, 3, 4, 5};
  EXPECT_EQ(flattened, expected);
}

TEST(MaxElementTest, BasicMaxElement) {
  std::vector<int> v = {1, 2, 3, 4, 5};
  auto maxElem = v | max_element(std::less<int>());
  EXPECT_EQ(maxElem, 5);
}

TEST(MaxElementTest, SingleElement) {
  std::vector<int> v = {10};
  auto maxElem = v | max_element(std::less<int>());
  EXPECT_EQ(maxElem, 10);
}

TEST(MaxElementTest, AllEqualElements) {
  std::vector<int> v = {7, 7, 7, 7};
  auto maxElem = v | max_element(std::less<int>());
  EXPECT_EQ(maxElem, 7);
}

// Цепочка адаптеров с использованием zip и cycle
TEST(ChainAdaptersTest, ZipAndCycle) {
  std::vector<int> v1 = {1, 2, 3};
  std::vector<char> v2 = {'a', 'b', 'c'};
  auto zipped = v1 | zip(v1, v2) | cycle<std::vector<std::pair<int, char>>>(2);
  std::vector<std::pair<int, char>> expected = {{1, 'a'}, {2, 'b'}, {3, 'c'}, {1, 'a'}, {2, 'b'}, {3, 'c'}};
  EXPECT_EQ(zipped, expected);
}

// Цепочка адаптеров с использованием flatten и transform
TEST(ChainAdaptersTest, FlattenAndTransform) {
  std::vector<std::vector<int>> nested = {{1, 2}, {3, 4, 5}, {6}};
  auto flattenedAndTransformed = nested | flatten<std::vector<int>>() | Transform([](int i) { return i * 2; });
  std::vector<int> expected = {2, 4, 6, 8, 10, 12};
  EXPECT_EQ(flattenedAndTransformed, expected);
}

// Цепочка адаптеров с использованием filter, transform и max_element
TEST(ChainAdaptersTest, FilterTransformAndMaxElement) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6};
  auto maxElem = v | Filter([](int i) { return i % 2 == 0; }) | Transform([](int i) { return i * 2; })
      | max_element(std::less<int>());
  EXPECT_EQ(maxElem, 12);
}

// Цепочка адаптеров с использованием flatten и cycle
TEST(ChainAdaptersTest, FlattenAndCycle) {
  std::vector<std::vector<int>> nested = {{1, 2}, {3, 4, 5}};
  auto flattenedAndCycled = nested | flatten<std::vector<int>>() | cycle<std::vector<int>>(2);
  std::vector<int> expected = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
  EXPECT_EQ(flattenedAndCycled, expected);
}

// Цепочка адаптеров с использованием zip и max_element
TEST(ChainAdaptersTest, ZipAndMaxElement) {
  std::vector<int> v1 = {1, 2, 3};
  std::vector<int> v2 = {4, 5, 6};
  auto maxPair = v1 | zip(v1, v2) | max_element([](const std::pair<int, int>& a, const std::pair<int, int>& b) {
    return (a.first + a.second) < (b.first + b.second);
  });
  std::pair<int, int> expected = {3, 6};
  EXPECT_EQ(maxPair, expected);
}

// Цепочка адаптеров с использованием filter, zip и flatten
TEST(ChainAdaptersTest, FilterZip) {
  std::vector<int> v1 = {1, 2, 3, 4};
  std::vector<int> v2 = {5, 6, 7, 8};
  auto filteredV1 = v1 | Filter([](int i) { return i % 2 == 0; });
  auto filteredV2 = v2 | Filter([](int i) { return i % 2 == 0; });
  auto zipped = filteredV1 | zip(filteredV1, filteredV2);

  std::vector<std::pair<int, int>> expected = {{2, 6}, {4, 8}};
  EXPECT_EQ(zipped, expected);
}

TEST(MinElementTest, IntVector) {
  std::vector<int> vec = {3, 1, 4, 1, 5, 9};
  auto min_val = vec | min_element(std::less<int>());
  EXPECT_EQ(min_val, 1);
}

TEST(MinElementTest, DoubleVector) {
  std::vector<double> vec = {2.3, 1.5, 3.6, 1.2};
  auto min_val = vec | min_element(std::less<double>());
  EXPECT_DOUBLE_EQ(min_val, 1.2);
}

TEST(MinElementTest, StringVector) {
  std::vector<std::string> vec = {"apple", "banana", "cherry"};
  auto min_val = vec | min_element(std::less<std::string>());
  EXPECT_EQ(min_val, "apple");
}

TEST(MinElementTest, SingleElementVector) {
  std::vector<int> vec = {10};
  auto min_val = vec | min_element(std::less<int>());
  EXPECT_EQ(min_val, 10);
}

TEST(SortTest, IntVectorAsc) {
  std::vector<int> vec = {3, 1, 4, 1, 5, 9};
  auto sorted_vec = vec | sort(std::less<int>());
  EXPECT_EQ(sorted_vec, std::vector<int>({1, 1, 3, 4, 5, 9}));
}

TEST(SortTest, DoubleVectorDesc) {
  std::vector<double> vec = {2.3, 1.5, 3.6, 1.2};
  auto sorted_vec = vec | sort(std::greater<double>());
  EXPECT_EQ(sorted_vec, std::vector<double>({3.6, 2.3, 1.5, 1.2}));
}

TEST(SortTest, StringVectorAsc) {
  std::vector<std::string> vec = {"cherry", "banana", "apple"};
  auto sorted_vec = vec | sort(std::less<std::string>());
  EXPECT_EQ(sorted_vec, std::vector<std::string>({"apple", "banana", "cherry"}));
}

TEST(SortTest, SingleElementVector) {
  std::vector<int> vec = {42};
  auto sorted_vec = vec | sort(std::less<int>());
  EXPECT_EQ(sorted_vec, std::vector<int>({42}));
}

TEST(DistinctTest, IntVector) {
  std::vector<int> vec = {3, 1, 4, 1, 5, 9, 3};
  auto distinct_vec = vec | distinct();
  EXPECT_EQ(distinct_vec, std::vector<int>({3, 1, 4, 5, 9}));
}

TEST(DistinctTest, DoubleVector) {
  std::vector<double> vec = {2.3, 1.5, 2.3, 3.6, 1.2};
  auto distinct_vec = vec | distinct();
  EXPECT_EQ(distinct_vec, std::vector<double>({2.3, 1.5, 3.6, 1.2}));
}

TEST(DistinctTest, StringVector) {
  std::vector<std::string> vec = {"apple", "banana", "apple", "cherry"};
  auto distinct_vec = vec | distinct();
  EXPECT_EQ(distinct_vec, std::vector<std::string>({"apple", "banana", "cherry"}));
}

TEST(DistinctTest, EmptyVector) {
  std::vector<int> vec = {};
  auto distinct_vec = vec | distinct();
  EXPECT_TRUE(distinct_vec.empty());
}

TEST(AdapterChainTest, SortAndDistinct) {
  std::vector<int> vec = {3, 1, 4, 1, 5, 9, 3};
  auto result = vec | sort(std::less<int>()) | distinct();
  EXPECT_EQ(result, std::vector<int>({1, 3, 4, 5, 9}));
}

TEST(AdapterChainTest, DistinctAndSortDesc) {
  std::vector<double> vec = {2.3, 1.5, 2.3, 3.6, 1.2};
  auto result = vec | distinct() | sort(std::greater<double>());
  EXPECT_EQ(result, std::vector<double>({3.6, 2.3, 1.5, 1.2}));
}

TEST(AdapterChainTest, MinElementInDistinct) {
  std::vector<std::string> vec = {"apple", "banana", "apple", "cherry"};
  auto result = vec | distinct() | min_element(std::less<std::string>());
  EXPECT_EQ(result, "apple");
}

TEST(AdapterChainTest, SortDistinctMinElement) {
  std::vector<int> vec = {7, 3, 5, 3, 7, 9, 2};
  auto result = vec | sort(std::less<int>()) | distinct() | min_element(std::less<int>());
  EXPECT_EQ(result, 2);
}

TEST(FirstTest, IntVector) {
  std::vector<int> vec = {3, 1, 4, 1, 5, 9};
  auto first_val = vec | first();
  EXPECT_EQ(first_val, 3);
}

TEST(FirstTest, DoubleVector) {
  std::vector<double> vec = {2.3, 1.5, 3.6, 1.2};
  auto first_val = vec | first();
  EXPECT_DOUBLE_EQ(first_val, 2.3);
}

TEST(FirstTest, StringVector) {
  std::vector<std::string> vec = {"apple", "banana", "cherry"};
  auto first_val = vec | first();
  EXPECT_EQ(first_val, "apple");
}

TEST(FirstTest, EmptyVector) {
  std::vector<int> vec;
  EXPECT_THROW(vec | first(), std::out_of_range);
}

TEST(LastTest, IntVector) {
  std::vector<int> vec = {3, 1, 4, 1, 5, 9};
  auto last_val = vec | last();
  EXPECT_EQ(last_val, 9);
}

TEST(LastTest, DoubleVector) {
  std::vector<double> vec = {2.3, 1.5, 3.6, 1.2};
  auto last_val = vec | last();
  EXPECT_DOUBLE_EQ(last_val, 1.2);
}

TEST(LastTest, StringVector) {
  std::vector<std::string> vec = {"apple", "banana", "cherry"};
  auto last_val = vec | last();
  EXPECT_EQ(last_val, "cherry");
}

TEST(LastTest, EmptyVector) {
  std::vector<int> vec;
  EXPECT_THROW(vec | last(), std::out_of_range);
}

TEST(AdapterChainTest, FirstAfterSortDistinct) {
  std::vector<int> vec = {3, 1, 4, 1, 5, 9, 3};
  auto result = vec | sort(std::less<int>()) | distinct() | first();
  EXPECT_EQ(result, 1);
}

TEST(AdapterChainTest, LastAfterDistinctSort) {
  std::vector<double> vec = {2.3, 1.5, 2.3, 3.6, 1.2};
  auto result = vec | distinct() | sort(std::greater<double>()) | last();
  EXPECT_EQ(result, 1.2);
}

TEST(AdapterChainTest, FirstElementStringVector) {
  std::vector<std::string> vec = {"apple", "banana", "apple", "cherry"};
  auto result = vec | distinct() | first();
  EXPECT_EQ(result, "apple");
}

TEST(AdapterChainTest, LastElementStringVector) {
  std::vector<std::string> vec = {"apple", "banana", "cherry", "banana"};
  auto result = vec | distinct() | last();
  EXPECT_EQ(result, "cherry");
}

TEST(IntersectTest, IntVector) {
  std::vector<int> vec1 = {1, 2, 3, 4};
  std::vector<int> vec2 = {3, 4, 5, 6};
  auto result = std::vector<int>() | intersect(vec1, vec2);
  EXPECT_EQ(result, std::vector<int>({4, 3}));
}

TEST(IntersectTest, DoubleVector) {
  std::vector<double> vec1 = {1.1, 2.2, 3.3, 4.4};
  std::vector<double> vec2 = {3.3, 4.4, 5.5, 6.6};
  auto result = std::vector<double>() | intersect(vec1, vec2);
  EXPECT_EQ(result, std::vector<double>({4.4, 3.3}));
}

TEST(IntersectTest, StringVector) {
  std::vector<std::string> vec1 = {"apple", "banana", "cherry"};
  std::vector<std::string> vec2 = {"banana", "cherry", "date"};
  auto result = std::vector<std::string>() | intersect(vec1, vec2);
  EXPECT_EQ(result, std::vector<std::string>({"cherry", "banana"}));
}

TEST(IntersectTest, NoIntersection) {
  std::vector<int> vec1 = {1, 2, 3};
  std::vector<int> vec2 = {4, 5, 6};
  auto result = std::vector<int>() | intersect(vec1, vec2);
  EXPECT_TRUE(result.empty());
}

TEST(IntersectTest, EmptyVector) {
  std::vector<int> vec1 = {1, 2, 3};
  std::vector<int> vec2 = {};
  auto result = std::vector<int>() | intersect(vec1, vec2);
  EXPECT_TRUE(result.empty());
}