#ifndef __TYPES_H__
#define __TYPES_H__

#ifdef __BORLANDC__
#define WIN32
#endif

#ifdef WIN32
   #define STDCALL   _stdcall
#else
   #define STDCALL
#endif

//----------------------------------------------------------------------------- 
// Define types if necessary 
//----------------------------------------------------------------------------- 
#ifdef WIN32
   #include <windows.h>
   
   #define HUGE
   typedef PCHAR            PCHARHUGE;
   typedef double           DOUBLE;
   typedef double *         PDOUBLE;
   typedef long double      LDOUBLE;
   typedef unsigned char    UBYTE;
   typedef struct BOARDREGSTRUCT_
   {
     long iobase;
     long timeout;
     long psoiobase;
     long psotimeout;
	 long interrupt;
   } BOARDREGSTRUCT;

   typedef struct REGSTRUCT_
   {
     BOARDREGSTRUCT board[4];
	 long	lzrbase;
	 long boardtype; 
   } REGSTRUCT;
   typedef REGSTRUCT *PREGSTRUCT;

   //Structure used for fiber alignment
   typedef struct GPIBSTRUCT_
   {
//	   long gpib_on;			// mode control for GPIB data fetching 0=off/1=on
	   char wstring[1024];		// string write buffer
	   long bytes;				// number of bytes to read
	   long Dev;				// GPIB handle
	   long bdindx;				// index of the access board for the device
	   long pad;				// primary GPIB address of the device
	   long sad;				// secondary GPIB address of the device
	   long tmo;				// the I/O timeout value
	   long eot;				// EOI mode of the devide
	   long eos;				// EOS character mode
   } GPIBSTRUCT;
   typedef GPIBSTRUCT *PGPIBSTRUCT;

   
#else
     #ifdef UNIX
     typedef char*      PCHARHUGE;
     typedef unsigned char*	PUCHAR;
     #endif

   #ifndef __vtoolsc_h_
     #define HUGE             _huge
     
     #ifndef UNIX   
     typedef char HUGE *      PCHARHUGE;
     #endif
  	 typedef void             VOID;
     typedef short            INT;
     typedef char             BOOL;
     typedef int              BOOLEAN ;
     typedef char             BYTE;
     typedef char *           PBYTE;
  #endif
   typedef unsigned short   USHORT;
   typedef unsigned short * PUSHORT;
   typedef short            SHORT;
   typedef short *          PSHORT;
   typedef void *           PVOID;
   typedef long             LONG;
   typedef long *           PLONG;
   typedef unsigned long    ULONG;
   typedef unsigned long *  PULONG;
   typedef char             CHAR;
   typedef unsigned char    UCHAR;
   typedef char *           PCHAR;
   typedef double           DOUBLE;
   typedef double *         PDOUBLE;
   typedef long double      LDOUBLE;
   typedef PVOID            HANDLE;
   typedef unsigned char    UBYTE;
   typedef char *           LPSTR;
   typedef float            FLOAT;
   typedef long             DWORD;
#endif

typedef ULONG        AERERR_CODE;
typedef AERERR_CODE  *PAERERR_CODE;

// Auxilarry debug function 
typedef VOID (STDCALL *PFN_AER_AUXDEBUG)( CHAR *pszText );

#endif
