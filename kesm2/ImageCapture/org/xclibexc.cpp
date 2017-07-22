/*
 *
 *	xclibexc.cpp	External	11-Jul-2005
 *
 *	Copyright (C)  2001-2005  EPIX, Inc.  All rights reserved.
 *
 *	Example program for the XCLIB 'C' Library with
 *	   SILICON VIDEO(R) 2112,  2112C
 *	   SILICON VIDEO(R) 1310,  1310C
 *	   SILICON VIDEO(R) 1281M, 1281C
 *	   SILICON VIDEO(R) 9M001, 9M001C
 *	   SILICON VIDEO(R) 9T001C
 *	   SILICON VIDEO(R) 642M, 642C
 *	cameras. Assumes Windows 95/98/ME/NT/2000/XP.
 *
 *	Demonstrates XCLIB and/or PXIPL functions for capture,
 *	display of imagery, and adjustment of camera.
 *	This Windows program must, of course, also make use of
 *	various Windows GDI API functions; however, this is not
 *	intended to serve as a Windows tutorial.
 *
 *	Many parts similar to the XCLIBEX2.CPP example; that
 *	example will also operate the PIXCI(R) and the SILICON VIDEO(R)
 *	cameras, demonstrates more display options including
 *	graphic overlays, demonstrates simultaneous use of
 *	multiple PIXCI(R) imaging boards and cameras,
 *	and demonstrates access to numeric pixel values and
 *	saving of images; but does not have camera specific controls.
 *	This example demonstrates camera specific controls, but
 *	for simplicity does not illustrate as many display options.
 *	Either example can be read and used to demonstrate
 *	the desired features.
 *
 */

/*
 *  INSTRUCTIONS:
 *
 *  1.1) Set 'define' options below according to the intended camera
 *	and video format.
 *
 *	For PIXCI(R) D2X imaging boards with SV2112, SV2112C, SV1310,
 *	SV1310C, SV1281M, SV1281C or PIXCI(R) SI imaging boards with
 *	SV9M001, SV9M001C, SV9T001C, SV642M, or SV642C
 *	use "default" to select the default format for the camera for which
 *	the PIXCI(R) imaging board is intended. For non default formats,
 *	use XCAP to save the video set-up to a file, and set FORMATFILE
 *	to the saved file's path name.
 *
 *	Alternately, this could be modified to use getenv("PIXCI"),
 *	GetPrivateProfileString(...), RegQueryValueEx(...), or any
 *	other convention chosen by the programmer to allow run time
 *	selection of the video format and resolution.
 *
 */
					// For PIXCI(R) D2X w.
					//	SV2112	or SV2112C
					//	SV1310	or SV1310C
					//	SV1281M  or SV1281C
					// For PIXCI(R) SI w.
					//	SV9M001 or SV9M001C
					//	SV9T001C
					//	SV642M, SV642C
#define FORMAT	"default"		// as per board's intended camera
//#define FORMATFILE  "xcvidset.fmt"	// using format file saved by XCAP


/*
 *  1.2) Select whether the SV2112, SV1310, SV1281, SV9M001, SV9T001,
 *  or SV642 is being used.
 *  The XCLIB capture and image access functions are the same,
 *  but camera-specific functions, such as gain and/or offset
 *  and other features implemented in the camera 'head' rather than
 *  the PIXCI(R) are unique.
 */
#define SV2112	0			// for SV2112 or SV2112C
#define SV1310	0			// for SV1310 or SV1310C
#define SV1281	0			// for SV1281M or SV1281C
#define SV9M001 1			// for SV9M001 or SV9M001C
#define SV9T001 0			// for SV9T001C
#define SV642	0			// for SV642M or SV642C

/*
 *  2.1) Set number of expected PIXCI(R) image boards and cameras,
 *  from 1 to 4. The XCLIB Simple 'C' Functions expect that the
 *  boards are the same model and operated at the same resolution.
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
 *  3.1) Choose wich form of image display (SHOWIM_), or image and
 *  graphics overlay (SHOWIG_), is to be demonstrated. Some of these
 *  options expect that the optional PXIPL library is present.
 *  Others may expect that the Windows DirectDraw SDK is present
 *  (available from Microsoft) and that the S/VGA supports DirectDraw.
 *
 *  Only one of these choices should have value 1, the others should be 0.
 *
 *  See the XCLIBEX2.CPP example for additional choices and options.
 */
#define SHOWIM_STRETCHDIBITS	1   // use XCLIB or XCLIB-Lite and GDI
#define SHOWIG_STRETCHDIBITS	0   // use XCLIB or XCLIB-Lite and GDI
#define SHOWIM_GDIDISPLAY	0   // use XCLIB and PXIPL
#define SHOWIM_DIRECTXDISPLAY	0   // use XCLIB and PXIPL and DirectDraw


/*
 *  3.2) Select how 'Live' mode should be demonstrated.
 *  Only one of these choices should have value 1, the others should be 0.
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
 *  3.3) Select how display updates should be triggered.
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
 *  3.4) Select whether software AGC should be enabled.
 */
#define SOFT_AGC	0  // 1: yes, 0: no


/*
 *  4a) Compile with Microsoft Visual C/C++ V6.0 for Windows 95/98/ME as:
 *
 *	    cl -c xclibexc.cpp
 *	    rc.exe /l 0x409 /fo tt.res xclibexc.rc
 *	    echo -subsystem:windows,4.0 -entry:WinMainCRTStartup >t1
 *	    echo /NODEFAULTLIB libc.lib oldnames.lib user32.lib gdi32.lib kernel32.lib vfw32.lib largeint.lib winmm.lib >t2
 *
 *	Either (XCLIB w/out PXIPL for Windows 95/98/ME)
 *	    link xclibexc.obj XCLIBW95.lib tt.res @t1 @t2
 *	or  (XCLIB+PXIPL only for Windows 95/98/ME)
 *	    link xclibexc.obj XCLIBW95.lib PXIPLW95.LIB tt.res @t1 @t2
 *	or (XCLIB-Lite for Windows 95/98/ME)
 *	    link xclibexc.obj XCLYBW95.LIB tt.res @t1 @t2
 *
 *	    del t1
 *	    del t2
 *	    del tt.res
 *	    del xclibexc.obj
 *
 *	Or use the IDE, compiling xclibexc.cpp as a Windows 95/98/ME app,
 *	using the XCLIBEXC.RC resource, linking to library(s):
 *	    XCLIB:	  XCLIBW95.LIB
 *	    XCLIB+PXIPL:  XCLIBW95.LIB PXIPLW95.LIB
 *	    XCLIB-Lite:   XCLYBW95.LIB
 *	For example, in the IDE for VISUAL C/C++ V5.0:
 *	    1. Start Integrated Development Environment.
 *	    2. Select 'File' - 'New'.
 *	    3. Enter Project Name and Select 'WIN 32 Application'.
 *	    4. Copy this file, XCLIBEXC.CPP, and XCLIBEXC.RC to your
 *	       project directory.
 *	    5. Select 'Project' - 'Add To Project' - 'Files ...' and select
 *	       this file, XCLIBEXC.CPP, and XCLIBEXC.RC.
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
 *  4b) Compile with Microsoft Visual C/C++ V6.0 for Windows NT/2000/XP: same as for
 *	Windows 95/98/ME, above, substituting XCLIBWNT.lib instead of XCLIBW95.lib
 *	and substituting PXIPLWNT.lib instead of PXIPLW95.lib:
 *
 *	Either (XCLIB w/out PXIPL for Windows NT/2000/XP)
 *	    link xclibexc.obj XCLIBWNT.lib tt.res @t1 @t2
 *	or  (XCLIB+PXIPL only for Windows NT/2000/XP)
 *	    link xclibexc.obj XCLIBWNT.lib PXIPLWNT.LIB tt.res @t1 @t2
 *	or (XCLIB-Lite for Windows NT/2000/XP)
 *	    link xclibexc.obj XCLYBWNT.LIB tt.res @t1 @t2
 *
 *  4c) Compile with Microsoft Visual Studio .NET 2003 command line tools for Windows NT/2000/XP as:
 *
 *	Open the Visual Studio .Net 2003 Command Prompt.
 *	It is loacted under Programs-> Microsoft Visual Studio .NET 2003-> Visual Studio .NET Tools.
 *	Enter the following commands:
 *	    cl -c xclibexc.cpp
 *	    rc.exe /l 0x409 /fo tt.res	xclibexc.rc
 *	    echo -subsystem:windows,4.0 -entry:WinMainCRTStartup comdlg32.lib ole32.lib >t1
 *	    echo /NODEFAULTLIB libc.lib oldnames.lib user32.lib gdi32.lib kernel32.lib vfw32.lib largeint.lib winmm.lib >t2
 *
 *	Either (XCLIB w/out PXIPL for Windows NT/2000/XP)
 *	    link xclibexc.obj XCLIBWNT.LIB tt.res @t1 @t2
 *	or  (XCLIB+PXIPL only for Windows NT/2000/XP)
 *	    link xclibexc.obj XCLIBWNT.LIB PXIPLWNT.LIB tt.res @t1 @t2
 *	or (XCLIB-Lite for Windows NT/2000/XP)
 *	    link xclibexc.obj XCLYBWNT.LIB tt.res @t1 @t2
 *
 *	    del t1
 *	    del t2
 *	    del tt.res
 *	    del xclibexc.obj
 *
 *	Or use the IDE, compiling xclibexc.cpp as a Windows NT/2000/XP app,
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
 *	     6. Select 'Project' and '"Your Project Name" Properties'.
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
 *		    XCLIB+PXIPL:  XCLIBWTN.DLL PXIPLWNT.DLL
 *		    XCLIB-Lite:   XCLYBWNT.DLL
 *		  to this project's directory.
 *	    14. Press F5 to build and run program.

 *
 *  For Borland, Watcom, and/or Delphi users: The XCLIBEX2.CPP example
 *  provides compilation instructions and/or hints which are also
 *  applicable to this example.
 *
 *
 */


/*
 *  NECESSARY INCLUDES:
 */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#if SHOWIM_DIRECTXDISPLAY
    #include <compobj.h>
    #include <ddraw.h>
#endif

extern "C" {
    #include "xcliball.h"
    #include "xclibex2.h"
    #if SHOWIM_GDIDISPLAY || SHOWIM_DIRECTXDISPLAY
	#include "pxipl.h"         
	#include "pximages.h"           
    #endif
}

/*
 * Global variables.
 */
static	HWND	hWnd;			/* the main window */
static	HWND	hDlg;			/* the main dialog */
static	double	loexp, hiexp;		/* min & max exposures */
static	double	lofps, hifps;		/* min & max frame rate */
static	int	liveon = 0;		/* whether live video was requested */

/*
 * Camera constants
 *
 * The min & max exposure and frame rate depends on
 * clock frequency, AOI, and/or subsampling and
 * are therefore determined on-the-fly.
 */
#if SV2112
    #define MAXWIDTH	1288
    #define MAXHEIGHT	1032
    #define MINGAIN	0.0
    #define MAXGAIN	14.0
    #define MINCLK	1.0
    #define MAXCLK	40.0
#elif SV1310
    #define MAXWIDTH	1280
    #define MAXHEIGHT	1024
    #define MINGAIN	0.483
    #define MAXGAIN	7.488
    #define MINCLK	1.0
    #define MAXCLK	20.0
    #define MINOFFSET	(-12*31)
    #define MAXOFFSET	(+12*31)
    #define SCRLOFFSET	(-MINOFFSET)	// for scrollbar controls, which require non-negative values
#elif SV1281
    #define MAXWIDTH	1280
    #define MAXHEIGHT	1024
    #define MINGAIN	log(1.5)
    #define MAXGAIN	log(39.375)
    #define MINCLK	1.0
    #define MAXCLK	60.0
    #define MINOFFSET	0
    #define MAXOFFSET	62
#elif SV9M001
    #define MAXWIDTH	1280
    #define MAXHEIGHT	1024
    #define MINGAIN	0.0
    #define MAXGAIN	(20*log10(15.0))
    #define MINCLK	25.0
    #define MAXCLK	70.0
#elif SV9T001
    #define MAXWIDTH	2048
    #define MAXHEIGHT	1536
    #define MINGAIN	0.0
    #define MAXGAIN	(20*log10(4.0))
    #define MINGAIN2	0.0
    #define MAXGAIN2	(20*log10(1+127.0/8))
    #define MINCLK	25.0
    #define MAXCLK	70.0
#elif SV642
    #define MAXWIDTH	640
    #define MAXHEIGHT	480
    #define MINCLK	25.0
    #define MAXCLK	70.0
    #define MINGAIN(Range)  (20*log10(( 0.0+1)*(Range/10.0)))	 // Range: 5 or 10
    #define MAXGAIN(Range)  (20*log10((17.0+1)*(Range/10.0)))	 //
#endif

/*
 * To improve readability when translating floating point
 * values, such as gain, pixel clock, etc., to and from scrollbar controls.
 */
#define ToScroll(Value, Scale)	   (int)((Value)*(Scale))
#define UnScroll(Value, Scale)	   ((double)(Value)/(Scale))


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
 * Init or re-init dialog controls
 *
 * The allowable range of exposure is a function
 * of many other camera parameters, such as decimation/subsample and pixel clock.
 * Thus, when decimation/subsample or pixel clock is changed the exposure
 * slider will, and should, change position!
 * We obtain the min and max by setting exposures which are beyond the high/low
 * limits and then querying for the corrected values.
 */
void initControls(HWND hDlg, int firsttime)
{

    #if SV2112
	int	d;
	double	oexp  = pxd_SV2112_getExposure(UNITSMAP);
	double	ogain = pxd_SV2112_getGain(UNITSMAP, 'R');
	pxd_SV2112_setExposureAndGain(UNITSMAP, 0, 0.0, ogain, ogain, ogain);
	loexp = pxd_SV2112_getExposure(UNITSMAP);
	pxd_SV2112_setExposureAndGain(UNITSMAP, 0, 99999.0, ogain, ogain, ogain);
	hiexp = pxd_SV2112_getExposure(UNITSMAP);
	pxd_SV2112_setExposureAndGain(UNITSMAP, 0, oexp, ogain, ogain, ogain);
	SetScrollRange(GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll(0.0, 10), ToScroll(hiexp-loexp, 10), TRUE);
	SetScrollPos  (GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll(oexp-loexp, 10), TRUE);
	if (firsttime) {
	    SetScrollRange(GetDlgItem(hDlg, IDGAINSCROLL), SB_CTL, ToScroll(MINGAIN, 10), ToScroll(MAXGAIN, 10), TRUE);
	    SetScrollPos  (GetDlgItem(hDlg, IDGAINSCROLL), SB_CTL, ToScroll(ogain, 10), TRUE);
	    SetScrollRange(GetDlgItem(hDlg, IDCLKSCROLL),  SB_CTL, ToScroll(MINCLK,20), ToScroll(MAXCLK,20), TRUE);
	    SetScrollPos  (GetDlgItem(hDlg, IDCLKSCROLL),  SB_CTL, ToScroll(pxd_SV2112_getPixelClock(UNITSMAP),20), TRUE);
	    SendMessage (GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Decimate Off");
	    SendMessage (GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Decimate 2x2");
	    SendMessage (GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Decimate 4x4");
	    EnableWindow(GetDlgItem(hDlg, IDLISTDECIMATE), TRUE);
	    //
	    // Continuous live & unlive buttons aren't applicable
	    // in single shot camera modes
	    //
	    if (pxd_SV2112_getVideoMode(UNITSMAP) == 'c'
	     && pxd_SV2112_getCtrlVideoMode(UNITSMAP) == 's') {
		EnableWindow(GetDlgItem(hDlg, IDLIVE), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDUNLIVE), FALSE);
	    }
	}
	d = pxd_SV2112_getDecimation(UNITSMAP);
	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL, (d==0x101? 0: d==0x202? 1: 2), 0);
    #elif SV1310
	int	d;
	double	oexp  = pxd_SV1310_getExposure(UNITSMAP);
	double	ogain = pxd_SV1310_getGain(UNITSMAP);
	double	ooffset = pxd_SV1310_getOffset(UNITSMAP);
	pxd_SV1310_setExposureGainOffset(UNITSMAP, 0, 0.0, ogain, ooffset, 0, 0);
	loexp = pxd_SV1310_getExposure(UNITSMAP);
	pxd_SV1310_setExposureGainOffset(UNITSMAP, 0, 99999.0, ogain, ooffset, 0, 0);
	hiexp = pxd_SV1310_getExposure(UNITSMAP);
	pxd_SV1310_setExposureGainOffset(UNITSMAP, 0, oexp, ogain, ooffset, 0, 0);
	SetScrollRange(GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll(0.0,10), ToScroll(hiexp-loexp,10), TRUE);
	SetScrollPos  (GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll(oexp-loexp,10), TRUE);
	if (firsttime) {
	    SetScrollRange(GetDlgItem(hDlg, IDGAINSCROLL),   SB_CTL, ToScroll(MINGAIN,10), ToScroll(MAXGAIN,10), TRUE);
	    SetScrollPos  (GetDlgItem(hDlg, IDGAINSCROLL),   SB_CTL, ToScroll(pxd_SV1310_getGain(UNITSMAP),10), TRUE);
	    SetScrollRange(GetDlgItem(hDlg, IDOFFSETSCROLL), SB_CTL, SCRLOFFSET+MINOFFSET, SCRLOFFSET+MAXOFFSET, TRUE);
	    SetScrollPos  (GetDlgItem(hDlg, IDOFFSETSCROLL), SB_CTL, SCRLOFFSET+(int)(pxd_SV1310_getOffset(UNITSMAP)), TRUE);
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 1x1");
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 2x2");
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 4x4");
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 8x8");
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 1x2");
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 2x1");
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 2x4");
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 4x2");
	    EnableWindow(GetDlgItem(hDlg, IDLISTDECIMATE), TRUE);
	    SetScrollRange(GetDlgItem(hDlg, IDCLKSCROLL), SB_CTL, ToScroll(MINCLK,20), ToScroll(MAXCLK,20), TRUE);
	    SetScrollPos  (GetDlgItem(hDlg, IDCLKSCROLL), SB_CTL, ToScroll(pxd_SV1310_getPixelClock(UNITSMAP),20), TRUE);
	    lofps = 0;
	    hifps = 20;
	    SetScrollRange(GetDlgItem(hDlg, IDFPSSCROLL), SB_CTL, ToScroll(lofps,100), ToScroll(hifps,100), TRUE);
	    SetScrollPos  (GetDlgItem(hDlg, IDFPSSCROLL), SB_CTL, ToScroll(1000.0/pxd_SV1310_getFramePeriod(UNITSMAP),100), TRUE);
	    //
	    SendMessage   (GetDlgItem(hDlg, IDVIDEOMODE),LB_ADDSTRING, 0, (LPARAM)"Free-Run");
	    SendMessage   (GetDlgItem(hDlg, IDVIDEOMODE),LB_ADDSTRING, 0, (LPARAM)"Ext. Trigger");
	    SendMessage   (GetDlgItem(hDlg, IDVIDEOMODE),LB_ADDSTRING, 0, (LPARAM)"'Snap' Trigger");
	}
	d = pxd_SV1310_getSubsample(UNITSMAP);
	switch (d) {
	  case 0x0101:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL, 0, 0);  break;
	  case 0x0202:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL, 1, 0);  break;
	  case 0x0404:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL, 2, 0);  break;
	  case 0x0808:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL, 3, 0);  break;
	  case 0x0102:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL, 4, 0);  break;
	  case 0x0201:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL, 5, 0);  break;
	  case 0x0204:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL, 6, 0);  break;
	  case 0x0402:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL, 7, 0);  break;
	}
	//
	// Continuous live & unlive buttons aren't applicable
	// in single shot camera modes
	//
	if (pxd_SV1310_getVideoMode(UNITSMAP) == 'c'
	 && pxd_SV1310_getCtrlVideoMode(UNITSMAP) == 's') {
	    EnableWindow(GetDlgItem(hDlg, IDLIVE),   FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDUNLIVE), FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDFPSSCROLL), FALSE);
	    if (pxd_SV1310_getCtrlTriggerMode(UNITSMAP) == 'b')
	       SendMessage(GetDlgItem(hDlg, IDVIDEOMODE), LB_SETCURSEL, 2, 0);
	    else
	       SendMessage(GetDlgItem(hDlg, IDVIDEOMODE), LB_SETCURSEL, 1, 0);
	} else {
	    EnableWindow(GetDlgItem(hDlg, IDLIVE),   TRUE);
	    EnableWindow(GetDlgItem(hDlg, IDUNLIVE), TRUE);
	    EnableWindow(GetDlgItem(hDlg, IDFPSSCROLL), TRUE);
	    SendMessage(GetDlgItem(hDlg, IDVIDEOMODE), LB_SETCURSEL, 0, 0);
	}
    #elif SV1281
	int	d;
	double	oexp  = pxd_SV1281_getExposure(UNITSMAP);
	double	ogain = pxd_SV1281_getGain(UNITSMAP);
	double	ooffset = pxd_SV1281_getOffset(UNITSMAP);
	pxd_SV1281_setExposureGainOffset(UNITSMAP, 0, 0.0, ogain, ooffset, 0, 0);
	loexp = pxd_SV1281_getExposure(UNITSMAP);
	pxd_SV1281_setExposureGainOffset(UNITSMAP, 0, 99999.0, ogain, ooffset, 0, 0);
	hiexp = pxd_SV1281_getExposure(UNITSMAP);
	pxd_SV1281_setExposureGainOffset(UNITSMAP, 0, oexp, ogain, ooffset, 0, 0);
	SetScrollRange(GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll(0.0,10), ToScroll(hiexp-loexp,10), TRUE);
	SetScrollPos  (GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll(oexp-loexp,10), TRUE);
	if (firsttime) {
	    SetScrollRange(GetDlgItem(hDlg, IDGAINSCROLL), SB_CTL, ToScroll(MINGAIN,10), ToScroll(MAXGAIN,10), TRUE);
	    SetScrollPos  (GetDlgItem(hDlg, IDGAINSCROLL), SB_CTL, ToScroll(pxd_SV1281_getGain(UNITSMAP),10), TRUE);
	    SetScrollRange(GetDlgItem(hDlg, IDOFFSETSCROLL), SB_CTL, MINOFFSET, MAXOFFSET, TRUE);
	    SetScrollPos  (GetDlgItem(hDlg, IDOFFSETSCROLL), SB_CTL, (int)(pxd_SV1281_getOffset(UNITSMAP)), TRUE);
	    SetScrollRange(GetDlgItem(hDlg, IDCLKSCROLL), SB_CTL, ToScroll(MINCLK,20), ToScroll(MAXCLK,20), TRUE);
	    SetScrollPos  (GetDlgItem(hDlg, IDCLKSCROLL), SB_CTL, ToScroll(pxd_SV1281_getPixelClock(UNITSMAP),20), TRUE);
	    //
	    // The AOI can be adjusted in much finer steps, as well as moved in position.
	    // To keep the GUI simple, only a few options shown.
	    SendMessage   (GetDlgItem(hDlg, IDLISTAOI),  LB_ADDSTRING, 0, (LPARAM)"1280x1024");
	    SendMessage   (GetDlgItem(hDlg, IDLISTAOI),  LB_ADDSTRING, 0, (LPARAM)"640x512");
	    SendMessage   (GetDlgItem(hDlg, IDLISTAOI),  LB_ADDSTRING, 0, (LPARAM)"320x256");
	    SendMessage   (GetDlgItem(hDlg, IDLISTAOI),  LB_ADDSTRING, 0, (LPARAM)"160x128");
	    //
	    SendMessage   (GetDlgItem(hDlg, IDVIDEOMODE),LB_ADDSTRING, 0, (LPARAM)"Free-Run");
	    SendMessage   (GetDlgItem(hDlg, IDVIDEOMODE),LB_ADDSTRING, 0, (LPARAM)"Ext. Trigger");
	    SendMessage   (GetDlgItem(hDlg, IDVIDEOMODE),LB_ADDSTRING, 0, (LPARAM)"'Snap' Trigger");
	}
	d = pxd_imageXdim();
	if (d == MAXWIDTH)
	    SendMessage(GetDlgItem(hDlg, IDLISTAOI), LB_SETCURSEL, 0, 0);
	else if (d >= MAXWIDTH/2)
	    SendMessage(GetDlgItem(hDlg, IDLISTAOI), LB_SETCURSEL, 1, 0);
	else if (d >= MAXWIDTH/4)
	    SendMessage(GetDlgItem(hDlg, IDLISTAOI), LB_SETCURSEL, 2, 0);
	else
	    SendMessage(GetDlgItem(hDlg, IDLISTAOI), LB_SETCURSEL, 3, 0);
	//
	// Continuous live & unlive buttons aren't applicable
	// in single shot camera modes
	//
	if (pxd_SV1281_getVideoMode(UNITSMAP) == 'c'
	 && pxd_SV1281_getCtrlVideoMode(UNITSMAP) == 's') {
	    EnableWindow(GetDlgItem(hDlg, IDLIVE),   FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDUNLIVE), FALSE);
	    if (pxd_SV1281_getCtrlTriggerMode(UNITSMAP) == 'b')
	       SendMessage(GetDlgItem(hDlg, IDVIDEOMODE), LB_SETCURSEL, 2, 0);
	    else
	       SendMessage(GetDlgItem(hDlg, IDVIDEOMODE), LB_SETCURSEL, 1, 0);
	} else {
	    EnableWindow(GetDlgItem(hDlg, IDLIVE),   TRUE);
	    EnableWindow(GetDlgItem(hDlg, IDUNLIVE), TRUE);
	    SendMessage(GetDlgItem(hDlg, IDVIDEOMODE), LB_SETCURSEL, 0, 0);
	}
    #elif SV9M001 || SV9T001
	int	d;
	double	oexp   = pxd_SV9M001_getExposure(UNITSMAP);
	double	ogain1 = pxd_SV9M001_getGain(UNITSMAP, 'R');
	double	ogain2 = pxd_SV9M001_getGain(UNITSMAP, 'B');
	double	ogain3 = pxd_SV9M001_getGain(UNITSMAP, 'R'^'G');
	double	ogain4 = pxd_SV9M001_getGain(UNITSMAP, 'B'^'G');
	pxd_SV9M001_setExposureAndGain(UNITSMAP, 0, 0.0, ogain1, ogain2, ogain3, ogain4);
	loexp = pxd_SV9M001_getExposure(UNITSMAP);
	pxd_SV9M001_setExposureAndGain(UNITSMAP, 0, 9999.0, ogain1, ogain2, ogain3, ogain4);
	hiexp = pxd_SV9M001_getExposure(UNITSMAP);
	pxd_SV9M001_setExposureAndGain(UNITSMAP, 0, oexp, ogain1, ogain2, ogain3, ogain4);
	SetScrollRange(GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll(0.0,10), ToScroll(hiexp-loexp,10), TRUE);
	SetScrollPos  (GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll(oexp-loexp,10), TRUE);
	if (firsttime) {
	    SetScrollRange(GetDlgItem(hDlg, IDGAINSCROLL),   SB_CTL, ToScroll(MINGAIN,10), ToScroll(MAXGAIN,10), TRUE);
	    SetScrollPos  (GetDlgItem(hDlg, IDGAINSCROLL),   SB_CTL, ToScroll(pxd_SV9M001_getGain(UNITSMAP, 'R'),10), TRUE);
	    #if SV9T001
		SetScrollRange(GetDlgItem(hDlg, IDGAIN2SCROLL),   SB_CTL, ToScroll(MINGAIN2,10), ToScroll(MAXGAIN2,10), TRUE);
		SetScrollPos  (GetDlgItem(hDlg, IDGAIN2SCROLL),   SB_CTL, ToScroll(pxd_SV9M001_getDigitalGain(UNITSMAP),10), TRUE);
	    #endif
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 1x1");
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 2x2");
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 4x4");
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 8x8");
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 1x2");
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 1x4");
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 1x8");
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 2x1");
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 2x4");
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 2x8");
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 4x1");
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 4x2");
	    SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE),  LB_ADDSTRING, 0, (LPARAM)"Sample 4x8");
	    EnableWindow(GetDlgItem(hDlg, IDLISTDECIMATE), TRUE);
	    SetScrollRange(GetDlgItem(hDlg, IDCLKSCROLL), SB_CTL, ToScroll(MINCLK,20), ToScroll(MAXCLK,20), TRUE);
	    SetScrollPos  (GetDlgItem(hDlg, IDCLKSCROLL), SB_CTL, ToScroll(pxd_SV9M001_getPixelClock(UNITSMAP),20), TRUE);
	    lofps = 0;
	    hifps = 100;
	    SetScrollRange(GetDlgItem(hDlg, IDFPSSCROLL), SB_CTL, ToScroll(lofps,100), ToScroll(hifps,100), TRUE);
	    SetScrollPos  (GetDlgItem(hDlg, IDFPSSCROLL), SB_CTL, ToScroll(1000.0/pxd_SV9M001_getFramePeriod(UNITSMAP),100), TRUE);
	    //
	    SendMessage   (GetDlgItem(hDlg, IDVIDEOMODE),LB_ADDSTRING, 0, (LPARAM)"Free-Run");
	    SendMessage   (GetDlgItem(hDlg, IDVIDEOMODE),LB_ADDSTRING, 0, (LPARAM)"Ext. Trigger");
	    SendMessage   (GetDlgItem(hDlg, IDVIDEOMODE),LB_ADDSTRING, 0, (LPARAM)"'Snap' Trigger");
	}
	d = pxd_SV9M001_getSubsample(UNITSMAP);
	switch (d) {
	  case 0x0101:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL,  0, 0);  break;
	  case 0x0202:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL,  1, 0);  break;
	  case 0x0404:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL,  2, 0);  break;
	  case 0x0808:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL,  3, 0);  break;
	  case 0x0102:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL,  4, 0);  break;
	  case 0x0104:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL,  5, 0);  break;
	  case 0x0108:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL,  6, 0);  break;
	  case 0x0201:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL,  7, 0);  break;
	  case 0x0204:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL,  8, 0);  break;
	  case 0x0208:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL,  9, 0);  break;
	  case 0x0401:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL, 10, 0);  break;
	  case 0x0402:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL, 11, 0);  break;
	  case 0x0408:	SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_SETCURSEL, 12, 0);  break;
	}
	//
	// Continuous live & unlive buttons aren't applicable
	// in single shot camera modes
	//
	if (pxd_SV9M001_getVideoMode(UNITSMAP) == 'c'
	 && pxd_SV9M001_getCtrlVideoMode(UNITSMAP) == 's') {
	    EnableWindow(GetDlgItem(hDlg, IDLIVE),   FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDUNLIVE), FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDFPSSCROLL), FALSE);
	    if (pxd_SV9M001_getCtrlTriggerMode(UNITSMAP) == 'b')
	       SendMessage(GetDlgItem(hDlg, IDVIDEOMODE), LB_SETCURSEL, 2, 0);
	    else
	       SendMessage(GetDlgItem(hDlg, IDVIDEOMODE), LB_SETCURSEL, 1, 0);
	} else {
	    EnableWindow(GetDlgItem(hDlg, IDLIVE),   TRUE);
	    EnableWindow(GetDlgItem(hDlg, IDUNLIVE), TRUE);
	    EnableWindow(GetDlgItem(hDlg, IDFPSSCROLL), TRUE);
	    SendMessage(GetDlgItem(hDlg, IDVIDEOMODE), LB_SETCURSEL, 0, 0);
	}
    #elif SV642
	int	d;
	double	oexp	   = pxd_SV642_getExposure(UNITSMAP);
	double	ogain1	   = pxd_SV642_getGain(UNITSMAP, 'R');
	double	ogain2	   = pxd_SV642_getGain(UNITSMAP, 'B');
	double	ogain3	   = pxd_SV642_getGain(UNITSMAP, 'R'^'G');
	double	ogain4	   = pxd_SV642_getGain(UNITSMAP, 'B'^'G');
	int	ogainrange = pxd_SV642_getGainRange(UNITSMAP);
	pxd_SV642_setExposureAndGain(UNITSMAP, 0, 0.0, ogain1, ogain2, ogain3, ogain4, ogainrange);
	loexp = pxd_SV642_getExposure(UNITSMAP);
	pxd_SV642_setExposureAndGain(UNITSMAP, 0, 9999.0, ogain1, ogain2, ogain3, ogain4, ogainrange);
	hiexp = pxd_SV642_getExposure(UNITSMAP);
	pxd_SV642_setExposureAndGain(UNITSMAP, 0, oexp, ogain1, ogain2, ogain3, ogain4, ogainrange);
	SetScrollRange(GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll(0.0,10), ToScroll(hiexp-loexp,10), TRUE);
	SetScrollPos  (GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll(oexp-loexp,10), TRUE);
	if (firsttime) {
	    double maxgain = MAXGAIN(ogainrange);
	    double mingain = MINGAIN(ogainrange);
	    SetScrollRange(GetDlgItem(hDlg, IDGAINSCROLL),   SB_CTL, ToScroll(mingain,10), ToScroll(maxgain,10), TRUE);
	    SetScrollPos  (GetDlgItem(hDlg, IDGAINSCROLL),   SB_CTL, ToScroll(pxd_SV642_getGain(UNITSMAP, 'R'),10), TRUE);
	    SetScrollRange(GetDlgItem(hDlg, IDCLKSCROLL), SB_CTL, ToScroll(MINCLK,20), ToScroll(MAXCLK,20), TRUE);
	    SetScrollPos  (GetDlgItem(hDlg, IDCLKSCROLL), SB_CTL, ToScroll(pxd_SV642_getPixelClock(UNITSMAP),20), TRUE);
	    lofps = 0;
	    hifps = 1000;
	    SetScrollRange(GetDlgItem(hDlg, IDFPSSCROLL), SB_CTL, ToScroll(lofps,100), ToScroll(hifps,100), TRUE);
	    SetScrollPos  (GetDlgItem(hDlg, IDFPSSCROLL), SB_CTL, ToScroll(1000.0/pxd_SV642_getFramePeriod(UNITSMAP),100), TRUE);
	    //
	    SendMessage   (GetDlgItem(hDlg, IDVIDEOMODE),LB_ADDSTRING, 0, (LPARAM)"Free-Run");
	    SendMessage   (GetDlgItem(hDlg, IDVIDEOMODE),LB_ADDSTRING, 0, (LPARAM)"Ext. Trigger");
	    SendMessage   (GetDlgItem(hDlg, IDVIDEOMODE),LB_ADDSTRING, 0, (LPARAM)"'Snap' Trigger");
	    //
	    // The AOI can be adjusted in much finer steps, as well as moved in position.
	    // To keep the GUI simple, only a few options shown.
	    SendMessage   (GetDlgItem(hDlg, IDLISTAOI),  LB_ADDSTRING, 0, (LPARAM)"640x480");
	    SendMessage   (GetDlgItem(hDlg, IDLISTAOI),  LB_ADDSTRING, 0, (LPARAM)"320x240");
	    SendMessage   (GetDlgItem(hDlg, IDLISTAOI),  LB_ADDSTRING, 0, (LPARAM)"160x120");
	    SendMessage   (GetDlgItem(hDlg, IDLISTAOI),  LB_ADDSTRING, 0, (LPARAM)"80x60");
	}
	d = pxd_imageXdim();
	if (d == MAXWIDTH)
	    SendMessage(GetDlgItem(hDlg, IDLISTAOI), LB_SETCURSEL, 0, 0);
	else if (d >= MAXWIDTH/2)
	    SendMessage(GetDlgItem(hDlg, IDLISTAOI), LB_SETCURSEL, 1, 0);
	else if (d >= MAXWIDTH/4)
	    SendMessage(GetDlgItem(hDlg, IDLISTAOI), LB_SETCURSEL, 2, 0);
	else
	    SendMessage(GetDlgItem(hDlg, IDLISTAOI), LB_SETCURSEL, 3, 0);
	//
	// Continuous live & unlive buttons aren't applicable
	// in single shot camera modes
	//
	if (pxd_SV642_getVideoMode(UNITSMAP) == 'c'
	 && pxd_SV642_getCtrlVideoMode(UNITSMAP) == 's') {
	    EnableWindow(GetDlgItem(hDlg, IDLIVE),   FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDUNLIVE), FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDFPSSCROLL), FALSE);
	    if (pxd_SV642_getCtrlTriggerMode(UNITSMAP) == 'b')
	       SendMessage(GetDlgItem(hDlg, IDVIDEOMODE), LB_SETCURSEL, 2, 0);
	    else
	       SendMessage(GetDlgItem(hDlg, IDVIDEOMODE), LB_SETCURSEL, 1, 0);
	} else {
	    EnableWindow(GetDlgItem(hDlg, IDLIVE),   TRUE);
	    EnableWindow(GetDlgItem(hDlg, IDUNLIVE), TRUE);
	    EnableWindow(GetDlgItem(hDlg, IDFPSSCROLL), TRUE);
	    SendMessage(GetDlgItem(hDlg, IDVIDEOMODE), LB_SETCURSEL, 0, 0);
	}
    #endif
    //
    // White balance is only for color cameras.
    //
    EnableWindow(GetDlgItem(hDlg, IDWHITEBAL), pxd_imageCdim()==3);
}


/*
 * Process gain scroll
 */
BOOL doGainScroll(WPARAM wParam)
{
    #if SV2112
	double g = pxd_SV2112_getGain(UNITSMAP, 'R');
	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    g += 3.0;	break;
	    case SB_LINEDOWN:	    g += 1.0;	break;
	    case SB_PAGEUP:	    g -= 3.0;	break;
	    case SB_LINEUP:	    g -= 1.0;	break;
	    case SB_TOP:	    g = MAXGAIN;break;
	    case SB_BOTTOM:	    g = MINGAIN;break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     g = UnScroll(HIWORD(wParam),10.0); break;
	    default:		    return(FALSE);
	}
	pxd_SV2112_setExposureAndGain(UNITSMAP, 0, pxd_SV2112_getExposure(UNITSMAP), g, g, g);
	SetScrollPos(GetDlgItem(hDlg, IDGAINSCROLL), SB_CTL, ToScroll(pxd_SV2112_getGain(UNITSMAP, 'R'),10), TRUE);
    #elif SV1310
	double g = pxd_SV1310_getGain(UNITSMAP);
	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    g += 1.0;	break;
	    case SB_LINEDOWN:	    g += 0.5;	break;
	    case SB_PAGEUP:	    g -= 1.0;	break;
	    case SB_LINEUP:	    g -= 0.5;	break;
	    case SB_TOP:	    g = MAXGAIN;break;
	    case SB_BOTTOM:	    g = MINGAIN;break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     g = UnScroll(HIWORD(wParam),10.0); break;
	    default:		    return(FALSE);
	}
	pxd_SV1310_setExposureGainOffset(UNITSMAP, 0, pxd_SV1310_getExposure(UNITSMAP), g, pxd_SV1310_getOffset(UNITSMAP), 0, 0);
	SetScrollPos(GetDlgItem(hDlg, IDGAINSCROLL), SB_CTL, ToScroll(pxd_SV1310_getGain(UNITSMAP),10), TRUE);
    #elif SV1281
	double g = pxd_SV1281_getGain(UNITSMAP);
	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    g += 0.5;	break;
	    case SB_LINEDOWN:	    g += 0.2;	break;
	    case SB_PAGEUP:	    g -= 0.5;	break;
	    case SB_LINEUP:	    g -= 0.2;	break;
	    case SB_TOP:	    g = MAXGAIN;break;
	    case SB_BOTTOM:	    g = MINGAIN;break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     g = UnScroll(HIWORD(wParam),10.0); break;
	    default:		    return(FALSE);
	}
	pxd_SV1281_setExposureGainOffset(UNITSMAP, 0, pxd_SV1281_getExposure(UNITSMAP), g, pxd_SV1281_getOffset(UNITSMAP), 0, 0);
	SetScrollPos(GetDlgItem(hDlg, IDGAINSCROLL), SB_CTL, ToScroll(pxd_SV1281_getGain(UNITSMAP),10), TRUE);
    #elif SV9M001 || SV9T001
	//
	// In color mode, the gains help implement white balancing,
	// and should be ganged together - adjusted in unison
	// and relative differences maintained. Not needed for mono cameras,
	// but neither is it detrimental.
	//
	double g1 = pxd_SV9M001_getGain(1, 'R');
	double g2 = pxd_SV9M001_getGain(1, 'B');
	double g3 = pxd_SV9M001_getGain(1, 'R'^'G');
	double g4 = pxd_SV9M001_getGain(1, 'B'^'G');
	double gang = min(g1, min(g2, min(g3, g4)));
	double g = gang;
	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    g += 1.0;	break;
	    case SB_LINEDOWN:	    g += 0.5;	break;
	    case SB_PAGEUP:	    g -= 1.0;	break;
	    case SB_LINEUP:	    g -= 0.5;	break;
	    case SB_TOP:	    g = MAXGAIN;break;
	    case SB_BOTTOM:	    g = MINGAIN;break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     g = UnScroll(HIWORD(wParam),10.0); break;
	    default:		    return(FALSE);
	}
	g1 += g-gang;	g2 += g-gang;	g3 += g-gang;	g4 += g-gang;
	gang = min(g1, min(g2, min(g3, g4)));
	if (gang < MINGAIN) {
	    g1 += MINGAIN-gang; g2 += MINGAIN-gang; g3 += MINGAIN-gang; g4 += MINGAIN-gang;
	}
	gang = max(g1, max(g2, max(g3, g4)));
	if (gang > MAXGAIN) {
	    g1 -= gang-MAXGAIN; g2 -= gang-MAXGAIN; g3 -= gang-MAXGAIN; g4 -= gang-MAXGAIN;
	}
	pxd_SV9M001_setExposureAndGain(UNITSMAP, 0, pxd_SV9M001_getExposure(UNITSMAP), g1, g2, g3, g4);
	SetScrollPos(GetDlgItem(hDlg, IDGAINSCROLL), SB_CTL, ToScroll(pxd_SV9M001_getGain(UNITSMAP,'R'),10), TRUE);
	#if SV9T001
	    SetScrollPos(GetDlgItem(hDlg, IDGAIN2SCROLL), SB_CTL, ToScroll(pxd_SV9M001_getDigitalGain(UNITSMAP),10), TRUE);
	#endif
    #elif SV642
	//
	// In color mode, the gains help implement white balancing,
	// and should be ganged together - adjusted in unison
	// and relative differences maintained. Not needed for mono cameras,
	// but neither is it detrimental.
	//
	int    gainrange = pxd_SV642_getGainRange(UNITSMAP);
	double g1 = pxd_SV642_getGain(1, 'R');
	double g2 = pxd_SV642_getGain(1, 'B');
	double g3 = pxd_SV642_getGain(1, 'R'^'G');
	double g4 = pxd_SV642_getGain(1, 'B'^'G');
	double gang = min(g1, min(g2, min(g3, g4)));
	double mingain = MINGAIN(gainrange);
	double maxgain = MAXGAIN(gainrange);
	double g = gang;
	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    g += 1.0;	break;
	    case SB_LINEDOWN:	    g += 0.5;	break;
	    case SB_PAGEUP:	    g -= 1.0;	break;
	    case SB_LINEUP:	    g -= 0.5;	break;
	    case SB_TOP:	    g = maxgain;break;
	    case SB_BOTTOM:	    g = mingain;break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     g = UnScroll(HIWORD(wParam),10.0); break;
	    default:		    return(FALSE);
	}
	g1 += g-gang;	g2 += g-gang;	g3 += g-gang;	g4 += g-gang;
	gang = min(g1, min(g2, min(g3, g4)));
	if (gang < mingain) {
	    g1 += mingain-gang; g2 += mingain-gang; g3 += mingain-gang; g4 += mingain-gang;
	}
	gang = max(g1, max(g2, max(g3, g4)));
	if (gang > maxgain) {
	    g1 -= gang-maxgain; g2 -= gang-maxgain; g3 -= gang-maxgain; g4 -= gang-maxgain;
	}
	pxd_SV642_setExposureAndGain(UNITSMAP, 0, pxd_SV642_getExposure(UNITSMAP), g1, g2, g3, g4, gainrange);
	SetScrollPos(GetDlgItem(hDlg, IDGAINSCROLL), SB_CTL, ToScroll(pxd_SV642_getGain(UNITSMAP,'R'),10), TRUE);
    #endif
    return(TRUE);
}

/*
 * Process gain2 scroll
 */
BOOL doGain2Scroll(WPARAM wParam)
{
    #if SV2112
    #elif SV1310
    #elif SV1281
    #elif SV9M001
    #elif SV9T001
	double g = pxd_SV9M001_getDigitalGain(UNITSMAP);
	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    g += 1.0;	    break;
	    case SB_LINEDOWN:	    g += 0.5;	    break;
	    case SB_PAGEUP:	    g -= 1.0;	    break;
	    case SB_LINEUP:	    g -= 0.5;	    break;
	    case SB_TOP:	    g = MAXGAIN2;   break;
	    case SB_BOTTOM:	    g = MINGAIN2;   break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     g = UnScroll(HIWORD(wParam),10.0); break;
	    default:		    return(FALSE);
	}
	pxd_SV9M001_setExposureAndDigitalGain(UNITSMAP, 0, pxd_SV9M001_getExposure(UNITSMAP), g, 0, 0, 0);
	SetScrollPos(GetDlgItem(hDlg, IDGAIN2SCROLL), SB_CTL, ToScroll(pxd_SV9M001_getDigitalGain(UNITSMAP),10), TRUE);
    #elif SV642
    #endif
    return(TRUE);
}


/*
 * Process offset (black level) scroll
 */
BOOL doOffsetScroll(WPARAM wParam)
{
    #if SV2112
    #elif SV1310
	double g = pxd_SV1310_getOffset(UNITSMAP);
	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    g += 5*12;	   break;
	    case SB_LINEDOWN:	    g += 12;	   break;
	    case SB_PAGEUP:	    g -= 5*12;	   break;
	    case SB_LINEUP:	    g -= 12;	   break;
	    case SB_TOP:	    g = MAXOFFSET; break;
	    case SB_BOTTOM:	    g = MINOFFSET; break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     g = HIWORD(wParam)-SCRLOFFSET; break;
	    default:		    return(FALSE);
	}
	pxd_SV1310_setExposureGainOffset(UNITSMAP, 0, pxd_SV1310_getExposure(UNITSMAP), pxd_SV1310_getGain(UNITSMAP), g, 0, 0);
	SetScrollPos(GetDlgItem(hDlg, IDOFFSETSCROLL), SB_CTL, SCRLOFFSET+(int)(pxd_SV1310_getOffset(UNITSMAP)), TRUE);
    #elif SV1281
	double g = pxd_SV1281_getOffset(UNITSMAP);
	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    g += 5;	    break;
	    case SB_LINEDOWN:	    g += 1;	    break;
	    case SB_PAGEUP:	    g -= 5;	    break;
	    case SB_LINEUP:	    g -= 1;	    break;
	    case SB_TOP:	    g =  MAXOFFSET; break;
	    case SB_BOTTOM:	    g =  MINOFFSET; break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     g = HIWORD(wParam); break;
	    default:		    return(FALSE);
	}
	pxd_SV1281_setExposureGainOffset(UNITSMAP, 0, pxd_SV1281_getExposure(UNITSMAP), pxd_SV1281_getGain(UNITSMAP), g, 0, 0);
	SetScrollPos(GetDlgItem(hDlg, IDOFFSETSCROLL), SB_CTL, (int)(pxd_SV1281_getOffset(UNITSMAP)), TRUE);
    #elif SV9M001 || SV9T001
    #elif SV642
    #endif
    return(TRUE);
}



/*
 * Process clock scroll
 */
BOOL doClockScroll(WPARAM wParam)
{
    #if SV2112
	double c = pxd_SV2112_getPixelClock(UNITSMAP);

	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    c += 2.0;	break;
	    case SB_LINEDOWN:	    c += .5;	break;
	    case SB_PAGEUP:	    c -= 2.0;	break;
	    case SB_LINEUP:	    c -= .5;	break;
	    case SB_TOP:	    c = MAXCLK; break;
	    case SB_BOTTOM:	    c = MINCLK; break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     c = UnScroll(HIWORD(wParam),20.0); break;
	    default:		    return(FALSE);
	}
	pxd_SV2112_setResolutionAndTiming(UNITSMAP, 0,
		      pxd_SV2112_getDecimation(UNITSMAP),
		      0, 0, MAXWIDTH, MAXHEIGHT, pxd_SV2112_getScanDirection(UNITSMAP), c, 0, 0);
	SetScrollPos(GetDlgItem(hDlg, IDCLKSCROLL), SB_CTL, ToScroll(pxd_SV2112_getPixelClock(UNITSMAP),20), TRUE);
	initControls(hDlg, 0);
    #elif SV1310
	double c = pxd_SV1310_getPixelClock(UNITSMAP);

	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    c += 2.0;	break;
	    case SB_LINEDOWN:	    c += .5;	break;
	    case SB_PAGEUP:	    c -= 2.0;	break;
	    case SB_LINEUP:	    c -= .5;	break;
	    case SB_TOP:	    c = MAXCLK; break;
	    case SB_BOTTOM:	    c = MINCLK; break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     c = UnScroll(HIWORD(wParam),20.0); break;
	    default:		    return(FALSE);
	}
	pxd_SV1310_setResolutionAndTiming(UNITSMAP, 0,
		      pxd_SV1310_getSubsample(UNITSMAP),
		      0, 0, MAXWIDTH, MAXHEIGHT,
		      pxd_SV1310_getReadoutDirection(UNITSMAP),
		      c,
		      pxd_SV1310_getFramePeriod(UNITSMAP), 0);
	SetScrollPos(GetDlgItem(hDlg, IDCLKSCROLL), SB_CTL, ToScroll(pxd_SV1310_getPixelClock(UNITSMAP),20), TRUE);
	initControls(hDlg, 0);
    #elif SV1281
	double c = pxd_SV1281_getPixelClock(UNITSMAP);

	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    c += 2.0;	break;
	    case SB_LINEDOWN:	    c += .5;	break;
	    case SB_PAGEUP:	    c -= 2.0;	break;
	    case SB_LINEUP:	    c -= .5;	break;
	    case SB_TOP:	    c = MAXCLK; break;
	    case SB_BOTTOM:	    c = MINCLK; break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     c = UnScroll(HIWORD(wParam),20.0); break;
	    default:		    return(FALSE);
	}
	pxd_SV1281_setResolutionAndTiming(UNITSMAP, 0, 0,
		     (MAXWIDTH-pxd_imageXdim())/2, (MAXHEIGHT-pxd_imageYdim())/2,
		      pxd_imageXdim(), pxd_imageYdim(),
		      0,
		      c,
		      0, 0);
	SetScrollPos(GetDlgItem(hDlg, IDCLKSCROLL), SB_CTL, ToScroll(pxd_SV1281_getPixelClock(UNITSMAP),20), TRUE);
	initControls(hDlg, 0);
    #elif SV9M001 || SV9T001
	double c = pxd_SV9M001_getPixelClock(UNITSMAP);

	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    c += 2.0;	break;
	    case SB_LINEDOWN:	    c += .5;	break;
	    case SB_PAGEUP:	    c -= 2.0;	break;
	    case SB_LINEUP:	    c -= .5;	break;
	    case SB_TOP:	    c = MAXCLK; break;
	    case SB_BOTTOM:	    c = MINCLK; break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     c = UnScroll(HIWORD(wParam),20.0); break;
	    default:		    return(FALSE);
	}
	pxd_SV9M001_setResolutionAndTiming(UNITSMAP, 0, pxd_SV9M001_getSubsample(UNITSMAP),
		     (MAXWIDTH-pxd_imageXdim())/2, (MAXHEIGHT-pxd_imageYdim())/2,
		      pxd_imageXdim(), pxd_imageYdim(),
		      0,
		      c,
		      pxd_SV9M001_getFramePeriod(UNITSMAP), 0);
	SetScrollPos(GetDlgItem(hDlg, IDCLKSCROLL), SB_CTL, ToScroll(pxd_SV9M001_getPixelClock(UNITSMAP),20), TRUE);
	initControls(hDlg, 0);
    #elif SV642
	double c = pxd_SV642_getPixelClock(UNITSMAP);

	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    c += 2.0;	break;
	    case SB_LINEDOWN:	    c += .5;	break;
	    case SB_PAGEUP:	    c -= 2.0;	break;
	    case SB_LINEUP:	    c -= .5;	break;
	    case SB_TOP:	    c = MAXCLK; break;
	    case SB_BOTTOM:	    c = MINCLK; break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     c = UnScroll(HIWORD(wParam),20.0); break;
	    default:		    return(FALSE);
	}
	pxd_SV642_setResolutionAndTiming(UNITSMAP, 0, 0,
		     (MAXWIDTH-pxd_imageXdim())/2, (MAXHEIGHT-pxd_imageYdim())/2,
		      pxd_imageXdim(), pxd_imageYdim(),
		      0,
		      c,
		      pxd_SV642_getFramePeriod(UNITSMAP), 0);
	SetScrollPos(GetDlgItem(hDlg, IDCLKSCROLL), SB_CTL, ToScroll(pxd_SV642_getPixelClock(UNITSMAP),20), TRUE);
	initControls(hDlg, 0);
    #endif
    return(TRUE);
}

/*
 * Process exposure scroll
 */
BOOL doExposureScroll(WPARAM wParam)
{
    #if SV2112
	double e = pxd_SV2112_getExposure(UNITSMAP);
	double g = pxd_SV2112_getGain(UNITSMAP, 'R');

	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    e += 2.0;	break;
	    case SB_LINEDOWN:	    e += .5;	break;
	    case SB_PAGEUP:	    e -= 2.0;	break;
	    case SB_LINEUP:	    e -= .5;	break;
	    case SB_TOP:	    e = hiexp;	break;
	    case SB_BOTTOM:	    e = loexp;	break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     e = loexp+UnScroll(HIWORD(wParam),10.0); break;
	    default:		    return(FALSE);
	}
	pxd_SV2112_setExposureAndGain(UNITSMAP, 0, e, g, g, g);
	SetScrollPos(GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll((pxd_SV2112_getExposure(UNITSMAP)-loexp),10), TRUE);
    #elif SV1310
	double e = pxd_SV1310_getExposure(UNITSMAP);
	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    e += 2.0;	break;
	    case SB_LINEDOWN:	    e += .5;	break;
	    case SB_PAGEUP:	    e -= 2.0;	break;
	    case SB_LINEUP:	    e -= .5;	break;
	    case SB_TOP:	    e = hiexp;	break;
	    case SB_BOTTOM:	    e = loexp;	break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     e = loexp+UnScroll(HIWORD(wParam),10.0); break;
	    default:		    return(FALSE);
	}
	pxd_SV1310_setExposureGainOffset(UNITSMAP, 0, e, pxd_SV1310_getGain(1), pxd_SV1310_getOffset(1), 0, 0);
	SetScrollPos(GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll((pxd_SV1310_getExposure(1)-loexp),10), TRUE);
    #elif SV1281
	double e = pxd_SV1281_getExposure(UNITSMAP);
	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    e += 2.0;	break;
	    case SB_LINEDOWN:	    e += .5;	break;
	    case SB_PAGEUP:	    e -= 2.0;	break;
	    case SB_LINEUP:	    e -= .5;	break;
	    case SB_TOP:	    e = hiexp;	break;
	    case SB_BOTTOM:	    e = loexp;	break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     e = loexp+UnScroll(HIWORD(wParam),10.0); break;
	    default:		    return(FALSE);
	}
	pxd_SV1281_setExposureGainOffset(UNITSMAP, 0, e, pxd_SV1281_getGain(1), pxd_SV1281_getOffset(1), 0, 0);
	SetScrollPos(GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll((pxd_SV1281_getExposure(1)-loexp),10), TRUE);
    #elif SV9M001 || SV9T001
	double e = pxd_SV9M001_getExposure(UNITSMAP);
	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    e += 2.0;	break;
	    case SB_LINEDOWN:	    e += .5;	break;
	    case SB_PAGEUP:	    e -= 2.0;	break;
	    case SB_LINEUP:	    e -= .5;	break;
	    case SB_TOP:	    e = hiexp;	break;
	    case SB_BOTTOM:	    e = loexp;	break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     e = loexp+UnScroll(HIWORD(wParam),10.0); break;
	    default:		    return(FALSE);
	}
	pxd_SV9M001_setExposureAndGain(UNITSMAP, 0, e, pxd_SV9M001_getGain(1, 'R'), pxd_SV9M001_getGain(1, 'B'),
						       pxd_SV9M001_getGain(1, 'R'^'G'), pxd_SV9M001_getGain(1, 'B'^'G'));
	SetScrollPos(GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll((pxd_SV9M001_getExposure(1)-loexp),10), TRUE);
    #elif SV642
	double e = pxd_SV642_getExposure(UNITSMAP);
	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    e += 2.0;	break;
	    case SB_LINEDOWN:	    e += .5;	break;
	    case SB_PAGEUP:	    e -= 2.0;	break;
	    case SB_LINEUP:	    e -= .5;	break;
	    case SB_TOP:	    e = hiexp;	break;
	    case SB_BOTTOM:	    e = loexp;	break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     e = loexp+UnScroll(HIWORD(wParam),10.0); break;
	    default:		    return(FALSE);
	}
	pxd_SV642_setExposureAndGain(UNITSMAP, 0, e, pxd_SV642_getGain(1, 'R'), pxd_SV642_getGain(1, 'B'),
						     pxd_SV642_getGain(1, 'R'^'G'), pxd_SV642_getGain(1, 'B'^'G'),
						     pxd_SV642_getGainRange(UNITSMAP));
	SetScrollPos(GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll((pxd_SV642_getExposure(1)-loexp),10), TRUE);
    #endif
    return(TRUE);
}

/*
 * Process frame rate scroll
 */
BOOL doFpsScroll(WPARAM wParam)
{
    #if SV2112
    #elif SV1310
	double c = 1000.0/pxd_SV1310_getFramePeriod(UNITSMAP);
	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    c += (hifps-lofps)/10.; break;
	    case SB_LINEDOWN:	    c += (hifps-lofps)/50.; break;
	    case SB_PAGEUP:	    c -= (hifps-lofps)/10.; break;
	    case SB_LINEUP:	    c -= (hifps-lofps)/50.; break;
	    case SB_TOP:	    c = hifps;	break;
	    case SB_BOTTOM:	    c = lofps;	break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     c = UnScroll(HIWORD(wParam),100.0); break;
	    default:		    return(FALSE);
	}
	pxd_SV1310_setResolutionAndTiming(UNITSMAP, 0,
		      pxd_SV1310_getSubsample(UNITSMAP),
		      0, 0, MAXWIDTH, MAXHEIGHT,
		      pxd_SV1310_getReadoutDirection(UNITSMAP),
		      pxd_SV1310_getPixelClock(UNITSMAP),
		      1000.0/c, 0);
	SetScrollPos(GetDlgItem(hDlg, IDFPSSCROLL), SB_CTL, ToScroll(1000.0/pxd_SV1310_getFramePeriod(UNITSMAP),100), TRUE);
	initControls(hDlg, 0);
    #elif SV1281
    #elif SV9M001 || SV9T001
	double c = 1000.0/pxd_SV9M001_getFramePeriod(UNITSMAP);
	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    c += (hifps-lofps)/10.; break;
	    case SB_LINEDOWN:	    c += (hifps-lofps)/50.; break;
	    case SB_PAGEUP:	    c -= (hifps-lofps)/10.; break;
	    case SB_LINEUP:	    c -= (hifps-lofps)/50.; break;
	    case SB_TOP:	    c = hifps;	break;
	    case SB_BOTTOM:	    c = lofps;	break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     c = UnScroll(HIWORD(wParam),100.0); break;
	    default:		    return(FALSE);
	}
	pxd_SV9M001_setResolutionAndTiming(UNITSMAP, 0,
		      pxd_SV9M001_getSubsample(UNITSMAP),
		      0, 0, MAXWIDTH, MAXHEIGHT,
		      pxd_SV9M001_getScanDirection(UNITSMAP),
		      pxd_SV9M001_getPixelClock(UNITSMAP),
		      1000.0/c, 0);
	SetScrollPos(GetDlgItem(hDlg, IDFPSSCROLL), SB_CTL, ToScroll(1000.0/pxd_SV9M001_getFramePeriod(UNITSMAP),100), TRUE);
	initControls(hDlg, 0);
    #elif SV642
	double c = 1000.0/pxd_SV642_getFramePeriod(UNITSMAP);
	switch (LOWORD(wParam)) {
	    case SB_PAGEDOWN:	    c += (hifps-lofps)/10.; break;
	    case SB_LINEDOWN:	    c += (hifps-lofps)/50.; break;
	    case SB_PAGEUP:	    c -= (hifps-lofps)/10.; break;
	    case SB_LINEUP:	    c -= (hifps-lofps)/50.; break;
	    case SB_TOP:	    c = hifps;	break;
	    case SB_BOTTOM:	    c = lofps;	break;
	    case SB_THUMBPOSITION:
	    case SB_THUMBTRACK:     c = UnScroll(HIWORD(wParam),100.0); break;
	    default:		    return(FALSE);
	}
	pxd_SV642_setResolutionAndTiming(UNITSMAP, 0, 0,
		      (MAXWIDTH-pxd_imageXdim())/2, (MAXHEIGHT-pxd_imageYdim())/2,
		      pxd_imageXdim(), pxd_imageYdim(),
		      0,
		      pxd_SV642_getPixelClock(UNITSMAP),
		      1000.0/c, 0);
	SetScrollPos(GetDlgItem(hDlg, IDFPSSCROLL), SB_CTL, ToScroll(1000.0/pxd_SV642_getFramePeriod(UNITSMAP),100), TRUE);
	initControls(hDlg, 0);
    #endif
    return(TRUE);
}

/*
 * Process decimation/subsample list
 */
BOOL doDecimateList()
{
    #if SV2112
	int d = SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_GETCURSEL, 0, 0);
	pxd_SV2112_setResolutionAndTiming(UNITSMAP, 0,
		      d==0? 0x101: d==1? 0x202: 0x404,
		      0, 0, MAXWIDTH, MAXHEIGHT, pxd_SV2112_getScanDirection(UNITSMAP), pxd_SV2112_getPixelClock(UNITSMAP), 0, 0);
	initControls(hDlg, 0);
    #elif SV1310
	int d = SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_GETCURSEL, 0, 0);
	switch (d) {
	  default:
	  case 0:   d = 0x0101; break;
	  case 1:   d = 0x0202; break;
	  case 2:   d = 0x0404; break;
	  case 3:   d = 0x0808; break;
	  case 4:   d = 0x0102; break;
	  case 5:   d = 0x0201; break;
	  case 6:   d = 0x0204; break;
	  case 7:   d = 0x0402; break;
	}
	pxd_SV1310_setResolutionAndTiming(UNITSMAP, 0, d,
		      0, 0, MAXWIDTH, MAXHEIGHT, pxd_SV1310_getReadoutDirection(UNITSMAP),
		      pxd_SV1310_getPixelClock(UNITSMAP), pxd_SV1310_getFramePeriod(UNITSMAP), 0);
	initControls(hDlg, 0);
    #elif SV1281
	int d = SendMessage(GetDlgItem(hDlg, IDLISTAOI), LB_GETCURSEL, 0, 0);
	switch (d) {
	  default:
	  case 0:   d = 1; break;
	  case 1:   d = 2; break;
	  case 2:   d = 4; break;
	  case 3:   d = 8; break;
	}
	//
	// The SV1281 doesn't have decimation.
	// Set an AOI, centered.
	//
	pxd_SV1281_setResolutionAndTiming(UNITSMAP, 0, 0,
		      (MAXWIDTH-MAXWIDTH/d)/2, (MAXHEIGHT-MAXHEIGHT/d)/2,
		      MAXWIDTH/d, MAXHEIGHT/d,
		      0,
		      pxd_SV1281_getPixelClock(UNITSMAP), 0, 0);
	initControls(hDlg, 0);
    #elif SV9M001 || SV9T001
	int d = SendMessage(GetDlgItem(hDlg, IDLISTDECIMATE), LB_GETCURSEL, 0, 0);
	switch (d) {
	  default:
	  case 0:   d = 0x0101; break;
	  case 1:   d = 0x0202; break;
	  case 2:   d = 0x0404; break;
	  case 3:   d = 0x0808; break;
	  case 4:   d = 0x0102; break;
	  case 5:   d = 0x0104; break;
	  case 6:   d = 0x0108; break;
	  case 7:   d = 0x0201; break;
	  case 8:   d = 0x0204; break;
	  case 9:   d = 0x0208; break;
	  case 10:  d = 0x0401; break;
	  case 11:  d = 0x0402; break;
	  case 12:  d = 0x0408; break;
	}
	pxd_SV9M001_setResolutionAndTiming(UNITSMAP, 0, d,
		      0, 0, MAXWIDTH, MAXHEIGHT, pxd_SV9M001_getScanDirection(UNITSMAP),
		      pxd_SV9M001_getPixelClock(UNITSMAP), pxd_SV9M001_getFramePeriod(UNITSMAP), 0);
	initControls(hDlg, 0);
    #elif SV642
	int d = SendMessage(GetDlgItem(hDlg, IDLISTAOI), LB_GETCURSEL, 0, 0);
	switch (d) {
	  default:
	  case 0:   d = 1; break;
	  case 1:   d = 2; break;
	  case 2:   d = 4; break;
	  case 3:   d = 8; break;
	}
	//
	// The SV642 doesn't have decimation.
	// Set an AOI, centered.
	//
	pxd_SV642_setResolutionAndTiming(UNITSMAP, 0, 0,
		      (MAXWIDTH-MAXWIDTH/d)/2, (MAXHEIGHT-MAXHEIGHT/d)/2,
		      MAXWIDTH/d, MAXHEIGHT/d,
		      0,
		      pxd_SV642_getPixelClock(UNITSMAP), pxd_SV642_getFramePeriod(UNITSMAP), 0);
	initControls(hDlg, 0);
    #endif
    return(TRUE);
}

/*
 * Process video mode
 * In this simple example, only the most popular video modes
 * are demonstrated.
 */
BOOL doVideoMode()
{
    #if SV2112
    #elif SV1310
	int d = SendMessage(GetDlgItem(hDlg, IDVIDEOMODE), LB_GETCURSEL, 0, 0);
	switch (d) {
	  case 0:   // free run
	    pxd_SV1310_setVideoAndTriggerMode(UNITSMAP, 0, 'f', 0, 0, 0, 0, 0);
	    break;
	  case 1:   // external trigger
	    pxd_SV1310_setVideoAndTriggerMode(UNITSMAP, 0, 'c', 's', '+', 0, 0, 0);
	    break;
	  case 2:   // 'snap' trigger
	    pxd_SV1310_setVideoAndTriggerMode(UNITSMAP, 0, 'c', 's', 'b', 0, 0, 0);
	    break;
	}
	initControls(hDlg, 0);
    #elif SV1281
	int d = SendMessage(GetDlgItem(hDlg, IDVIDEOMODE), LB_GETCURSEL, 0, 0);
	switch (d) {
	  case 0:   // free run
	    pxd_SV1281_setVideoAndTriggerMode(UNITSMAP, 0, 'f', 0, 0, 0, 0, 0);
	    break;
	  case 1:   // external trigger
	    pxd_SV1281_setVideoAndTriggerMode(UNITSMAP, 0, 'c', 's', '+', 0, 0, 0);
	    break;
	  case 2:   // 'snap' trigger
	    pxd_SV1281_setVideoAndTriggerMode(UNITSMAP, 0, 'c', 's', 'b', 0, 0, 0);
	    break;
	}
	initControls(hDlg, 0);
    #elif SV9M001 || SV9T001
	int d = SendMessage(GetDlgItem(hDlg, IDVIDEOMODE), LB_GETCURSEL, 0, 0);
	switch (d) {
	  case 0:   // free run
	    pxd_SV9M001_setVideoAndTriggerMode(UNITSMAP, 0, 'f', 0, 0, 0, 0, 0);
	    break;
	  case 1:   // external trigger
	    pxd_SV9M001_setVideoAndTriggerMode(UNITSMAP, 0, 'c', 's', '+', 0, 0, 0);
	    break;
	  case 2:   // 'snap' trigger
	    pxd_SV9M001_setVideoAndTriggerMode(UNITSMAP, 0, 'c', 's', 'b', 0, 0, 0);
	    break;
	}
	initControls(hDlg, 0);
    #elif SV642
	int d = SendMessage(GetDlgItem(hDlg, IDVIDEOMODE), LB_GETCURSEL, 0, 0);
	switch (d) {
	  case 0:   // free run
	    pxd_SV642_setVideoAndTriggerMode(UNITSMAP, 0, 'f', 0, 0, 0, 0, 0);
	    break;
	  case 1:   // external trigger
	    pxd_SV642_setVideoAndTriggerMode(UNITSMAP, 0, 'c', 's', '+', 0, 0, 0);
	    break;
	  case 2:   // 'snap' trigger
	    pxd_SV642_setVideoAndTriggerMode(UNITSMAP, 0, 'c', 's', 'b', 0, 0, 0);
	    break;
	}
	initControls(hDlg, 0);
    #endif
    return(TRUE);
}


/*
 * Process white balance button.
 *
 * Some cameras can implement coarse white balance
 * via color specific gain settings - this only
 * adjusts the postcapture white balance corrections.
 * Typically, a video setup file exported from XCAP
 * is used to set the coarse settings.
 */
BOOL doWhiteButton()
{
    unsigned int reference[3] = {0,0,0};
    unsigned int target[3]    = {0,0,0};
    double  masses[] = {0, 0, 0};
    ushort  pixels[9*9];
    int     midx, midy, i;
    //
    // This follows the white balance application note so explanatory
    // comments are minimized.
    // This assumes that a white image has already been captured.
    //
    pxd_setImageBrightBalance(UNITSMAP, reference, target, 0.00);
    midx = pxd_imageXdim()/2;
    midy = pxd_imageYdim()/2;
    pxd_readushort(1,1,midx-9/2,midy-9/2,midx+1+9/2,midy+1+9/2,pixels,9*9,"RofRGB");
    for (i = 0; i < 9*9; i++)
	masses[0] += pixels[i];
    pxd_readushort(1,1,midx-9/2,midy-9/2,midx+1+9/2,midy+1+9/2,pixels,9*9,"GofRGB");
    for (i = 0; i < 9*9; i++)
	masses[1] += pixels[i];
    pxd_readushort(1,1,midx-9/2,midy-9/2,midx+1+9/2,midy+1+9/2,pixels,9*9,"BofRGB");
    for (i = 0; i < 9*9; i++)
	masses[2] += pixels[i];
    reference[0] = (unsigned int)(masses[0]/(9*9));
    reference[1] = (unsigned int)(masses[1]/(9*9));
    reference[2] = (unsigned int)(masses[2]/(9*9));
    target[0] = target[1] = target[2] = max(max(reference[0], reference[1]), reference[2]);
    pxd_setImageBrightBalance(UNITSMAP, reference, target, 1.00);
    return(TRUE);
}


/*
 * AGC Example.
 * AGC is a software feature. The current image
 * intensity is compared to a desired target, and the
 * camera's gain and/or exposure are adjusted.
 * There are many possible variations, such as whether
 * gain or exposure are adjusted, or both, how fast they
 * are adjusted, etc. This demonstrates the basics and
 * outlines the possible variations.
 */
void doAGC()
{
    static  DWORD   lasttickcount = 0;
    static  ulong   lastcapturedfieldcount = 0;
    DWORD   tickcount;
    ulong   capturedfieldcount;

    //
    // How often should be attempt to adjust?
    // Don't waste time adjusting too often,
    // or of nothing new has been captured.
    // We choose every .25 seconds and two fields.
    //
    tickcount = GetTickCount();
    capturedfieldcount =  pxd_capturedFieldCount(0x1);
    if ((tickcount-lasttickcount) < 250
     || (capturedfieldcount-lastcapturedfieldcount) < 2)
	return;
    lasttickcount = tickcount;
    lastcapturedfieldcount = capturedfieldcount;

    //
    // Obtain some metric of image brightness.
    // This could be mean or max, of the whole image
    // or of an AOI, done with a PXIPL function such as pxip8_masscenter()
    // or pxip8_histab, or done with pxd_readushort or pxd_readuchar.
    // We choose to get the max of a thin sample through the vertical
    // and horizontal w/out PXIPL.
    //
    double  metric = 0;
    uchar   pixel;
    int xdim = pxd_imageXdim();
    int ydim = pxd_imageYdim();
    for (int i = 0; i < ydim; i++) {
	pxd_readuchar(0x1,1,xdim/2,i,1+xdim/2,i+1,&pixel,1,"Grey");
	// mean?
	//metric += ((double)pixel/(ydim+xdim)) * (100/255.0);	 // scaled to 100% max
	// max?
	metric = max(metric, pixel * (100/255.0));
    }
    for (i = 0; i < xdim; i++) {
	pxd_readuchar(0x1,1,i,ydim/2,i+1,1+ydim/2,&pixel,1,"Grey");
	// mean?
	//metric += ((double)pixel/(ydim+xdim)) * (100/255.0);	 // scaled to 100% max
	// max?
	metric = max(metric, pixel * (100/255.0));
    }

    //
    // Compare metric to desired target.
    // We choose 75% of max.
    //
    double delta = metric - 75.0;

    //
    // If we are close, don't adjust
    // so as to avoid gain/exposure fluctuations for
    // very small changes. We choose 10% hysteresis.
    //
    if (fabs(delta) < 10.0)
	return;

    //
    // Get current gain and exposure values.
    //
    //
    #if SV2112
	double	gain = pxd_SV2112_getGain(1, 'R');
	double	exps = pxd_SV2112_getExposure(1);
    #elif SV1310
	double	gain = pxd_SV1310_getGain(1);
	double	exps = pxd_SV1310_getExposure(1);
	double	offs = pxd_SV1310_getOffset(1);
    #elif SV1281
	double	gain = pxd_SV1281_getGain(1);
	double	exps = pxd_SV1281_getExposure(1);
	double	offs = pxd_SV1281_getOffset(1);
    #elif SV9M001 || SV9T001
	double	exps = pxd_SV9M001_getExposure(1);
    #elif SV642
	double	exps = pxd_SV9M001_getExposure(1);
    #endif

    //
    // Choose whether to implement AGC by modifying
    // gain or exposure. In general, given a choice,
    // it is better to adjust exposure, as increasing gain
    // adds noise. In some applications, it may be beneficial
    // to do both. Here we choose to adjust exposure.
    //
    exps *= (delta < 0? 1.10: 0.90);
    if (fabs(delta) >= 20.0)
	exps *= (delta < 0? 1.10: 0.90);


    //
    // Set. Don't worry if the new gain or exposure value is out of bounds.
    // They will be corrected.
    // Modify GUI controls so we can see the effect.
    //
    #if SV2112
	pxd_SV2112_setExposureAndGain(UNITSMAP, 0, exps, gain, gain, gain);
	SetScrollPos(GetDlgItem(hDlg, IDGAINSCROLL), SB_CTL, ToScroll(pxd_SV2112_getGain(UNITSMAP, 'R'),10), TRUE);
	SetScrollPos(GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll((pxd_SV2112_getExposure(UNITSMAP)-loexp),10), TRUE);
    #elif SV1310
	pxd_SV1310_setExposureGainOffset(UNITSMAP, 0, exps, gain, offs, 0, 0);
	SetScrollPos(GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll((pxd_SV1310_getExposure(1)-loexp),10), TRUE);
	SetScrollPos(GetDlgItem(hDlg, IDGAINSCROLL), SB_CTL, ToScroll(pxd_SV1310_getGain(UNITSMAP),10), TRUE);
    #elif SV1281
	pxd_SV1281_setExposureGainOffset(UNITSMAP, 0, exps, gain, offs, 0, 0);
	SetScrollPos(GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll((pxd_SV1281_getExposure(1)-loexp),10), TRUE);
	SetScrollPos(GetDlgItem(hDlg, IDGAINSCROLL), SB_CTL, ToScroll(pxd_SV1281_getGain(UNITSMAP),10), TRUE);
    #elif SV9M001 || SV9T001
	pxd_SV9M001_setExposureAndGain(UNITSMAP, 0, exps,
				       pxd_SV9M001_getGain(1, 'R'), pxd_SV9M001_getGain(1, 'B'),
				       pxd_SV9M001_getGain(1, 'R'^'G'), pxd_SV9M001_getGain(1, 'B'^'G'));
	SetScrollPos(GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll((pxd_SV9M001_getExposure(1)-loexp),10), TRUE);
	SetScrollPos(GetDlgItem(hDlg, IDGAINSCROLL), SB_CTL, ToScroll(pxd_SV9M001_getGain(UNITSMAP, 'R'),10), TRUE);
    #elif SV642
	pxd_SV642_setExposureAndGain(UNITSMAP, 0, exps,
				       pxd_SV642_getGain(1, 'R'), pxd_SV642_getGain(1, 'B'),
				       pxd_SV642_getGain(1, 'R'^'G'), pxd_SV642_getGain(1, 'B'^'G'),
				       pxd_SV642_getGainRange(UNITSMAP));
	SetScrollPos(GetDlgItem(hDlg, IDEXPSCROLL), SB_CTL, ToScroll((pxd_SV642_getExposure(1)-loexp),10), TRUE);
	SetScrollPos(GetDlgItem(hDlg, IDGAINSCROLL), SB_CTL, ToScroll(pxd_SV642_getGain(UNITSMAP, 'R'),10), TRUE);
    #endif
}


/*
 * The Dialog
 */
BOOL CALLBACK
PIXCIDialogProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    static  HWND	hWndImage;			    // child window of dialog for image display
    static  UINT	svgaBits;			    // pixel format of S/VGA
    static  pxvbtime_t	lastcapttime[UNITS] = {0};	    // when was image last captured
    static  struct	pxywindow windImage[max(4,UNITS)];  // subwindow of child window for image display
    static  ulong	displaycount = 0;		    // display rate statistics
    int     err;

    #if SHOWIM_DIRECTXDISPLAY
	static LPDIRECTDRAW lpDD = NULL;
	static HINSTANCE    hDDLibrary = NULL;
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
	// Either FORMAT or FORMATFILE should have been
	// selected above.
	//
	#if defined(FORMAT)
	    if (pxd_PIXCIopen(driverparms, FORMAT, "") < 0)
		pxd_mesgFault(UNITSMAP);
	#endif
	#if defined(FORMATFILE)
	    if (pxd_PIXCIopen(driverparms, "", FORMATFILE) < 0)
		pxd_mesgFault(UNITSMAP);
	#endif


	//
	// Set our title.
	//
	#if SV2112
	    SetWindowText(hDlg, "EPIX(R) SILICON VIDEO(R) 2112(C) Example");
	#elif SV1310
	    SetWindowText(hDlg, "EPIX(R) SILICON VIDEO(R) 1310(C) Example");
	#elif SV1281
	    SetWindowText(hDlg, "EPIX(R) SILICON VIDEO(R) 1281(C) Example");
	#elif SV9M001
	    SetWindowText(hDlg, "EPIX(R) SILICON VIDEO(R) 9M001(C) Example");
	#elif SV9T001
	    SetWindowText(hDlg, "EPIX(R) SILICON VIDEO(R) 9T001(C) Example");
	#elif SV642
	    SetWindowText(hDlg, "EPIX(R) SILICON VIDEO(R) 642(C) Example");
	#endif

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
		MessageBox(NULL, "Can't create image update display event!", "XCLIBEXC", MB_OK|MB_TASKMODAL);
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
	if (UNITS > 1) {
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
	// Warn if LIVE_LIVE2 was selected and there is only one buffer.
	//
	#if LIVE_LIVE2
	    if (pxd_imageZdim() < 2)
		MessageBox(NULL, "Only 1 image frame buffer available!", "XCLIBEXC", MB_OK|MB_TASKMODAL);
	#endif

	//
	// Customize and init dialog controls
	//
	initControls(hDlg, 1);

	return(TRUE);
      }

      case WM_COMMAND:
	switch (LOWORD(wParam)) {

	  case IDLISTAOI:
	  case IDLISTDECIMATE:
	    if (HIWORD(wParam) != LBN_SELCHANGE)
		return(FALSE);
	    if (doDecimateList()) {
		if (liveon)	// changing decimation may imply turning video off
		    goto live;
		return(TRUE);
	    }
	    return(FALSE);

	  case IDWHITEBAL:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    return(doWhiteButton());

	  case IDSNAP:
	    if (HIWORD(wParam) != BN_CLICKED)
	       return(FALSE);
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
	    err = pxd_goSnap(UNITSMAP, 1);
	    if (err < 0)
		MessageBox(NULL, pxd_mesgErrorCode(err), "XCLIBEXC", MB_OK|MB_TASKMODAL);
	    return(TRUE);

	  case IDVIDEOMODE:
	    if (HIWORD(wParam) != LBN_SELCHANGE)
		return(FALSE);
	    // Go unlive. In case we were in trigger mode
	    // and snap'ed awaiting a trigger use AbortLive to cancel.
	    pxd_goAbortLive(UNITSMAP);
	    liveon = FALSE;
	    return(doVideoMode());

	  case IDLIVE:
	    if (HIWORD(wParam) != BN_CLICKED)
	       return(FALSE);
	  live:
	    liveon = TRUE;
	    #if LIVE_LIVE
		err = pxd_goLive(UNITSMAP, 1L);
	    #elif LIVE_LIVE2
		err = pxd_goLivePair(UNITSMAP, 1L, 2L);
	    #elif LIVE_SNAP
		err = pxd_goSnap(UNITSMAP, 1);
	    #endif
	    if (err < 0)
		MessageBox(NULL, pxd_mesgErrorCode(err), "XCLIBEXC", MB_OK|MB_TASKMODAL);
	    return(TRUE);

	  case IDUNLIVE:
	    if (HIWORD(wParam) != BN_CLICKED)
	       return(FALSE);
	    #if LIVE_LIVE
		pxd_goUnLive(UNITSMAP);
		liveon = FALSE;
	    #elif LIVE_LIVE2
		pxd_goUnLive(UNITSMAP);
		liveon = FALSE;
	    #elif LIVE_SNAP
		liveon = FALSE;
	    #endif
	    return(TRUE);

	}
	break;

      case WM_HSCROLL:
	switch (GetWindowLong((HWND)lParam, GWL_ID)) {
	  case IDEXPSCROLL:
	    return(doExposureScroll(wParam));
	  case IDGAINSCROLL:
	    return(doGainScroll(wParam));
	  case IDGAIN2SCROLL:
	    return(doGain2Scroll(wParam));
	  case IDOFFSETSCROLL:
	    return(doOffsetScroll(wParam));
	  case IDCLKSCROLL:
	    if (doClockScroll(wParam)) {
		if (liveon) // changing clock may imply turning video off
		    goto live;
		return(TRUE);
	    }
	    return(FALSE);
	  case IDFPSSCROLL:
	    if (doFpsScroll(wParam)) {
		if (liveon) // changing fps may imply turning video off
		    goto live;
		return(TRUE);
	    }
	    return(FALSE);
	}
	return(FALSE);

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

	DestroyWindow(hWnd);
	EndDialog(hDlg, 0);
	return(TRUE);

      case WM_TIMER:	// if using UPDATE_TIMER, update image display
	//
	// Monitor for asynchronous faults, such as video
	// being disconnected while capturing. These faults
	// can't be reported by functions such as pxd_goLive()
	// which initiate capture and return immediately.
	//
	// Should there be a fault pxd_mesgFault() pop up a dialg,
	// the Windows TIMER will continue in a new thread. Thus the
	// 'faulting' variable and logic to limit to one dialog at a time.
	//
	if (pxd_infoUnits()) {	 // implies whether librry is open
	    static int faulting = 0;
	    if (!faulting) {
		faulting++;
		pxd_mesgFault(UNITSMAP);
		faulting--;
	    }
	    //
	    // Show
	    //	spatial resolution
	    //	bit depth & color
	    //	measured video rate
	    //	measured display rate
	    //	selected video format
	    //
	    // We are measuring the frame rate against the imprecise TIMER
	    // as a reference; the measurement will not be exact
	    // and may fluctuate. We do some averaging of the computed
	    // framerates to reduce fluctuations, at the expense of speed
	    // of response when the frame rate actually changes.
	    //
	    static DWORD lasttickcount = 0;
	    static ulong lastfieldcount = 0;
	    DWORD ticks = GetTickCount();
	    if ((ticks - lasttickcount) > 1000) { // update once per second
		static	double fpsave = 0;
		ulong	fields = pxd_getFieldCount(1);
		double	fps1, fps2;
		char	buf[100];
		fps1 = lasttickcount?1000.0*(double)(fields-lastfieldcount)/((ticks-lasttickcount)*pxd_videoFieldsPerFrame()):0;
						    // the raw computed framerate
		if (fps1 == 0)
		    fpsave = 0;
		fpsave += fps1;
		fps2 = fpsave/3;		    // the averaged framerate
		fpsave -= fps2;
		sprintf(buf, "dim: %dx%d  res: %dx%d  fps: %.3f disp: %.3f",
			pxd_imageXdim(), pxd_imageYdim(), pxd_imageBdim(), pxd_imageCdim(), fps2,
			1000.0*(double)displaycount/(ticks-lasttickcount));
		SetWindowText(GetDlgItem(hDlg, IDSTATUS1), buf);
		sprintf(buf, "camera: %s  fmt: %s",
		    #if SV2112
			"SV2112(C)",
		    #elif SV1310
			"SV1310(C)",
		    #elif SV1281
			"SV1281(C)",
		    #elif SV9M001
			"SV9M001(C)",
		    #elif SV9T001
			"SV9T001C",
		    #elif SV642
			"SV642(C)",
		    #else
			"",
		    #endif
		    #if defined(FORMAT)
			FORMAT);
		    #elif defined(FORMATFILE_LOAD)
			FORMATFILE_LOAD);
		    #elif defined(FORMATFILE_COMP)
			"compiled");
		    #else
			"");
		    #endif
		SetWindowText(GetDlgItem(hDlg, IDSTATUS2), buf);
		lasttickcount = ticks;
		lastfieldcount = fields;
		displaycount   = 0;
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
	    // Show image using PXIPL pxio8_GDIDisplay.
	    // Altho not used in this example, pxio8_GDIDisplay also allows drawing
	    // a full screen cross hair cursor over the image.
	    //
	    #if SHOWIM_GDIDISPLAY
		pxio8_GDIDisplay(NULL, pxd_defineImage(1<<u, buf, 0, 0, -1, -1, "Display"),
				 NULL, 0, 'n', 0, 0,hDC,&windImage[u], NULL, NULL);
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

	    ReleaseDC(hWndImage, hDC);

	    //
	    // After display, if in live mode using LIVE_SNAP,
	    // having displayed one image start snapping the next
	    //
	    #if LIVE_SNAP
		if (liveon)
		    pxd_goSnap(1<<u, 1);
	    #endif

	    //
	    // For display rate statistics.
	    //
	    displaycount++;

	}
	//
	// Implement AGC, calling periodically?
	//
	#if SOFT_AGC
	    doAGC();
	#endif
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
			"XCLIBEXC Windows Example",
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

    //
    // Launch dialog.
    //
    #if SV2112
	hDlg = CreateDialogParam(hInstance, "PIXCISV2112DIALOG", NULL, (DLGPROC)PIXCIDialogProc, NULL);
    #elif SV1310
	hDlg = CreateDialogParam(hInstance, "PIXCISV1310DIALOG", NULL, (DLGPROC)PIXCIDialogProc, NULL);
    #elif SV1281
	hDlg = CreateDialogParam(hInstance, "PIXCISV1281DIALOG", NULL, (DLGPROC)PIXCIDialogProc, NULL);
    #elif SV9M001
	hDlg = CreateDialogParam(hInstance, "PIXCISV9M001DIALOG", NULL, (DLGPROC)PIXCIDialogProc, NULL);
    #elif SV9T001
	hDlg = CreateDialogParam(hInstance, "PIXCISV9T001DIALOG", NULL, (DLGPROC)PIXCIDialogProc, NULL);
    #elif SV642
	hDlg = CreateDialogParam(hInstance, "PIXCISV642DIALOG", NULL, (DLGPROC)PIXCIDialogProc, NULL);
    #endif
    if (!hDlg) {
	MessageBox(NULL, "Missing Dialog Resource - Compilation or Link Error!", "XCLIBEXC", MB_OK|MB_TASKMODAL);
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
