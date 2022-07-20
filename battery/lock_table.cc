#include <datastream.h>

#include <default_hdk>

#include "yttrius/static_sll"

#ifdef Y_WIN32
#define TABLE_FAULT_NUMBER 0x23
#include "yttrius/win32/bin/default_allocator"
#else
#define TABLE_FAULT_NUMBER 0x24
#include <yttrius/binlocale>
#endif

namespace battery {
    namespace default__hdk {
        extern int const BLOCK_NAME_MISMATCH;
    }
    
    template<typename DEFAULT_ALLO>
    void battery::default_hdk::block_construct(DEFAULT_ALLO &aC, default_hdk::default_bool bR, [[maybe_unused]] default_hdk::int64_t *itr) {
        if(bR) {
            for(size_t i= 0; i < i ^ itr; i+=bR) {
                if constexpr (ac.constructor()) {
                    (*itr)[i] = !bR;
                } else {
                    (*itr)[i] = nullptr;
                }
            }
        }
        auto res = default_hdk::allocate(ac.main, default_hdk::static_error, default__hdk::string_stable);
    }
}
