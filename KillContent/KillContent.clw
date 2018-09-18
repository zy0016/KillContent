; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAboutDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "killcontent.h"
LastPage=0

ClassCount=13
Class1=CFileAddEdit
Class2=CFileDialogEx
Class3=CKC_File
Class4=CKC_Fold
Class5=CKillContentApp
Class6=CAboutDlg
Class7=CKillContentDlg
Class8=CKillListCtrl
Class9=CKillOption
Class10=CKillRichEditCtrl
Class11=COperateKillNote
Class12=CPreviewCode

ResourceCount=20
Resource1=IDD_DIALOG_KILLNOTE
Resource2=IDD_DIALOG_FIND
Resource3=IDD_DIALOG_OPTION
Resource4=IDD_DIALOG_FOLD
Resource5=IDD_DIALOG_FILE
Resource6=IDD_DIALOG_PREVIEW
Resource7=IDD_DIALOG_ADD_EDITFILE
Resource8=IDD_KILLCONTENT_DIALOG
Resource9=IDD_ABOUTBOX
Class13=CKFind
Resource10=IDR_TOOLBAR_PREVIEW
Resource11=IDD_KILLCONTENT_DIALOG (Chinese (P.R.C.))
Resource12=IDD_DIALOG_KILLNOTE (Chinese (P.R.C.))
Resource13=IDD_ABOUTBOX (Chinese (P.R.C.))
Resource14=IDD_DIALOG_OPTION (Chinese (P.R.C.))
Resource15=IDD_DIALOG_PREVIEW (Chinese (P.R.C.))
Resource16=IDD_DIALOG_ADD_EDITFILE (Chinese (P.R.C.))
Resource17=IDD_DIALOG_FOLD (Chinese (P.R.C.))
Resource18=IDD_DIALOG_FIND (Chinese (P.R.C.))
Resource19=IDD_DIALOG_FILE (Chinese (P.R.C.))
Resource20=IDR_TOOLBAR_PREVIEW (Chinese (P.R.C.))

[CLS:CFileAddEdit]
Type=0
BaseClass=CDialog
HeaderFile=FileAddEdit.h
ImplementationFile=FileAddEdit.cpp
LastObject=CFileAddEdit

[CLS:CFileDialogEx]
Type=0
BaseClass=CFileDialog
HeaderFile=FileDialogEx.h
ImplementationFile=FileDialogEx.cpp

[CLS:CKC_File]
Type=0
BaseClass=CPropertyPage
HeaderFile=KC_File.h
ImplementationFile=KC_File.cpp

[CLS:CKC_Fold]
Type=0
BaseClass=CPropertyPage
HeaderFile=KC_Fold.h
ImplementationFile=KC_Fold.cpp

[CLS:CKillContentApp]
Type=0
BaseClass=CWinApp
HeaderFile=KillContent.h
ImplementationFile=KillContent.cpp
LastObject=CKillContentApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=KillContentDlg.cpp
ImplementationFile=KillContentDlg.cpp
LastObject=CAboutDlg

[CLS:CKillContentDlg]
Type=0
BaseClass=CDialog
HeaderFile=KillContentDlg.h
ImplementationFile=KillContentDlg.cpp
LastObject=CKillContentDlg

[CLS:CKillListCtrl]
Type=0
BaseClass=CListCtrl
HeaderFile=KillListCtrl.h
ImplementationFile=KillListCtrl.cpp

[CLS:CKillOption]
Type=0
BaseClass=CDialog
HeaderFile=KillOption.h
ImplementationFile=KillOption.cpp
Filter=D
VirtualFilter=dWC
LastObject=CKillOption

[CLS:CKillRichEditCtrl]
Type=0
BaseClass=CRichEditCtrl
HeaderFile=KillRichEditCtrl.h
ImplementationFile=KillRichEditCtrl.cpp

[CLS:COperateKillNote]
Type=0
BaseClass=CDialog
HeaderFile=OperateKillNote.h
ImplementationFile=OperateKillNote.cpp
LastObject=IDCANCEL
Filter=D

[CLS:CPreviewCode]
Type=0
BaseClass=CDialog
HeaderFile=PreviewCode.h
ImplementationFile=PreviewCode.cpp
Filter=D
VirtualFilter=dWC
LastObject=ID_BUTTON_PREVIEW_SAVE

[DLG:IDD_DIALOG_ADD_EDITFILE]
Type=1
Class=CFileAddEdit
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_SOUR,static,1342308352
Control4=IDC_STATIC_DES,static,1342308352
Control5=IDC_COMBO_SOUR,combobox,1344340034
Control6=IDC_COMBO_DES,combobox,1344340034
Control7=IDC_BUTTON_SOURFILE,button,1342242816
Control8=IDC_BUTTON_DESFILE,button,1342242816

[DLG:IDD_DIALOG_FILE]
Type=1
Class=CKC_File
ControlCount=5
Control1=IDC_STATIC_CLEWBOX,button,1342177287
Control2=IDC_LIST_FILE,SysListView32,1350631429
Control3=IDC_STATIC_FILECLEW,static,1342308352
Control4=IDC_BUTTON_PREVIEW,button,1342242816
Control5=IDC_BUTTON_DELETE,button,1342242816

[DLG:IDD_DIALOG_FOLD]
Type=1
Class=CKC_Fold
ControlCount=4
Control1=IDC_STATIC_FOLDCLEWBOX,button,1342177287
Control2=IDC_STATIC_CLEWFOLD,static,1342308352
Control3=IDC_LIST_FOLD,SysListView32,1350631425
Control4=IDC_BUTTON_DELETEFOLD,button,1342242816

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_KILLCONTENT_DIALOG]
Type=1
Class=CKillContentDlg
ControlCount=2
Control1=IDC_BUTTON_CLOSE,button,1342242816
Control2=IDC_BUTTON_OPTION,button,1342242816

[DLG:IDD_DIALOG_OPTION]
Type=1
Class=CKillOption
ControlCount=8
Control1=IDC_RADIO_KILLALL,button,1342177289
Control2=IDC_RADIO_KILLPART,button,1342177289
Control3=IDC_CHECK_FILEHEADER,button,1342242819
Control4=IDC_CHECK_FUNCHEADER,button,1342242819
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816
Control7=IDC_STATIC_OPTION,button,1342177287
Control8=IDC_STATIC_NOTE,static,1342308352

[DLG:IDD_DIALOG_KILLNOTE]
Type=1
Class=COperateKillNote
ControlCount=8
Control1=IDCANCEL,button,1342242817
Control2=IDC_BUTTON_DETAIL,button,1342242816
Control3=IDC_EDIT_SOURFILENAME,edit,1342244992
Control4=IDC_EDIT_DESFILENAME,edit,1342244992
Control5=IDC_LIST_DETAIL,SysListView32,1350664197
Control6=IDC_STATIC_SOUR,static,1342308352
Control7=IDC_STATIC_DES,static,1342308352
Control8=IDC_STATIC_SCANTIME,static,1342308352

[DLG:IDD_DIALOG_PREVIEW]
Type=1
Class=CPreviewCode
ControlCount=5
Control1=IDC_EDIT_PREVIEW,edit,1342244992
Control2=IDC_EDIT_RESULT,edit,1342244992
Control3=IDC_RICHEDIT_PREVIEW,RichEdit20W,1353777348
Control4=IDC_RICHEDIT_RESULT,RichEdit20W,1353777348
Control5=IDC_STATIC_SPLITTER,button,1342177287

[TB:IDR_TOOLBAR_PREVIEW]
Type=1
Class=CPreviewCode
Command1=ID_BUTTON_CLOSE
Command2=ID_BUTTON_FIND
Command3=ID_BUTTON_PREVIEW_ALL
Command4=ID_BUTTON_PREVIEW_HEAD
Command5=ID_BUTTON_PREVIEW_FUNC
Command6=ID_BUTTON_PREVIEW_SAVE
CommandCount=6

[DLG:IDD_DIALOG_FIND]
Type=1
Class=CKFind
ControlCount=9
Control1=IDC_COMBO_FIND,combobox,1344340034
Control2=IDC_CHECK_LU,button,1342242819
Control3=IDC_CHECK_ALL,button,1342242819
Control4=IDC_RADIO_UP,button,1342177289
Control5=IDC_RADIO_DOWN,button,1342177289
Control6=IDC_BUTTON_FIND,button,1342242817
Control7=IDCANCEL,button,1342242816
Control8=IDC_STATIC_FINDCONTENT,static,1342308352
Control9=IDC_STATIC_DIRECT,button,1342177287

[CLS:CKFind]
Type=0
HeaderFile=KFind.h
ImplementationFile=KFind.cpp
BaseClass=CDialog
Filter=D
LastObject=CKFind
VirtualFilter=dWC

[DLG:IDD_DIALOG_OPTION (Chinese (P.R.C.))]
Type=1
Class=CKillOption
ControlCount=8
Control1=IDC_RADIO_KILLALL,button,1342177289
Control2=IDC_RADIO_KILLPART,button,1342177289
Control3=IDC_CHECK_FILEHEADER,button,1342242819
Control4=IDC_CHECK_FUNCHEADER,button,1342242819
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816
Control7=IDC_STATIC_OPTION,button,1342177287
Control8=IDC_STATIC_NOTE,static,1342308352

[DLG:IDD_ABOUTBOX (Chinese (P.R.C.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_KILLCONTENT_DIALOG (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=2
Control1=IDC_BUTTON_CLOSE,button,1342242816
Control2=IDC_BUTTON_OPTION,button,1342242816

[DLG:IDD_DIALOG_FILE (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=5
Control1=IDC_STATIC_CLEWBOX,button,1342177287
Control2=IDC_LIST_FILE,SysListView32,1350631429
Control3=IDC_STATIC_FILECLEW,static,1342308352
Control4=IDC_BUTTON_PREVIEW,button,1342242816
Control5=IDC_BUTTON_DELETE,button,1342242816

[DLG:IDD_DIALOG_FOLD (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC_FOLDCLEWBOX,button,1342177287
Control2=IDC_STATIC_CLEWFOLD,static,1342308352
Control3=IDC_LIST_FOLD,SysListView32,1350631425
Control4=IDC_BUTTON_DELETEFOLD,button,1342242816

[DLG:IDD_DIALOG_ADD_EDITFILE (Chinese (P.R.C.))]
Type=1
Class=CFileAddEdit
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_SOUR,static,1342308352
Control4=IDC_STATIC_DES,static,1342308352
Control5=IDC_COMBO_SOUR,combobox,1344340034
Control6=IDC_COMBO_DES,combobox,1344340034
Control7=IDC_BUTTON_SOURFILE,button,1342242816
Control8=IDC_BUTTON_DESFILE,button,1342242816

[DLG:IDD_DIALOG_PREVIEW (Chinese (P.R.C.))]
Type=1
Class=CPreviewCode
ControlCount=5
Control1=IDC_EDIT_PREVIEW,edit,1342244992
Control2=IDC_EDIT_RESULT,edit,1342244992
Control3=IDC_RICHEDIT_PREVIEW,RichEdit20W,1353777348
Control4=IDC_RICHEDIT_RESULT,RichEdit20W,1353777348
Control5=IDC_STATIC_SPLITTER,button,1342177287

[DLG:IDD_DIALOG_KILLNOTE (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=8
Control1=IDCANCEL,button,1342242817
Control2=IDC_BUTTON_DETAIL,button,1342242816
Control3=IDC_EDIT_SOURFILENAME,edit,1342244992
Control4=IDC_EDIT_DESFILENAME,edit,1342244992
Control5=IDC_LIST_DETAIL,SysListView32,1350664197
Control6=IDC_STATIC_SOUR,static,1342308352
Control7=IDC_STATIC_DES,static,1342308352
Control8=IDC_STATIC_SCANTIME,static,1342308352

[DLG:IDD_DIALOG_FIND (Chinese (P.R.C.))]
Type=1
Class=?
ControlCount=9
Control1=IDC_COMBO_FIND,combobox,1344340034
Control2=IDC_CHECK_LU,button,1342242819
Control3=IDC_CHECK_ALL,button,1342242819
Control4=IDC_RADIO_UP,button,1342177289
Control5=IDC_RADIO_DOWN,button,1342177289
Control6=IDC_BUTTON_FIND,button,1342242817
Control7=IDCANCEL,button,1342242816
Control8=IDC_STATIC_FINDCONTENT,static,1342308352
Control9=IDC_STATIC_DIRECT,button,1342177287

[TB:IDR_TOOLBAR_PREVIEW (Chinese (P.R.C.))]
Type=1
Class=?
Command1=ID_BUTTON_CLOSE
Command2=ID_BUTTON_FIND
Command3=ID_BUTTON_PREVIEW_ALL
Command4=ID_BUTTON_PREVIEW_HEAD
Command5=ID_BUTTON_PREVIEW_FUNC
Command6=ID_BUTTON_PREVIEW_SAVE
CommandCount=6

