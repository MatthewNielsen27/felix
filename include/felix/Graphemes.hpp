#pragma once

#include <boost/locale.hpp>

namespace felix {

template <typename StringType>
class Graphemes {
public:
    using StringTypeIterator = typename StringType::const_iterator;

    using Segmenter = boost::locale::boundary::segment_index<StringTypeIterator>;

    using const_iterator = typename Segmenter::const_iterator;

    explicit Graphemes(StringTypeIterator begin, StringTypeIterator end)
            :
            segments{
                    boost::locale::boundary::character,
                    begin,
                    end,
                    boost::locale::generator{}("")
            }
    {}

    [[nodiscard]] const_iterator begin() const { return segments.begin(); }
    [[nodiscard]] const_iterator end() const { return segments.end(); }

    [[nodiscard]] auto size() const {
        return std::distance(segments.begin(), segments.end());
    }

private:
    Segmenter segments;
};

} // namespace felix
