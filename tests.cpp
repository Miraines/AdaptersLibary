#include <gtest/gtest.h>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include "adapter.h"

// Тесты для Filter
TEST(FilterTest, OddNumbers) {
  std::vector<int> v = {1, 2, 3, 4, 5};
  auto result = Filter(v, [](int x) { return x % 2 != 0; });
  std::vector<int> expected = {1, 3, 5};
  ASSERT_TRUE(std::equal(result.begin(), result.end(), expected.begin()));
}

TEST(FilterTest, EmptyContainer) {
  std::vector<int> v = {};
  auto result = Filter(v, [](int x) { return x % 2 != 0; });
  ASSERT_TRUE(result.begin() == result.end());
}

TEST(FilterTest, AllFilteredOut) {
  std::vector<int> v = {2, 4, 6, 8};
  auto result = Filter(v, [](int x) { return x % 2 != 0; });
  ASSERT_TRUE(result.begin() == result.end());
}

// Тест для Transform
TEST(TransformTest, SquareOfNumbers) {
  std::vector<int> numbers = {1, 2, 3, 4};
  std::vector<int> expected = {1, 4, 9, 16};

  auto result = Transform(numbers, [](int x) { return x * x; });
  ASSERT_TRUE(std::equal(result.begin(), result.end(), expected.begin()));
}

TEST(TransformTest, NegateNumbers) {
  std::vector<int> numbers = {1, -2, 3, -4};
  std::vector<int> expected = {-1, 2, -3, 4};

  auto result = Transform(numbers, [](int x) { return -x; });
  ASSERT_TRUE(std::equal(result.begin(), result.end(), expected.begin()));
}

TEST(TransformTest, IncrementNumbers) {
  std::vector<int> numbers = {0, 1, 2, 3};
  std::vector<int> expected = {1, 2, 3, 4};

  auto result = Transform(numbers, [](int x) { return x + 1; });
  ASSERT_TRUE(std::equal(result.begin(), result.end(), expected.begin()));
}

TEST(TransformTest, EmptyVector) {
  std::vector<int> numbers;
  auto result = Transform(numbers, [](int x) { return x * x; });
  ASSERT_TRUE(result.begin() == result.end());
}

// Тест для Take
TEST(TakeTest, TakeFirstTwoNumbers) {
  std::vector<int> numbers = {1, 2, 3, 4};
  std::vector<int> expected = {1, 2};

  auto taken = Take(numbers, 2); // Сохраняем результат вызова Take в переменную
  ASSERT_TRUE(std::equal(taken.begin(), taken.end(), expected.begin()));
}


TEST(TakeTest, TakeMoreThanExists) {
  std::vector<int> numbers = {1, 2};
  auto result = Take(numbers, 5); // Пробуем взять больше элементов, чем есть
  ASSERT_EQ(std::distance(result.begin(), result.end()), 2); // Ожидаем, что получим только 2 элемента
}

TEST(TakeTest, TakeNone) {
  std::vector<int> numbers = {1, 2, 3, 4};
  auto result = Take(numbers, 0); // Не берем ни одного элемента
  ASSERT_TRUE(result.begin() == result.end()); // Ожидаем пустой диапазон
}

TEST(TakeTest, TakeExactNumber) {
  std::vector<int> numbers = {1, 2, 3, 4};
  std::vector<int> expected = {1, 2};
  auto result = Take(numbers, 2); // Берем ровно два элемента
  ASSERT_TRUE(std::equal(result.begin(), result.end(), expected.begin()));
}

// Тест для Drop
TEST(DropTest, DropFirstTwoNumbers) {
  std::vector<int> numbers = {1, 2, 3, 4};
  std::vector<int> expected = {3, 4};

  auto result = Drop(numbers, 2);
  ASSERT_TRUE(std::equal(result.begin(), result.end(), expected.begin()));
}

TEST(DropTest, DropNone) {
  std::vector<int> numbers = {1, 2, 3, 4};
  auto result = Drop(numbers, 0); // Не убираем ни одного элемента
  ASSERT_EQ(std::distance(result.begin(), result.end()), 4); // Ожидаем, что количество элементов не изменится
}

TEST(DropTest, DropAll) {
  std::vector<int> numbers = {1, 2, 3, 4};
  auto result = Drop(numbers, 4); // Убираем все элементы
  ASSERT_TRUE(result.begin() == result.end()); // Ожидаем пустой диапазон
}

TEST(DropTest, DropMoreThanExists) {
  std::vector<int> numbers = {1, 2};
  auto result = Drop(numbers, 5); // Пробуем убрать больше элементов, чем есть
  ASSERT_TRUE(result.begin() == result.end()); // Ожидаем пустой диапазон
}

// Тесты для Reverse
TEST(ReverseTest, ReverseVector) {
  std::vector<int> numbers = {1, 2, 3, 4};
  std::vector<int> expected = {4, 3, 2, 1};

  auto reversed = Reverse(numbers);
  ASSERT_TRUE(std::equal(reversed.begin(), reversed.end(), expected.begin()));
}

TEST(ReverseTest, EmptyContainer) {
  std::vector<int> numbers;

  auto reversed = Reverse(numbers);
  ASSERT_TRUE(reversed.begin() == reversed.end());
}

TEST(ReverseTest, SingleElement) {
  std::vector<int> numbers = {1};

  auto reversed = Reverse(numbers);
  ASSERT_EQ(*reversed.begin(), 1);
}

TEST(ReverseTest, ReverseString) {
  std::string str = "hello";
  std::string expected = "olleh";

  auto reversed = Reverse(str);
  ASSERT_TRUE(std::equal(reversed.begin(), reversed.end(), expected.begin()));
}

//Тесты для Keys
TEST(KeysTest, NonEmptyMap) {
  std::map<int, std::string> m = {{1, "one"}, {2, "two"}, {3, "three"}};
  auto keys = Keys(m);
  std::vector<int> expected_keys = {1, 2, 3};
  std::vector<int> actual_keys;
  for (auto key : keys) {
    actual_keys.push_back(key);
  }
  EXPECT_EQ(actual_keys, expected_keys);
}

TEST(KeysTest, EmptyMap) {
  std::map<int, std::string> m;
  auto keys = Keys(m);
  EXPECT_EQ(keys.begin(), keys.end());
}

TEST(KeysTest, NegativeKeys) {
  std::map<int, std::string> m = {{-1, "minus one"}, {-2, "minus two"}};
  auto keys = Keys(m);
  std::vector<int> expected_keys = {-2, -1};
  std::vector<int> actual_keys;
  for (auto key : keys) {
    actual_keys.push_back(key);
  }
  EXPECT_EQ(actual_keys, expected_keys);
}

TEST(KeysTest, DuplicateKeys) {
  std::multimap<int, std::string> m = {{1, "one"}, {1, "uno"}};
  auto keys = Keys(m);
  std::vector<int> expected_keys = {1, 1};
  std::vector<int> actual_keys;
  for (auto key : keys) {
    actual_keys.push_back(key);
  }
  EXPECT_EQ(actual_keys, expected_keys);
}

//Тесты для Values
TEST(ValuesTest, NonEmptyMap) {
  std::map<int, std::string> m = {{1, "one"}, {2, "two"}, {3, "three"}};
  auto values = Values(m);
  std::vector<std::string> expected_values = {"one", "two", "three"};
  std::vector<std::string> actual_values;
  for (const auto& value : values) {
    actual_values.push_back(value);
  }
  EXPECT_EQ(actual_values, expected_values);
}

TEST(ValuesTest, EmptyMap) {
  std::map<int, std::string> m;
  auto values = Values(m);
  EXPECT_EQ(values.begin(), values.end());
}

TEST(ValuesTest, SpecialCharacters) {
  std::map<int, std::string> m = {{1, "one&two"}, {2, "three>four"}};
  auto values = Values(m);
  std::vector<std::string> expected_values = {"one&two", "three>four"};
  std::vector<std::string> actual_values;
  for (const auto& value : values) {
    actual_values.push_back(value);
  }
  EXPECT_EQ(actual_values, expected_values);
}

TEST(ValuesTest, LargeValues) {
  std::string large_value(1000, 'x');
  std::map<int, std::string> m = {{1, large_value}};
  auto values = Values(m);
  std::vector<std::string> actual_values;
  for (const auto& value : values) {
    actual_values.push_back(value);
  }
  EXPECT_EQ(actual_values.size(), 1);
  EXPECT_EQ(actual_values[0], large_value);
}

//Комбинированные тесты
TEST(AdapterTests, FilterTransformTest) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6};
  std::vector<int> results;

  auto filtered = Filter(v, [](int i) { return i % 2; });
  auto transformed = Transform(filtered, [](int i) { return i * i; });

  std::copy(transformed.begin(), transformed.end(), std::back_inserter(results));

  std::vector<int> expected = {1, 9, 25};
  EXPECT_EQ(results, expected);
}

TEST(AdapterTests, DropTransformTest) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6};
  std::vector<int> results;

  auto dropped = Drop(v, 2);
  auto transformed = Transform(dropped, [](int i) { return i * 3; });

  std::copy(transformed.begin(), transformed.end(), std::back_inserter(results));

  std::vector<int> expected = {9, 12, 15, 18};
  EXPECT_EQ(results, expected);
}

TEST(AdapterTests, FilterTransformTakeTest) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};
  std::vector<int> results;

  auto filtered = Filter(v, [](int i) { return i % 2 == 0; });
  auto transformed = Transform(filtered, [](int i) { return i * 2; });
  auto taken = Take(transformed, 2);

  std::copy(taken.begin(), taken.end(), std::back_inserter(results));

  std::vector<int> expected = {4, 8};
  EXPECT_EQ(results, expected);
}

TEST(AdapterTests, ValuesTransformTest) {
  std::map<int, int> m = {{1, 2}, {2, 3}, {3, 4}};
  std::vector<int> values;

  auto values_view = Values(m);
  auto incremented_values = Transform(values_view, [](int v) { return v + 1; });

  std::copy(incremented_values.begin(), incremented_values.end(), std::back_inserter(values));

  std::vector<int> expected = {3, 4, 5};
  EXPECT_EQ(values, expected);
}

TEST(AdapterTests, KeysReverseTest) {
  std::map<int, std::string> m = {{1, "one"}, {2, "two"}, {3, "three"}};
  std::vector<int> keys;

  auto keys_view = Keys(m);
  auto reversed_keys = Reverse(keys_view);
  std::copy(reversed_keys.begin(), reversed_keys.end(), std::back_inserter(keys));

  std::vector<int> expected_keys = {3, 2, 1};
  EXPECT_EQ(keys, expected_keys);
}

TEST(AdapterTests, DropTransformFilterReverseVectorTest) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};
  std::vector<int> results;

  auto dropped = Drop(v, 2);
  auto filtered = Filter(dropped, [](int i) { return i % 2 == 0; });
  auto transformed = Transform(filtered, [](int i) { return i * 2; });
  auto reversed = Reverse(transformed);

  std::copy(reversed.begin(), reversed.end(), std::back_inserter(results));

  std::vector<int> expected = {16, 12, 8};
  EXPECT_EQ(results, expected);
}

TEST(AdapterTests, ReverseTakeTransformTest) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};
  std::vector<int> results;

  auto reversed = Reverse(v);
  auto taken = Take(reversed, 4);
  auto transformed = Transform(taken, [](int i) { return i * 2; });

  std::copy(transformed.begin(), transformed.end(), std::back_inserter(results));

  std::vector<int> expected = {16, 14, 12, 10};
  EXPECT_EQ(results, expected);
}

TEST(AdapterTests, TransformTakeKeysMapTest) {
  std::map<int, int> m = {{1, 10}, {2, 20}, {3, 30}, {4, 40}};
  std::vector<int> results;

  auto keys = Keys(m);
  auto transformed = Transform(keys, [](int key) { return key * key; });
  auto taken = Take(transformed, 3);

  std::copy(taken.begin(), taken.end(), std::back_inserter(results));

  std::vector<int> expected = {1, 4, 9};
  EXPECT_EQ(results, expected);
}

TEST(AdapterTests, DoubleFilterTransformTest) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  std::vector<int> results;

  auto first_filter = Filter(v, [](int i) { return i % 2 == 0; });
  auto second_filter = Filter(first_filter, [](int i) { return i > 4; });
  auto transformed = Transform(second_filter, [](int i) { return i * 2; });

  std::copy(transformed.begin(), transformed.end(), std::back_inserter(results));

  std::vector<int> expected = {12, 16, 20};
  EXPECT_EQ(results, expected);
}

TEST(AdapterTests, DropTakeFilterTest) {
  std::vector<int> v = {10, 20, 30, 40, 50, 60};
  std::vector<int> results;

  auto dropped = Drop(v, 1);
  auto taken = Take(dropped, 3);
  auto filtered = Filter(taken, [](int i) { return i % 20 == 0; });

  std::copy(filtered.begin(), filtered.end(), std::back_inserter(results));

  std::vector<int> expected = {20, 40};
  EXPECT_EQ(results, expected);
}

TEST(AdapterTests, TransformTakeDropTest) {
  std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  std::vector<int> results;

  auto transformed = Transform(v, [](int i) { return i + 10; });
  auto taken = Take(transformed, 5);
  auto dropped = Drop(taken, 2);

  std::copy(dropped.begin(), dropped.end(), std::back_inserter(results));

  std::vector<int> expected = {13, 14, 15};
  EXPECT_EQ(results, expected);
}

TEST(AdapterTests, DoubleDropDoubleTakeTest) {
  std::vector<int> v = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<int> results;

  auto dropped_once = Drop(v, 2);
  auto taken_once = Take(dropped_once, 5);
  auto dropped_twice = Drop(taken_once, 1);
  auto taken_twice = Take(dropped_twice, 3);

  std::copy(taken_twice.begin(), taken_twice.end(), std::back_inserter(results));

  // Expected: [3, 4, 5]
  std::vector<int> expected = {3, 4, 5};
  EXPECT_EQ(results, expected);
}
