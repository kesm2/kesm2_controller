/*
 *
 *	xclibex5.cpp	External	19-Aug-2004
 *
 *	Copyright (C)  1998-2004  EPIX, Inc.  All rights reserved.
 *
 *	Example program for the XCLIB 'C' Library.
 *	Example assumes Windows 95/98/ME/NT/2000/XP.
 *
 *	Demonstrates XCLIB and/or PXIPL functions for capture and
 *	display from multiple PIXCI(R) imaging boards where
 *	each board is operating at a different resolution or camera,
 *	or the boards may be different models.
 *	This Windows program must, of course, also make use of
 *	various Windows GDI API functions; however, this is not
 *	intended to serve as a Windows tutorial.
 *
 *	Many parts similar to the XCLIBEX2.CPP example; that
 *	example provides PIXCI SV2, SV3, SV4, SV5, SV5A, and SV6 specific
 *	controls, and demonstrates more display options including
 *	graphic overlays. It also demonstrates access to numeric
 *	pixel values and use of Events rather than a timer.
 *	It also demonstrates use of multiple boards and cameras
 *	of the same type and operated at the same resolution.
 *	For simplicity this example concentrates on operating
 *	multiple boards at different resolutions and does not illustrate
 *	as many display options nor duplicate the other features
 *	demonstrated in that example. This example also
 *	demonstrates use of the non pxd_ style functions.
 *
 */


/*
 *  INSTRUCTIONS:
 *
 *  1)	Set 'define' options below according to the intended camera
 *	and video format(s).
 *
 *	For PIXCI(R) SV2, SV3, SV4, SV5, SV5A, and SV6 imaging boards
 *	common choices are RS-170, NSTC, NTSC/YC, CCIR, PAL, or PAL/YC.
 *	(The SV5A does not support NTSC/YC or PAL/YC).
 *
 *	For PIXCI(R) D, A, D24, D32, D2X, D3X, CL1, CL2, E1, E4, E1DB, CL3SD, and SI
 *	imaging boards, use "default" to select the default format for
 *	 the camera for which the PIXCI(R) imaging board is intended.
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
//#define FORMAT_UNIT0	"RS-170"    // RS-170 on input 2
  #define FORMAT_UNIT0	"NTSC"	    // NTSC on input 2
//#define FORMAT_UNIT0	"NTSC/YC"   // NSTC S-Video on input 1		    (N/A on SV5A)
//#define FORMAT_UNIT0	"CCIR"	    // CCIR on input 2
//#define FORMAT_UNIT0	"PAL"	    // PAL (B,D,G,H,I) on input 2
//#define FORMAT_UNIT0	"PAL/YC"    // PAL (B,D,G,H,I) S-Video on input 1   (N/A on SV5A)
  #define FORMAT_UNIT1	"RS-170"    // RS-170 on input 2
//#define FORMAT_UNIT1	"NTSC"	    // NTSC on input 2
//#define FORMAT_UNIT1	"NTSC/YC"   // NSTC S-Video on input 1		    (N/A on SV5A)
//#define FORMAT_UNIT1	"CCIR"	    // CCIR on input 2
//#define FORMAT_UNIT1	"PAL"	    // PAL (B,D,G,H,I) on input 2
//#define FORMAT_UNIT1	"PAL/YC"    // PAL (B,D,G,H,I) S-Video on input 1   (N/A on SV5A)

				    // For PIXCI(R) D, D24, D32, A, D2X, D3X, CL1, CL2, E1, E4, E1DB, CL3SD, SI
//#define FORMAT_UNIT0	"default"   // as per board's intended camera
//#define FORMAT_UNIT1	"default"   // as per board's intended camera

				    // For any PIXCI(R) imaging board
				    // using a format file saved by XCAP
//#define FORMATFILE_LOAD   "xcvidset.fmt"  // loaded from file during execution



/*
 *  2.1) Number of expected PIXCI(R) image boards.
 *  Currently, this example's code only supports 1 or 2;
 *  althouh there are partial 'hooks' to support 1 through 4.
 */
#define UNITS	    2

/*
 *  2.2) Choose whether we are operating the same model boards
 *  but with different resolutions and/or cameras.
 *  Or whether we are operating different model boards.
 *  The former can be done with one open instance of the library.
 *  and allows performing some actions on all boards with one function call.
 *  The latter requires opening multiple instances of the library,
 *  one for each different type of board.
 *
 *  Only one of these choices should have value 1, the others should be 0.
 */
#define ONELIB_TWOUNIT	    1	// open one instance which controls multiple boards
#define TWOLIB_ONEUNIT	    0	// open multiple instances, each controlling one board
				// (supported by library, but not yet shown in this example)


/*
 *  2.3) Optionally, set driver configuration parameters.
 *  These are normally left to the default, "".
 *  The actual driver configuration parameters include the
 *  desired PIXCI(R) imaging boards, but to make configuation easier,
 *  code, below, will automatically add board selection to this.
 */
//#define DRIVERPARMS "-QU 0"	// don't use interrupts
  #define DRIVERPARMS ""	// default


/*
 *  3.1) Choose which form of image display is to be demonstrated.
 *  Some of these  options expect that the optional PXIPL library is present.
 *  Others may expect that the Windows DirectDraw SDK is present
 *  (available from Microsoft) and that the S/VGA supports DirectDraw.
 *
 *  Only one of these choices should have value 1, the others should be 0.
 */
#define SHOWIM_STRETCHDIBITS	1   // use XCLIB or XCLIB-Lite and GDI
#define SHOWIM_DRAWDIBDRAW	0   // use XCLIB or XCLIB-Lite and Video for Windows
#define SHOWIM_DRAWDIBDISPLAY	0   // use XCLIB and PXIPL and Video for Windows
#define SHOWIM_GDIDISPLAY	0   // use XCLIB and PXIPL
#define SHOWIM_DIRECTXDISPLAY	0   // use XCLIB and PXIPL and DirectDraw


/*
 *  4a) Compile with Microsoft Visual C/C++ V6.0 for Windows 95/98/ME as:
 *
 *	    cl -c xclibex5.cpp
 *	    rc.exe /l 0x409 /fo tt.res xclibex2.rc
 *	    echo -subsystem:windows,4.0 -entry:WinMainCRTStartup comdlg32.lib ole32.lib >t1
 *	    echo /NODEFAULTLIB libc.lib oldnames.lib user32.lib gdi32.lib kernel32.lib vfw32.lib largeint.lib winmm.lib >t2
 *
 *	Either (XCLIB w/out PXIPL for Windows 95/98/ME)
 *	    link xclibex5.obj XCLIBW95.lib tt.res @t1 @t2
 *	or  (XCLIB+PXIPL only for Windows 95/98/ME)
 *	    link xclibex5.obj XCLIBW95.lib PXIPLW95.LIB tt.res @t1 @t2
 *	or (XCLIB-Lite for Windows 95/98/ME)
 *	    link xclibex5.obj XCLYBW95.LIB tt.res @t1 @t2
 *
 *	    del t1
 *	    del t2
 *	    del tt.res
 *	    del xclibex5.obj
 *
 *	Or use the IDE, compiling xclibex5.cpp as a Windows 95/98/ME app,
 *	using the XCLIBEX2.RC resource, linking to library(s):
 *	    XCLIB:	  XCLIBW95.LIB
 *	    XCLIB+PXIPL:  XCLIBW95.LIB PXIPLW95.LIB
 *	    XCLIB-Lite:   XCLYBW95.LIB
 *	For example, in the IDE for VISUAL C/C++ V5.0:
 *	    1. Start Integrated Development Environment.
 *	    2. Select 'File' - 'New'.
 *	    3. Enter Project Name and Select 'WIN 32 Application'.
 *	    4. Copy this file, XCLIBEX5.CPP, and XCLIBEX2.RC to your
 *	       project directory.
 *	    5. Select 'Project' - 'Add To Project' - 'Files ...' and select
 *	       this file, XCLIBEX5.CPP, and XCLIBEX2.RC.
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
 *	    link xclibex5.obj XCLIBWNT.lib tt.res @t1 @t2
 *	or  (XCLIB+PXIPL only for Windows NT/2000/XP)
 *	    link xclibex5.obj XCLIBWNT.lib PXIPLWNT.LIB tt.res @t1 @t2
 *	or (XCLIB-Lite for Windows NT/2000/XP)
 *	    link xclibex5.obj XCLYBWNT.LIB tt.res @t1 @t2
 *
 *
 *  For Borland, Watcom, and/or Delphi users: The XCLIBEX2.CPP example
 *  provides compilation instructions and/or hints which are also
 *  applicable to this example.
 *
 */


/*
 *  NECESSARY INCLUDES:
 */
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
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
    #if SHOWIM_GDIDISPLAY || SHOWIM_DIRECTXDISPLAY || SHOWIM_DRAWDIBDISPLAY || USE_PXIPL
	#include "pxipl.h"         
	#include "pximages.h"           
    #endif
}

/*
 * Global variables.
 */
static	HWND	hWnd;	    /* the main window */
static	HWND	hDlg;	    /* the main dialog */
#if SHOWIM_DRAWDIBDRAW || SHOWIM_DRAWDIBDISPLAY
    static HDRAWDIB	hDrawDib = NULL;    /* VFW handle */
#endif
#if SHOWIM_DIRECTXDISPLAY
    static LPDIRECTDRAW lpDD = NULL;
    static HINSTANCE	hDDLibrary = NULL;  /* DDraw handles */
#endif

/*
 * Utility
 */
void mesg(const char *format, ...)
{
    va_list argp;
    char    *buf;
    #define MAXLEN 1024

    if (!(buf = (char *)malloc(MAXLEN))) {
	MessageBox(NULL, "No memory!", "XCLIBEX5", MB_OK|MB_TASKMODAL);
	return;
    }
    va_start(argp, format);
    vsprintf(buf, format, argp);
    va_end(argp);
    MessageBox(NULL, buf, "XCLIBEX5", MB_OK|MB_TASKMODAL);
    free(buf);
}

/*
 * Display specified buffer from specified unit,
 * in specified AOI of specified HWND,
 * using a compile-time selected method.
 */
void DisplayBuffer(xclibs_s *xclib, int stateid, int unit, pxbuffer_t buf, HWND hWndImage, struct pxywindow windImage)
{
    HDC  hDC;

    hDC = GetDC(hWndImage);

    //
    // Show image using pxd_renderStretchDIBits.
    //
    #if SHOWIM_STRETCHDIBITS
	SetStretchBltMode(hDC, STRETCH_DELETESCANS);
	xclib->pxaux.StretchDIBits(&xclib->pxaux, 0, 1<<unit, stateid, buf, 0, 0, -1, -1, 0,
			 hDC, windImage.nw.x, windImage.nw.y,
			 windImage.se.x-windImage.nw.x,
			 windImage.se.y-windImage.nw.y);
    #endif

    //
    // Show image using PXIPL pxio8_GDIDisplay.
    // Altho not used in this example, pxio8_GDIDisplay also allows drawing
    // a full screen cross hair cursor over the image.
    //
    #if SHOWIM_GDIDISPLAY
    {
	pximage_s   image[PXMAX_FIRBGF];
	xclib->pxlib.initFilteredPximage(&xclib->pxlib,1<<unit,image,PXMAX_FIRBGF,PXHINTBGR,0,stateid,buf, 0, 0x7);
	pxio8_GDIDisplay(NULL, image,
			 NULL, 0, 'n', 0, 0,hDC,&windImage, NULL, NULL);
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
	pximage_s   image[PXMAX_FIRBGF];
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
		windImage.nw.x += pt.x;
		windImage.nw.y += pt.y;
		windImage.se.x += pt.x;
		windImage.se.y += pt.y;
		xclib->pxlib.initFilteredPximage(&xclib->pxlib,1<<unit,image,PXMAX_FIRBGF,PXHINTBGR,0,stateid,buf, 0, 0x7);
		pxio8_DirectXDisplay(NULL, image,
				     NULL, 0, 'n', 0, 0, ddrs, &windImage, NULL, hDC, NULL, NULL);
		windImage.nw.x -= pt.x;
		windImage.nw.y -= pt.y;
		windImage.se.x -= pt.x;
		windImage.se.y -= pt.y;
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
    {
	BITMAPINFOHEADER FAR *dib;
	HGLOBAL hDIB = NULL;

	xclib->pxaux.makeDIB(&xclib->pxaux, 0, 1<<unit, stateid, buf, 0, 0, -1, -1, &hDIB);
	if (hDIB) {
	    if (dib = (BITMAPINFOHEADER FAR *)GlobalLock(hDIB)) {
		DrawDibDraw(hDrawDib, hDC, windImage.nw.x, windImage.nw.y,
					   windImage.se.x-windImage.nw.x, windImage.se.y-windImage.nw.y,
			    (BITMAPINFOHEADER *)dib,
			    (uchar FAR*)dib+dib->biSize+dib->biClrUsed*sizeof(RGBQUAD),
			    0, 0, pxd_imageXdim(), pxd_imageYdim(), 0);
		GlobalUnlock(hDIB);
	    }
	    xclib->pxaux.freeDIB(&xclib->pxaux, 0, 0, stateid, 0, 0, 0, 0, 0, &hDIB);
	}
    }
    #endif

    //
    // Show image using PXIPL and Video for Windows.
    // Altho not used in this example, pxio8_DrawDibDisplay also allows drawing
    // a full screen cross hair cursor over the image.
    //
    #if SHOWIM_DRAWDIBDISPLAY
    {
	pximage_s   image[PXMAX_FIRBGF];
	xclib->pxlib.initFilteredPximage(&xclib->pxlib,1<<unit,image,PXMAX_FIRBGF,PXHINTBGR,0,stateid,buf, 0, 0x7);
	pxio8_DrawDibDisplay(NULL, image,
			 NULL, 0, 'n', 0, 0,hDrawDib,hDC,&windImage, NULL, NULL);
    }
    #endif

    ReleaseDC(hWndImage, hDC);
}

/*
 * The Dialog
 */
BOOL CALLBACK
PIXCIDialogProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    static  xclibs_s	xclib[max(4,UNITS)] = { {0},{0},{0},{0} };
							    // library instances
    static  UINT	svgaBits;			    // pixel format of S/VGA
    static  int 	liveon = 0;
    static  int 	isopen = 0;
    static  pxvbtime_t	lastcapttime[UNITS] = {0};	    // when was image last captured
    static  struct	pxywindow windImage[max(4,UNITS)];  // subwindow of child window for image display
    static  HWND	hWndImage;			    // child window of dialog for image display
	    int 	u, r;

    //
    // To cleanly convert code between ONELIB_TWOUNIT and TWOLIB_ONEUNIT modes.
    // In the former, there is one instance of xclib_s, and a 'unitmap'
    // parameter to various API calls selects a unit.
    // In the latter, there are multiple instances of xclib_s, and the
    // 'unitmap' parameter is always 0x1.
    //
    // N.B. The -DM parameter to xclib_open is different than the 'unitmap'
    // API parameter. The former selects one or more physical boards, the
    // latter selects one or more 'logical' boards relative to those opened
    // via that xclib_s instance.
    //
    static  int 	unitselect   [max(4,UNITS)];
    static  xclibs_s	*xclibselect [max(4,UNITS)];
    static  int 	stateidselect[max(4,UNITS)];


    switch (wMsg) {
      case WM_INITDIALOG:
      {
	RECT	rectImage;

	//
	// Open the PIXCI(R) imaging boards.
	//
	// Either FORMAT_UNIT? or FORMATFILE_LOAD
	// should have been selected above.
	//
	#if ONELIB_TWOUNIT
	    memset(&xclib[0], 0, sizeof(xclib[0]));
	    xclib[0].ddch.len = sizeof(xclib[0]);
	    xclib[0].ddch.mos = XCMOS_LIBS;
	    char driverparms[80];
	    sprintf(driverparms, "-DM 0x%x %s", (1<<UNITS)-1, DRIVERPARMS);
	    #if defined(FORMAT_UNIT0)
		if ((r = xclib_open(&xclib[0], NULL, driverparms, FORMAT_UNIT0, NULL)) >= 0)
		    isopen++;
		else if (xclib[0].pxaux.faultMessageBox)
		    xclib[0].pxaux.faultMessageBox(&xclib[0].pxaux, 0, (1<<UNITS)-1, NULL, NULL, NULL, 0);
		else
		    mesg("xclib_open: Error=%d", r);
		if (!isopen)
		    return(TRUE);
		//
		// The xclib_open will have set the PXMODE_DIGI+0 as per FORMAT_UNIT0,
		// we now must create a PXMODE_DIGI+1 as per FORMAT_UNIT1.
		//
		#if UNITS > 1
		    pxvidstate_s *statep = NULL;
		    pxdevinfo_s  info;
		    memset(&info, 0, sizeof(info));
		    info.ddch.len  = sizeof(info);
		    info.ddch.mos = PXMOS_DEVINFO;

		    if ((r = xclib[0].pxdev.getDevInfo(&xclib[0].pxdev, 1<<1, 0, &info)) < 0
		     || (r = xclib[0].pxlib.allocStateCopy(&xclib[0].pxlib, 0, 0, &statep)) < 0
		     || (r = xclib[0].pxlib.initStateCopy(&xclib[0].pxlib, 0,0,statep,&info,FORMAT_UNIT1,PXMODE_DIGI)) < 0
		     || (r = xclib[0].pxlib.defineState(&xclib[0].pxlib,0,PXMODE_DIGI+1,statep)) < 0) {
			mesg("initStateCopy: Unit=%d Error=%d", 1, r);
		    }
		    xclib[0].pxlib.freeStateCopy(&xclib[0].pxlib, 0, 0, &statep);
		#endif

	    #endif
	    #if defined(FORMATFILE_LOAD)
		//
		// This is expected to load a format file saved by XCAP
		// which was set for individual controls for each of the
		// units. Thus, the format file defines PXMODE_DIGI+0,
		//  PXMODE_DIGI+1, etc.
		//
		if ((r = xclib_open(&xclib[0], NULL, driverparms, NULL, FORMATFILE_LOAD)) >= 0)
		    isopen++;
		else if (xclib[0].pxaux.faultMessageBox)
		    xclib[0].pxaux.faultMessageBox(&xclib[0].pxaux, 0, (1<<UNITS)-1, NULL, NULL, NULL, 0);
		else
		    mesg("xclib_open: Error=%d", r);
	    #endif
	    if (!isopen)
		return(TRUE);

	    //
	    // Init boards' configuration as per video states.
	    //
	    for (u = 0; u < UNITS; u++) {
		if ((r = xclib[0].xcdev.setVideoConfig(&xclib[0].xcdev, 1<<u, 0, PXMODE_DIGI+u, NULL, NULL)) < 0)
		    mesg("setVideoConfig: Unit=%d Error=%d", u, r);
		if ((r = xclib[0].xcdev.setCameraConfig(&xclib[0].xcdev, 1<<u, 1, PXMODE_DIGI+u, NULL, NULL)) < 0)
		    if (r !=  PXERNOFEATURE)
			mesg("setCameraConfig: Unit=%d Error=%d", u, r);
	    }

	    //
	    // Setup for cleaner common code able to work in both
	    // ONELIB_TWOUNIT and TWOLIB_ONEUNIT modes.
	    //
	    xclibselect[0] = xclibselect[1] =
	    xclibselect[2] = xclibselect[3] = &xclib[0];
	    unitselect[0] = 0;
	    unitselect[1] = 1;
	    unitselect[2] = 2;
	    unitselect[3] = 3;
	    stateidselect[0] = PXMODE_DIGI+0;
	    stateidselect[1] = PXMODE_DIGI+1;
	    stateidselect[2] = PXMODE_DIGI+2;
	    stateidselect[3] = PXMODE_DIGI+3;
	#endif

	#if TWOLIB_ONEUNIT
	    for (u = 0; u < UNITS; u++) {
		//
		// Open
		// This example doesn't currently demonstrate FORMATFILE_LOAD
		// under the TWOLIB_ONEUNIT option.
		//
		memset(&xclib[u], 0, sizeof(xclib[u]));
		xclib[u].ddch.len = sizeof(xclib[u]);
		xclib[u].ddch.mos = XCMOS_LIBS;
		char driverparms[80];
		sprintf(driverparms, "-DM 0x%x %s", 1<<u, DRIVERPARMS);
		if ((r = xclib_open(&xclib[u], NULL, driverparms, u==0? FORMAT_UNIT0: FORMAT_UNIT1, NULL)) >= 0)
		    isopen++;
		else if (xclib[u].pxaux.faultMessageBox) {
		    xclib[u].pxaux.faultMessageBox(&xclib[u].pxaux, 0, 0x1, NULL, NULL, NULL, 0);
		    return(TRUE);
		} else {
		    mesg("xclib_open: Error=%d", r);
		    return(TRUE);
		}
		//
		// Init boards' configuration as per video states.
		//
		if ((r = xclib[u].xcdev.setVideoConfig(&xclib[u].xcdev, 0x1, 0, PXMODE_DIGI, NULL, NULL)) < 0)
		    mesg("setVideoConfig: Unit=%d Error=%d", u, r);
		if ((r = xclib[u].xcdev.setCameraConfig(&xclib[u].xcdev, 0x1, 1, PXMODE_DIGI, NULL, NULL)) < 0)
		    if (r !=  PXERNOFEATURE)
			mesg("setCameraConfig: Unit=%d Error=%d", u, r);
	    }

	    //
	    // Setup for cleaner common code able to work in both
	    // ONELIB_TWOUNIT and TWOLIB_ONEUNIT modes.
	    //
	    xclibselect[0] = &xclib[0];
	    xclibselect[1] = &xclib[1];
	    xclibselect[2] = &xclib[2];
	    xclibselect[3] = &xclib[3];
	    unitselect[0] = unitselect[1] =
	    unitselect[2] = unitselect[3] = 0;
	    stateidselect[0] = stateidselect[1] =
	    stateidselect[2] = stateidselect[3] = PXMODE_DIGI+0;
	#endif

	//
	// Set our title.
	//
	SetWindowText(hDlg, "EPIX(R) PIXCI(R) Imaging Board Example");

	//
	// Enable timer, for live video updates, checking for faults,
	// and timed display fo sequences.
	// See xclibex2.cpp for an alternate, using an Event
	// instead of a timer.
	//
	SetTimer(hDlg, 1, 5, NULL);

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
	// Partition display area for the units.
	//
	windImage[0].nw.x = windImage[0].nw.y = 0;
	windImage[0].se.x = rectImage.right+1;	     // inclusive->exclusive
	windImage[0].se.y = rectImage.bottom+1;      // inclusive->exclusive
	if (UNITS == 1) {
	    ;
	} else if (UNITS == 2) {
	    windImage[1] = windImage[0];
	    windImage[0].se.y /= 2;			// top half
	    windImage[1].nw.y = windImage[1].se.y/2;	// bottom half
	} else {
	    windImage[1] = windImage[0];
	    windImage[2] = windImage[0];
	    windImage[3] = windImage[0];
	    windImage[0].se.x /= 2;
	    windImage[0].se.y /= 2;			// upper left quadrant
	    windImage[1].nw.x = windImage[1].se.x/2;
	    windImage[1].se.y /= 2;			// upper right quadrant
	    windImage[2].se.x /= 2;
	    windImage[2].nw.y = windImage[2].se.y/2;	// lower left quadrant
	    windImage[3].nw.x = windImage[3].se.x/2;
	    windImage[3].nw.y = windImage[3].se.y/2;	// lower right quadrant
	}

	//
	// Determine displayed size.
	// We could simply fill up the hWndImage, but
	// much rather adjust the displayed image for
	// correct aspect ratio.
	//
	for (u = 0; u < UNITS; u++) {
	    pximage_s	image[PXMAX_IRBGF];
	    double  scalex, scaley, aspect;
	    if ((r = xclibselect[u]->pxlib.initPximage(&xclibselect[u]->pxlib, 1, image, PXMAX_IRBGF, PXHINTNONE, 0, stateidselect[u], 1, 0)) < 0)
		mesg("initPximage: Error=%d", r);
	    else {
		if (image[0].h.pixelwidth != 0)
		    aspect = image[0].h.pixelheight/image[0].h.pixelwidth;
		else
		    aspect = 1.0;
		scalex = (windImage[u].se.x-windImage[u].nw.x)/(double)image[0].imdim.se.x;
		scaley = (windImage[u].se.y-windImage[u].nw.y)/((double)image[0].imdim.se.y*aspect);
		scalex = min(scalex, scaley);
		windImage[u].se.x = windImage[u].nw.x + (int)(image[0].imdim.se.x * scalex);
		windImage[u].se.y = windImage[u].nw.y + (int)(image[0].imdim.se.y * scalex * aspect);
		//
		// As long as we have image dimensions, display them
		// so as to show the user that the boards are operating
		// at the desired, and perhaps different, resolution.
		//
		mesg("Unit %d: xdim=%d ydim=%d bitdepth=%d pixies=%d", u, image[0].imdim.se.x, image[0].imdim.se.y, image[0].d.u.i.bitsused, image[0].d.pixies);
	    }
	}

	//
	// If StrecthDIBits is to be used, some VGA card drivers
	// abhor horizontal dimensions which are not a multiple of 4.
	// This isn't needed for other rendering methods, but doesn't hurt.
	//
	for (u = 0; u < UNITS; u++) {
	    windImage[u].se.x = windImage[u].nw.x + ((windImage[u].se.x-windImage[u].nw.x)&~3);
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

	return(TRUE);
      }

      case WM_COMMAND:
	switch (LOWORD(wParam)) {

	  case IDSNAP:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    for (u = 0; u < UNITS; u++)
		xclibselect[u]->xcdev.setSnapBuf(&xclibselect[u]->xcdev, 1<<unitselect[u], 0, stateidselect[u],NULL,NULL,1, 1);
	    liveon = FALSE;
	    return(TRUE);

	  case IDLIVE:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    for (u = 0; u < UNITS; u++)
		xclibselect[u]->xcdev.setLiveBuf(&xclibselect[u]->xcdev, 1<<unitselect[u], 0,stateidselect[u],NULL,NULL, 1, 1);
	    liveon = TRUE;
	    return(TRUE);

	  case IDSTOP:
	    if (HIWORD(wParam) != BN_CLICKED)
		return(FALSE);
	    for (u = 0; u < UNITS; u++)
		xclibselect[u]->xcdev.setUnLive(&xclibselect[u]->xcdev, 1<<unitselect[u], 0,stateidselect[u],NULL, NULL);
	    liveon = FALSE;
	    return(TRUE);

	}
	break;

      case WM_CLOSE:
	isopen = 0;
	#if ONELIB_TWOUNIT
	    xclib_close(&xclib[0]);
	#endif
	#if TWOLIB_ONEUNIT
	    for (u = 0; u < UNITS; u++)
		xclib_close(&xclib[u]);
	#endif
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

	DestroyWindow(hWnd);
	EndDialog(hDlg, 0);
	return(TRUE);

      case WM_TIMER:
	//
	// Monitor for asynchronous faults, such as video
	// being disconnected while capturing. These faults
	// can't be reported by functions such as pxd_goLive()
	// which initiate capture and return immediately.
	//
	// Should there be a fault pxd_mesgFault() pop up a dialog,
	// the Windows TIMER will continue in a new thread. Thus the
	// 'faulting' variable and logic to limit to one dialog at a time.
	//
	if (isopen) {	// implies whether library is open
	    static int faulting = 0;
	    if (!faulting) {
		faulting++;
		#if ONELIB_TWOUNIT
		    xclib[0].pxaux.faultMessageBox(&xclib[0].pxaux, 0, (1<<UNITS)-1, NULL, NULL, NULL, 0);
		#endif
		#if TWOLIB_ONEUNIT
		    for (u = 0; u < UNITS; u++)
			xclib[u].pxaux.faultMessageBox(&xclib[u].pxaux, 0, 0x1, NULL, NULL, NULL, 0);
		#endif
		faulting--;
	    }
	}

	//
	// Has a new field or frame been captured
	// since the last time we checked?
	//
	pxbuffer_t  buf = 1;
	for (u = 0; u < UNITS; u++) {
	    pxvbtime_t lasttime = xclibselect[u]->xcdev.getLiveStatus(&xclibselect[u]->xcdev, 1<<unitselect[u], 0, PXVIST_DONE | PXVIST_VCNT);
	    if (lastcapttime[u] == lasttime)
		continue;
	    lastcapttime[u] = lasttime;
	    DisplayBuffer(xclibselect[u], stateidselect[u], unitselect[u], buf, hWndImage, windImage[u]);
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
			"XCLIBEX5 Windows Example",
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
    hDlg = CreateDialogParam(hInstance, "PIXCIDIALOG", NULL, (DLGPROC)PIXCIDialogProc, NULL);
    if (!hDlg) {
	MessageBox(NULL, "Missing Dialog Resource - Compilation or Link Error!", "XCLIBEX5", MB_OK|MB_TASKMODAL);
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
