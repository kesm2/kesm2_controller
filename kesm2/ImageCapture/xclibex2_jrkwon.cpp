/*
 *
 *	xclibex2.cpp	External	29-Oct-2004
 *
 *	Copyright (C)  1998-2003  EPIX, Inc.  All rights reserved.
 *
 *	Example program for the XCLIB 'C' Library.
 *	Assumes Windows 95/98/ME/NT/2000/XP.
 *
 *	Has dialog controls for PIXCI(R) SV2/SV3/SV4/SV5/SV5A/SV6.
 *	Can be used with other PIXCI(R) imaging boards,
 *	altho some controls may be meaningless.
 *
 *	Demonstrates XCLIB and/or PXIPL functions for capture and
 *	display of imagery and graphics.  Also demonstrates access
 *	to numeric pixel values, and saving of an image.
 *	This Windows program must, of course, also make use of
 *	various Windows GDI API functions; however, this is not
 *	intended to serve as a Windows tutorial.
 *
 *	Also optionally demonstrates use of multiple PIXCI(R)
 *	imaging boards operating simultaneously.
 *
 *
 */
#include <time.h>

#include "..\DataShare.h"

#pragma comment(lib, "XCLIBWNT.lib")
#pragma comment(lib, "PXIPLWNT.lib")

//#define DLG_TITLE "Image Capture for Brain Scan System v0.01 - Texas A&M CS Dept."
#define TIMER_ID_SEQ_SAVE	2

int gbCapturing = 0;

/*
 *  INSTRUCTIONS:
 *
 *  1)	Set 'define' options below according to the intended camera
 *	and video format.
 *
 *	For PIXCI(R) SV2, SV3, SV4, SV5, SV5A, and SV6 imaging boards
 *	common choices are RS-170, NSTC, NTSC/YC, CCIR, PAL, or PAL/YC.
 *	(The SV5A does not support NTSC/YC or PAL/YC).
 *
 *	For PIXCI(R) D, A, D24, D32, D2X, D3X, CL1, CL2, CL3SD, and SI
 *	imaging boards, use "default" to select the default format for
 *	the camera for which the PIXCI(R) imaging board is intended.
 *
 *	For non default formats, use XCAP to save the video set-up to
 *	a file, and set FORMATFILE to the saved file's path name.
 *	For camera's with RS-232 control, note that the saved
 *	video set-up only resets the PIXCI(R) imaging board's
 *	settings, but XCLIB does not reset the camera's settings.
 *
 *	Alternately, this could be modified to use getenv("PIXCI"),
 *	GetPrivateProfileString(...), RegQueryValueEx(...), or any
 *	other convention chosen by the programmer to allow run time
 *	selection of the video format and resolution.
 *
 */
				// For PIXCI(R) SV2, SV3, SV4, SV5, SV5A, SV6
//#define FORMAT  "RS-170"	// RS-170 on input 2
//#define FORMAT  "NTSC"	// NTSC on input 2
//#define FORMAT  "NTSC/YC"	// NSTC S-Video on input 1		(N/A on SV5A)
//#define FORMAT  "CCIR"	// CCIR on input 2
//#define FORMAT  "PAL" 	// PAL (B,D,G,H,I) on input 2
//#define FORMAT  "PAL/YC"	// PAL (B,D,G,H,I) S-Video on input 1	(N/A on SV5A)
//#define FORMAT  "default"	// NSTC S-Video on input 1

				// For PIXCI(R) D, D24, D32, A, D2X, D3X, CL1, CL2, CL3SD, SI
  #define FORMAT  "default"	// as per board's intended camera

				// For any PIXCI(R) imaging board
				// using a format file saved by XCAP
				// which can either be:
//#define FORMATFILE_LOAD   "xcvidset.fmt"  // loaded from file during execution
#define FORMATFILE_COMP   "xcvidset.fmt"  // or compiled into this application



/*
 *  2.1) Set number of expected PIXCI(R) image boards, from 1 to 4.
 *  The XCLIB Simple 'C' Functions expect that the boards are
 *  the same model and operated at the same resolution.
 */
#define UNITS	    1
#define UNITSMAP    ((1<<UNITS)-1)  /* shorthand - bitmap of all units */


/*
 *  2.2) Optionally, set driver configuration parameters.
 *  These are normally left to the default, "".
 *  The actual driver configuration parameters include the
 *  desired PIXCI(R) imaging boards, but to make configuation easier,
 *  code, below, will automatically add board selection to this.
 */
//#define DRIVERPARMS "-QU 0"	// don't use interrupts
  #define DRIVERPARMS ""	// default


/*
 *  3.1) Choose which form of image display (SHOWIM_), or image and
 *  graphics overlay (SHOWIG_), is to be demonstrated. Some of these
 *  options expect that the optional PXIPL library is present.
 *  Others may expect that the Windows DirectDraw SDK is present
 *  (available from Microsoft) and that the S/VGA supports DirectDraw.
 *
 *  The _DIRECTVIDEO options send video directly from compatible
 *  PIXCI(R) imaging cards to compatible S/VGA cards, rather
 *  than capturing to PC memory and displaying therefrom.
 *  Only SV2, SV3, SV4, SV5, SV5A, SV6 models can use these modes, and only
 *  with compatible S/VGA cards. Further, some S/VGA cards
 *  may support direct video but not support overlay graphics.
 *  These modes currently support only a single imaging board.
 *
 *  Only one of these choices should have value 1, the others should be 0.
 *
 */
#define SHOWIM_STRETCHDIBITS	1   // use XCLIB or XCLIB-Lite and GDI
#define SHOWIG_STRETCHDIBITS	0   // use XCLIB or XCLIB-Lite and GDI
#define SHOWIM_STRETCHDIBITS2	0   // use XCLIB or XCLIB-Lite and GDI
#define SHOWIG_STRETCHDIBITS2	0   // use XCLIB or XCLIB-Lite and GDI
#define SHOWIG_STRETCHDIBITS3	0   // use XCLIB, PXIPL and GDI
#define SHOWIM_DRAWDIBDRAW	0   // use XCLIB or XCLIB-Lite and Video for Windows
#define SHOWIM_DRAWDIBDISPLAY	0   // use XCLIB and PXIPL and Video for Windows
#define SHOWIM_GDIDISPLAY	0   // use XCLIB and PXIPL
#define SHOWIM_GDIDISPLAY2	0   // use XCLIB and PXIPL
#define SHOWIG_GDIDISPLAY	0   // use XCLIB and PXIPL
#define SHOWIM_DIRECTXDISPLAY	0   // use XCLIB and PXIPL and DirectDraw
#define SHOWIG_DIRECTVIDEO	0   // use XCLIB or XCLIB-Lite, DirectDraw, and S/VGA with support
				    // for compatible pixel formats
#define SHOWIM_DIRECTVIDEO	0   // use XCLIB or XCLIB-Lite, DirectDraw, and S/VGA with support
				    // for compatible pixel formats and overlay graphics

/*
 *  3.2) Optionally choose to show a pixel line profile
 *  graph, of the middle line of the first imaging board.
 *  If used, the number of UNITS should be limited to 3,
 *  as the graph is drawn in the space otherwised used by
 *  the fourth board.
 *
 *  This feature is incompatible with SHOWIG_DIRECTVIDEO
 *  and SHOWIM_DIRECTVIDEO options, above.
 *
 */
#define SHOW_GRAPH	0

/*
 *  3.3) Select how 'Live' mode should be demonstrated.
 *  Only one of these choices should have value 1, the others should be 0.
 *
 *  These options don't apply if the SHOWIG_DIRECTVIDEO
 *  or SHOWIM_DIRECTVIDEO options, above, are used.
 *
 */
#define LIVE_LIVE	1   // Continuous capture into frame buffer,
			    // display from same frame buffer while
			    // next frame is being captured.
#define LIVE_SNAP	0   // Snap single frame into frame buffer,
			    // display, repeat.
#define LIVE_LIVE2	0   // Continuous capture into alternate buffers,
			    // display from frame bufer which is not being
			    // captured into. Requires sufficient frame buffer
			    // memory for at least two frame buffers.

/*
 *  3.4) Select how display updates should be triggered.
 *  Only one of these choices should have value 1, the others should be 0.
 */
#define UPDATE_TIMER	1   // Use timer to periodically poll library
			    // and test for a new image to be displayed.
			    // Simpler for those not familiar with threads
			    // and events.
#define UPDATE_EVENT	0   // Use an event to trigger display of new image.
			    // Slightly more efficient, but requires use of
			    // threads and events.


/*
 *  4a) Compile with Microsoft Visual C/C++ V6.0 for Windows 95/98/ME as:
 *
 *	    cl -c xclibex2.cpp
 *	    rc.exe /l 0x409 /fo tt.res	xclibex2.rc
 *	    echo -subsystem:windows,4.0 -entry:WinMainCRTStartup comdlg32.lib ole32.lib >t1
 *	    echo /NODEFAULTLIB libc.lib oldnames.lib user32.lib gdi32.lib kernel32.lib vfw32.lib largeint.lib winmm.lib >t2
 *
 *	Either (XCLIB w/out PXIPL for Windows 95/98/ME)
 *	    link xclibex2.obj XCLIBW95.LIB tt.res @t1 @t2
 *	or  (XCLIB+PXIPL only for Windows 95/98/ME)
 *	    link xclibex2.obj XCLIBW95.LIB PXIPLW95.LIB tt.res @t1 @t2
 *	or (XCLIB-Lite for Windows 95/98/ME)
 *	    link xclibex2.obj XCLYBW95.LIB tt.res @t1 @t2
 *
 *	    del t1
 *	    del t2
 *	    del tt.res
 *	    del xclibex2.obj
 *
 *	Or use the IDE, compiling xclibex2.cpp as a Windows 95/98/ME app,
 *	using the XCLIBEX2.RC resource, linking to library(s):
 *	    XCLIB:	  XCLIBW95.LIB
 *	    XCLIB+PXIPL:  XCLIBW95.LIB PXIPLW95.LIB
 *	    XCLIB-Lite:   XCLYBW95.LIB
 *	For example, in the IDE for VISUAL C/C++ V5.0:
 *	    1. Start Integrated Development Environment.
 *	    2. Select 'File' - 'New'.
 *	    3. Enter Project Name and Select 'WIN 32 Application'.
 *	    4. Copy this file, XCLIBEX2.CPP, and XCLIBEX2.RC to your
 *	       project directory.
 *	    5. Select 'Project' - 'Add To Project' - 'Files ...' and select
 *	       this file, XCLIBEX2.CPP, and XCLIBEX2.RC.
 *	    6. Select 'Project' and 'Settings' or ALT+F7.
 *	    7. Select the 'C/C++' Tab and Category - 'Preprocessor'.
 *	    8. In the 'Additional Include Directories' text box add the
 *	       path for *.h files such as 'C:\XCLIB'.
 *	    9. While still in Project Settings, select 'Link' Tab.
 *	   10. In Category - 'General' go to the 'Object/Library Modules'
 *	       text box and add
 *		    XCLIB:	  C:\XCLIB\XCLIBW95.LIB
 *		    XCLIB+PXIPL:  C:\XCLIB\XCLIBW95.LIB C:\XCLIB\PXIPLW95.LIB
 *		    XCLIB-Lite:   C:\XCLIB\XCLYBW95.LIB
 *	       to the list of modules already there. Be certain to enter
 *	       a space between your entry and the adjacent name.
 *	   11. Click on OK.
 *	   12. On the Main Menu Bar, Select 'Build' and 'Rebuild All';
 *	       you should receive '0 error(s), 0 Warning(s)'.
 *	       If you receive a 'unresolved external symbol _main' error,
 *	       then the IDE was set to compile a DOS or console application,
 *	       rather than a Windows application (see 'Project', 'Settings',
 *	       and switch /subsystem:console to /subsystem:windows).
 *	   13. Copy the file(s)
 *		    XCLIB:	  XCLIBW95.DLL
 *		    XCLIB+PXIPL:  XCLIBW95.DLL PXIPLW95.DLL
 *		    XCLIB-Lite:   XCLYBW95.DLL
 *	       to this project's directory.
 *	   14. Go back to the 'Build' Menu and Select 'Execute' or CNTL+F5
 *	       to run program.
 *
 *	Note: Use of VISUAL C/C++ V5.0 requires Service Pack 3
 *	to be compatible with the library format produced by V6.0.
 *
 *	Note: Do not use the '-u' compiler option which removes all predefined macros.
 *
 *  4b) Compile with Microsoft Visual C/C++ V6.0 for Windows NT/2000/XP as:
 *	same as for Windows 95/98/ME, above, substituting XCLIBWNT.LIB
 *	for XCLIBW95.LIB, substituting PXIPLWNT.LIB for PXIPLW95.LIB,
 *	and substituting XCLYBWNT.LIB for XCLYBW95.LIB.
 *
 *	Either (XCLIB w/out PXIPL for Windows NT/2000/XP)
 *	    link xclibex2.obj XCLIBWNT.LIB tt.res @t1 @t2
 *	or  (XCLIB+PXIPL only for Windows NT/2000/XP)
 *	    link xclibex2.obj XCLIBWNT.LIB PXIPLWNT.LIB tt.res @t1 @t2
 *	or (XCLIB-Lite for Windows NT/2000/XP)
 *	    link xclibex2.obj XCLYBWNT.LIB tt.res @t1 @t2
 *
 *  4c) Compile with Microsoft Visual Studio .NET 2003 command line tools for Windows NT/2000/XP as:
 *
 *	Open the Visual Studio .Net 2003 Command Prompt.
 *	It is loacted under Programs-> Microsoft Visual Studio .NET 2003-> Visual Studio .NET Tools.
 *	Enter the following commands:
 *	    cl -c xclibex2.cpp
 *	    rc.exe /l 0x409 /fo tt.res	xclibex2.rc
 *	    echo -subsystem:windows,4.0 -entry:WinMainCRTStartup comdlg32.lib ole32.lib >t1
 *	    echo /NODEFAULTLIB libc.lib oldnames.lib user32.lib gdi32.lib kernel32.lib vfw32.lib largeint.lib winmm.lib >t2
 *
 *	Either (XCLIB w/out PXIPL for Windows NT/2000/XP)
 *	    link xclibex2.obj XCLIBWNT.LIB tt.res @t1 @t2
 *	or  (XCLIB+PXIPL only for Windows NT/2000/XP)
 *	    link xclibex2.obj XCLIBWNT.LIB PXIPLWNT.LIB tt.res @t1 @t2
 *	or (XCLIB-Lite for Windows NT/2000/XP)
 *	    link xclibex2.obj XCLYBWNT.LIB tt.res @t1 @t2
 *
 *	    del t1
 *	    del t2
 *	    del tt.res
 *	    del xclibex2.obj
 *
 *	Or use the IDE, compiling xclibex2.cpp as a Windows NT/2000/XP app,
 *	using the XCLIBEX2.RC resource, linking to library(s):
 *	    XCLIB:	  XCLIBWNT.LIB
 *	    XCLIB+PXIPL:  XCLIBWNT.LIB PXIPLWNT.LIB
 *	    XCLIB-Lite:   XCLYBWNT.LIB
 *	For example, in the IDE for Visual Studio .NET 2003:
 *	     1. Launch Microsoft Visual Studio .NET 2003.
 *	     2. Select 'File' -> 'New' -> 'Project'.
 *	     3. Select 'Win32 Project', do NOT select 'Win32 Console Project'.
 *	     4. Enter Project Name and press the 'OK' button, and then the 'Finish' button.
 *	     5. A window with example code will be displayed, replace with code from this file.
 *	     4. Copy XCLIBEX2.RC to your project directory.
 *	     5. Select 'Project' -> 'Add Existing Item' and select XCLIBEX2.RC.
 *	     6. Select 'Project' and 'Your Project Name" Properties'.
 *	     7. Change the Configuration selection from Active to All Configurations.
 *	     8. Under C/C++-> General-> Additional Include Directories specify 'C:\XCLIB'.
 *	     9. Under C/C++-> Precompiled Headers-> Create/Use Precompiled Header
 *		select 'Not Using Precompiled Headers'.
 *	    10. Under Linker-> General-> Additional Include Directories specify 'C:\XCLIB'.
 *	    11. Under Linker-> Input-> Additional Dependencies add
 *		    XCLIB:	  XCLIBWNT.LIB
 *		    XCLIB+PXIPL:  XCLIBWNT.LIB PXIPLWNT.LIB
 *		    XCLIB-Lite:   XCLYBWNT.LIB
 *		to the list of modules already there. Be certain to enter
 *		a space between your entry and the adjacent name.
 *	    12. Click on OK.
 *	    13. Copy the file(s)
 *		    XCLIB:	  XCLIBWNT.DLL
 *		    XCLIB+PXIPL:  XCLIBWNT.DLL PXIPLWNT.DLL
 *		    XCLIB-Lite:   XCLYBWNT.DLL
 *		  to this project's directory.
 *	    14. Press F5 to build and run program.
 *
 *  4d) Compile with Borland C++ V5.0 for Windows 95/98/ME, replacing the
 *	'c:\bc5\lib' with the appropriate path, such as:
 *
 *	    coff2omf XCLIBW95.LIB xcbcc.lib
 *	or  coff2omf XCLYBW95.LIB xcbcc.lib
 *
 *	    bcc32 -tW -X -O2 -c xclibex2.cpp
 *	    tlink32 /x /V4.0 /c -LC:\bc5\lib xclibex2.obj C:\bc5\lib\c0w32.obj,,, cw32.lib import32.lib xcbcc.lib,
 *
 *	Or, use the coff2omf as above, then use the IDE,
 *	compiling xclibex2.cpp as a Windows 95/98/ME app, linking with xcbcc.lib.
 *
 *	Borland's C++ V5.0 has several problems using Windows 95/98/ME DLLs
 *	and LIBs, as Borland C++ and C++ Builder products do not support
 *	Microsoft's new COFF library format. The coff2omf.exe conversion utility
 *	is provided with C++ Builder, or is available from Borland/Inprise
 *	at www.borland.com/bcppbuilder/freecompiler/cppc55steps.html (as of 7/2001).
 *
 *  4e) Compile with Borland C++ V5.0 for Windows NT/2000/XP:  same as for
 *	Windows 95/98/ME, above, substituting XCLIBWNT.LIB instead of XCLIBW95.LIB.
 *
 *  4f) Hints for Delphi users: As per Microsoft convention, the true function
 *	names in the XCLIB DLL are 'decorated'. The ctobas.exe utility
 *	translates XCLIB .H files into Visual Basic style declarations.
 *	These can be read with a text editor and the true function names
 *	ascertained for use in constructing Delphi declarations.
 *
 *	There are also utilities available from the Delphi Community
 *	website which translate C/C++ headers into Delphi. One such
 *	utility is 'HEADCONV'.
 *
 *  4g) Hints for Watcom users: The V10 or V11 linker does not recognize
 *	new Microsoft format (V6) library files. Use the Watcom WLIB utility,
 *	    wlib xclibw95.lib + xclibw95.dll
 *	or
 *	    wlib pxiplw95.lib + pxiplw95.dll
 *	or
 *	    wlib xclybw95.lib + xclybw95.dll
 *	to create a new .lib, in a supported format, from the .dll.
 *
 */


/*
 *  NECESSARY INCLUDES:
 */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#if SHOWIM_DRAWDIBDRAW || SHOWIM_DRAWDIBDISPLAY
    #include <vfw.h>
#endif
#if SHOWIM_DIRECTXDISPLAY
    #include <compobj.h>
    #include <ddraw.h>
#endif

extern "C" {
    #include "xcliball.h"
    #include "xclibex2.h"
    #if SHOWIM_GDIDISPLAY || SHOWIG_GDIDISPLAY || SHOWIM_DIRECTXDISPLAY || SHOWIG_STRETCHDIBITS3 || SHOWIM_GDIDISPLAY2 || SHOWIM_DRAWDIBDISPLAY
	#include "pxipl.h"         
	#include "pximages.h"           
    #endif
}

/*
 * Utility for SV 2/3/4/5.
 * Get current multiplexor input, and set controls.
 */
void setGuiMux(HWND hDlg)
{
    int   vidmux = pxd_getVidMux(1);

    SendMessage(GetDlgItem(hDlg, IDSVIDEO), BM_SETCHECK, vidmux == 1, 0);
    SendMessage(GetDlgItem(hDlg, IDBNC1),   BM_SETCHECK, vidmux == 2, 0);
    SendMessage(GetDlgItem(hDlg, IDBNC2),   BM_SETCHECK, vidmux == 3, 0);
}

/*
 * Utilities for SV2/3/4/5.
 * Get/set current brightness and gain/contrast.
 * These translate the XCLIB's generic floating, scaled,
 * values to integers, so they will be more
 * useful for scroll bars.
 */
int getBright()
{
    return( max(-128, min(127, (int)(pxd_getBrightness(1)*128/50.0))));
}
void setBright(int b)
{
    pxd_setContrastBrightness(UNITSMAP, pxd_getContrast(1), b*50.0/128.0);
}
int getGain()
{
    return( max(0, min(511, (int)((pxd_getContrast(1)*511.0+117.0)/236.57))));
}
void setGain(int b)
{
    pxd_setContrastBrightness(UNITSMAP, (b*236.57+255.0)/511.0, pxd_getBrightness(1));
}


/*
 * Global variables.
 */
static	HWND	hWnd;	/* the main window */
static	HWND	hDlg;	/* the main dialog */


/*
 * Thread to watch for field events, and advise
 * when the image should be redrawn.
 * To simplify synchronization with the main thread,
 * instead of directly drawing the image, a message is
 * posted to the main thread.
 */
#if UPDATE_EVENT
DWORD WINAPI ServiceThread(PVOID hEventR3)
{
    for (;;) {
	//
	// Wait for signal.
	//
	WaitForSingleObject((HANDLE)hEventR3, INFINITE);
	//
	// For very fast cameras (500+ fps?) or very slow
	// machines, drawing may not keep up. Don't flood the
	// message queue with redundant messages.
	//
	MSG	msg;
	if (PeekMessage(&msg, hDlg, WM_USER, WM_USER, PM_NOREMOVE|PM_NOYIELD) != 0)
	    continue;
	//
	// Send update image message.
	//
	PostMessage(hDlg, WM_USER, 0, 0);
    }
    return(0);
}
#endif


/*
 * The Dialog
 */
BOOL CALLBACK
PIXCIDialogProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    static  HWND	hWndImage;			    // child window of dialog for image display
    static  UINT	svgaBits;			    // pixel format of S/VGA
    static  int 	liveon = 0;
    static  pxvbtime_t	lastcapttime[UNITS] = {0};	    // when was image last captured
    static  struct	pxywindow windImage[max(4,UNITS)];  // subwindow of child window for image display

    #if SHOWIM_DIRECTXDISPLAY
	static LPDIRECTDRAW lpDD = NULL;
	static HINSTANCE    hDDLibrary = NULL;
    #endif
    #if SHOWIM_DRAWDIBDRAW || SHOWIM_DRAWDIBDISPLAY
	static HDRAWDIB     hDrawDib = NULL;
    #endif


    switch (wMsg) {
    case WM_INITDIALOG:
    {
	RECT	rectImage;

	//
	// Open the PIXCI(R) imaging board.
	// If this program were to only support a single PIXCI(R)
	// imaging board, the first parameter could be simplified to:
	//
	//	if (pxd_PIXCIopen("", FORMAT, NULL) < 0)
	//	    pxd__mesgFault(1);
	//
	// But, for the sake of multiple PIXCI(R) imaging boards
	// specify which units are to be used.
	//
	char driverparms[80];
	sprintf(driverparms, "-DM 0x%x %s", UNITSMAP, DRIVERPARMS);
	//
	// Either FORMAT or FORMATFILE_LOAD or FORMATFILE_COMP
	// should have been selected above.
	//
	#if defined(FORMAT)
	    if (pxd_PIXCIopen(driverparms, FORMAT, "") < 0)
		pxd_mesgFault(UNITSMAP);
	#endif
	#if defined(FORMATFILE_LOAD)
	    //
	    // The FORMATFILE can be read and loaded
	    // during the pxd_PIXCIopen(), for convenience
	    // of changing the format file without recompiling.
	    //
	    if (pxd_PIXCIopen(driverparms, "", FORMATFILE_LOAD) < 0)
		pxd_mesgFault(UNITSMAP);
	#endif
	#if defined(FORMATFILE_COMP)
	    //
	    // Or the FORMATFILE can be compiled into this application,
	    // reducing the number of files that must be distributed, or
	    // possibly lost.
	    //
	    // Note: On MSVC 6.0, if the precompiled header option is used,
	    // the compiler objects to this code (error C2006) when
	    // FORMATFILE_COMP is not defined and thus not intended to be used,
	    // even though this code shouldn't be compiled when
	    // FORMATFILE_COMP is not defined.
	    // Either turn off the 'Use Precompiled Headers' option,
	    // remove this code, or choose to use the FORMATFILE_COMP option.
	    //
	    if (pxd_PIXCIopen(driverparms, "Default", "") < 0)
		pxd_mesgFault(UNITSMAP);
	    {
		#include FORMATFILE_COMP
		pxd_videoFormatAsIncluded(0);
	    }
	#endif

	//
	// Set our title.
	//
	SetWindowText(hDlg, BSS_DCAPTURE_TITLE);
	//SetWindowText(hDlg, "EPIX(R) PIXCI(R) Imaging Board Example");

	//
	// Tweak controls.
	// The SV5A has a BNC connector, rather than an S-Video connector.
	//
	if (pxd_infoModel(1) == PIXCI_SV5 && pxd_infoSubmodel(1) == PIXCI_SV5_SV5A)
	    SetWindowText(GetDlgItem(hDlg, IDSVIDEO), "BNC 0");


	//
	// Enable timer, for live video updates.
	// Or, create event and thread for live video updates.
	// Use of the timer is simpler; use of an event and thread
	// is only slightly more efficient.
	//
	#if UPDATE_TIMER
	    SetTimer(hDlg, 1, 5, NULL);
	#endif
	#if UPDATE_EVENT
	    HANDLE  Event;
	    DWORD   ThreadId;
	    Event = pxd_eventCapturedFieldCreate(0x1);
	    if (!Event)
		MessageBox(NULL, "Can't create image update display event!", "XCLIBEX2", MB_OK|MB_TASKMODAL);
	    else
		CreateThread(0, 0x1000, ServiceThread, Event, 0, &ThreadId);
	    //
	    // Timer pops every .25 seconds to check for faults,
	    // not display video.
	    //
	    SetTimer(hDlg, 1, 250, NULL);
	#endif

	//
	// Get handle to image display area of dialog,
	// then get its device context and size.
	//
	hWndImage = GetDlgItem(hDlg, IDIMAGE);
	{
	    HDC  hDC = GetDC(hWndImage);
	    GetClientRect(hWndImage, &rectImage);
	    svgaBits = GetDeviceCaps(hDC, PLANES) * GetDeviceCaps(hDC, BITSPIXEL);
	    ReleaseDC(hWndImage, hDC);
	}

	//
	// Determine displayed size.
	// We could simply fill up the hWndImage, but
	// much rather adjust the displayed image for
	// correct aspect ratio.
	//
	windImage[0].nw.x = windImage[0].nw.y = 0;
	windImage[0].se.x = rectImage.right+1;		 // inclusive->exclusive
	windImage[0].se.y = rectImage.bottom+1; 	 // inclusive->exclusive
	{
	    double  scalex, scaley, aspect;
	    aspect = pxd_imageAspectRatio();
	    if (aspect == 0.0)
		aspect = 1.0;
	    scalex = windImage[0].se.x/(double)pxd_imageXdim();
	    scaley = windImage[0].se.y/((double)pxd_imageYdim()*aspect);
	    scalex = min(scalex, scaley);
	    windImage[0].se.x = (int)(pxd_imageXdim() * scalex);
	    windImage[0].se.y = (int)(pxd_imageYdim() * scalex * aspect);
	}

	//
	// If StrecthDIBits is to be used, some VGA card drivers
	// abhor horizontal dimensions which are not a multiple of 4.
	// This isn't needed for other rendering methods, but doesn't hurt.
	//
	windImage[0].se.x &= ~3;

	//
	// For multiple units, display each of four units
	// in quadrant of display area.
	// Or, even for one unit, with the optional graph enabled.
	//
	if (UNITS > 1 || SHOW_GRAPH) {
	    windImage[0].se.x &= ~0xF;	 // See above StretchDIBits comment above
	    windImage[1] = windImage[0];
	    windImage[2] = windImage[0];
	    windImage[3] = windImage[0];
	    windImage[0].se.x /= 2;
	    windImage[0].se.y /= 2;
	    windImage[1].nw.x = windImage[1].se.x/2;
	    windImage[1].se.y /= 2;
	    windImage[2].se.x /= 2;
	    windImage[2].nw.y = windImage[2].se.y/2;
	    windImage[3].nw.x = windImage[3].se.x/2;
	    windImage[3].nw.y = windImage[3].se.y/2;
	}

	//
	// Customize and init dialog controls as per current PIXCI(R)
	// imaging board model. Note that the pxd_infoModel()
	// for the SV5 and SV5A is the same.
	//
	switch (pxd_infoModel(UNITSMAP)) {
	  case PIXCI_SV2:
	    EnableWindow(GetDlgItem(hDlg, IDBNC2), FALSE);
	    // fall thru
	  case PIXCI_SV3:
	    // fall thru
	  case PIXCI_SV6:
	    // fall thru
	  case PIXCI_SV4:
	    // fall thru
	  case PIXCI_SV5:
	    SetScrollRange(GetDlgItem(hDlg, IDGAINSCROLL),   SB_CTL, 0, 511, TRUE);
	    SetScrollRange(GetDlgItem(hDlg, IDOFFSETSCROLL), SB_CTL, 0, 255, TRUE);
	    setGuiMux(hDlg);
	    SetScrollPos(GetDlgItem(hDlg, IDOFFSETSCROLL), SB_CTL, getBright()+128, TRUE);
	    SetScrollPos(GetDlgItem(hDlg, IDGAINSCROLL),   SB_CTL, getGain(), TRUE);
	    break;
	  case PIXCI_D:
	  case PIXCI_D24:
	  case PIXCI_D32:
	  case PIXCI_A:
	  case PIXCI_D2X:
	  case PIXCI_D3X:
	  case PIXCI_CL1:
	  case PIXCI_CL2:
	  case PIXCI_CL3SD:
	  case PIXCI_SI:
	  default:
	    EnableWindow(GetDlgItem(hDlg, IDBNC2),   FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDBNC1),   FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDSVIDEO), FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDGAINSCROLL),   FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDOFFSETSCROLL), FALSE);
	    break;
	}

	//
	// If using DirectDraw, initialize access to it.
	//
	// DirectDraw may not be available!
	// Error reporting should be added!
	//
	#if SHOWIM_DIRECTXDISPLAY
	{
	    HRESULT	    h;
	    hDDLibrary = LoadLibrary("DDRAW");
	    if (hDDLibrary) {
		typedef HRESULT (WINAPI* OPEN)(void FAR*,LPDIRECTDRAW FAR*, void FAR*);
		OPEN	lpfnDM;
		lpfnDM = (OPEN)GetProcAddress(hDDLibrary, "DirectDrawCreate");
		if (lpfnDM) {
		    h = (*lpfnDM)(NULL, &lpDD, NULL);
		    if (lpDD) {
			h = lpDD->SetCooperativeLevel((HWND)hWnd, DDSCL_NORMAL);
		    }
		}
	    }
	}
	#endif

	//
	// If using Video for Windows, initialize access to it.
	//
	#if SHOWIM_DRAWDIBDRAW || SHOWIM_DRAWDIBDISPLAY
	    hDrawDib = DrawDibOpen();
	#endif

	//
	// If using Direct Video to S/VGA, initialize access to it.
	//
	#if SHOWIG_DIRECTVIDEO | SHOWIM_DIRECTVIDEO
	    if (pxd_renderDirectVideoInit(UNITSMAP, hWnd) < 0)
		MessageBox(NULL, "Direct Video not available!", "XCLIBEX2", MB_OK|MB_TASKMODAL);
	#endif

	//
	// Warn if LIVE_LIVE2 was selected and there is only one buffer.
	//
	#if LIVE_LIVE2
	    if (pxd_imageZdim() < 2)
		MessageBox(NULL, "Only 1 image frame buffer available!", "XCLIBEX2", MB_OK|MB_TASKMODAL);
	#endif

	// jrkwon
	{
		HWND hWnd = FindWindow(NULL, BSS_STAGECTLR_TITLE);
		if(hWnd)
		{
			PostMessage(hWnd, WM_APP_BSS, WP_DC_LAUNCH_DONE, 0);
		}
	}
	return(TRUE);
    }

    case WM_COMMAND:
	switch (LOWORD(wParam)) {

	case IDSNAP:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    #if SHOWIG_DIRECTVIDEO | SHOWIM_DIRECTVIDEO
		if (liveon) {
		    pxd_renderDirectVideoUnLive(UNITSMAP, hWndImage);
		    liveon = FALSE;
		} else {
		    MessageBox(NULL, "Snap not available with the SHOWIG_DIRECTVIDEO option!", "XCLIBEX2", MB_OK|MB_TASKMODAL);
		}
	    #else
		if (liveon) {
		    pxd_goUnLive(UNITSMAP);
		    liveon = FALSE;
		    return(TRUE);
		}

		//
		// As the UPDATE_TIMER or UPDATE_EVENT code monitors
		// completed capture buffers, there is no need to wait
		// for the snap to be done.
		//
		pxd_goSnap(UNITSMAP, 1);
	    #endif
	    return(TRUE);

	case IDLIVE:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    liveon = TRUE;
	    #if SHOWIG_DIRECTVIDEO
		if (pxd_renderDirectVideoLive(UNITSMAP, hWndImage, windImage[0].nw.x, windImage[0].nw.y,
					windImage[0].se.x, windImage[0].se.y, CLR_INVALID, CLR_INVALID) < 0)
		    MessageBox(NULL, "Direct Video not available!", "XCLIBEX2", MB_OK|MB_TASKMODAL);
	    #elif SHOWIM_DIRECTVIDEO
		if (pxd_renderDirectVideoLive(UNITSMAP, hWndImage, windImage[0].nw.x, windImage[0].nw.y,
				    windImage[0].se.x, windImage[0].se.y, RGB(0,0,189), RGB(0,0,189)) < 0)
		    MessageBox(NULL, "Direct Video with graphics overlay not available!", "XCLIBEX2", MB_OK|MB_TASKMODAL);
		//
		// If we are using Chroma keying, then any graphics or text
		// written via GDI (i.e. the 'normal' method of putting
		// graphics on the window) will appear, so long as the
		// graphics isn't drawn in the RGB(0,0,189) color.
		//
		HDC hDC;
		if (hDC = GetDC(hWndImage)) {
		    RECT rect;
		    SetRect(&rect, 20, 20, 40, 40);
		    //DrawFocusRect(hDC, &rect);
		    InvertRect(hDC, &rect);
		    ReleaseDC(hWnd, hDC);
		}
	    #elif LIVE_LIVE
		pxd_goLive(UNITSMAP, 1L);
	    #elif LIVE_LIVE2
		pxd_goLivePair(UNITSMAP, 1L, 2L);
	    #elif LIVE_SNAP
		pxd_goSnap(UNITSMAP, 1);
	    #endif
	    return(TRUE);

	case IDUNLIVE:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    #if SHOWIG_DIRECTVIDEO | SHOWIM_DIRECTVIDEO
		pxd_renderDirectVideoUnLive(UNITSMAP, hWndImage);
		liveon = FALSE;
	    #elif LIVE_LIVE
		pxd_goUnLive(UNITSMAP);
		liveon = FALSE;
	    #elif LIVE_LIVE2
		pxd_goUnLive(UNITSMAP);
		liveon = FALSE;
	    #elif LIVE_SNAP
		liveon = FALSE;
	    #endif
	    return(TRUE);

	case IDSVIDEO:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    pxd_setVidMux(UNITSMAP, 1);
	    setGuiMux(hDlg);
	    return(TRUE);

	case IDBNC1:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    pxd_setVidMux(UNITSMAP, 2);
	    setGuiMux(hDlg);
	    return(TRUE);

	case IDBNC2:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    pxd_setVidMux(UNITSMAP, 3);
	    setGuiMux(hDlg);
	    return(TRUE);

	case IDSAVE:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    {
		OPENFILENAME ofn;
		char	pathname[_MAX_PATH] = "";
		int	r;
		memset(&ofn, 0, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner	= hWnd;
		ofn.lpstrFilter = "TIFF Files (*.tif)\0*.tif\0\0";
		ofn.lpstrFile	= pathname;
		ofn.nMaxFile	= _MAX_PATH;
		ofn.lpstrTitle	= "Save";
		ofn.Flags   |= OFN_EXPLORER|OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY;
		r = GetSaveFileName(&ofn);
		if (r != 0) {
		    r = pxd_saveTiff(0x1, pathname, 1, 0, 0, -1, -1, 0, 0);
		    if (r < 0)
			MessageBox(NULL, pxd_mesgErrorCode(r), "Error!", MB_OK|MB_TASKMODAL);
		}
	    }
	    return(TRUE);

	case IDSTART:
	    if (HIWORD(wParam) != BN_CLICKED)
			return(FALSE);

		//SetTimer(hDlg, TIMER_ID_SEQ_SAVE, 1, NULL);
		SetTimer(hDlg, TIMER_ID_SEQ_SAVE, 1000, NULL); // 1 sec
		gbCapturing = 1;

	    return(TRUE);

	case IDSTOP:
	    if (HIWORD(wParam) != BN_CLICKED)
			return(FALSE);

		KillTimer(hDlg, TIMER_ID_SEQ_SAVE);
		gbCapturing = 0;

	    return(TRUE);

	}
	break;

    case WM_HSCROLL:
      {
	HWND hCtrl = (HWND)lParam;
	switch (GetWindowLong(hCtrl, GWL_ID)) {
	case IDGAINSCROLL:
	  {
	    int g = getGain();
	    switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    g += 5;	break;
	    case SB_LINEDOWN:	    g += 1;	break;
	    case SB_PAGEUP:	    g -= 5;	break;
	    case SB_LINEUP:	    g -= 1;	break;
	    case SB_TOP:	    g = 511;	break;
	    case SB_BOTTOM:	    g = 0;	break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     g = HIWORD(wParam); break;
	    default:
		return(FALSE);
	    }
	    setGain(g);
	    SetScrollPos(GetDlgItem(hDlg, IDGAINSCROLL), SB_CTL, getGain(), TRUE);
	    return(TRUE);
	  }
	case IDOFFSETSCROLL:
	  {
	    int g = getBright()+128;
	    switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    g += 5;	break;
	    case SB_LINEDOWN:	    g += 1;	break;
	    case SB_PAGEUP:	    g -= 5;	break;
	    case SB_LINEUP:	    g -= 1;	break;
	    case SB_TOP:	    g = 255;	break;
	    case SB_BOTTOM:	    g = 0;	break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     g = HIWORD(wParam); break;
	    default:
		return(FALSE);
	    }
	    setBright(g-128);
	    SetScrollPos(GetDlgItem(hDlg, IDOFFSETSCROLL), SB_CTL, getBright()+128, TRUE);
	    return(TRUE);
	  }
	}
	return(FALSE);
      }

    case WM_CLOSE:
	pxd_PIXCIclose();
	//DestroyWindow(GetParent(hDlg));
	#if SHOWIM_DIRECTXDISPLAY
	    if (lpDD)
		lpDD->Release();
	    if (hDDLibrary)
		FreeLibrary(hDDLibrary);
	    lpDD = NULL;
	    hDDLibrary = NULL;
	#endif
	#if SHOWIM_DRAWDIBDRAW || SHOWIM_DRAWDIBDISPLAY
	    if (hDrawDib)
		DrawDibClose(hDrawDib);
	    hDrawDib = NULL;
	#endif
	#if SHOWIG_DIRECTVIDEO | SHOWIM_DIRECTVIDEO
	    pxd_renderDirectVideoDone(UNITSMAP, hWnd);
	#endif

	DestroyWindow(hWnd);
	EndDialog(hDlg, 0);
	return(TRUE);

    case WM_WINDOWPOSCHANGING:
    case WM_WINDOWPOSCHANGED:
	//
	// When using pxd_renderDirectVideo,
	// the window position must be tracked.
	//
	#if SHOWIG_DIRECTVIDEO | SHOWIM_DIRECTVIDEO
	    if (liveon)
		pxd_renderDirectVideoUnLive(UNITSMAP, hWndImage);
	#endif
	break;

    case WM_MOVE:
	#if SHOWIG_DIRECTVIDEO
	    if (liveon)
		pxd_renderDirectVideoLive(UNITSMAP, hWndImage, windImage[0].nw.x, windImage[0].nw.y,
				    windImage[0].se.x, windImage[0].se.y, CLR_INVALID, CLR_INVALID);
	#elif SHOWIM_DIRECTVIDEO
	    if (liveon)
		pxd_renderDirectVideoLive(UNITSMAP, hWndImage, windImage[0].nw.x, windImage[0].nw.y,
				    windImage[0].se.x, windImage[0].se.y, RGB(0,0,189), CLR_INVALID);
	#endif
	break;

	// jrkwon
	case WM_APP_BSS:
	{
		HWND hWnd;

		if(wParam == WP_START_CAPTURE)
		{
			hWnd = FindWindow(NULL, BSS_STAGECTLR_TITLE);
			if(hWnd)
			{
				PostMessage(hWnd, WM_APP_BSS, WP_START_CAPTURE, 0);
			}

			SetTimer(hDlg, TIMER_ID_SEQ_SAVE, 1000, NULL); // 1 sec
			gbCapturing = 1;

			// live
			liveon = TRUE;
			#if LIVE_LIVE
			pxd_goLive(UNITSMAP, 1L);
			#endif

			return TRUE;
		}
		else if(wParam == WP_STOP_CAPTURE)
			{
			hWnd = FindWindow(NULL, BSS_STAGECTLR_TITLE);
			if(hWnd)
			{
				PostMessage(hWnd, WM_APP_BSS, WP_STOP_CAPTURE, 0);
			}
			KillTimer(hDlg, TIMER_ID_SEQ_SAVE);
			gbCapturing = 0;

			// unlive
			liveon = FALSE;
			#if LIVE_LIVE
			pxd_goUnLive(UNITSMAP);
			#endif

			return TRUE;
		}
	}

    case WM_TIMER:

	if(wParam == TIMER_ID_SEQ_SAVE)
	{
		char	szFileName[_MAX_PATH] = "";
		char szPathName[_MAX_PATH];
		char buf[100];
		//SYSTEMTIME sys;
		int	r;
		static pxbuffer_t lastbuf = 0;
		UINT32 capturedSysTick = 0;
		// jrkwon
		struct tm *newtime;
		time_t aclock;

		if(pxd_capturedSysTicks(1) == capturedSysTick)
			return TRUE;
		capturedSysTick = pxd_capturedSysTicks(1);

		//GetCurrentTime(&sys);

		time( &aclock );   // Get time in seconds
		newtime = localtime( &aclock );   // Convert time to struct tm form 

		sprintf(szFileName, "%d-%02d-%02d %02d %02d %02d.tif", 
			1900+newtime->tm_year, newtime->tm_mon+1, newtime->tm_mday, newtime->tm_hour, newtime->tm_min, newtime->tm_sec);
			//sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
		
		sprintf(szPathName, "..\\Data\\%s", szFileName);
		r = pxd_saveTiff(0x1, szPathName, 1, 0, 0, -1, -1, 0, 0);
		if (r < 0)
		{
			HWND hWnd = FindWindow(NULL, BSS_DDISPATCHER_TITLE);//BSS_STAGECTLR_TITLE);
			if(hWnd)
			{
				PostMessage(hWnd, WM_APP_BSS, WP_DC_SAVE_ERROR, 0);
			}
			//MessageBox(NULL, pxd_mesgErrorCode(r), "Error!", MB_OK|MB_TASKMODAL);
			KillTimer(hDlg, TIMER_ID_SEQ_SAVE);
		}
		else
		{
			HWND hWnd = FindWindow(NULL, BSS_DDISPATCHER_TITLE);//BSS_STAGECTLR_TITLE);
			if(hWnd)
			{
				// send filename data
				{
					COPYDATASTRUCT cpd;
					cpd.dwData = 0;
					cpd.cbData = strlen(szFileName)+1; //strDataToSend.GetLength();
					cpd.lpData = (void*)szFileName; //strDataToSend.GetBuffer(cpd.cbData);
					SendMessage(hWnd, WM_COPYDATA, (WPARAM)hDlg, (LPARAM)&cpd);
				}
				PostMessage(hWnd, WM_APP_BSS, WP_DC_SAVE_DONE, 0L);
			}
		}

		sprintf(buf, "Image Capturing : %s", szFileName);
		SetWindowText(GetDlgItem(hDlg, IDSTATUS), buf);

		return TRUE;
	}

	//
	// Monitor for asynchronous faults, such as video
	// being disconnected while capturing. These faults
	// can't be reported by functions such as pxd_goLive()
	// which initiate capture and return immediately.
	//
	// Should there be a fault and pxd_mesgFault() pop up a dialog,
	// the Windows TIMER will continue in a new thread. Thus the
	// 'faulting' variable and logic to limit to one dialog at a time.
	//
	if ((gbCapturing == 0) && pxd_infoUnits()) {	 // implies whether librry is open
	    static int faulting = 0;
	    if (!faulting) {
		faulting++;
		pxd_mesgFault(UNITSMAP);
		faulting--;
	    }
	    //
	    // Show frame rate and resolution.
	    //
	    static DWORD lasttickcount = 0;
	    static ulong lastfieldcount = 0;
	    DWORD ticks = GetTickCount();
	    if (ticks - lasttickcount > 1000) {
		ulong fields = pxd_getFieldCount(1);
		char  buf[100];
		sprintf(buf, "fps: %.5g  dim: %dx%d",
			lasttickcount?1000.0*(double)(fields-lastfieldcount)/((ticks-lasttickcount)*pxd_videoFieldsPerFrame()):0,
			pxd_imageXdim(), pxd_imageYdim());
		SetWindowText(GetDlgItem(hDlg, IDSTATUS), buf);
		lasttickcount = ticks;
		lastfieldcount = fields;
	    }
	}
	#if UPDATE_EVENT
	    return(TRUE);   // if using UPDATE_EVENT, don't update image display
	#endif
	// fall thru!	    // if using UPDATE_TIMER, update image display

    case WM_USER:	    // if using UPDATE_EVENT, update image display

	//
	// Has a new field or frame been captured
	// since the last time we checked?
	//
	for (int u = 0; u < UNITS; u++) {
	    HDC  hDC;
	    pxvbtime_t lasttime = pxd_capturedFieldCount(1<<u);
	    if (lastcapttime[u] == lasttime)
		continue;
	    lastcapttime[u] = lasttime;

	    //
	    // In SHOWIG_DIRECTVIDEO mode, display is
	    // already done.
	    //
	    #if SHOWIG_DIRECTVIDEO | SHOWIM_DIRECTVIDEO
		continue;
	    #endif

	    hDC = GetDC(hWndImage);

	    //
	    // In LIVE_LIVE2 mode the captured buffer
	    // rotates between 1 and 2.
	    // In other modes, the code above always
	    // does snap or live into buffer 1.
	    //
	    pxbuffer_t	buf = 1;
	    #if LIVE_LIVE2
		buf = pxd_capturedBuffer(1<<u);
	    #endif

	    //
	    // Show image using pxd_renderStretchDIBits.
	    //
	    #if SHOWIM_STRETCHDIBITS
		SetStretchBltMode(hDC, STRETCH_DELETESCANS);
		pxd_renderStretchDIBits(1<<u, buf, 0, 0, -1, -1, 0,
				 hDC, windImage[u].nw.x, windImage[u].nw.y,
				 windImage[u].se.x-windImage[u].nw.x,
				 windImage[u].se.y-windImage[u].nw.y, 0);
	    #endif

	    //
	    // Show image using pxd_renderStretchDIBits
	    // and graphics using GDI.
	    // As this draws graphics after the image, a fast
	    // update rate may cause the graphics to appear to flicker
	    // (depending on the vga speed, vga display rate, etc).
	    //
	    #if SHOWIG_STRETCHDIBITS
	    {
		RECT rect;
		SetStretchBltMode(hDC, STRETCH_DELETESCANS);
		pxd_renderStretchDIBits(1<<u, buf, 0, 0, -1, -1, 0,
				  hDC, windImage[u].nw.x, windImage[u].nw.y,
				  windImage[u].se.x-windImage[u].nw.x,
				  windImage[u].se.y-windImage[u].nw.y, 0);
		SetRect(&rect, windImage[u].nw.x+(windImage[u].se.x-windImage[u].nw.x)/4,
			       windImage[u].nw.y+(windImage[u].se.y-windImage[u].nw.y)/4,
			       windImage[u].nw.x+(windImage[u].se.x-windImage[u].nw.x)*3/4,
			       windImage[u].nw.y+(windImage[u].se.y-windImage[u].nw.y)*3/4);
		DrawFocusRect(hDC, &rect);
		//
		// This, optionally, slows down the image update rate, reducing flicker.
		//
		lastcapttime[u] += 10;
	    }
	    #endif

	    //
	    // Show image using pxd_renderDIBCreate and StretchDIBits.
	    // This may be less efficient than pxd_renderStretchDIBits,
	    // but would allow the application to modify the bitmap
	    // before display.
	    //
	    #if SHOWIM_STRETCHDIBITS2
	    {
		HGLOBAL hDIB;
		BITMAPINFOHEADER FAR *dib;

		hDIB = pxd_renderDIBCreate(1<<u, buf, 0, 0, -1, -1, 0, 0);
		if (hDIB) {
		    if (dib = (BITMAPINFOHEADER FAR *)GlobalLock(hDIB)) {
			SetStretchBltMode(hDC, STRETCH_DELETESCANS);
			StretchDIBits(hDC, windImage[u].nw.x, windImage[u].nw.y,
					   windImage[u].se.x-windImage[u].nw.x, windImage[u].se.y-windImage[u].nw.y,
					   0, 0, pxd_imageXdim(), pxd_imageYdim(),
				    (uchar FAR*)dib+dib->biSize+dib->biClrUsed*sizeof(RGBQUAD),
				    (BITMAPINFO *)dib, DIB_RGB_COLORS, SRCCOPY);
			GlobalUnlock(hDIB);
		    }
		    pxd_renderDIBFree(hDIB);
		}
	    }
	    #endif


	    //
	    // Show image using pxd_renderDIBCreate and graphics using GDI.
	    // Uses an off-screen buffer so the displayed
	    // graphics do not flicker. Note that the graphics
	    // are drawn in (a copy of) the image before resizing,
	    // thus the graphics are resized.
	    // It also end up performing one extra copying of the image,
	    // from the DIB to the DDB.
	    //
	    #if SHOWIG_STRETCHDIBITS2
	    {
		HGLOBAL hDIB = pxd_renderDIBCreate(1<<u, buf, 0, 0, -1, -1, 0, 0);
		if (hDIB) {
		    BITMAPINFOHEADER FAR *dib = (BITMAPINFOHEADER FAR *)GlobalLock(hDIB);
		    if (dib) {
			HBITMAP hDIB2 = CreateDIBitmap(hDC, dib, CBM_INIT,
				    (uchar FAR*)dib+dib->biSize+dib->biClrUsed*sizeof(RGBQUAD),
				    (BITMAPINFO*)dib, DIB_RGB_COLORS);
			if (hDIB2) {
			    HDC hdcDIB = CreateCompatibleDC(hDC);
			    if (hdcDIB) {
				HGDIOBJ  ohDIB = SelectObject(hdcDIB, hDIB2);
				TextOut(hdcDIB, 20, 20, "This is text", 12);
				SetStretchBltMode(hDC, STRETCH_DELETESCANS);
				StretchBlt(hDC, windImage[u].nw.x, windImage[u].nw.y,
						windImage[u].se.x-windImage[u].nw.x, windImage[u].se.y-windImage[u].nw.y,
					hdcDIB, 0, 0, pxd_imageXdim(), pxd_imageYdim(), SRCCOPY);
				SelectObject(hdcDIB, ohDIB);
				DeleteDC(hdcDIB);
			    }
			    DeleteObject(hDIB2);
			}
			GlobalUnlock(hDIB);
		    }
		    pxd_renderDIBFree(hDIB);
		}
	    }
	    #endif

	    //
	    // Show image using pxio8_GDIDisplay and graphics using GDI.
	    // Uses an off-screen buffer so the displayed
	    // graphics do not flicker. Note that the graphics
	    // are drawn in (a copy of) the image after resizing,
	    // thus the graphics are not resized, yielding crisper graphics.
	    //
	    #if SHOWIG_STRETCHDIBITS3
	    {
		HBITMAP hDIB = NULL;
		HGDIOBJ hODIB;
		HDC	hDCDIB;
		RECT	rect;

		//
		// Create HBITMAP as off-screen buffer.
		// And create DC for it.
		//
		hDIB = CreateCompatibleBitmap(hDC, windImage[u].se.x-windImage[u].nw.x,
						   windImage[u].se.y-windImage[u].nw.y);
		hDCDIB = CreateCompatibleDC(hDC);
		hODIB = SelectObject(hDCDIB, hDIB);

		//
		// Draw image into it, resizing.
		// The "Display" selection is equivalent to using
		//     pxd_imageCdim()==1?"GREY":"BGR"
		//
		pxio8_GDIDisplay(NULL, pxd_defineImage(1<<u, buf, 0, 0, -1, -1, "Display"),
				NULL, 0, 'n', 0, 0,hDCDIB, &windImage[u],
				NULL, NULL);

		//
		// Use GDI to draw graphics over image.
		//
		SetRect(&rect, windImage[u].nw.x+(windImage[u].se.x-windImage[u].nw.x)/4,
			       windImage[u].nw.y+(windImage[u].se.y-windImage[u].nw.y)/4,
			       windImage[u].nw.x+(windImage[u].se.x-windImage[u].nw.x)*3/4,
			       windImage[u].nw.y+(windImage[u].se.y-windImage[u].nw.y)*3/4);
		DrawFocusRect(hDCDIB, &rect);
		DrawText(hDCDIB, "This is text", 12, &rect, 0);

		//
		// Display image & graphics.
		//
		BitBlt(hDC, windImage[u].nw.x, windImage[u].nw.y,
			    windImage[u].se.x, windImage[u].se.y,
		       hDCDIB, 0, 0, SRCCOPY);

		SelectObject(hDCDIB, hODIB);
		DeleteDC(hDCDIB);
		DeleteObject(hDIB);
	    }
	    #endif


	    //
	    // Show image using PXIPL pxio8_GDIDisplay.
	    // Altho not used in this example, pxio8_GDIDisplay also allows drawing
	    // a full screen cross hair cursor over the image.
	    //
	    #if SHOWIM_GDIDISPLAY
		pxio8_GDIDisplay(NULL, pxd_defineImage(1<<u, buf, 0, 0, -1, -1, "Display"),
				 NULL, 0, 'n', 0, 0,hDC,&windImage[u], NULL, NULL);
	    #endif

	    //
	    // Show image using PXIPL pxio8_GDIDisplay with cursor overlay.
	    //
	    #if SHOWIM_GDIDISPLAY2
	    {
		struct	pximage cursimage;
		void	*	cursbuf = NULL;
		uint	color[3] = {255, 0, 255};
		struct	pxy	xy;
		int	r;

		//
		// Obtain buffer and pximage
		// to hold cursor data; i.e. the pixel values
		// that will form the cursor.
		//
		xy.x =	 (windImage[u].se.x-windImage[u].nw.x)
		       * (windImage[u].se.y-windImage[u].nw.y);
		xy.y = 1;
		r = pximage_memmalloc(&cursimage, &cursbuf, &xy, PXDATUCHAR, 8, 3, PXHINTBGR);
		if (r >= 0) {
		    //
		    // Init cursor pixel values.
		    //
		    pxip8_pixset3(NULL, &cursimage, color);

		    //
		    // Display image, with cursor in middle
		    //
		    xy.x = pxd_imageXdim()/2;
		    xy.y = pxd_imageYdim()/2;
		    pxio8_GDIDisplay(NULL, pxd_defineImage(1<<u, buf, 0, 0, -1, -1, "Display"),
				     NULL, 0, 'n', 0, 0,hDC, &windImage[u], &xy, &cursimage);

		    //
		    // The cursimage not contains the pixel values which were
		    // overwritten by the cursor. The cursor can be removed
		    // without redrawing the image by
		    //
		    // pxio8_GDICursor(NULL, &image[u], 'n', 0, hDC, &windImage[u],  &xy, &cursimage);
		    //
		    // And then redrawn in a new position by
		    //
		    // xy.x /= 2;
		    // xy.y /= 2;
		    // pxio8_GDICursor(NULL, pxd_defineImage(1<<u, 1, 0, 0, -1, -1, "Display"),
		    //		       'n', 0, hDC, &windImage[u],  &xy, &cursimage);
		    //
		    // Although redrawing the cursor here, now, isn't
		    // really useful, so it is commented out.
		    //

		    //
		    // All done.
		    //
		    pximage_memfree(&cursimage, &cursbuf);
		}
	    }
	    #endif


	    //
	    // Show image and graphics using PXIPL pxio8_GDIDisplay.
	    // This draws graphics in an off-line, pre-resized,
	    // so the displayed graphics aren't resized and don't flicker.
	    //
	    #if SHOWIG_GDIDISPLAY
	    {
		pximage_s   tempimage;
		void	   *tempbuf = NULL;
		pxywindow_s box;
		uint	color[3] = {0, 255, 255};
		int	r;

		//
		// First, acquire a temporary image buffer,
		// sized to the display dimensions.
		//
		r = pximage_memmalloc(&tempimage, &tempbuf, &windImage[u].se, PXDATUCHAR, 8, 3, PXHINTBGR);
		if (r >= 0) {
		    //
		    // Next, 'display' the image into the temporary buffer.
		    //
		    pxio8_vgadisplay(NULL, pxd_defineImage(1<<u, buf, 0, 0, -1, -1, pxd_imageCdim()==1?"GREY":"BGR"),
				     NULL, 0, 'n', 0, 0, &tempimage, NULL, NULL);

		    //
		    // Prepare to Draw Graphics.
		    // Since this is drawn in the already resized image,
		    // we map the desired image coordinates into actual
		    // display coordinates.
		    //
		    box.nw.x = pxd_imageXdim()/4;      // desired
		    box.nw.y = pxd_imageYdim()/4;
		    box.se.x = pxd_imageXdim()*3/4;
		    box.se.y = pxd_imageYdim()*3/4;
		    pxio8_vgacoord(pxd_defineImage(1<<u, 1, 0, 0, -1, -1, pxd_imageCdim()==1?"GREY":"BGR"),'n',0, &tempimage, &box.nw);
		    pxio8_vgacoord(pxd_defineImage(1<<u, 1, 0, 0, -1, -1, pxd_imageCdim()==1?"GREY":"BGR"),'n',0, &tempimage, &box.se);

		    //
		    // Draw Graphics.
		    // We choose to make the box out of thick dots.
		    //
		    pxip8_drawbox(NULL, &tempimage, &box, 6, 3, 's', color, NULL, NULL);

		    //
		    // Display image and graphics.
		    // The image and graphics are already resized,
		    // so this draws 1-1.
		    //
		    pxio8_GDIDisplay(NULL, &tempimage, NULL, 0, '1', 0, 0,hDC,&windImage[u],
			 NULL, NULL);

		    //
		    // All done with temp image and its buffer.
		    //
		    pximage_memfree(&tempimage, &tempbuf);
		}
	    }
	    #endif

	    //
	    // Show image using DirectDraw.
	    // Note that this only supports S/VGA in
	    // 24 or 32 bits per pixel mode.
	    // This might be slightly quicker if the primary surface
	    // were acquired once, and kept till the program exits.
	    //
	    // This can be extended to draw graphics in
	    // the same manner as for pxio8_GDIDisplay, above.
	    //
	    // The pixel format of the S/VGA may not be acceptable!
	    // Error reporting should be added!
	    //
	    #if SHOWIM_DIRECTXDISPLAY
	    {
		DDSURFACEDESC	surfacedesc;
		LPDIRECTDRAWSURFACE ddrs = NULL;
		HRESULT     h;
		POINT	    pt;
		if (lpDD) {
		    //
		    // Get primary surface of full screen.
		    //
		    surfacedesc.dwFlags = DDSD_CAPS;
		    surfacedesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE|DDSCAPS_VIDEOMEMORY;
		    surfacedesc.dwSize = sizeof(surfacedesc);
		    h = lpDD->CreateSurface(&surfacedesc, &ddrs, NULL);
		    if (ddrs) {
			//
			// Adjust for position of dialog on screen.
			//
			pt.x = pt.y = 0;
			ClientToScreen(hWndImage, &pt);
			windImage[u].nw.x += pt.x;
			windImage[u].nw.y += pt.y;
			windImage[u].se.x += pt.x;
			windImage[u].se.y += pt.y;
			pxio8_DirectXDisplay(NULL, pxd_defineImage(1<<u, buf, 0, 0, -1, -1, "Display"),
					     NULL, 0, 'n', 0, 0, ddrs, &windImage[u], NULL, hDC, NULL, NULL);
			windImage[u].nw.x -= pt.x;
			windImage[u].nw.y -= pt.y;
			windImage[u].se.x -= pt.x;
			windImage[u].se.y -= pt.y;
			((LPDIRECTDRAWSURFACE)ddrs)->Release();
		    }
		}
	    }
	    #endif

	    //
	    // Show image using pxd_renderDIBCreate to create a
	    // standard Windows DIB, and display with Video for Windows.
	    //
	    #if SHOWIM_DRAWDIBDRAW
		BITMAPINFOHEADER FAR *dib;
		HGLOBAL hDIB;

		hDIB = pxd_renderDIBCreate(1<<u, buf, 0, 0, -1, -1, 0, 0);
		if (hDIB) {
		    if (dib = (BITMAPINFOHEADER FAR *)GlobalLock(hDIB)) {
			DrawDibDraw(hDrawDib, hDC, windImage[u].nw.x, windImage[u].nw.y,
						   windImage[u].se.x-windImage[u].nw.x, windImage[u].se.y-windImage[u].nw.y,
				    (BITMAPINFOHEADER *)dib,
				    (uchar FAR*)dib+dib->biSize+dib->biClrUsed*sizeof(RGBQUAD),
				    0, 0, pxd_imageXdim(), pxd_imageYdim(), 0);
			GlobalUnlock(hDIB);
		    }
		    pxd_renderDIBFree(hDIB);
		}
	    #endif

	    //
	    // Show image using PXIPL and Video for Windows.
	    // Altho not used in this example, pxio8_DrawDibDisplay also allows drawing
	    // a full screen cross hair cursor over the image.
	    //
	    #if SHOWIM_DRAWDIBDISPLAY
		pxio8_DrawDibDisplay(NULL, pxd_defineImage(1<<u, buf, 0, 0, -1, -1, "Display"),
				 NULL, 0, 'n', 0, 0,hDrawDib,hDC,&windImage[u], NULL, NULL);
	    #endif


	    //
	    // Optionally, draw a graph of the pixel values
	    // from the middle image line. This is primarily
	    // intended to demonstrate access to pixel data,
	    // rather than to demonstrate use of the Windows GDI.
	    //
	    // Note that use of pxd_readuchar() is valid
	    // even for imaging boards that have more than 8 bits,
	    // letting the XCLIB handle conversion.
	    // Also note that having requested RGB data, XCLIB will
	    // supply data as requested, even if the imaging board
	    // is outputting monochrome or YUYV.
	    //
	    if (SHOW_GRAPH) {
		HBRUSH	hbr;
		HGDIOBJ hbrold;
		RECT	rect;
		uchar	*buffer;

		hbr = CreateSolidBrush(RGB(0,0,0));
		hbrold = SelectObject(hDC, hbr);
		rect.left   = windImage[3].nw.x;
		rect.top    = windImage[3].nw.y;
		rect.right  = windImage[3].se.x+1;
		rect.bottom = windImage[3].se.y+1;
		FillRect(hDC, &rect, hbr);
		SelectObject(hDC, hbrold);
		DeleteObject(hbr);

		buffer = (uchar*)malloc(sizeof(uchar)*3*pxd_imageXdim());
		if (buffer) {
		    pxd_readuchar(1<<0, buf, 0, pxd_imageYdim()/2, -1, 1+pxd_imageYdim()/2, buffer, 3*pxd_imageXdim(), "RGB");
		    for (int i = 0; i < pxd_imageXdim(); i++) {
			SetPixel(hDC,
				 windImage[3].nw.x+((windImage[3].se.x-windImage[3].nw.x)*i)/pxd_imageXdim(),
				 windImage[3].se.y-1-(((windImage[3].se.y-windImage[3].nw.y)*buffer[i*3+0])/255),
				 RGB(255,0,0));
			SetPixel(hDC,
				 windImage[3].nw.x+((windImage[3].se.x-windImage[3].nw.x)*i)/pxd_imageXdim(),
				 windImage[3].se.y-1-(((windImage[3].se.y-windImage[3].nw.y)*buffer[i*3+1])/255),
				 RGB(0,255,0));
			SetPixel(hDC,
				 windImage[3].nw.x+((windImage[3].se.x-windImage[3].nw.x)*i)/pxd_imageXdim(),
				 windImage[3].se.y-1-(((windImage[3].se.y-windImage[3].nw.y)*buffer[i*3+2])/255),
				 RGB(0,0,255));
		    }
		    free(buffer);
		}
	    }
	    ReleaseDC(hWndImage, hDC);

	    //
	    // After display, if in live mode using LIVE_SNAP,
	    // having displayed one image start snapping the next
	    //
	    #if LIVE_SNAP
		if (liveon)
		    pxd_goSnap(1<<u, 1);
	    #endif

	}

	return(TRUE);

    }
    return(FALSE);
}

LRESULT CALLBACK MainWndProc(
    HWND	hWnd,
    unsigned	message,
    WPARAM	wParam,
    LPARAM	lParam
){
    switch (message) {
	case WM_CLOSE:
	    DestroyWindow(hWnd);
	    return(0);
	case WM_DESTROY:
	    PostQuitMessage(0);
	    return(0);
    }
    return(DefWindowProc(hWnd, message, wParam, lParam));
}

/*
 * The Main
 */
int APIENTRY WinMain(
    HINSTANCE  hInstance,
    HINSTANCE  hPrevInstance,
    LPSTR      lpCmdLine,
    int        nCmdShow
){
    MSG       msg;
    WNDCLASS  wc;

    wc.style	     = CS_BYTEALIGNWINDOW;
    wc.lpfnWndProc   = MainWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon	     = 0;
    wc.hCursor	     = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName  =	NULL;
    wc.lpszClassName = "PXlibWClass";

    if (!RegisterClass(&wc))
	return (FALSE);

    if (hPrevInstance)
	return(FALSE);

    hWnd = CreateWindow("PXlibWClass",
			"XCLIBEX2 Windows Example",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			0, 0, hInstance, NULL );
    if (!hWnd)
	return (FALSE);

    //
    // This main window of this example does nothing useful.
    // Don't bother showing it.
    //
    //ShowWindow(hWnd, nCmdShow);
    //UpdateWindow(hWnd);
    hDlg = CreateDialogParam(hInstance, "PIXCISVXDIALOG", NULL, (DLGPROC)PIXCIDialogProc, NULL);
    if (!hDlg) {
	MessageBox(NULL, "Missing Dialog Resource - Compilation or Link Error!", "XCLIBEX2", MB_OK|MB_TASKMODAL);
	return(FALSE);
    }

    while (GetMessage(&msg, 0, 0, 0)) {
	if (!hDlg || !IsDialogMessage(hDlg, &msg)) {
	    TranslateMessage(&msg);
	    DispatchMessage(&msg);
	}
    }
    return (msg.wParam);
}
