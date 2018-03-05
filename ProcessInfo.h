#include "stdafx.h"
#include "windows.h"
#include <vector>
#include <tlhelp32.h>
#include <PSAPI.H>
#pragma comment( lib, "PSAPI.LIB" )


BOOL EnablePrivilege(HANDLE hToken,LPCSTR szPrivName)
{
	
	TOKEN_PRIVILEGES tkp;
	
	LookupPrivilegeValue( NULL,szPrivName,&tkp.Privileges[0].Luid );//�޸Ľ���Ȩ��
	tkp.PrivilegeCount=1;
	tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges( hToken,FALSE,&tkp,sizeof tkp,NULL,NULL );//֪ͨϵͳ�޸Ľ���Ȩ��
	
	return( (GetLastError()==ERROR_SUCCESS) );
	
}


BOOL GetProcessList(std::vector<PROCESSINFO*> *pProcInfo)
{
    DWORD processid[1024],needed;
    HANDLE hProcess;
    HMODULE hModule;
    char path[MAX_PATH] = "";
	char temp[256] = "";
	
    CString path_convert=path;

    pProcInfo->clear();
    HANDLE handle = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    PROCESSENTRY32 *info = new PROCESSENTRY32;
    info->dwSize=sizeof(PROCESSENTRY32);
	int i = 0;
	
    PROCESSINFO *Proc = new PROCESSINFO;
    if(Process32First(handle,info))
    {
		//��Ӵ��� new ����
		Proc = new PROCESSINFO;
		memset(Proc, 0,sizeof(PROCESSINFO));
		//////////////////////////////////////////////////////////////////////////
	
		Proc->PID      =  info->th32ProcessID;
		HANDLE hToken;
		lstrcpy(Proc->ProcName,info->szExeFile);
		
		if ( OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken) )
		{
			if (EnablePrivilege(hToken,SE_DEBUG_NAME))
			{
				
				EnumProcesses(processid, sizeof(processid), &needed);
				
				hProcess=OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,false,processid[i]);
				if (hProcess)
				{
					EnumProcessModules(hProcess, &hModule, sizeof(hModule), &needed);
					GetModuleFileNameEx(hProcess, hModule, path, sizeof(path));
					GetShortPathName(path,path,260);
					//Proc.ProcPath=path;
					lstrcpy(Proc->ProcPath,path);					
				}
			}
			
		}
		i++;
		pProcInfo->push_back(Proc);
	}
	while(Process32Next(handle,info)!=FALSE)
	{
		//��Ӵ��� new ����
		Proc = new PROCESSINFO;
		memset(Proc, 0,sizeof(PROCESSINFO));
		//////////////////////////////////////////////////////////////////////////
	
		Proc->PID      =  info->th32ProcessID;
        lstrcpy(Proc->ProcName,info->szExeFile);
		HANDLE hToken;
		
		if ( OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken) )
		{
			if (EnablePrivilege(hToken,SE_DEBUG_NAME))                             //��������Ȩ��
			{
				
				EnumProcesses(processid, sizeof(processid), &needed);
				
				hProcess=OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,false,processid[i]);
				if (hProcess)
				{
					EnumProcessModules(hProcess, &hModule, sizeof(hModule), &needed);
					GetModuleFileNameEx(hProcess, hModule, path, sizeof(path));
					GetShortPathName(path,path,260);
					lstrcpy(Proc->ProcPath,path);
					
				}
			}
			
		}
		
		i++;
		pProcInfo->push_back(Proc);
	}

    CloseHandle(handle);
    return true;
}

BOOL KillProcess(DWORD pid)
{
//////////////////////////////////////////////////////////////////////////
//ƥ�����
//////////////////////////////////////////////////////////////////////////

	HANDLE hkernel32;    //��ע����̵ľ��
    HANDLE hSnap;
    PROCESSENTRY32 pe; 
    BOOL bNext;

    pe.dwSize = sizeof(pe);
    hSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    bNext=Process32First(hSnap, &pe); 
    while(bNext) 
    {
		//if (EnablePrivilege(hSnap,SE_DEBUG_NAME))
		//{
			if(pe.th32ProcessID=pid)        //--->>
			{
				
				hkernel32=OpenProcess(PROCESS_TERMINATE|PROCESS_CREATE_THREAD|PROCESS_VM_WRITE|PROCESS_VM_OPERATION,1,pe.th32ProcessID);
				TerminateProcess(hkernel32,0);
				break;
			}
		//}
        bNext=Process32Next(hSnap, &pe); 
    }

    CloseHandle(hSnap);

	return true;
}