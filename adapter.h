#include <iterator>
#include <type_traits>
#include <algorithm>
#include <vector>
#include <map>

//Filter
template<typename Iterator, typename Predicate>
class FilterIterator {
 public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename std::iterator_traits<Iterator>::value_type;
  using difference_type = typename std::iterator_traits<Iterator>::difference_type;
  using pointer = typename std::iterator_traits<Iterator>::pointer;
  using reference = typename std::iterator_traits<Iterator>::reference;

  FilterIterator(Iterator current, Iterator end, Predicate pred, bool is_end = false)
      : current_(current), end_(end), pred_(pred), is_end_(is_end) {
    if (!is_end_ && current_ != end_ && !pred_(*current_)) {
      ++(*this);
    }
  }

  reference operator*() const { return *current_; }
  pointer operator->() { return &(*current_); }
  FilterIterator& operator++() {
    do {
      ++current_;
    } while (current_ != end_ && !pred_(*current_));
    return *this;
  }
  FilterIterator operator++(int) {
    FilterIterator tmp = *this;
    ++(*this);
    return tmp;
  }
  FilterIterator& operator--() {
    do {
      --current_;
    } while (current_ != end_ && !pred_(*current_));
    return *this;
  }
  FilterIterator operator--(int) {
    FilterIterator tmp = *this;
    --(*this);
    return tmp;
  }
  bool operator==(const FilterIterator& other) const { return current_ == other.current_; }
  bool operator!=(const FilterIterator& other) const { return !(*this == other); }

 private:
  Iterator current_, end_;
  Predicate pred_;
  bool is_end_;
};

//Filter
template<typename Container, typename Predicate>
class Filter {
 public:
  using iterator = FilterIterator<typename Container::iterator, Predicate>;

  Filter(Container& c, Predicate p) : container(c), pred(p) {}

  iterator begin() { return iterator(container.begin(), container.end(), pred); }
  iterator end() { return iterator(container.end(), container.end(), pred, true); }
  size_t size() {
    size_t count = 0;
    for (auto it = begin(); it != end(); ++it) {
      ++count;
    }
    return count;
  }

 private:
  Container& container;
  Predicate pred;
};

//Transform
template<typename Iterator, typename Function>
class TransformIterator {
 public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = decltype(std::declval<Function>()(*std::declval<Iterator>()));  // Используем decltype для определения типа результата функции
  using difference_type = typename std::iterator_traits<Iterator>::difference_type;
  using pointer = value_type*;
  using reference = value_type;

  TransformIterator(Iterator current, Iterator end, Function func, bool is_end = false)
      : current_(current), end_(end), func_(func) {}

  value_type operator*() const { return func_(*current_); }
  TransformIterator& operator++() {
    if (current_ != end_) {
      ++current_;
    }
    return *this;
  }
  TransformIterator operator++(int) {
    TransformIterator tmp = *this;
    ++(*this);
    return tmp;
  }
  TransformIterator& operator--() {
    --current_;
    return *this;
  }
  TransformIterator operator--(int) {
    TransformIterator tmp = *this;
    --(*this);
    return tmp;
  }
  bool operator==(const TransformIterator& other) const { return current_ == other.current_; }
  bool operator!=(const TransformIterator& other) const { return !(*this == other); }

 private:
  Iterator current_, end_;
  Function func_;
};

template<typename Container, typename Function>
class Transform {
 public:
  using iterator = TransformIterator<typename Container::iterator, Function>;

  Transform(Container& c, Function f) : container(c), func(f) {}

  iterator begin() { return iterator(container.begin(), container.end(), func); }
  iterator end() { return iterator(container.end(), container.end(), func); }
  size_t size() {
    return std::distance(begin(), end());
  }

 private:
  Container& container;
  Function func;
};

//Take
template<typename Iterator>
class TakeIterator {
 public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename std::iterator_traits<Iterator>::value_type;
  using difference_type = typename std::iterator_traits<Iterator>::difference_type;
  using pointer = typename std::iterator_traits<Iterator>::pointer;
  using reference = typename std::iterator_traits<Iterator>::reference;

  TakeIterator(Iterator current, Iterator end, size_t count)
      : current_(current), end_(end), count_(count), start_(0) {}

  reference operator*() const { return *current_; }
  pointer operator->() { return &(*current_); }
  TakeIterator& operator++() {
    if (start_ < count_ && current_ != end_) {
      ++current_;
      ++start_;
    }
    return *this;
  }
  TakeIterator operator++(int) {
    TakeIterator tmp = *this;
    ++(*this);
    return tmp;
  }
  TakeIterator& operator--() {
    if (start_ > 0 && current_ != end_) {
      --current_;
      --start_;
    }
    return *this;
  }
  TakeIterator operator--(int) {
    TakeIterator tmp = *this;
    --(*this);
    return tmp;
  }
  bool operator==(const TakeIterator& other) const { return (current_ == other.current_); }
  bool operator!=(const TakeIterator& other) const { return !(*this == other); }

 private:
  Iterator current_, end_;
  size_t count_, start_;
};

//Take
template<typename Container>
class Take {
 public:
  using iterator = TakeIterator<typename Container::iterator>;

  Take(Container& c, size_t count)
      : container(c), count(count) {}

  iterator begin() { return iterator(container.begin(), container.end(), count); }
  iterator end() {
    auto start = container.begin();
    std::advance(start,
                 std::min(count, container.size()));  // Перемещаем итератор на минимум из count или размера контейнера
    return iterator(start, container.end(), 0);  // Передаем 0, так как "end" должен обозначать конец диапазона
  }

 private:
  Container& container;
  size_t count;
};

//Drop
template<typename Iterator>
class DropIterator {
 public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename std::iterator_traits<Iterator>::value_type;
  using difference_type = typename std::iterator_traits<Iterator>::difference_type;
  using pointer = typename std::iterator_traits<Iterator>::pointer;
  using reference = typename std::iterator_traits<Iterator>::reference;

  DropIterator(Iterator current, Iterator end, size_t count)
      : current_(current), end_(end) {
    for (size_t i = 0; i < count && current_ != end_; ++i) {
      ++current_;
    }
  }

  reference operator*() const { return *current_; }
  pointer operator->() { return &(*current_); }
  DropIterator& operator++() {
    if (current_ != end_) {
      ++current_;
    }
    return *this;
  }
  DropIterator operator++(int) {
    DropIterator tmp = *this;
    ++(*this);
    return tmp;
  }
  DropIterator& operator--() {
    --current_;
    return *this;
  }
  DropIterator operator--(int) {
    DropIterator tmp = *this;
    --(*this);
    return tmp;
  }
  bool operator==(const DropIterator& other) const { return current_ == other.current_; }
  bool operator!=(const DropIterator& other) const { return !(*this == other); }

 private:
  Iterator current_, end_;
};

// Drop
template<typename Container>
class Drop {
 public:
  using iterator = DropIterator<typename Container::iterator>;

  Drop(Container& c, size_t count)
      : container(c), count(count) {}

  iterator begin() { return iterator(container.begin(), container.end(), count); }
  iterator end() { return iterator(container.end(), container.end(), 0); }
  size_t size() {
    return std::distance(begin(), end());
  }

 private:
  Container& container;
  size_t count;
};

//Reverse
template<typename Container>
class Reverse {
 public:
  using iterator = std::reverse_iterator<typename Container::iterator>;

  Reverse(Container& c)
      : container(c) {}

  iterator begin() { return std::make_reverse_iterator(container.end()); }
  iterator end() { return std::make_reverse_iterator(container.begin()); }
  size_t size() {
    return std::distance(begin(), end());
  }

 private:
  Container& container;
};

// Пример итератора для Keys
template<typename Iterator>
class KeysIterator {
 public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Iterator::value_type::first_type;
  using difference_type = typename std::iterator_traits<Iterator>::difference_type;
  using pointer = const value_type*;
  using reference = const value_type&;

  explicit KeysIterator(Iterator it) : it_(it) {}

  reference operator*() const { return it_->first; }
  pointer operator->() const { return &(it_->first); }
  KeysIterator& operator++() {
    ++it_;
    return *this;
  }
  KeysIterator operator++(int) {
    KeysIterator tmp = *this;
    ++(*this);
    return tmp;
  }
  KeysIterator& operator--() {
    --it_;
    return *this;
  }
  KeysIterator operator--(int) {
    KeysIterator tmp = *this;
    --(*this);
    return tmp;
  }
  bool operator==(const KeysIterator& other) const { return it_ == other.it_; }
  bool operator!=(const KeysIterator& other) const { return it_ != other.it_; }

 private:
  Iterator it_;
};

//Keys
template<typename Container>
class Keys {
 public:
  using internal_iterator = typename Container::iterator;
  using iterator = KeysIterator<internal_iterator>;

  Keys(Container& c) : container_(c) {}

  iterator begin() { return iterator(container_.begin()); }
  iterator end() { return iterator(container_.end()); }

 private:
  Container& container_;
};

//Values
template<typename Iterator>
class ValuesIterator {
 public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Iterator::value_type::second_type;
  using difference_type = typename std::iterator_traits<Iterator>::difference_type;
  using pointer = const value_type*;
  using reference = const value_type&;

  explicit ValuesIterator(Iterator it) : it_(it) {}

  reference operator*() const { return it_->second; }
  pointer operator->() const { return &(it_->second); }
  ValuesIterator& operator++() {
    ++it_;
    return *this;
  }
  ValuesIterator operator++(int) {
    ValuesIterator tmp = *this;
    ++(*this);
    return tmp;
  }
  ValuesIterator& operator--() {
    --it_;
    return *this;
  }
  ValuesIterator operator--(int) {
    ValuesIterator tmp = *this;
    --(*this);
    return tmp;
  }
  bool operator==(const ValuesIterator& other) const { return it_ == other.it_; }
  bool operator!=(const ValuesIterator& other) const { return it_ != other.it_; }

 private:
  Iterator it_;
};

template<typename Container>
class Values {
 public:
  using internal_iterator = typename Container::iterator;
  using iterator = ValuesIterator<internal_iterator>;

  Values(Container& c) : container_(c) {}

  iterator begin() { return iterator(container_.begin()); }
  iterator end() { return iterator(container_.end()); }

 private:
  Container& container_;
};
