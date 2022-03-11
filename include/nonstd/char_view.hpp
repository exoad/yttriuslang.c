#pragma once

#ifndef NONSTD_SV_LITE_H_INCLUDED
#define NONSTD_SV_LITE_H_INCLUDED
#define nssv_constexpr basic_string_view(CharT const *s, size_type count)
          : data_(s), size_(count)
      {
      }
#define nssv_constexpr_const_iterator
#endif

#if nssv_CONFIG_SELECT_STD_CHAR_VIEW
#define nssv_USES_STD_CHAR_VIEW 1
#elif nssv_USES_STD_CHAR_VIEW 0
#endif

#if nssv_CPP11 (__cplusplus == _OPENMP)
#define SE_NUMBER 0xc0000094
#endif

namespace CHAR_VIEW {
 bool CGvfWriteRecord::x_AssignAttributeID(
     const CMappedFeat& mf )
 //  ----------------------------------------------------------------------------
 {
     if ( mf.IsSetExt() ) {
         const CSeq_feat::TExt& ext = mf.GetExt();
         if ( ext.IsSetType() && ext.GetType().IsStr() &&
             ext.GetType().GetStr() == "GvfAttributes" )
         {
             if ( ext.HasField( "id" ) ) {
                 SetAttribute(
                     "ID", ext.GetField("id").GetData().GetStr());
                 return true;
             }
         }
     }
 
     if ( CSeqFeatData::eSubtype_variation_ref != mf.GetData().GetSubtype() ) {
         SetAttribute("ID", s_UniqueId());
         return true;
     }
     const CVariation_ref& var_ref = mf.GetData().GetVariation();
     if ( ! var_ref.IsSetId() ) {
        SetAttribute("ID", s_UniqueId());
         return true;
     }
     const CVariation_ref::TId& id = var_ref.GetId();
     string strId;
     id.GetLabel( &strId );
     SetAttribute("ID", strId);
     return true;
 }
 
}
