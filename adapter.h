#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <unordered_set>

template<typename Derived>
class Adapter {
 public:
  template<typename Container>
  auto operator()(const Container& container) const {

    return static_cast<const Derived*>(this)->apply(container);
  }
};

template<typename Func>
class Transform : public Adapter<Transform<Func>> {
 public:
  explicit Transform(Func func) : func(func) {}

  template<typename Container>
  auto apply(const Container& container) const {
    Container result;
    result.reserve(container.size());
    for (const auto& elem : container) {
      result.push_back(func(elem));
    }

    return result;
  }

 private:
  Func func;
};

template<typename Func>
class Filter : public Adapter<Filter<Func>> {
 public:
  explicit Filter(Func func) : func(func) {}

  template<typename Container>
  auto apply(const Container& container) const {
    Container result;
    for (const auto& elem : container) {
      if (func(elem)) {
        result.push_back(elem);
      }
    }

    return result;
  }

 private:
  Func func;
};

class Take : public Adapter<Take> {
 public:
  explicit Take(size_t n) : n(n) {}

  template<typename Container>
  auto apply(const Container& container) const {
    Container result;
    size_t count = 0;
    for (const auto& elem : container) {
      if (count++ >= n) break;
      result.push_back(elem);
    }

    return result;
  }

 private:
  size_t n;
};

class Drop : public Adapter<Drop> {
 public:
  explicit Drop(size_t n) : n(n) {}

  template<typename Container>
  auto apply(const Container& container) const {
    Container result;
    size_t count = 0;
    for (const auto& elem : container) {
      if (count++ < n) continue;
      result.push_back(elem);
    }

    return result;
  }

 private:
  size_t n;
};

class Reverse : public Adapter<Reverse> {
 public:
  template<typename Container>
  auto apply(const Container& container) const {
    Container result(container.rbegin(), container.rend());

    return result;
  }
};

class Keys : public Adapter<Keys> {
 public:
  template<typename Container>
  auto apply(const Container& container) const {
    std::vector<typename Container::key_type> result;
    result.reserve(container.size());
    for (const auto& elem : container) {
      result.push_back(elem.first);
    }
    std::reverse(result.begin(), result.end());

    return result;
  }
};

class Values : public Adapter<Values> {
 public:
  template<typename Container>
  auto apply(const Container& container) const {
    std::vector<typename Container::mapped_type> result;
    result.reserve(container.size());
    for (const auto& elem : container) {
      result.push_back(elem.second);
    }
    std::reverse(result.begin(), result.end());

    return result;
  }
};

// Позволяет превращать многомерный контейнер в линейный
template<typename Container>
class Flatten : public Adapter<Flatten<Container>> {
 public:
  Flatten() {}

  template<typename InnerContainer>
  auto apply(const std::vector<InnerContainer>& container) const {
    using ValueType = typename InnerContainer::value_type;
    std::vector<ValueType> result;

    for (const auto& inner : container) {
      result.insert(result.end(), inner.begin(), inner.end());
    }

    return result;
  }
};

template<typename Container>
auto flatten() {

  return Flatten<Container>();
}

// Пакуем две коллекции в пары ключ значение
template<typename Container1, typename Container2>
class Zip : public Adapter<Zip<Container1, Container2>> {
 public:
  Zip(const Container1& c1, const Container2& c2) : c1(c1), c2(c2) {}

  template<typename Unused>
  auto apply(const Unused&) const {
    using ValueType1 = typename Container1::value_type;
    using ValueType2 = typename Container2::value_type;
    using ResultType = std::vector<std::pair<ValueType1, ValueType2>>;

    ResultType result;
    auto it1 = c1.begin();
    auto it2 = c2.begin();

    while (it1 != c1.end() && it2 != c2.end()) {
      result.emplace_back(*it1, *it2);
      ++it1;
      ++it2;
    }

    return result;
  }

 private:
  const Container1& c1;
  const Container2& c2;
};

template<typename Container1, typename Container2>
auto zip(const Container1& c1, const Container2& c2) {

  return Zip<Container1, Container2>(c1, c2);
}

//Делаем циклической коллекцию n раз
template<typename Container>
class Cycle : public Adapter<Cycle<Container>> {
 public:
  explicit Cycle(size_t n) : n(n) {}

  auto apply(const Container& container) const {
    using ValueType = typename Container::value_type;
    std::vector<ValueType> result;

    for (size_t i = 0; i < n; ++i) {
      result.insert(result.end(), container.begin(), container.end());
    }

    return result;
  }

 private:
  size_t n;
};

template<typename Container>
auto cycle(size_t n) {

  return Cycle<Container>(n);
}

// Максимальный элемент
template<typename Comparator>
class MaxElement : public Adapter<MaxElement<Comparator>> {
 public:
  explicit MaxElement(Comparator comp) : comp(comp) {}

  template<typename Container>
  auto apply(const Container& container) const {

    return *std::max_element(container.begin(), container.end(), comp);
  }

 private:
  Comparator comp;
};

template<typename Comparator>
auto max_element(Comparator comp) {

  return MaxElement<Comparator>(comp);
}

//Сортировка по некоторому признаку
template<typename Comparator = std::less<>>
class Sort : public Adapter<Sort<Comparator>> {
 public:
  explicit Sort(Comparator comp = Comparator()) : comp(comp) {}

  template<typename Container>
  auto apply(Container container) const {
    std::sort(container.begin(), container.end(), comp);

    return container;
  }

 private:
  Comparator comp;
};

template<typename Comparator = std::less<>>
auto sort(Comparator comp = Comparator()) {

  return Sort<Comparator>(comp);
}

//минимальный элемент
template<typename Comparator>
class MinElement : public Adapter<MinElement<Comparator>> {
 public:
  explicit MinElement(Comparator comp) : comp(comp) {}

  template<typename Container>
  auto apply(const Container& container) const {

    return *std::min_element(container.begin(), container.end(), comp);
  }

 private:
  Comparator comp;
};

template<typename Comparator>
auto min_element(Comparator comp) {

  return MinElement<Comparator>(comp);
}

//Удаляет дубликаты из коллекции
class Distinct : public Adapter<Distinct> {
 public:
  template<typename Container>
  auto apply(const Container& container) const {
    Container result;
    std::unordered_set<typename Container::value_type> seen;
    for (const auto& elem : container) {
      if (seen.insert(elem).second) {
        result.push_back(elem);
      }
    }

    return result;
  }
};

auto distinct() {

  return Distinct();
}

//Первый элемент в коллекции
class First : public Adapter<First> {
 public:
  template<typename Container>
  auto apply(const Container& container) const {
    if (container.empty()) {
      throw std::out_of_range("Container is empty");
    }

    return container.front();
  }
};

auto first() {

  return First();
}

//Последний элемент в коллекции
class Last : public Adapter<Last> {
 public:
  template<typename Container>
  auto apply(const Container& container) const {
    if (container.empty()) {
      throw std::out_of_range("Container is empty");
    }

    return container.back();
  }
};

auto last() {

  return Last();
}

//Пересечение двух коллекций
template<typename Container1, typename Container2>
class Intersect : public Adapter<Intersect<Container1, Container2>> {
 public:
  Intersect(const Container1& c1, const Container2& c2) : c1(c1), c2(c2) {}

  auto apply(const std::vector<typename Container1::value_type>&) const {
    using ValueType = typename Container1::value_type;
    std::unordered_set<ValueType> set1(c1.begin(), c1.end());
    std::unordered_set<ValueType> set2(c2.begin(), c2.end());

    std::vector<ValueType> result;
    for (const auto& elem : set1) {
      if (set2.find(elem) != set2.end()) {
        result.push_back(elem);
      }
    }

    return result;
  }

 private:
  const Container1& c1;
  const Container2& c2;
};

template<typename Container1, typename Container2>
auto intersect(const Container1& c1, const Container2& c2) {

  return Intersect<Container1, Container2>(c1, c2);
}

// Оператор для цепочки адаптеров
template<typename Container, typename Adapter>
auto operator|(const Container& container, const Adapter& adapter) {

  return adapter(container);
}
