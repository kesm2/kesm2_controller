/*
 *
 *	xclibex1.c	External	23-Nov-2004
 *
 *	Copyright (C)  1996-2004  EPIX, Inc.  All rights reserved.
 *
 *	Example program for XCLIB C Library, SCF Level functions.
 *	Assumes DOS or Windows 'command line' environment.
 *
 */


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
 *	For PIXCI(R) D, A, D24, D32, D2X, D3X, CL1, CL2, E1, CL3SD, and SI
 *	imaging boards, use "default" to select the default format for
 *	the camera for which the PIXCI(R) imaging board is intended.
 *	For non default formats, use XCAP to save the video set-up to a
 *	file, and set FORMAT to the saved file's name.
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

				// For PIXCI(R) D, D24, D32, A, D2X, D3X, CL1, CL2, E1, CL3SD, SI
  #define FORMAT  "default"	// as per board's intended camera

					// For any PIXCI(R) imaging board
//#define FORMATFILE  "xcvidset.fmt"	// using format file saved by XCAP


/*
 *  2.1) Set number of expected PIXCI(R) image boards.
 *  The XCLIB Simple 'C' Functions expect that the boards are
 *  the same model and operated at the same resolution.
 *  This example expects only one unit.
 */
#define UNITS	    1
#define UNITSMAP    ((1<<UNITS)-1)  // shorthand - bitmap of all units

/*
 *  2.2) Optionally, set driver configuration parameters.
 *  These are normally left to the default, "".
 *  The actual driver configuration parameters include the
 *  desired PIXCI(R) imaging boards, but to make configuation easier,
 *  code, below, will automatically add board selection to this.
 *
 *  Note: Under Windows, the image frame buffer memory can't be set as
 *  a run time option. It MUST be set via SYSTEM.INI for Win 95/98,
 *  or via the Registry for Win NT, so the memory can be reserved
 *  during Window's initialization.
 */
//#define DRIVERPARMS "-QU 0"	    // don't use interrupts
//#define DRIVERPARMS "-IM 8192"    // request 8192 mbyte of frame buffer memory
  #define DRIVERPARMS ""	    // default

/*
 *  3)	Choose whether the PXIPL Image Processing Library
 *	is available, with which images can be displayed on the S/VGA.
 *	(Image display on S/VGA is available under Windows with or
 *	without the PXIPL library, but not shown in this command-line
 *	oriented example).
 *
 */
#define USE_PXIPL   0


/*
 *  4a) Compile with Watcom C/C++ V10.0 for Tenberry/Rational DOS as:
 *
 *	    wcl386 -mf xclibex1.c -k8k XCLBW0DF.lib		    (w/out PXIPL)
 *	    wcl386 -mf xclibex1.c -k8k XCLBW0DF.lib PXPLW0DF.LIB    (w. PXIPL)
 *	    4gwbind 4gwpro.exe xclibex1.exe app.exe -N
 *
 *	Note: Don't use the -Za option to enforce ANSI compatibility;
 *	it also disables several predefined identifiers which allow
 *	XCLIB include'd macros to identify the Watcom compiler.
 *
 *	Run as:
 *	    app
 *
 *  4b) Compile with Microsoft Visual C/C++ V6.0 for Windows 95/98/ME as:
 *	    cl xclibex1.c -link  XCLIBW95.LIB
 *
 *  4c) Compile with Microsoft Visual C/C++ V6.0 for Windows NT/2000/XP as:
 *	    cl xclibex1.c -link  XCLIBWNT.LIB
 *
 */


/*
 *  NECESSARY INCLUDES:
 */
#include <stdio.h>		// c library
#include <signal.h>		// c library
#include <stdlib.h>		// c library
#include <stdarg.h>		// c library
#if defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#if USE_PXIPL
#include "pxipl.h"         
#include "pxextras.h"           
#endif
#include "xcliball.h"		// function prototypes


/*
 *  SUPPORT STUFF:
 *
 *  Catch CTRL+C and floating point exceptions so that once
 *  opened, the PIXCI(R) library is always closed before exit.
 *  In some environments, this isn't critical; the operating system
 *  advises the PIXCI(R) driver that the program has terminated.
 *  In other environments, the operating system isn't as helpful
 *  and the driver would be left open.
 */
void signaled(int sig)
{
    /*
     * Some languages/environments don't allow
     * use of printf from a signal catcher.
     * Printing the message isn't important anyhow.
     *
    if (sig == SIGINT)
	printf("Break\n");
    if (sig == SIGFPE)
	printf("Float\n");
    */

    pxd_PIXCIclose();
    exit(1);
}

/*
 *  SUPPORT STUFF:
 *
 *  Slow down execution speed so
 *  each step/function can be observed.
 */
void user(char *mesg)
{
    if (mesg && *mesg)
	printf("%s\n", mesg);
    printf("\n\nContinue (Key ENTER) ?");
    while (fgetchar() != '\n') ;
    printf("\n");
}

/*
 * Call back function, for demonstrating interrupt hooks.
 * This one function must always have stack checking disabled.
 * Since Watcom doesn't provide a pragma to restore stack checking
 * as per command line options, the '#pragma on (check_stack)'
 * is shown, but may be removed as per the programmer's
 * intent for the remainder of the program.
 */
#if !(defined(_WIN32) || defined(__WIN32__))

#pragma off (check_stack)
int fieldirqcount = 0;
int _cfunfcc irqfunction(void *p, int a, int b)
{
    fieldirqcount++;
    return(0);
}
#pragma on (check_stack)    // optional

#endif


/*
 * Open the XCLIB C Library for use.
 */
int do_open(void)
{
    int i;

    printf("Opening EPIX(R) PIXCI(R) Imaging Board,\n");
    printf("using configuration parameters '%s',\n", DRIVERPARMS? DRIVERPARMS: "default");

    //
    // Either FORMAT or FORMATFILE should have been
    // selected above.
    //
    #if defined(FORMAT)
	printf("and using predefined format '%s'.\n\n", FORMAT);
	i = pxd_PIXCIopen(DRIVERPARMS, FORMAT, "");
    #endif
    #if defined(FORMATFILE)
	printf("and using format file '%s'.\n\n", FORMATFILE);
	i = pxd_PIXCIopen(DRIVERPARMS, "", FORMATFILE);
    #endif
    if (i >= 0) {
	//
	// For the sake of demonstrating optional interrupt hooks.
	//
	#if !(defined(_WIN32) || defined(__WIN32__))
	    pxd_eventCapturedFieldCreate(1, irqfunction, NULL);
	#endif

	user("Open OK");
    } else {
	printf("Open Error %d\a\a\n", i);
	pxd_mesgFault(UNITSMAP);
	user("");
    }
    return(i);
}

/*
 * Report image frame buffer memory size
 */
void do_imsize(void)
{
    printf("Image frame buffer memory size: %lu Kbytes\n", pxd_infoMemsize(UNITSMAP)/1024);
    printf("Image frame buffers           : %d\n", pxd_imageZdim());
    printf("Number of boards              : %d\n", pxd_infoUnits());
    user("");
}

/*
 * Report image resolution.
 */
void do_vidsize(void)
{
    printf("Image resolution:\n");
    printf("xdim           = %d\n", pxd_imageXdim());
    printf("ydim           = %d\n", pxd_imageYdim());
    printf("colors         = %d\n", pxd_imageCdim());
    printf("bits per pixel = %d\n", pxd_imageCdim()*pxd_imageBdim());
    user("");
}


/*
 * Capture
 */
void do_video1(void)
{
    int r;
    //
    // Capture image into buffer 1.
    //
    printf("Field count before snap=%d\n", pxd_videoFieldCount(1));
    r = pxd_doSnap(UNITSMAP, 1, 0);
    printf("Snap: %s\n", r>=0? "Ok": pxd_mesgErrorCode(r));
    printf("Field count after  snap=%d\n", pxd_videoFieldCount(1));
    #if !(defined(_WIN32) || defined(__WIN32__))
	printf("Captured field count from IRQ hook=%d\n", fieldirqcount);
    #endif
    //
    // Check for faults, such as erratic sync
    // or insufficient PCI bus bandwidth
    //
    pxd_mesgFault(UNITSMAP);
    user("Image snapped into buffer 1");
}

/*
 * General purpose output will pulse, running for several seconds.
 */
void do_extout1(void)
{
    pxd_getGPIn(UNITSMAP, 0);
    user("Ready to watch general purpose output signal?");
}
void do_extout2(void)
{
    int     j;

    printf("Toggling general purpose output ..\n");
    for (j = 60*3; j--; ) {
	pxd_getGPIn(UNITSMAP, 0x4);
	pxd_getGPIn(UNITSMAP, 0x0);
    }
    user("general purpose output pulses done");
}

/*
 * The general purpose input is often used as a flag to start an
 * activity, with the application code looping until
 * the general purpose input changes.
 * No signal may be present during this demo, so as to avoid
 * indefinite loops the count is simply printed twice,
 * giving the user an opportunity to toggle the input.
 * This (silently) sets the GPOut, to provide a convenient
 * signal source for testing.
 */
void do_extin1(void)
{
    //
    // On PIXCI(R) D, D24, D32 imaging boards the input, if available,
    // is latched, until explicitly reset.
    // This has no effect on other PIXCI(R) imaging boards
    // in which the inputs are level sensitive.
    //
    pxd_setGPIn(UNITSMAP, 0x0);

    pxd_setGPOut(UNITSMAP, 0x0);
    printf("Current value of general purpose input: 0x%x\n", pxd_getGPIn(UNITSMAP, 0));
    user("");
}
void do_extin2(void)
{
    pxd_setGPIn(UNITSMAP, 0x0);
    pxd_setGPOut(UNITSMAP, 0x1);
    printf("Current value of general purpose input: 0x%x\n", pxd_getGPIn(UNITSMAP, 0));
    user("");
}

/*
 * Pixels are transferred to a PC buffer, and numerically displayed.
 */
#define AOI_XDIM    3
#define AOI_YDIM    10
#define COLORS	    3
uchar	colorimage_buf[AOI_XDIM*AOI_YDIM*COLORS];

void color_display1(void)
{
    int     i;
    //
    // Transfer the color data from a selected AOI in
    // the image buffer into the PC buffer in
    // YCrCb (Intensity, Red Chroma and Blue Chroma)
    // format.	The number of bytes to be transfered
    // is 3 times the number of pixels.
    //
    if (pxd_imageBdim() > 8) {
	printf("YCrCb not available for %d bit pixel component values", pxd_imageBdim());
	user("");
	return;
    }
    if ((i = pxd_readuchar(UNITSMAP, 1, 30, 40, 30+AOI_XDIM, 40+AOI_YDIM, colorimage_buf, AOI_XDIM*AOI_YDIM*3, "YCRCB")) != AOI_XDIM*AOI_YDIM*3) {
	printf("pxd_readuchar error = %d != %d\n", i, AOI_XDIM*AOI_YDIM*3);
	user("");
	return;
    }
    user("Image area of interest transfered for YCrCb");
}
void color_display2(void)
{
    int     x, y;
    //
    // Display data from the PC buffer.
    //
    if (pxd_imageBdim() > 8) {
	printf("YCrCb not available for %d bit pixel component values", pxd_imageBdim());
	user("");
	return;
    }
    for (y = 0; y < AOI_YDIM; y++) {
	for (x = 0; x < AOI_XDIM; x++) {
	    printf(" Y=%3d  ", colorimage_buf[(y*AOI_XDIM+x)*3+0]);
	    printf("Cr=%3d  ", colorimage_buf[(y*AOI_XDIM+x)*3+1]);
	    printf("Cb=%3d  ", colorimage_buf[(y*AOI_XDIM+x)*3+2]);
	}
	printf("\n");
    }
    user("");
}
void color_display3(void)
{
    int     i;
    //
    // Transfer the color data from a selected AOI
    // in the image buffer into the PC buffer in
    // RGB format.
    //
    if ((i = pxd_readuchar(UNITSMAP, 1, 30, 40, 30+AOI_XDIM, 40+AOI_YDIM, colorimage_buf, AOI_XDIM*AOI_YDIM*3, "RGB")) != AOI_XDIM*AOI_YDIM*3) {
	printf("pxd_readuchar error = %d != %d\n", i, AOI_XDIM*AOI_YDIM);
	user("");
	return;
    }
    user("Image area of interest transfered for RGB");
}
void color_display4(void)
{
    int     x, y;
    //
    // Display data from the PC buffer.
    //
    for (y = 0; y < AOI_YDIM; y++) {
	for (x = 0; x < AOI_XDIM; x++) {
	    printf("R=%3d ", colorimage_buf[(y*AOI_XDIM+x)*3+0]);
	    printf("G=%3d ", colorimage_buf[(y*AOI_XDIM+x)*3+1]);
	    printf("B=%3d ", colorimage_buf[(y*AOI_XDIM+x)*3+2]);
	}
	printf("\n");
    }
    user("");
}
void color_display5(void)
{
    int     i;
    //
    // Transfer the color data from a selected AOI
    // in the image buffer into the PC buffer in
    // BSH format.
    //
    if ((i = pxd_readuchar(UNITSMAP, 1, 30, 40, 30+AOI_XDIM, 40+AOI_YDIM, colorimage_buf, AOI_XDIM*AOI_YDIM*3, "BSH")) != AOI_XDIM*AOI_YDIM*3) {
	printf("pxd_readuchar error = %d != %d\n", i, AOI_XDIM*AOI_YDIM*3);
	user("");
	return;
    }
    user("Image area of interest transfered for BSH");
}
void color_display6(void)
{
    int     x, y;
    //
    // Display data from the PC buffer.
    // The HSB values, ranging from 0 to 255, are rescaled
    // as displayed into the more typical 0 to 1 for S and B,
    // and 0 to 360 for H.
    //
    for (y = 0; y < AOI_YDIM; y++) {
	for (x = 0; x < AOI_XDIM; x++) {
	    printf(" B=%1.3f ", colorimage_buf[(y*AOI_XDIM+x)*3+0]/255.);
	    printf(" S=%1.3f ", colorimage_buf[(y*AOI_XDIM+x)*3+1]/255.);
	    printf(" H=%3.1f ", colorimage_buf[(y*AOI_XDIM+x)*3+2]*360./256.);
	}
	printf("\n");
    }
    user("");
}

void color_display7(int component)
{
    int     i;
    char    *colorspace = "";
    //
    // Transfer one component of the color data from a selected AOI
    // in the image buffer into the PC buffer in RGB or BSH format.
    //
    switch (component) {
      case 0:	colorspace = "RofRGB";	break;
      case 1:	colorspace = "GofRGB";	break;
      case 2:	colorspace = "BofRGB";	break;
      case 3:	colorspace = "BofBSH";	break;
      case 4:	colorspace = "SofBSH";	break;
      case 5:	colorspace = "HofBSH";	break;
    }
    if ((i = pxd_readuchar(UNITSMAP, 1, 30, 40, 30+AOI_XDIM, 40+AOI_YDIM, colorimage_buf, AOI_XDIM*AOI_YDIM, colorspace)) != AOI_XDIM*AOI_YDIM) {
	printf("pxd_readuchar error = %d != %d\n", i, AOI_XDIM*AOI_YDIM);
	user("");
	return;
    }
    user("Image area of interest transfered for one color component");
}

void color_display8(int component)
{
    int     x, y;
    //
    // Display data from the PC buffer.
    // The HSB values, ranging from 0 to 255, are rescaled
    // as displayed into the more typical 0 to 1 for S and B,
    // and 0 to 360 for H.
    //
    for (y = 0; y < AOI_YDIM; y++) {
	for (x = 0; x < AOI_XDIM; x++) {
	    switch (component) {
	      case 0:
		printf("R=%3d ", colorimage_buf[(y*AOI_XDIM+x)*1+0]);
		break;
	      case 1:
		printf("G=%3d ", colorimage_buf[(y*AOI_XDIM+x)*1+0]);
		break;
	      case 2:
		printf("B=%3d ", colorimage_buf[(y*AOI_XDIM+x)*1+0]);
		break;
	      case 3:
		printf(" B=%1.3f ", colorimage_buf[(y*AOI_XDIM+x)*1+0]/255.);
		break;
	      case 4:
		printf(" S=%1.3f ", colorimage_buf[(y*AOI_XDIM+x)*1+0]/255.);
		break;
	      case 5:
		printf(" H=%3.1f ", colorimage_buf[(y*AOI_XDIM+x)*1+0]*360./256.);
		break;
	    }
	}
	printf("\n");
    }
    user("");
}

#undef	AOI_XDIM
#undef	AOI_YDIM


/*
 * Pixels are transferred to a PC buffer, and numerically displayed.
 */
#define AOI_XDIM    9
#define AOI_YDIM    10
uchar	monoimage_buf8[AOI_XDIM*AOI_YDIM];
ushort	monoimage_buf16[AOI_XDIM*AOI_YDIM];

void bw_display1(void)
{
    int     i;
    //
    // Transfer the monochrome data from a selected AOI in
    // the image buffer into the PC buffer, as 8 bit pixels.
    // Or,
    // Transfer the monochrome data from a selected AOI in
    // the image buffer into the PC buffer, as 8 to 16 bit pixels.
    //
    // The ushort array could be used for both for 8 bit pixels, but
    // users of 8 bit pixels commonly assume pixel<=>byte,
    // and is more efficient.
    //
    if (pxd_imageBdim() <= 8) {
	if ((i = pxd_readuchar(UNITSMAP, 1, 30, 40, 30+AOI_XDIM, 40+AOI_YDIM, monoimage_buf8, AOI_XDIM*AOI_YDIM, "Grey")) != AOI_XDIM*AOI_YDIM) {
	    printf("pxd_readuchar error: %d != %d\n", i, AOI_XDIM*AOI_YDIM);
	    user("");
	    return;
	}
    } else {
	if (i = pxd_readushort(UNITSMAP, 1, 30, 40, 30+AOI_XDIM, 40+AOI_YDIM, monoimage_buf16, AOI_XDIM*AOI_YDIM, "Grey") != AOI_XDIM*AOI_YDIM) {
	    printf("pxd_readushort error: %d != %d\n", i, AOI_XDIM*AOI_YDIM);
	    user("");
	    return;
	}
    }
    user("Image area of interest transfered");
}
void bw_display2(void)
{
    int     x, y;
    //
    // Display data from the PC buffer.
    //
    for (y = 0; y < AOI_YDIM; y++) {
	printf("I = ");
	for (x = 0; x < AOI_XDIM; x++)
	    if (pxd_imageBdim() <= 8)
		printf("%4d ", monoimage_buf8[y*AOI_XDIM+x]);
	    else
		printf("%4d ", monoimage_buf16[y*AOI_XDIM+x]);
	printf("\n");
    }
    user("");
}


/*
 * Be nice and careful: For sake of this example which uses a
 * file name not selected by the user, and which might already exist,
 * don't overwrite the file if the file already exists.
 * This check for a pre-existant file is the only reason
 * that fopen() need be done; this section of code
 * isn't normally needed.
 */
int checkexist(char *name)
{
    FILE    *fp;
    if ((fp = fopen(name, "rb")) != NULL) {
	fclose(fp);
	printf("Image not saved, file %s already exists\n", name);
	user("");
	return(1);
    }
    return(0);
}

/*
 * Save image in industry standard .tif format.
 */
void do_savetif()
{
    int     i;
    char    *name = "image.tif";

    //
    // Don't overwrite existing file.
    //
    if (checkexist(name))
	return;

    //
    // Do save of entire image to disk in TIFF format.
    //
    printf("Image buffer 1 being saved to file %s\n", name);
    if ((i = pxd_saveTiff(UNITSMAP, name, 1, 0, 0, -1, -1, 0, 0)) < 0) {
	printf("error in pxd_saveTiff= %s\n", pxd_mesgErrorCode(i));
	user("");
	return;
    }
    user("Image buffer saved");
}



/*
 * Save image in the Windows .bmp format.
 * This feature is available under DOS as well.
 */
void do_savebmp()
{
    int     i;
    char    *name = "image.bmp";

    //
    // Don't overwrite existing file.
    //
    if (checkexist(name))
	return;

    //
    // Do save of entire image to disk in Bitmap format.
    // Monochrome image buffers are saved as an 8 bit monochrome image,
    // color image buffers are saved as an 24 bit RGB color image.
    //
    printf("Image buffer 1 being saved to file %s\n", name);
    if ((i = pxd_saveBmp(UNITSMAP, name, 1, 0, 0, -1, -1, 0, 0)) < 0) {
	printf("error in pxd_saveBmp= %s\n", pxd_mesgErrorCode(i));
	user("");
	return;
    }
    user("Image buffer saved");
}

/*
 * Save image in the JPEG format.
 * This requires the PXIPL library.
 */
void do_savejpeg()
{
    #if USE_PXIPL
	int	i;
	char	*name = "image.jpg";

	//
	// Don't overwrite existing file.
	//
	if (checkexist(name))
	    return;

	//
	// Do save of entire image to disk in JPEG format.
	//
	printf("Image buffer 1 being saved to file %s\n", name);
	if ((i = pxio8_jpegwrite(NULL, pxd_defineImage(1,1,0,0,-1,-1,"RGB"), NULL, name, 8, NULL)) < 0) {
	    printf("error in pxio8_jpegwrite= %s\n", pxd_mesgErrorCode(i));
	    user("");
	    return;
	}
	user("Image buffer saved");
    #endif
}

/*
 * Display image on the S/VGA, using the PXIPL Library.
 * (Image display on S/VGA is available under Windows with or
 * without the PXIPL library, but not shown in this command-line
 * oriented example).
 */
#if !(defined(_WIN32) || defined(__WIN32__))
void do_vgadisplay()
{
  #if USE_PXIPL

    struct  pximage vga;

    //
    // Put S/VGA in graphics mode. The vga_open supports high resolution,
    // but it can't detect which monitor is connected. Use a (monitor) safe
    // 640x480 resolution.
    //
    vga_open(0, 640, 480, 24, 1, &vga, NULL);

    //
    // One way to check success is whether the `vga' access
    // has a nonzero dimension, or non zero number of pixies.
    //
    if (!vga.d.pixies) {
	user("Can't put S/VGA into graphics mode");
	return;
    }

    //
    // Show image buffer 1 in center 1/4 of screen.
    // Selecting 7 bits in certain modes reflects how vga_open()
    // optionally set up the vga palette, for 8 bit per pixel VGA modes.
    //
    vga.wind.nw.x = vga.wind.se.x/4;
    vga.wind.nw.y = vga.wind.se.y/4;
    vga.wind.se.x = 3*vga.wind.se.x/4;
    vga.wind.se.y = 3*vga.wind.se.y/4;
    (vga.xwind)(&vga, &vga.wind, 's');
    pxio8_vgadisplay(NULL, pxd_defineImage(UNITSMAP, 1, 0, 0, -1, -1, "Display"), NULL,
	    (vga.d.pixies == 1 && vga.d.u.i.bitsused == 8)? 7: vga.d.u.i.bitsused,
	    'n', 1, 0, &vga, NULL, NULL);

    //
    // Show message.
    // Assume worst case, that S/VGA BIOS doesn't support printf in this mode.
    //
    {
	struct pxy xy	     = {0, 0};
	uint   forecolor[3]  = {255, 255, 0};
	uint   backcolor[3]  = {0, 0, 0};

	(vga.xwind)(&vga, NULL, 's');	// set full window
	pxip8_drawchars(NULL, &vga, &xy,
			"S/VGA graphics mode set, and image displayed.",
			45, 8, 8, 0, 1, 0, backcolor, forecolor, 0);
	xy.y += 10;
	pxip8_drawchars(NULL, &vga, &xy, "Continue (Key ENTER) ?",
			22, 8, 8, 0, 1, 0, backcolor, forecolor, 0);
    }

    //
    // Wait for user
    //
    user("");

    //
    // Reset S/VGA so the remainder of this example,
    // which uses printf(), can proceed.
    //
    vga_close(&vga, NULL);

  #endif
}
#endif


/*
 * Close the PIXCI(R) imaging board
 */
void do_close(void)
{
    pxd_PIXCIclose();
    user("PIXCI(R) imaging board closed");
}


void hello(void)
{
    printf("\n\n");
    printf("PIXCI(R) Imaging Boards -  XCLIB 'C' Library\n");
    printf("XCLIBEX1.C - Example program\n");
    printf("Copyright (C)  1996-2004  EPIX, Inc.  All rights reserved.\n");
    printf("\n");
    printf("This program is best used by executing this program\n");
    printf("one step at a time, while simultaneously reading\n");
    printf("the XCLIB documentation and program listing.\n");
    user("");
}

/*
 *  MAIN:
 *
 *  Each library function is demonstrated in its own subroutine,
 *  the main calls each subroutine to produce the interactive demonstration.
 *
 *  It is suggested that this program source be read at the same time
 *  as the compiled program is executed.
 *
 */
main(void)
{
    //
    // Say Hello
    //
    hello();

    //
    // Catch signals.
    //
    signal(SIGINT, signaled);
    signal(SIGFPE, signaled);

    //
    // Open and set video format.
    //
    if (do_open() < 0)
	return(1);

    //
    // Basic video operations
    //
    do_imsize();
    do_vidsize();
    do_video1();
    do_video1();

    //
    // Show pixel values
    //
    if (pxd_imageCdim() == 1) {
	bw_display1();
	bw_display2();
    } else {
	int i;
	color_display1();
	color_display2();
	color_display3();
	color_display4();
	color_display5();
	color_display6();
	for (i = 0; i < 6; i++) {
	    color_display7(i);
	    color_display8(i);
	}
    }

    //
    // Save image
    //
    do_savetif();
    do_savebmp();
    #if USE_PXIPL
	do_savejpeg();
    #endif

    //
    // Display image.
    //
    #if !(defined(_WIN32) || defined(__WIN32__))
	do_vgadisplay();
    #endif

    //
    // Do General Purpose Input/Output signals
    //
    do_extin1();
    do_extin2();
    do_extout1();
    do_extout2();

    //
    // All done
    //
    do_close();
    return(0);
}
