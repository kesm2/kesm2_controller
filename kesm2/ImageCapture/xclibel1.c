/*
 *
 *	xclibel1.c	External	26-Oct-2003
 *
 *	Copyright (C)  2002-2003  EPIX, Inc.  All rights reserved.
 *
 *	Example program for XCLIB C Library, SCF Level functions.
 *	Example assumes Linux 'command line' environment.
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
 *
 *	For PIXCI(R) D, A, D24, D32, D2X, D3X, CL1, CL2, E1, E4, E1DB, CL3SD, and SI imaging boards,
 *	use "default" to select the default format for the camera
 *	for which the PIXCI(R) imaging board is intended.
 *	For non default formats, use XCAP to save the video set-up to a
 *	file, and set FORMAT to the saved file's name.
 *	For camera's with RS-232 control, note that the saved
 *	video set-up only resets the PIXCI(R) imaging board's
 *	settings, but XCLIB does not reset the camera's settings.
 *
 *	Alternately, this could be modified to use any
 *	other convention chosen by the programmer to allow
 *	run time selection of the video format and resolution.
 *
 */

				// For PIXCI(R) SV2, SV3, SV4, SV5, SV6
//#define FORMAT  "RS-170"	// RS-170 on input 2
//#define FORMAT  "NTSC"	// NTSC on input 2
//#define FORMAT  "NTSC/YC"	// NSTC S-Video on input 1
//#define FORMAT  "CCIR"	// CCIR on input 2
//#define FORMAT  "PAL" 	// PAL (B,D,G,H,I) on input 2
//#define FORMAT  "PAL/YC"	// PAL (B,D,G,H,I) S-Video on input 1
  #define FORMAT  "default"	// NSTC S-Video on input 1

				// For PIXCI(R) D, D24, D32, A, D2X, D3X, CL1, CL2, E1, E4, E1DB, CL3SD, SI
//#define FORMAT  "default"	// as per board's intended camera

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
 *  Note: Under Linux, the image frame buffer memory can't be set as
 *  a run time option. It MUST be set via insmod so the memory can
 *  be reserved during Linux's initialization.
 */
//#define DRIVERPARMS "-QU 0"	    // don't use interrupts
//#define DRIVERPARMS "-IM 8192"    // request 8192 mbyte of frame buffer memory
  #define DRIVERPARMS ""	    // default

/*
 *  3)	Choose whether the optional PXIPL Image Processing Library
 *	is available.
 *
 */
#define USE_PXIPL   0


/*
 *  4a) Compile with GCC w/out PXIPL as:
 *
 *	    gcc -DC_GNU32=296 -DOS_LINUX_GNU xclibel1.c xclib_i386.a -lm
 *	    gcc -DC_GNU64=400 -DOS_LINUX_GNU xclibel1.c xclib_i386.a -lm
 *
 *  Compile with GCC with PXIPL as:
 *
 *	    gcc -DC_GNU32=296 -DOS_LINUX_GNU xclibel1.c pxipl_i386.a xclib_i386.a -lm
 *	    gcc -DC_GNU64=400 -DOS_LINUX_GNU xclibel1.c pxipl_i386.a xclib_i386.a -lm
 *
 *	Run as:
 *	    ./a.out
 *
 */


/*
 *  NECESSARY INCLUDES:
 */
#include <stdio.h>		// c library
#include <signal.h>		// c library
#include <stdlib.h>		// c library
#include <stdarg.h>		// c library
#include "xcliball.h"		// function prototypes
#if USE_PXIPL
  #include "pxipl.h"		// function prototypes
#endif


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
void sigintfunc(int sig)
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
    while (getchar() != '\n') ;
    printf("\n");
}

/*
 * Video 'interrupt' callback function.
 */
int fieldirqcount = 0;
void videoirqfunc(int sig)
{
    fieldirqcount++;
}

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
	signal(SIGUSR1, videoirqfunc);
	pxd_eventCapturedFieldCreate(1, SIGUSR1, NULL);

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
    printf("Snap: %s\n", r>=0? "Ok": pxerrnomesg(r));
    printf("Field count after  snap=%d\n", pxd_videoFieldCount(1));
    printf("Captured field count from IRQ hook=%d\n", fieldirqcount);
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
    // in the the image buffer into the PC buffer in
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
    // Display data from the the PC buffer.
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
 * This feature is available under Linux as well.
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
 * Display image as text characters.
 *
 * Not expected to produce a nice looking image,
 * but the best that can be done with a text mode display.
 * Nor does it bother to try to do so efficiently; normally
 * the pxd_readuchar() would be called once for the entire
 * image, or perhaps once per line.
 */
void do_display()
{
    uchar   charcodes[] = { ' ', '.', ':', '-',
			    '+', '*', 'e', 'm',
			    'F', 'H', 'B', 'M',
			  /* 0xB0, 0xB1, 0xB2, 0xDB */ };
    int     dx, dy;
    #define DISPX   72
    #define DISPY   20

    for (dy = 0; dy < DISPY; dy++) {
	for (dx = 0; dx < DISPX; dx++) {
	    uchar   value[1];
	    int     ix = (dx*pxd_imageXdim())/DISPX;
	    int     iy = (dy*pxd_imageYdim())/DISPY;
	    pxd_readuchar(0x1, 1, ix, iy, ix+1, iy+1, value, 1, "Grey");
	    value[0] = (value[0]*sizeof(charcodes)-1)/255;
	    printf("%c", charcodes[value[0]]);
	}
	printf("\n");
    }
    user("Image buffer 'displayed'");
}


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
    printf("XCLIBEL1.C - Example program\n");
    printf("Copyright (C)  2004  EPIX, Inc.  All rights reserved.\n");
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
    signal(SIGINT, sigintfunc);
    signal(SIGFPE, sigintfunc);

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
	color_display1();
	color_display2();
	color_display3();
	color_display4();
	color_display5();
	color_display6();
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
    do_display();

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
