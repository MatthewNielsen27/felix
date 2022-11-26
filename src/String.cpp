#include "felix/String.hpp"

namespace felix {

String::SmallString::SmallString(std::string_view view) {
    assert(view.size() <= 8);

    data.reserve(8);

    std::copy(view.begin(), view.end(), std::back_inserter(data));
}

String::String(std::string_view input) {
    if (input.size() > 8) {
        m_data.emplace<LargeStringPtr>(std::make_shared<LargeStringPtr::element_type>(input.begin(), input.end()));
    } else {
        m_data.emplace<SmallString>(input);
    }
}

std::size_t String::size() const {
    return std::visit(
            detail::overloaded {
                    [&](const LargeStringPtr & largeStringPtr) -> std::size_t {
                        return largeStringPtr->size();
                    },
                    [] (const SmallString& smallString) -> std::size_t {
                        return smallString.data.size();
                    }
            },
            m_data
    );
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

String::const_iterator String::begin() const {
    return std::visit(
            detail::overloaded {
                    [&](const LargeStringPtr& largeString) {
                        return largeString->cbegin();
                    },
                    [] (const SmallString& smallString) {
                        return smallString.data.cbegin();
                    }
            },
            m_data
    );
}

String::const_iterator String::end() const {
    return std::visit(
            detail::overloaded {
                    [&](const LargeStringPtr& largeString) {
                        return largeString->cend();
                    },
                    [] (const SmallString& smallString) {
                        return smallString.data.cend();
                    }
            },
            m_data
    );
}

bool String::is_small() const {
    return std::holds_alternative<SmallString>(m_data);
}

} // namespace felix
