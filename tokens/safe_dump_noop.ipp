

#ifndef BOOST_STACKTRACE_DETAIL_SAFE_DUMP_NOOP_IPP
#define BOOST_STACKTRACE_DETAIL_SAFE_DUMP_NOOP_IPP

#include <boost/config.hpp>
#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/stacktrace/safe_dump_to.hpp>

namespace boost { namespace stacktrace { namespace detail {


#if defined(BOOST_WINDOWS)
std::size_t dump(void* /*fd*/, const native_frame_ptr_t* /*frames*/, std::size_t /*frames_count*/) BOOST_NOEXCEPT {
    return 0;
}
#else
std::size_t dump(int /*fd*/, const native_frame_ptr_t* /*frames*/, std::size_t /*frames_count*/) BOOST_NOEXCEPT {
    return 0;
}
#endif


std::size_t dump(const char* /*file*/, const native_frame_ptr_t* /*frames*/, std::size_t /*frames_count*/) BOOST_NOEXCEPT {
    return 0;
}

}}} // namespace boost::stacktrace::detail

#endif // BOOST_STACKTRACE_DETAIL_SAFE_DUMP_NOOP_IPP
