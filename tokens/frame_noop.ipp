

#ifndef BOOST_STACKTRACE_DETAIL_FRAME_NOOP_IPP
#define BOOST_STACKTRACE_DETAIL_FRAME_NOOP_IPP

#include <boost/config.hpp>
#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/stacktrace/frame.hpp>

namespace boost { namespace stacktrace { namespace detail {

std::string to_string(const frame* /*frames*/, std::size_t /*count*/) {
    return std::string();
}

} // namespace detail

std::string frame::name() const {
    return std::string();
}

std::string frame::source_file() const {
    return std::string();
}

std::size_t frame::source_line() const {
    return 0;
}

std::string to_string(const frame& /*f*/) {
    return std::string();
}


}} // namespace boost::stacktrace

#endif // BOOST_STACKTRACE_DETAIL_FRAME_NOOP_IPP
