#pragma once

#include <memory>
#include <variant>
#include <vector>
#include <string>

#include "felix/Graphemes.hpp"

namespace felix {

/// This is an immutable, ref-counted, string with sso (small string optimization).
class String {
protected:
    // TODO: this probable needs to be a simple dynamic sized array class, with iterator.
    using Storage = std::vector<char>;

    using LargeStringPtr = std::shared_ptr<const Storage>;

    struct SmallString {
        SmallString() = default;
        SmallString(const SmallString& other) = default;
        explicit SmallString(std::string_view view);
        Storage data;
    };

public:
    explicit String(std::string_view input);

    using const_iterator = Storage::const_iterator;

    String(const String& other) = default;

    //! @returns the size of the string (in bytes).
    [[nodiscard]] std::size_t size() const;

    [[nodiscard]] std::size_t use_count() const;

    [[nodiscard]] bool is_small() const;

    friend std::ostream& operator<<(std::ostream& stream, const String& str);

    [[nodiscard]] const_iterator begin() const;
    [[nodiscard]] const_iterator end() const;

    [[nodiscard]] Graphemes<String> graphemes() const {
        return Graphemes<String>{begin(), end()};
    }

private:
    std::variant<SmallString, LargeStringPtr> m_data;
};

namespace detail {

// helper type for the visitor #4
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

} // namespace felix::detail

inline std::ostream& operator<<(std::ostream& stream, const String& str) {
    auto addToStream = [&] (const auto& sequence) {
        std::string out{sequence.begin(), sequence.end()};
        stream << out;
    };

    std::visit(
            detail::overloaded {
                    [&](const String::LargeStringPtr& largeStringPtr) {
                        addToStream(*largeStringPtr);
                    },
                    [&](const String::SmallString& smallString) {
                        addToStream(smallString.data);
                    },
                    addToStream
            },
            str.m_data
    );

    return stream;
}

} // namespace felix
