; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSimImageCaptureDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SimImageCapture.h"

ClassCount=3
Class1=CSimImageCaptureApp
Class2=CSimImageCaptureDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SIMIMAGECAPTURE_DIALOG
Resource4=IDD_ABOUTBOX (English (U.S.))
Resource5=IDD_SIMIMAGECAPTURE_DIALOG (English (U.S.))

[CLS:CSimImageCaptureApp]
Type=0
HeaderFile=SimImageCapture.h
ImplementationFile=SimImageCapture.cpp
Filter=N

[CLS:CSimImageCaptureDlg]
Type=0
HeaderFile=SimImageCaptureDlg.h
ImplementationFile=SimImageCaptureDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LST_LOG

[CLS:CAboutDlg]
Type=0
HeaderFile=SimImageCaptureDlg.h
ImplementationFile=SimImageCaptureDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_SIMIMAGECAPTURE_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CSimImageCaptureDlg

[DLG:IDD_SIMIMAGECAPTURE_DIALOG (English (U.S.))]
Type=1
Class=CSimImageCaptureDlg
ControlCount=2
Control1=IDC_BTN_START_STOP,button,1342242816
Control2=IDC_LST_LOG,listbox,1352728835

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

