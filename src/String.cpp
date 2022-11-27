#include "felix/String.hpp"

namespace felix {

String::String(LargeStringPtr str) : m_data(std::move(str)) {}
String::String(SmallString str) : m_data(str) {}

String::String(std::string_view input) {
    if (input.size() > 8) {
        m_data.emplace<LargeStringPtr>(std::make_shared<LargeStringPtr::element_type>(input));
    } else {
        m_data.emplace<SmallString>(input);
    }
}

std::size_t String::use_count() const {
    return std::visit(
            detail::overloaded {
                    [&](const LargeStringPtr& largeString) -> std::size_t {
                        return static_cast<std::size_t>(largeString.use_count());
                    },
                    [] (const SmallString& smallString) -> std::size_t {
                        return 0;
                    }
            },
            m_data
    );
}

std::size_t String::size() const {
    return visit([](const auto& str) -> std::size_t { return str.size(); });
}

String::const_iterator String::begin() const {
    return cbegin();
}

String::const_iterator String::end() const {
    return cend();
}

String::const_iterator String::cbegin() const {
    return visit([](const auto& str){ return str.cbegin(); });
}

String::const_iterator String::cend() const {
    return visit([](const auto& str){ return str.cend(); });
}

bool String::is_small() const {
    return std::holds_alternative<SmallString>(m_data);
}

bool String::operator==(const String& other) const {
    if (size() != other.size()) {
        return false;
    } else {
        return std::equal(cbegin(), cend(), other.cbegin());
    }
}

bool String::operator==(std::string_view other) const {
    return view() == other;
}

String String::append(const String& str) const {
    return append(str.view());
}

String String::append(std::string_view str) const {
    const auto combined_size = size() + str.size();

    const auto lhs_size = size();
    const auto lhs_ptr = visit([](const auto& x) { return x.data(); });

    if (combined_size > 8) {
        auto newStr = std::make_unique<detail::LargeContiguousStorage>(combined_size);

        // now we need to copy the start of our data
        std::memcpy(newStr->data(), lhs_ptr, lhs_size);
        std::memcpy(newStr->data() + lhs_size, str.data(), str.size());

        return String{std::move(newStr)};
    } else {
        auto newStr = detail::SmallContiguousStorage(static_cast<detail::SmallContiguousStorage::size_type>(combined_size));

        // now we need to copy the start of our data
        std::memcpy(newStr.data(), lhs_ptr, lhs_size);
        std::memcpy(newStr.data() + lhs_size, str.data(), str.size());

        return String{newStr};
    }
}

std::string_view String::view() const {
    return {visit([](const auto& f) { return f.data(); }), size()};
}

} // namespace felix
