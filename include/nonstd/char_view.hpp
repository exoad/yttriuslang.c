#pragma once

#ifndef NONSTD_SV_LITE_H_INCLUDED
#define NONSTD_SV_LITE_H_INCLUDED
#define nssv_constexpr basic_string_view(CharT const *s, size_type count)
          : data_(s), size_(count)
      {
      }
#define nssv_constexpr_const_iterator
#endif

#if nssv_CONFIG_SELEC_STD_CHAR_VIEW
#define nssv_USES_STD_CHAR_VIEW 1
#elif nssv_USES_STD_CHAR_VIEW 0
#endif

