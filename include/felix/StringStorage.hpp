#pragma once

#include <cstddef>
#include <iterator>
#include <string_view>

namespace felix::detail {

using char_type = char;

// todo: this needs to be adapted to c++20 and makes use of concepts.
struct Iterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = char_type;
    using pointer           = value_type*;
    using reference         = value_type&;

    explicit Iterator(pointer ptr) : m_ptr(ptr) {}

    reference operator*() const { return *m_ptr; }
    pointer operator->() { return m_ptr; }

    // Prefix increment
    Iterator& operator++() { m_ptr++; return *this; }

    // Postfix increment
    Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

    friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
    friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

private:
    pointer m_ptr;
};

// todo: this needs to be adapted to c++20 and makes use of concepts.
struct ConstIterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = char_type;
    using pointer           = const value_type*;
    using reference         = const value_type&;

    ConstIterator() = default;
    explicit ConstIterator(pointer ptr) : m_ptr(ptr) {}

    reference operator*() const { return *m_ptr; }
    pointer operator->() { return m_ptr; }

    // Prefix increment
    ConstIterator& operator++() { m_ptr++; return *this; }

    // Postfix increment
    ConstIterator operator++(int) { ConstIterator tmp = *this; ++(*this); return tmp; }

    friend bool operator== (const ConstIterator& a, const ConstIterator& b) { return a.m_ptr == b.m_ptr; };
    friend bool operator!= (const ConstIterator& a, const ConstIterator& b) { return a.m_ptr != b.m_ptr; };

private:
    pointer m_ptr;
};

class LargeContiguousStorage {
public:
    using size_type = std::size_t;
    using value_type = char_type;

    explicit LargeContiguousStorage(std::string_view view)
        : m_size(view.size())
        , m_data(new value_type[m_size]) // raw new :)
    {
        std::memcpy(m_data, view.data(), view.size());
    }

    explicit LargeContiguousStorage(std::size_t size)
            : m_size(size)
            , m_data(new value_type[m_size]) // raw new :)
    {}

    ~LargeContiguousStorage() {
        delete[] m_data;
    }

    [[nodiscard]] Iterator begin() const { return Iterator(&m_data[0]); }
    [[nodiscard]] Iterator end()   const { return Iterator(&m_data[m_size]); }

    [[nodiscard]] ConstIterator cbegin() const { return ConstIterator(&m_data[0]); }
    [[nodiscard]] ConstIterator cend()   const { return ConstIterator(&m_data[m_size]); }

    [[nodiscard]] size_type size() const { return m_size; }

    [[nodiscard]] value_type* data() { return m_data; }
    [[nodiscard]] const value_type* data() const { return m_data; }

private:
    size_type m_size;
    value_type* m_data;
};

class SmallContiguousStorage {
public:
    using size_type = std::uint8_t;
    using value_type = char_type;

    SmallContiguousStorage() = default;

    explicit SmallContiguousStorage(size_type size) : m_size(size) {}
    explicit SmallContiguousStorage(std::string_view view)
        : m_size(view.size())
    {
        assert(m_size <= 8);
        std::memcpy(m_data, view.data(), m_size);
    }

    [[nodiscard]] Iterator begin() { return Iterator(&m_data[0]); }
    [[nodiscard]] Iterator end()   { return Iterator(&m_data[m_size]); }

    [[nodiscard]] ConstIterator cbegin() const { return ConstIterator(&m_data[0]); }
    [[nodiscard]] ConstIterator cend()   const { return ConstIterator(&m_data[m_size]); }

    [[nodiscard]] size_type size() const {
        return m_size;
    }

    [[nodiscard]] value_type* data() { return m_data; }
    [[nodiscard]] const value_type* data() const { return m_data; }

private:
    size_type m_size = 0;
    value_type m_data[8] = {0};
};

} // namespace felix::detail
