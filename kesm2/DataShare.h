#ifndef _DATA_SHARE_H_
#define _DATA_SHARE_H_

// SC: Stage Controller
// DC: Data Capture
// DD: Data Dispatcher

#define WM_APP_BSS				(WM_APP+100)

#define WP_START_CAPTURE		(1)	 // SC --> DC
#define WP_STOP_CAPTURE			(2)   // SC --> DC

#define WP_DD_LAUNCH_DONE		(10)  // DD --> SC
#define WP_DC_LAUNCH_DONE		(11)  // DC --> SC

#define WP_DC_SAVE_DONE			(20)	 // DC --> DD & SC
#define WP_DC_SAVE_ERROR		(21)  // DC --> SC
//#define WP_NOW_CAPTURING		(21)  // DC --> SC

#define WA_DD_SEND_DONE			(30)  // DD --> SC

// local server test
#define WA_DD_SEND_START		(40)  // DD --> DS

#define BSS_STAGECTLR_TITLE		"Stage Controller - Brain Network Lab @ Texas A&M"
#define BSS_DDISPATCHER_TITLE	"Data Dispatcher - Brain Network Lab @ Texas A&M"
#define BSS_DCAPTURE_TITLE		"Data Capture - Brain Network Lab @ Texas A&M"
#define BSS_DSERVER_TITLE		"Data Server - Brain Network Lab @ Texas A&M"

#define BSS_RELEASE_APP

#ifdef BSS_RELEASE_APP

#define BSS_PATH_DATA_DISPATCHER	"DataDispatcher.exe"
//#define BSS_PATH_IMAGE_CAPTURE      "ImageCapture.exe"
// jwyoo, hwang, 2-25-2016 
#define BSS_PATH_IMAGE_CAPTURE      "..\\GrabDemo\\GrabDemo.exe"

#else

#define BSS_PATH_DATA_DISPATCHER    "..\\DataDispatcher\\Debug\\DataDispatcher.exe"
#define BSS_PATH_IMAGE_CAPTURE      "..\\ImageCapture\\Debug\\ImageCapture.exe"

#endif

#endif /* _DATA_SHARE_H_ */