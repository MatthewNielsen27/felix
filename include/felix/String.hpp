#pragma once

#include <memory>
#include <variant>
#include <string>

#include "felix/Graphemes.hpp"
#include "felix/StringStorage.hpp"

namespace felix {

// --
// This classic snippet comes from: https://en.cppreference.com/w/cpp/utility/variant/visit
//
// todo: this can go away when I convert to c++20
//
namespace detail{
// helper type for the visitor #4
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
} // namespace felix::detail

/// This is an immutable, ref-counted, string with sso (small string optimization).
class String {
    using LargeStringPtr = std::shared_ptr<const detail::LargeContiguousStorage>;
    using SmallString = detail::SmallContiguousStorage;

    explicit String(LargeStringPtr);
    explicit String(SmallString);
public:
    explicit String(std::string_view input);

    String(const String& other) = default;

    //! @returns the size of the string (in bytes).
    [[nodiscard]] std::size_t size() const;

    //! @returns the ref-count of the contained string.
    [[nodiscard]] std::size_t use_count() const;

    //! @returns true if small-string-optimization is applied to the contained string.
    [[nodiscard]] bool is_small() const;

    using const_iterator = detail::ConstIterator;

    [[nodiscard]] const_iterator begin() const;
    [[nodiscard]] const_iterator end() const;
    [[nodiscard]] const_iterator cbegin() const;
    [[nodiscard]] const_iterator cend() const;

    //! @returns the result of appending 'str' to this.
    [[nodiscard]] String append(std::string_view str) const;
    [[nodiscard]] String append(const String& str) const;

    [[nodiscard]] bool operator==(const String& other) const;
    [[nodiscard]] bool operator==(std::string_view other) const;

    template <typename T>
    [[nodiscard]] bool operator!=(const T& other) const {
        return !(*this == other);
    }

    [[nodiscard]] std::string_view view() const;

    //! @returns the grapheme clusters of the contained string.
    [[nodiscard]] Graphemes<String> graphemes() const {
        return Graphemes<String>{begin(), end()};
    }

    friend std::ostream& operator<<(std::ostream& stream, const String& str);

private:
    std::variant<SmallString, LargeStringPtr> m_data;

    template <class F>
    auto visit(F fn) const {
        return std::visit(
            detail::overloaded{
                [&](const LargeStringPtr& largeString) {
                    return std::forward<decltype(fn)>(fn)(*largeString);
                },
                [&](const SmallString& smallString) {
                    return std::forward<decltype(fn)>(fn)(smallString);
                }
            },
            m_data
        );
    }

};

inline std::ostream& operator<<(std::ostream& stream, const String& str) {
    str.visit(
            [&] (const auto& sequence) {
                stream << std::string{sequence.cbegin(), sequence.cend()};
            }
    );

    return stream;
}

} // namespace felix
