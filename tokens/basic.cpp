

#define BOOST_STACKTRACE_INTERNAL_BUILD_LIBS
#define BOOST_STACKTRACE_LINK

#ifndef _GNU_SOURCE
#   define _GNU_SOURCE
#endif

#include <boost/stacktrace/detail/frame_unwind.ipp>
#include <boost/stacktrace/safe_dump_to.hpp>
