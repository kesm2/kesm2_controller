#ifndef __BUILD_H__
#define __BUILD_H__

/*
   Just so you know!

   The extended   attribute   syntax   for   specifying  storage-class
   information uses the __declspec keyword, which  specifies  that  an
   instance  of a given type is to be stored with a Microsoft-specific
   storage-class attribute (thread, naked, dllimport,  or  dllexport).
   Examples  of  other  storage-class modifiers include the static and
   extern keywords. However, these  keywords  are  part  of  the  ANSI
   specification  of  the  C  and  C++  languages, and as such are not
   covered by extended attribute syntax.

   The dllexport    and   dllimport   storage-class   attributes   are
   Microsoft-specific extensions to the  C  and  C++  languages.  They
   enable you to export and import functions, data, and objects to and
   from  a DLL. These attributes explicitly define the DLL's interface
   to its client, which can be the executable  file  or  another  DLL.
   Declaring   functions  as  dllexport  eliminates  the  need  for  a
   module-definition  (.DEF)  file,  at  least  with  respect  to  the
   specification  of  exported functions. Note that dllexport replaces
   the __export keyword. The declaration of  dllexport  and  dllimport
   uses extended attribute syntax.

*/

#ifdef WIN32
   #define DLLEXPORT __declspec(dllexport)
   #define DLLIMPORT __declspec(dllimport)
   #define STDCALL   _stdcall

   // defined by Aerotech Inc
   #ifdef __AER_SYSTEM_DLL__
      #define AER_LIBENTRY       DLLEXPORT STDCALL
      #define AER_LIBENTRY_CDECL DLLEXPORT
   #else
      #define AER_LIBENTRY       DLLIMPORT STDCALL
      #define AER_LIBENTRY_CDECL DLLIMPORT 
   #endif
	
   /* !Win32 */
#else
   #define DLLEXPORT
   #define DLLIMPORT
   #define STDCALL
   #define AER_LIBENTRY
   #define AER_LIBENTRY_CDECL
#endif

// this define is for "new" firmware which does not require axes to be mapped
//   into planes.
#define NOMAPPING

#define AXIS_MAPPING // cleanup/simplification of axis XYZU to drive 0-3 code
		     // AXIS1L..AXIS4L were previously XYZU and the qlib
		     // would convert to u500 channel/drive (0-3).
		     // Now they are the u500 channel/drive directly
		     // ie. in qlib AXIS1=channel/drive0, AXIS2=1, etc.
		     // ( requires firmware change also )
		     // V5.15 - ARW

#if defined WIN32
     #define DO_AXIS_5_8 // include code for U500PCI axis 5-8
#endif

 // define this to use allow 14 character axis names. V5.15
#define LONG_AXIS_NAMES

#endif
