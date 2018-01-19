// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME libCT_Dict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "./CTcoinTime.h"
#include "./CTkinDist.h"
#include "./CTpidCut.h"
#include "./CTRun.h"
#include "./Help.h"
#include "./Plot_Data_Simc.h"
#include "./Plot_Data_Simc_eSHMS.h"
#include "./Plot_Data_Simc_pSHMS.h"
#include "./Plot_kine_compare.h"
#include "./Plot_kine_compare_eSHMS.h"
#include "./Plot_kine_compare_pSHMS.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_CTRun(void *p = 0);
   static void *newArray_CTRun(Long_t size, void *p);
   static void delete_CTRun(void *p);
   static void deleteArray_CTRun(void *p);
   static void destruct_CTRun(void *p);
   static void streamer_CTRun(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CTRun*)
   {
      ::CTRun *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CTRun >(0);
      static ::ROOT::TGenericClassInfo 
         instance("CTRun", ::CTRun::Class_Version(), "CTRun.h", 18,
                  typeid(::CTRun), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CTRun::Dictionary, isa_proxy, 16,
                  sizeof(::CTRun) );
      instance.SetNew(&new_CTRun);
      instance.SetNewArray(&newArray_CTRun);
      instance.SetDelete(&delete_CTRun);
      instance.SetDeleteArray(&deleteArray_CTRun);
      instance.SetDestructor(&destruct_CTRun);
      instance.SetStreamerFunc(&streamer_CTRun);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CTRun*)
   {
      return GenerateInitInstanceLocal((::CTRun*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::CTRun*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr CTRun::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *CTRun::Class_Name()
{
   return "CTRun";
}

//______________________________________________________________________________
const char *CTRun::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CTRun*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int CTRun::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CTRun*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CTRun::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CTRun*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CTRun::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CTRun*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void CTRun::Streamer(TBuffer &R__b)
{
   // Stream an object of class CTRun.

   TObject::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CTRun(void *p) {
      return  p ? new(p) ::CTRun : new ::CTRun;
   }
   static void *newArray_CTRun(Long_t nElements, void *p) {
      return p ? new(p) ::CTRun[nElements] : new ::CTRun[nElements];
   }
   // Wrapper around operator delete
   static void delete_CTRun(void *p) {
      delete ((::CTRun*)p);
   }
   static void deleteArray_CTRun(void *p) {
      delete [] ((::CTRun*)p);
   }
   static void destruct_CTRun(void *p) {
      typedef ::CTRun current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_CTRun(TBuffer &buf, void *obj) {
      ((::CTRun*)obj)->::CTRun::Streamer(buf);
   }
} // end of namespace ROOT for class ::CTRun

namespace {
  void TriggerDictionaryInitialization_libCT_Dict_Impl() {
    static const char* headers[] = {
"./CTcoinTime.h",
"./CTkinDist.h",
"./CTpidCut.h",
"./CTRun.h",
"./Help.h",
"./Plot_Data_Simc.h",
"./Plot_Data_Simc_eSHMS.h",
"./Plot_Data_Simc_pSHMS.h",
"./Plot_kine_compare.h",
"./Plot_kine_compare_eSHMS.h",
"./Plot_kine_compare_pSHMS.h",
0
    };
    static const char* includePaths[] = {
"/u/site/12gev_phys/2.0/Linux_RedHat6.9-x86_64-gcc5.2.0/root/6.08.00/include",
"/u/home/latif/GIT/PublicCT/src/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCT_Dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$./CTRun.h")))  CTRun;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCT_Dict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "./CTcoinTime.h"
#include "./CTkinDist.h"
#include "./CTpidCut.h"
#include "./CTRun.h"
#include "./Help.h"
#include "./Plot_Data_Simc.h"
#include "./Plot_Data_Simc_eSHMS.h"
#include "./Plot_Data_Simc_pSHMS.h"
#include "./Plot_kine_compare.h"
#include "./Plot_kine_compare_eSHMS.h"
#include "./Plot_kine_compare_pSHMS.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"CTRun", payloadCode, "@",
"CTcoinTime", payloadCode, "@",
"CTkinDist", payloadCode, "@",
"CTpidCut", payloadCode, "@",
"Help", payloadCode, "@",
"Plot_Data_Simc", payloadCode, "@",
"Plot_Data_Simc_eSHMS", payloadCode, "@",
"Plot_Data_Simc_pSHMS", payloadCode, "@",
"Plot_kine_compare", payloadCode, "@",
"Plot_kine_compare_eSHMS", payloadCode, "@",
"Plot_kine_compare_pSHMS", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCT_Dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCT_Dict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCT_Dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCT_Dict() {
  TriggerDictionaryInitialization_libCT_Dict_Impl();
}
