#include <iterator>
#include <type_traits>
#include <functional>
#include <utility>
#include <vector>

template<typename Container, typename UnaryFunction>
class TransformAdapter {
 private:
  const Container& container;
  UnaryFunction function;

 public:
  TransformAdapter(const Container& c, UnaryFunction f) : container(c), function(f) {}

  class iterator {
   private:
    typename Container::const_iterator current;
    UnaryFunction function;

   public:
    using iterator_category = std::input_iterator_tag;
    using value_type = decltype(function(*current));
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    iterator(typename Container::const_iterator it, UnaryFunction f) : current(it), function(f) {}

    iterator& operator++() {
      ++current;
      return *this;
    }

    iterator operator++(int) {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const iterator& other) const {
      return current == other.current;
    }

    bool operator!=(const iterator& other) const {
      return !(*this == other);
    }

    value_type operator*() const {
      return function(*current);
    }
  };

  iterator begin() const {
    return iterator(container.begin(), function);
  }

  iterator end() const {
    return iterator(container.end(), function);
  }
};

template<typename Container, typename Predicate>
class FilterAdapter {
};

template<typename Container>
class TakeAdapter {
};

template<typename Container>
class DropAdapter {
};

template<typename Container>
class ReverseAdapter {
};

template<typename AssociativeContainer>
class KeysAdapter {
};

template<typename AssociativeContainer>
class ValuesAdapter {
};

