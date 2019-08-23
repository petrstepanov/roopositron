// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME roopositronmIdictionary
#define R__NO_DEPRECATION

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

// The generated code does not explicitly qualifies STL entities
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "src/model/Constants.h"
#include "src/model/ParametersPool.h"
#include "src/roofit/ReverseAddPdf.h"
#include "src/roofit/ModelCommonizer.h"
#include "src/roofit/PdfFactory.h"
#include "src/roofit/providers/GrainProvider.h"
#include "src/roofit/providers/Powder2Provider.h"
#include "src/roofit/providers/ExpSourceProvider.h"
#include "src/roofit/providers/PowderProvider.h"
#include "src/roofit/providers/ThreeExpSourceProvider.h"
#include "src/roofit/providers/ThreeGaussProvider.h"
#include "src/roofit/providers/TwoExpProvider.h"
#include "src/roofit/providers/AbstractProvider.h"
#include "src/roofit/providers/LiquidProvider.h"
#include "src/roofit/providers/ExpProvider.h"
#include "src/roofit/providers/TwoGaussProvider.h"
#include "src/roofit/providers/TwoExpSourceProvider.h"
#include "src/roofit/providers/LiquidSimpleProvider.h"
#include "src/roofit/providers/OneGaussProvider.h"
#include "src/roofit/providers/TrappingProvider.h"
#include "src/roofit/providers/ThreeExpProvider.h"
#include "src/roofit/pdfs/TwoGaussian.h"
#include "src/roofit/pdfs/TrapPdf.h"
#include "src/roofit/pdfs/Powder2Pdf.h"
#include "src/roofit/pdfs/ThreeExpPdf.h"
#include "src/roofit/pdfs/TwoExpPdf.h"
#include "src/roofit/pdfs/ExpPdf.h"
#include "src/roofit/pdfs/ThreeGaussian.h"
#include "src/roofit/pdfs/ConstantPdf.h"
#include "src/roofit/pdfs/PowderPdf.h"
#include "src/roofit/pdfs/LiquidPdf.h"
#include "src/roofit/pdfs/GrainPdf.h"
#include "src/roofit/pdfs/LiquidSimplePdf.h"
#include "src/roofit/AdditiveConvolutionPdf.h"
#include "src/util/RootHelper.h"
#include "src/util/MathUtil.h"
#include "src/util/HistProcessor.h"
#include "src/util/FileUtils.h"
#include "src/util/ObjectNamer.h"
#include "src/util/Debug.h"
#include "src/util/StringUtils.h"
#include "src/util/GraphicsHelper.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TwoGaussian(void *p = 0);
   static void *newArray_TwoGaussian(Long_t size, void *p);
   static void delete_TwoGaussian(void *p);
   static void deleteArray_TwoGaussian(void *p);
   static void destruct_TwoGaussian(void *p);
   static void streamer_TwoGaussian(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TwoGaussian*)
   {
      ::TwoGaussian *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TwoGaussian >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TwoGaussian", ::TwoGaussian::Class_Version(), "src/roofit/pdfs/TwoGaussian.h", 24,
                  typeid(::TwoGaussian), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TwoGaussian::Dictionary, isa_proxy, 16,
                  sizeof(::TwoGaussian) );
      instance.SetNew(&new_TwoGaussian);
      instance.SetNewArray(&newArray_TwoGaussian);
      instance.SetDelete(&delete_TwoGaussian);
      instance.SetDeleteArray(&deleteArray_TwoGaussian);
      instance.SetDestructor(&destruct_TwoGaussian);
      instance.SetStreamerFunc(&streamer_TwoGaussian);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TwoGaussian*)
   {
      return GenerateInitInstanceLocal((::TwoGaussian*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TwoGaussian*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TrapPdf(void *p = 0);
   static void *newArray_TrapPdf(Long_t size, void *p);
   static void delete_TrapPdf(void *p);
   static void deleteArray_TrapPdf(void *p);
   static void destruct_TrapPdf(void *p);
   static void streamer_TrapPdf(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TrapPdf*)
   {
      ::TrapPdf *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TrapPdf >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TrapPdf", ::TrapPdf::Class_Version(), "src/roofit/pdfs/TrapPdf.h", 22,
                  typeid(::TrapPdf), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TrapPdf::Dictionary, isa_proxy, 16,
                  sizeof(::TrapPdf) );
      instance.SetNew(&new_TrapPdf);
      instance.SetNewArray(&newArray_TrapPdf);
      instance.SetDelete(&delete_TrapPdf);
      instance.SetDeleteArray(&deleteArray_TrapPdf);
      instance.SetDestructor(&destruct_TrapPdf);
      instance.SetStreamerFunc(&streamer_TrapPdf);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TrapPdf*)
   {
      return GenerateInitInstanceLocal((::TrapPdf*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TrapPdf*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_Powder2Pdf(void *p = 0);
   static void *newArray_Powder2Pdf(Long_t size, void *p);
   static void delete_Powder2Pdf(void *p);
   static void deleteArray_Powder2Pdf(void *p);
   static void destruct_Powder2Pdf(void *p);
   static void streamer_Powder2Pdf(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Powder2Pdf*)
   {
      ::Powder2Pdf *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Powder2Pdf >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Powder2Pdf", ::Powder2Pdf::Class_Version(), "src/roofit/pdfs/Powder2Pdf.h", 19,
                  typeid(::Powder2Pdf), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Powder2Pdf::Dictionary, isa_proxy, 16,
                  sizeof(::Powder2Pdf) );
      instance.SetNew(&new_Powder2Pdf);
      instance.SetNewArray(&newArray_Powder2Pdf);
      instance.SetDelete(&delete_Powder2Pdf);
      instance.SetDeleteArray(&deleteArray_Powder2Pdf);
      instance.SetDestructor(&destruct_Powder2Pdf);
      instance.SetStreamerFunc(&streamer_Powder2Pdf);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Powder2Pdf*)
   {
      return GenerateInitInstanceLocal((::Powder2Pdf*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Powder2Pdf*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ThreeExpPdf(void *p = 0);
   static void *newArray_ThreeExpPdf(Long_t size, void *p);
   static void delete_ThreeExpPdf(void *p);
   static void deleteArray_ThreeExpPdf(void *p);
   static void destruct_ThreeExpPdf(void *p);
   static void streamer_ThreeExpPdf(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ThreeExpPdf*)
   {
      ::ThreeExpPdf *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ThreeExpPdf >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ThreeExpPdf", ::ThreeExpPdf::Class_Version(), "src/roofit/pdfs/ThreeExpPdf.h", 23,
                  typeid(::ThreeExpPdf), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ThreeExpPdf::Dictionary, isa_proxy, 16,
                  sizeof(::ThreeExpPdf) );
      instance.SetNew(&new_ThreeExpPdf);
      instance.SetNewArray(&newArray_ThreeExpPdf);
      instance.SetDelete(&delete_ThreeExpPdf);
      instance.SetDeleteArray(&deleteArray_ThreeExpPdf);
      instance.SetDestructor(&destruct_ThreeExpPdf);
      instance.SetStreamerFunc(&streamer_ThreeExpPdf);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ThreeExpPdf*)
   {
      return GenerateInitInstanceLocal((::ThreeExpPdf*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ThreeExpPdf*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TwoExpPdf(void *p = 0);
   static void *newArray_TwoExpPdf(Long_t size, void *p);
   static void delete_TwoExpPdf(void *p);
   static void deleteArray_TwoExpPdf(void *p);
   static void destruct_TwoExpPdf(void *p);
   static void streamer_TwoExpPdf(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TwoExpPdf*)
   {
      ::TwoExpPdf *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TwoExpPdf >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TwoExpPdf", ::TwoExpPdf::Class_Version(), "src/roofit/pdfs/TwoExpPdf.h", 23,
                  typeid(::TwoExpPdf), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TwoExpPdf::Dictionary, isa_proxy, 16,
                  sizeof(::TwoExpPdf) );
      instance.SetNew(&new_TwoExpPdf);
      instance.SetNewArray(&newArray_TwoExpPdf);
      instance.SetDelete(&delete_TwoExpPdf);
      instance.SetDeleteArray(&deleteArray_TwoExpPdf);
      instance.SetDestructor(&destruct_TwoExpPdf);
      instance.SetStreamerFunc(&streamer_TwoExpPdf);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TwoExpPdf*)
   {
      return GenerateInitInstanceLocal((::TwoExpPdf*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TwoExpPdf*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ExpPdf(void *p = 0);
   static void *newArray_ExpPdf(Long_t size, void *p);
   static void delete_ExpPdf(void *p);
   static void deleteArray_ExpPdf(void *p);
   static void destruct_ExpPdf(void *p);
   static void streamer_ExpPdf(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ExpPdf*)
   {
      ::ExpPdf *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ExpPdf >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ExpPdf", ::ExpPdf::Class_Version(), "src/roofit/pdfs/ExpPdf.h", 22,
                  typeid(::ExpPdf), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ExpPdf::Dictionary, isa_proxy, 16,
                  sizeof(::ExpPdf) );
      instance.SetNew(&new_ExpPdf);
      instance.SetNewArray(&newArray_ExpPdf);
      instance.SetDelete(&delete_ExpPdf);
      instance.SetDeleteArray(&deleteArray_ExpPdf);
      instance.SetDestructor(&destruct_ExpPdf);
      instance.SetStreamerFunc(&streamer_ExpPdf);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ExpPdf*)
   {
      return GenerateInitInstanceLocal((::ExpPdf*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ExpPdf*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ThreeGaussian(void *p = 0);
   static void *newArray_ThreeGaussian(Long_t size, void *p);
   static void delete_ThreeGaussian(void *p);
   static void deleteArray_ThreeGaussian(void *p);
   static void destruct_ThreeGaussian(void *p);
   static void streamer_ThreeGaussian(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ThreeGaussian*)
   {
      ::ThreeGaussian *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ThreeGaussian >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ThreeGaussian", ::ThreeGaussian::Class_Version(), "src/roofit/pdfs/ThreeGaussian.h", 24,
                  typeid(::ThreeGaussian), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ThreeGaussian::Dictionary, isa_proxy, 16,
                  sizeof(::ThreeGaussian) );
      instance.SetNew(&new_ThreeGaussian);
      instance.SetNewArray(&newArray_ThreeGaussian);
      instance.SetDelete(&delete_ThreeGaussian);
      instance.SetDeleteArray(&deleteArray_ThreeGaussian);
      instance.SetDestructor(&destruct_ThreeGaussian);
      instance.SetStreamerFunc(&streamer_ThreeGaussian);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ThreeGaussian*)
   {
      return GenerateInitInstanceLocal((::ThreeGaussian*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ThreeGaussian*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ConstantPdf(void *p = 0);
   static void *newArray_ConstantPdf(Long_t size, void *p);
   static void delete_ConstantPdf(void *p);
   static void deleteArray_ConstantPdf(void *p);
   static void destruct_ConstantPdf(void *p);
   static void streamer_ConstantPdf(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ConstantPdf*)
   {
      ::ConstantPdf *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ConstantPdf >(0);
      static ::ROOT::TGenericClassInfo 
         instance("ConstantPdf", ::ConstantPdf::Class_Version(), "src/roofit/pdfs/ConstantPdf.h", 22,
                  typeid(::ConstantPdf), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ConstantPdf::Dictionary, isa_proxy, 16,
                  sizeof(::ConstantPdf) );
      instance.SetNew(&new_ConstantPdf);
      instance.SetNewArray(&newArray_ConstantPdf);
      instance.SetDelete(&delete_ConstantPdf);
      instance.SetDeleteArray(&deleteArray_ConstantPdf);
      instance.SetDestructor(&destruct_ConstantPdf);
      instance.SetStreamerFunc(&streamer_ConstantPdf);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ConstantPdf*)
   {
      return GenerateInitInstanceLocal((::ConstantPdf*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ConstantPdf*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PowderPdf(void *p = 0);
   static void *newArray_PowderPdf(Long_t size, void *p);
   static void delete_PowderPdf(void *p);
   static void deleteArray_PowderPdf(void *p);
   static void destruct_PowderPdf(void *p);
   static void streamer_PowderPdf(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PowderPdf*)
   {
      ::PowderPdf *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PowderPdf >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PowderPdf", ::PowderPdf::Class_Version(), "src/roofit/pdfs/PowderPdf.h", 25,
                  typeid(::PowderPdf), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PowderPdf::Dictionary, isa_proxy, 16,
                  sizeof(::PowderPdf) );
      instance.SetNew(&new_PowderPdf);
      instance.SetNewArray(&newArray_PowderPdf);
      instance.SetDelete(&delete_PowderPdf);
      instance.SetDeleteArray(&deleteArray_PowderPdf);
      instance.SetDestructor(&destruct_PowderPdf);
      instance.SetStreamerFunc(&streamer_PowderPdf);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PowderPdf*)
   {
      return GenerateInitInstanceLocal((::PowderPdf*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PowderPdf*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_LiquidPdf(void *p = 0);
   static void *newArray_LiquidPdf(Long_t size, void *p);
   static void delete_LiquidPdf(void *p);
   static void deleteArray_LiquidPdf(void *p);
   static void destruct_LiquidPdf(void *p);
   static void streamer_LiquidPdf(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::LiquidPdf*)
   {
      ::LiquidPdf *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::LiquidPdf >(0);
      static ::ROOT::TGenericClassInfo 
         instance("LiquidPdf", ::LiquidPdf::Class_Version(), "src/roofit/pdfs/LiquidPdf.h", 27,
                  typeid(::LiquidPdf), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::LiquidPdf::Dictionary, isa_proxy, 16,
                  sizeof(::LiquidPdf) );
      instance.SetNew(&new_LiquidPdf);
      instance.SetNewArray(&newArray_LiquidPdf);
      instance.SetDelete(&delete_LiquidPdf);
      instance.SetDeleteArray(&deleteArray_LiquidPdf);
      instance.SetDestructor(&destruct_LiquidPdf);
      instance.SetStreamerFunc(&streamer_LiquidPdf);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::LiquidPdf*)
   {
      return GenerateInitInstanceLocal((::LiquidPdf*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::LiquidPdf*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_GrainPdf(void *p = 0);
   static void *newArray_GrainPdf(Long_t size, void *p);
   static void delete_GrainPdf(void *p);
   static void deleteArray_GrainPdf(void *p);
   static void destruct_GrainPdf(void *p);
   static void streamer_GrainPdf(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::GrainPdf*)
   {
      ::GrainPdf *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::GrainPdf >(0);
      static ::ROOT::TGenericClassInfo 
         instance("GrainPdf", ::GrainPdf::Class_Version(), "src/roofit/pdfs/GrainPdf.h", 25,
                  typeid(::GrainPdf), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::GrainPdf::Dictionary, isa_proxy, 16,
                  sizeof(::GrainPdf) );
      instance.SetNew(&new_GrainPdf);
      instance.SetNewArray(&newArray_GrainPdf);
      instance.SetDelete(&delete_GrainPdf);
      instance.SetDeleteArray(&deleteArray_GrainPdf);
      instance.SetDestructor(&destruct_GrainPdf);
      instance.SetStreamerFunc(&streamer_GrainPdf);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::GrainPdf*)
   {
      return GenerateInitInstanceLocal((::GrainPdf*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::GrainPdf*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_LiquidSimplePdf(void *p = 0);
   static void *newArray_LiquidSimplePdf(Long_t size, void *p);
   static void delete_LiquidSimplePdf(void *p);
   static void deleteArray_LiquidSimplePdf(void *p);
   static void destruct_LiquidSimplePdf(void *p);
   static void streamer_LiquidSimplePdf(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::LiquidSimplePdf*)
   {
      ::LiquidSimplePdf *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::LiquidSimplePdf >(0);
      static ::ROOT::TGenericClassInfo 
         instance("LiquidSimplePdf", ::LiquidSimplePdf::Class_Version(), "src/roofit/pdfs/LiquidSimplePdf.h", 25,
                  typeid(::LiquidSimplePdf), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::LiquidSimplePdf::Dictionary, isa_proxy, 16,
                  sizeof(::LiquidSimplePdf) );
      instance.SetNew(&new_LiquidSimplePdf);
      instance.SetNewArray(&newArray_LiquidSimplePdf);
      instance.SetDelete(&delete_LiquidSimplePdf);
      instance.SetDeleteArray(&deleteArray_LiquidSimplePdf);
      instance.SetDestructor(&destruct_LiquidSimplePdf);
      instance.SetStreamerFunc(&streamer_LiquidSimplePdf);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::LiquidSimplePdf*)
   {
      return GenerateInitInstanceLocal((::LiquidSimplePdf*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::LiquidSimplePdf*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TwoGaussian::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TwoGaussian::Class_Name()
{
   return "TwoGaussian";
}

//______________________________________________________________________________
const char *TwoGaussian::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TwoGaussian*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TwoGaussian::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TwoGaussian*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TwoGaussian::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TwoGaussian*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TwoGaussian::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TwoGaussian*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TrapPdf::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TrapPdf::Class_Name()
{
   return "TrapPdf";
}

//______________________________________________________________________________
const char *TrapPdf::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TrapPdf*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TrapPdf::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TrapPdf*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TrapPdf::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TrapPdf*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TrapPdf::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TrapPdf*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Powder2Pdf::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Powder2Pdf::Class_Name()
{
   return "Powder2Pdf";
}

//______________________________________________________________________________
const char *Powder2Pdf::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Powder2Pdf*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Powder2Pdf::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Powder2Pdf*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Powder2Pdf::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Powder2Pdf*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Powder2Pdf::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Powder2Pdf*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ThreeExpPdf::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ThreeExpPdf::Class_Name()
{
   return "ThreeExpPdf";
}

//______________________________________________________________________________
const char *ThreeExpPdf::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ThreeExpPdf*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ThreeExpPdf::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ThreeExpPdf*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ThreeExpPdf::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ThreeExpPdf*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ThreeExpPdf::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ThreeExpPdf*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TwoExpPdf::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TwoExpPdf::Class_Name()
{
   return "TwoExpPdf";
}

//______________________________________________________________________________
const char *TwoExpPdf::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TwoExpPdf*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TwoExpPdf::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TwoExpPdf*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TwoExpPdf::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TwoExpPdf*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TwoExpPdf::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TwoExpPdf*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ExpPdf::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ExpPdf::Class_Name()
{
   return "ExpPdf";
}

//______________________________________________________________________________
const char *ExpPdf::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ExpPdf*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ExpPdf::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ExpPdf*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ExpPdf::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ExpPdf*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ExpPdf::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ExpPdf*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ThreeGaussian::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ThreeGaussian::Class_Name()
{
   return "ThreeGaussian";
}

//______________________________________________________________________________
const char *ThreeGaussian::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ThreeGaussian*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ThreeGaussian::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ThreeGaussian*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ThreeGaussian::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ThreeGaussian*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ThreeGaussian::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ThreeGaussian*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ConstantPdf::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *ConstantPdf::Class_Name()
{
   return "ConstantPdf";
}

//______________________________________________________________________________
const char *ConstantPdf::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConstantPdf*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int ConstantPdf::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ConstantPdf*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ConstantPdf::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConstantPdf*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ConstantPdf::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ConstantPdf*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PowderPdf::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *PowderPdf::Class_Name()
{
   return "PowderPdf";
}

//______________________________________________________________________________
const char *PowderPdf::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PowderPdf*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PowderPdf::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PowderPdf*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PowderPdf::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PowderPdf*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PowderPdf::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PowderPdf*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr LiquidPdf::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *LiquidPdf::Class_Name()
{
   return "LiquidPdf";
}

//______________________________________________________________________________
const char *LiquidPdf::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LiquidPdf*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int LiquidPdf::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LiquidPdf*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *LiquidPdf::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LiquidPdf*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *LiquidPdf::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LiquidPdf*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr GrainPdf::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *GrainPdf::Class_Name()
{
   return "GrainPdf";
}

//______________________________________________________________________________
const char *GrainPdf::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GrainPdf*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int GrainPdf::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GrainPdf*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *GrainPdf::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GrainPdf*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *GrainPdf::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GrainPdf*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr LiquidSimplePdf::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *LiquidSimplePdf::Class_Name()
{
   return "LiquidSimplePdf";
}

//______________________________________________________________________________
const char *LiquidSimplePdf::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LiquidSimplePdf*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int LiquidSimplePdf::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::LiquidSimplePdf*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *LiquidSimplePdf::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LiquidSimplePdf*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *LiquidSimplePdf::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::LiquidSimplePdf*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TwoGaussian::Streamer(TBuffer &R__b)
{
   // Stream an object of class TwoGaussian.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      RooAbsPdf::Streamer(R__b);
      x.Streamer(R__b);
      mean.Streamer(R__b);
      sigma1.Streamer(R__b);
      sigma2.Streamer(R__b);
      i2.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, TwoGaussian::IsA());
   } else {
      R__c = R__b.WriteVersion(TwoGaussian::IsA(), kTRUE);
      RooAbsPdf::Streamer(R__b);
      x.Streamer(R__b);
      mean.Streamer(R__b);
      sigma1.Streamer(R__b);
      sigma2.Streamer(R__b);
      i2.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TwoGaussian(void *p) {
      return  p ? new(p) ::TwoGaussian : new ::TwoGaussian;
   }
   static void *newArray_TwoGaussian(Long_t nElements, void *p) {
      return p ? new(p) ::TwoGaussian[nElements] : new ::TwoGaussian[nElements];
   }
   // Wrapper around operator delete
   static void delete_TwoGaussian(void *p) {
      delete ((::TwoGaussian*)p);
   }
   static void deleteArray_TwoGaussian(void *p) {
      delete [] ((::TwoGaussian*)p);
   }
   static void destruct_TwoGaussian(void *p) {
      typedef ::TwoGaussian current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TwoGaussian(TBuffer &buf, void *obj) {
      ((::TwoGaussian*)obj)->::TwoGaussian::Streamer(buf);
   }
} // end of namespace ROOT for class ::TwoGaussian

//______________________________________________________________________________
void TrapPdf::Streamer(TBuffer &R__b)
{
   // Stream an object of class TrapPdf.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      tauBulk.Streamer(R__b);
      tauDef.Streamer(R__b);
      kappaDef.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, TrapPdf::IsA());
   } else {
      R__c = R__b.WriteVersion(TrapPdf::IsA(), kTRUE);
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      tauBulk.Streamer(R__b);
      tauDef.Streamer(R__b);
      kappaDef.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TrapPdf(void *p) {
      return  p ? new(p) ::TrapPdf : new ::TrapPdf;
   }
   static void *newArray_TrapPdf(Long_t nElements, void *p) {
      return p ? new(p) ::TrapPdf[nElements] : new ::TrapPdf[nElements];
   }
   // Wrapper around operator delete
   static void delete_TrapPdf(void *p) {
      delete ((::TrapPdf*)p);
   }
   static void deleteArray_TrapPdf(void *p) {
      delete [] ((::TrapPdf*)p);
   }
   static void destruct_TrapPdf(void *p) {
      typedef ::TrapPdf current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TrapPdf(TBuffer &buf, void *obj) {
      ((::TrapPdf*)obj)->::TrapPdf::Streamer(buf);
   }
} // end of namespace ROOT for class ::TrapPdf

//______________________________________________________________________________
void Powder2Pdf::Streamer(TBuffer &R__b)
{
   // Stream an object of class Powder2Pdf.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      L.Streamer(R__b);
      kd1.Streamer(R__b);
      kd2.Streamer(R__b);
      Pps.Streamer(R__b);
      lb.Streamer(R__b);
      ld1.Streamer(R__b);
      ld2.Streamer(R__b);
      lPs.Streamer(R__b);
      Mr.Streamer(R__b);
      mu.Streamer(R__b);
      l2g.Streamer(R__b);
      V0.Streamer(R__b);
      Vth.Streamer(R__b);
      chW.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, Powder2Pdf::IsA());
   } else {
      R__c = R__b.WriteVersion(Powder2Pdf::IsA(), kTRUE);
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      L.Streamer(R__b);
      kd1.Streamer(R__b);
      kd2.Streamer(R__b);
      Pps.Streamer(R__b);
      lb.Streamer(R__b);
      ld1.Streamer(R__b);
      ld2.Streamer(R__b);
      lPs.Streamer(R__b);
      Mr.Streamer(R__b);
      mu.Streamer(R__b);
      l2g.Streamer(R__b);
      V0.Streamer(R__b);
      Vth.Streamer(R__b);
      chW.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Powder2Pdf(void *p) {
      return  p ? new(p) ::Powder2Pdf : new ::Powder2Pdf;
   }
   static void *newArray_Powder2Pdf(Long_t nElements, void *p) {
      return p ? new(p) ::Powder2Pdf[nElements] : new ::Powder2Pdf[nElements];
   }
   // Wrapper around operator delete
   static void delete_Powder2Pdf(void *p) {
      delete ((::Powder2Pdf*)p);
   }
   static void deleteArray_Powder2Pdf(void *p) {
      delete [] ((::Powder2Pdf*)p);
   }
   static void destruct_Powder2Pdf(void *p) {
      typedef ::Powder2Pdf current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_Powder2Pdf(TBuffer &buf, void *obj) {
      ((::Powder2Pdf*)obj)->::Powder2Pdf::Streamer(buf);
   }
} // end of namespace ROOT for class ::Powder2Pdf

//______________________________________________________________________________
void ThreeExpPdf::Streamer(TBuffer &R__b)
{
   // Stream an object of class ThreeExpPdf.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      tau1.Streamer(R__b);
      tau2.Streamer(R__b);
      tau3.Streamer(R__b);
      i2.Streamer(R__b);
      i3.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, ThreeExpPdf::IsA());
   } else {
      R__c = R__b.WriteVersion(ThreeExpPdf::IsA(), kTRUE);
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      tau1.Streamer(R__b);
      tau2.Streamer(R__b);
      tau3.Streamer(R__b);
      i2.Streamer(R__b);
      i3.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ThreeExpPdf(void *p) {
      return  p ? new(p) ::ThreeExpPdf : new ::ThreeExpPdf;
   }
   static void *newArray_ThreeExpPdf(Long_t nElements, void *p) {
      return p ? new(p) ::ThreeExpPdf[nElements] : new ::ThreeExpPdf[nElements];
   }
   // Wrapper around operator delete
   static void delete_ThreeExpPdf(void *p) {
      delete ((::ThreeExpPdf*)p);
   }
   static void deleteArray_ThreeExpPdf(void *p) {
      delete [] ((::ThreeExpPdf*)p);
   }
   static void destruct_ThreeExpPdf(void *p) {
      typedef ::ThreeExpPdf current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ThreeExpPdf(TBuffer &buf, void *obj) {
      ((::ThreeExpPdf*)obj)->::ThreeExpPdf::Streamer(buf);
   }
} // end of namespace ROOT for class ::ThreeExpPdf

//______________________________________________________________________________
void TwoExpPdf::Streamer(TBuffer &R__b)
{
   // Stream an object of class TwoExpPdf.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      tau1.Streamer(R__b);
      tau2.Streamer(R__b);
      i2.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, TwoExpPdf::IsA());
   } else {
      R__c = R__b.WriteVersion(TwoExpPdf::IsA(), kTRUE);
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      tau1.Streamer(R__b);
      tau2.Streamer(R__b);
      i2.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TwoExpPdf(void *p) {
      return  p ? new(p) ::TwoExpPdf : new ::TwoExpPdf;
   }
   static void *newArray_TwoExpPdf(Long_t nElements, void *p) {
      return p ? new(p) ::TwoExpPdf[nElements] : new ::TwoExpPdf[nElements];
   }
   // Wrapper around operator delete
   static void delete_TwoExpPdf(void *p) {
      delete ((::TwoExpPdf*)p);
   }
   static void deleteArray_TwoExpPdf(void *p) {
      delete [] ((::TwoExpPdf*)p);
   }
   static void destruct_TwoExpPdf(void *p) {
      typedef ::TwoExpPdf current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TwoExpPdf(TBuffer &buf, void *obj) {
      ((::TwoExpPdf*)obj)->::TwoExpPdf::Streamer(buf);
   }
} // end of namespace ROOT for class ::TwoExpPdf

//______________________________________________________________________________
void ExpPdf::Streamer(TBuffer &R__b)
{
   // Stream an object of class ExpPdf.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      tau.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, ExpPdf::IsA());
   } else {
      R__c = R__b.WriteVersion(ExpPdf::IsA(), kTRUE);
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      tau.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ExpPdf(void *p) {
      return  p ? new(p) ::ExpPdf : new ::ExpPdf;
   }
   static void *newArray_ExpPdf(Long_t nElements, void *p) {
      return p ? new(p) ::ExpPdf[nElements] : new ::ExpPdf[nElements];
   }
   // Wrapper around operator delete
   static void delete_ExpPdf(void *p) {
      delete ((::ExpPdf*)p);
   }
   static void deleteArray_ExpPdf(void *p) {
      delete [] ((::ExpPdf*)p);
   }
   static void destruct_ExpPdf(void *p) {
      typedef ::ExpPdf current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ExpPdf(TBuffer &buf, void *obj) {
      ((::ExpPdf*)obj)->::ExpPdf::Streamer(buf);
   }
} // end of namespace ROOT for class ::ExpPdf

//______________________________________________________________________________
void ThreeGaussian::Streamer(TBuffer &R__b)
{
   // Stream an object of class ThreeGaussian.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      RooAbsPdf::Streamer(R__b);
      x.Streamer(R__b);
      mean.Streamer(R__b);
      sigma1.Streamer(R__b);
      sigma2.Streamer(R__b);
      sigma3.Streamer(R__b);
      i2.Streamer(R__b);
      i3.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, ThreeGaussian::IsA());
   } else {
      R__c = R__b.WriteVersion(ThreeGaussian::IsA(), kTRUE);
      RooAbsPdf::Streamer(R__b);
      x.Streamer(R__b);
      mean.Streamer(R__b);
      sigma1.Streamer(R__b);
      sigma2.Streamer(R__b);
      sigma3.Streamer(R__b);
      i2.Streamer(R__b);
      i3.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ThreeGaussian(void *p) {
      return  p ? new(p) ::ThreeGaussian : new ::ThreeGaussian;
   }
   static void *newArray_ThreeGaussian(Long_t nElements, void *p) {
      return p ? new(p) ::ThreeGaussian[nElements] : new ::ThreeGaussian[nElements];
   }
   // Wrapper around operator delete
   static void delete_ThreeGaussian(void *p) {
      delete ((::ThreeGaussian*)p);
   }
   static void deleteArray_ThreeGaussian(void *p) {
      delete [] ((::ThreeGaussian*)p);
   }
   static void destruct_ThreeGaussian(void *p) {
      typedef ::ThreeGaussian current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ThreeGaussian(TBuffer &buf, void *obj) {
      ((::ThreeGaussian*)obj)->::ThreeGaussian::Streamer(buf);
   }
} // end of namespace ROOT for class ::ThreeGaussian

//______________________________________________________________________________
void ConstantPdf::Streamer(TBuffer &R__b)
{
   // Stream an object of class ConstantPdf.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      bins.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, ConstantPdf::IsA());
   } else {
      R__c = R__b.WriteVersion(ConstantPdf::IsA(), kTRUE);
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      bins.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ConstantPdf(void *p) {
      return  p ? new(p) ::ConstantPdf : new ::ConstantPdf;
   }
   static void *newArray_ConstantPdf(Long_t nElements, void *p) {
      return p ? new(p) ::ConstantPdf[nElements] : new ::ConstantPdf[nElements];
   }
   // Wrapper around operator delete
   static void delete_ConstantPdf(void *p) {
      delete ((::ConstantPdf*)p);
   }
   static void deleteArray_ConstantPdf(void *p) {
      delete [] ((::ConstantPdf*)p);
   }
   static void destruct_ConstantPdf(void *p) {
      typedef ::ConstantPdf current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_ConstantPdf(TBuffer &buf, void *obj) {
      ((::ConstantPdf*)obj)->::ConstantPdf::Streamer(buf);
   }
} // end of namespace ROOT for class ::ConstantPdf

//______________________________________________________________________________
void PowderPdf::Streamer(TBuffer &R__b)
{
   // Stream an object of class PowderPdf.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      L.Streamer(R__b);
      kv.Streamer(R__b);
      Pps.Streamer(R__b);
      lb.Streamer(R__b);
      lv.Streamer(R__b);
      lPs.Streamer(R__b);
      Mr.Streamer(R__b);
      mu.Streamer(R__b);
      l2g.Streamer(R__b);
      V0.Streamer(R__b);
      Vth.Streamer(R__b);
      chW.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, PowderPdf::IsA());
   } else {
      R__c = R__b.WriteVersion(PowderPdf::IsA(), kTRUE);
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      L.Streamer(R__b);
      kv.Streamer(R__b);
      Pps.Streamer(R__b);
      lb.Streamer(R__b);
      lv.Streamer(R__b);
      lPs.Streamer(R__b);
      Mr.Streamer(R__b);
      mu.Streamer(R__b);
      l2g.Streamer(R__b);
      V0.Streamer(R__b);
      Vth.Streamer(R__b);
      chW.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PowderPdf(void *p) {
      return  p ? new(p) ::PowderPdf : new ::PowderPdf;
   }
   static void *newArray_PowderPdf(Long_t nElements, void *p) {
      return p ? new(p) ::PowderPdf[nElements] : new ::PowderPdf[nElements];
   }
   // Wrapper around operator delete
   static void delete_PowderPdf(void *p) {
      delete ((::PowderPdf*)p);
   }
   static void deleteArray_PowderPdf(void *p) {
      delete [] ((::PowderPdf*)p);
   }
   static void destruct_PowderPdf(void *p) {
      typedef ::PowderPdf current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_PowderPdf(TBuffer &buf, void *obj) {
      ((::PowderPdf*)obj)->::PowderPdf::Streamer(buf);
   }
} // end of namespace ROOT for class ::PowderPdf

//______________________________________________________________________________
void LiquidPdf::Streamer(TBuffer &R__b)
{
   // Stream an object of class LiquidPdf.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      Pqf.Streamer(R__b);
      lb.Streamer(R__b);
      lqf.Streamer(R__b);
      lplus.Streamer(R__b);
      lpo.Streamer(R__b);
      lopc.Streamer(R__b);
      lox.Streamer(R__b);
      l2g.Streamer(R__b);
      chW.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, LiquidPdf::IsA());
   } else {
      R__c = R__b.WriteVersion(LiquidPdf::IsA(), kTRUE);
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      Pqf.Streamer(R__b);
      lb.Streamer(R__b);
      lqf.Streamer(R__b);
      lplus.Streamer(R__b);
      lpo.Streamer(R__b);
      lopc.Streamer(R__b);
      lox.Streamer(R__b);
      l2g.Streamer(R__b);
      chW.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_LiquidPdf(void *p) {
      return  p ? new(p) ::LiquidPdf : new ::LiquidPdf;
   }
   static void *newArray_LiquidPdf(Long_t nElements, void *p) {
      return p ? new(p) ::LiquidPdf[nElements] : new ::LiquidPdf[nElements];
   }
   // Wrapper around operator delete
   static void delete_LiquidPdf(void *p) {
      delete ((::LiquidPdf*)p);
   }
   static void deleteArray_LiquidPdf(void *p) {
      delete [] ((::LiquidPdf*)p);
   }
   static void destruct_LiquidPdf(void *p) {
      typedef ::LiquidPdf current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_LiquidPdf(TBuffer &buf, void *obj) {
      ((::LiquidPdf*)obj)->::LiquidPdf::Streamer(buf);
   }
} // end of namespace ROOT for class ::LiquidPdf

//______________________________________________________________________________
void GrainPdf::Streamer(TBuffer &R__b)
{
   // Stream an object of class GrainPdf.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      lambdaJ.Streamer(R__b);
      lambdaBulk.Streamer(R__b);
      lambdaGrain.Streamer(R__b);
      lambdaVac.Streamer(R__b);
      kappaVac.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, GrainPdf::IsA());
   } else {
      R__c = R__b.WriteVersion(GrainPdf::IsA(), kTRUE);
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      lambdaJ.Streamer(R__b);
      lambdaBulk.Streamer(R__b);
      lambdaGrain.Streamer(R__b);
      lambdaVac.Streamer(R__b);
      kappaVac.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_GrainPdf(void *p) {
      return  p ? new(p) ::GrainPdf : new ::GrainPdf;
   }
   static void *newArray_GrainPdf(Long_t nElements, void *p) {
      return p ? new(p) ::GrainPdf[nElements] : new ::GrainPdf[nElements];
   }
   // Wrapper around operator delete
   static void delete_GrainPdf(void *p) {
      delete ((::GrainPdf*)p);
   }
   static void deleteArray_GrainPdf(void *p) {
      delete [] ((::GrainPdf*)p);
   }
   static void destruct_GrainPdf(void *p) {
      typedef ::GrainPdf current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_GrainPdf(TBuffer &buf, void *obj) {
      ((::GrainPdf*)obj)->::GrainPdf::Streamer(buf);
   }
} // end of namespace ROOT for class ::GrainPdf

//______________________________________________________________________________
void LiquidSimplePdf::Streamer(TBuffer &R__b)
{
   // Stream an object of class LiquidSimplePdf.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      Pps.Streamer(R__b);
      lplus.Streamer(R__b);
      lpo.Streamer(R__b);
      l2g.Streamer(R__b);
      lopc.Streamer(R__b);
      chW.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, LiquidSimplePdf::IsA());
   } else {
      R__c = R__b.WriteVersion(LiquidSimplePdf::IsA(), kTRUE);
      RooAbsPdf::Streamer(R__b);
      t.Streamer(R__b);
      Pps.Streamer(R__b);
      lplus.Streamer(R__b);
      lpo.Streamer(R__b);
      l2g.Streamer(R__b);
      lopc.Streamer(R__b);
      chW.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_LiquidSimplePdf(void *p) {
      return  p ? new(p) ::LiquidSimplePdf : new ::LiquidSimplePdf;
   }
   static void *newArray_LiquidSimplePdf(Long_t nElements, void *p) {
      return p ? new(p) ::LiquidSimplePdf[nElements] : new ::LiquidSimplePdf[nElements];
   }
   // Wrapper around operator delete
   static void delete_LiquidSimplePdf(void *p) {
      delete ((::LiquidSimplePdf*)p);
   }
   static void deleteArray_LiquidSimplePdf(void *p) {
      delete [] ((::LiquidSimplePdf*)p);
   }
   static void destruct_LiquidSimplePdf(void *p) {
      typedef ::LiquidSimplePdf current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_LiquidSimplePdf(TBuffer &buf, void *obj) {
      ((::LiquidSimplePdf*)obj)->::LiquidSimplePdf::Streamer(buf);
   }
} // end of namespace ROOT for class ::LiquidSimplePdf

namespace {
  void TriggerDictionaryInitialization_roopositronmIdictionary_Impl() {
    static const char* headers[] = {
"src/model/Constants.h",
"src/model/ParametersPool.h",
"src/roofit/ReverseAddPdf.h",
"src/roofit/ModelCommonizer.h",
"src/roofit/PdfFactory.h",
"src/roofit/providers/GrainProvider.h",
"src/roofit/providers/Powder2Provider.h",
"src/roofit/providers/ExpSourceProvider.h",
"src/roofit/providers/PowderProvider.h",
"src/roofit/providers/ThreeExpSourceProvider.h",
"src/roofit/providers/ThreeGaussProvider.h",
"src/roofit/providers/TwoExpProvider.h",
"src/roofit/providers/AbstractProvider.h",
"src/roofit/providers/LiquidProvider.h",
"src/roofit/providers/ExpProvider.h",
"src/roofit/providers/TwoGaussProvider.h",
"src/roofit/providers/TwoExpSourceProvider.h",
"src/roofit/providers/LiquidSimpleProvider.h",
"src/roofit/providers/OneGaussProvider.h",
"src/roofit/providers/TrappingProvider.h",
"src/roofit/providers/ThreeExpProvider.h",
"src/roofit/pdfs/TwoGaussian.h",
"src/roofit/pdfs/TrapPdf.h",
"src/roofit/pdfs/Powder2Pdf.h",
"src/roofit/pdfs/ThreeExpPdf.h",
"src/roofit/pdfs/TwoExpPdf.h",
"src/roofit/pdfs/ExpPdf.h",
"src/roofit/pdfs/ThreeGaussian.h",
"src/roofit/pdfs/ConstantPdf.h",
"src/roofit/pdfs/PowderPdf.h",
"src/roofit/pdfs/LiquidPdf.h",
"src/roofit/pdfs/GrainPdf.h",
"src/roofit/pdfs/LiquidSimplePdf.h",
"src/roofit/AdditiveConvolutionPdf.h",
"src/util/RootHelper.h",
"src/util/MathUtil.h",
"src/util/HistProcessor.h",
"src/util/FileUtils.h",
"src/util/ObjectNamer.h",
"src/util/Debug.h",
"src/util/StringUtils.h",
"src/util/GraphicsHelper.h",
0
    };
    static const char* includePaths[] = {
"/home/petrstepanov/Applications/root_v6.18.00/include",
"/home/petrstepanov/Applications/root_v6.18.00/include",
"/home/petrstepanov/Development/roopositron/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "roopositronmIdictionary dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$src/roofit/pdfs/TwoGaussian.h")))  TwoGaussian;
class __attribute__((annotate("$clingAutoload$src/roofit/pdfs/TrapPdf.h")))  TrapPdf;
class __attribute__((annotate("$clingAutoload$src/roofit/pdfs/Powder2Pdf.h")))  Powder2Pdf;
class __attribute__((annotate("$clingAutoload$src/roofit/pdfs/ThreeExpPdf.h")))  ThreeExpPdf;
class __attribute__((annotate("$clingAutoload$src/roofit/pdfs/TwoExpPdf.h")))  TwoExpPdf;
class __attribute__((annotate("$clingAutoload$src/roofit/pdfs/ExpPdf.h")))  ExpPdf;
class __attribute__((annotate("$clingAutoload$src/roofit/pdfs/ThreeGaussian.h")))  ThreeGaussian;
class __attribute__((annotate("$clingAutoload$src/roofit/pdfs/ConstantPdf.h")))  ConstantPdf;
class __attribute__((annotate("$clingAutoload$src/roofit/pdfs/PowderPdf.h")))  PowderPdf;
class __attribute__((annotate("$clingAutoload$src/roofit/pdfs/LiquidPdf.h")))  LiquidPdf;
class __attribute__((annotate("$clingAutoload$src/roofit/pdfs/GrainPdf.h")))  GrainPdf;
class __attribute__((annotate("$clingAutoload$src/roofit/pdfs/LiquidSimplePdf.h")))  LiquidSimplePdf;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "roopositronmIdictionary dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "src/model/Constants.h"
#include "src/model/ParametersPool.h"
#include "src/roofit/ReverseAddPdf.h"
#include "src/roofit/ModelCommonizer.h"
#include "src/roofit/PdfFactory.h"
#include "src/roofit/providers/GrainProvider.h"
#include "src/roofit/providers/Powder2Provider.h"
#include "src/roofit/providers/ExpSourceProvider.h"
#include "src/roofit/providers/PowderProvider.h"
#include "src/roofit/providers/ThreeExpSourceProvider.h"
#include "src/roofit/providers/ThreeGaussProvider.h"
#include "src/roofit/providers/TwoExpProvider.h"
#include "src/roofit/providers/AbstractProvider.h"
#include "src/roofit/providers/LiquidProvider.h"
#include "src/roofit/providers/ExpProvider.h"
#include "src/roofit/providers/TwoGaussProvider.h"
#include "src/roofit/providers/TwoExpSourceProvider.h"
#include "src/roofit/providers/LiquidSimpleProvider.h"
#include "src/roofit/providers/OneGaussProvider.h"
#include "src/roofit/providers/TrappingProvider.h"
#include "src/roofit/providers/ThreeExpProvider.h"
#include "src/roofit/pdfs/TwoGaussian.h"
#include "src/roofit/pdfs/TrapPdf.h"
#include "src/roofit/pdfs/Powder2Pdf.h"
#include "src/roofit/pdfs/ThreeExpPdf.h"
#include "src/roofit/pdfs/TwoExpPdf.h"
#include "src/roofit/pdfs/ExpPdf.h"
#include "src/roofit/pdfs/ThreeGaussian.h"
#include "src/roofit/pdfs/ConstantPdf.h"
#include "src/roofit/pdfs/PowderPdf.h"
#include "src/roofit/pdfs/LiquidPdf.h"
#include "src/roofit/pdfs/GrainPdf.h"
#include "src/roofit/pdfs/LiquidSimplePdf.h"
#include "src/roofit/AdditiveConvolutionPdf.h"
#include "src/util/RootHelper.h"
#include "src/util/MathUtil.h"
#include "src/util/HistProcessor.h"
#include "src/util/FileUtils.h"
#include "src/util/ObjectNamer.h"
#include "src/util/Debug.h"
#include "src/util/StringUtils.h"
#include "src/util/GraphicsHelper.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"ConstantPdf", payloadCode, "@",
"ExpPdf", payloadCode, "@",
"GrainPdf", payloadCode, "@",
"LiquidPdf", payloadCode, "@",
"LiquidSimplePdf", payloadCode, "@",
"Powder2Pdf", payloadCode, "@",
"PowderPdf", payloadCode, "@",
"ThreeExpPdf", payloadCode, "@",
"ThreeGaussian", payloadCode, "@",
"TrapPdf", payloadCode, "@",
"TwoExpPdf", payloadCode, "@",
"TwoGaussian", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("roopositron-dictionary",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_roopositronmIdictionary_Impl, {}, classesHeaders, /*has no C++ module*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_roopositronmIdictionary_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_roopositronmIdictionary() {
  TriggerDictionaryInitialization_roopositronmIdictionary_Impl();
}
