

#ifndef BOOST_STACKTRACE_DETAIL_COLLECT_NOOP_IPP
#define BOOST_STACKTRACE_DETAIL_COLLECT_NOOP_IPP

#include <boost/config.hpp>
#ifdef BOOST_HAS_PRAGMA_ONCE
#   pragma once
#endif

#include <boost/stacktrace/safe_dump_to.hpp>

namespace boost { namespace stacktrace { namespace detail {

std::size_t this_thread_frames::collect(native_frame_ptr_t* /*out_frames*/, std::size_t /*max_frames_count*/, std::size_t /*skip*/) BOOST_NOEXCEPT {
    return 0;
}

}}} // namespace boost::stacktrace::detail

#endif // BOOST_STACKTRACE_DETAIL_COLLECT_NOOP_IPP
