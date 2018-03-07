// GetScreenToBitmap.cpp: implementation of the CGetScreenToBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyServer.h"
#include "GetScreenToBitmap.h"
//#include "jpeglib.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGetScreenToBitmap::CGetScreenToBitmap()
{
	m_lpBmpData = NULL;
	m_dwBmpSize = 0;
	m_dwBmpInfoSize = 0;
}

CGetScreenToBitmap::~CGetScreenToBitmap()
{
   ResetVariable();
}


void CGetScreenToBitmap::ResetVariable()
{
    if(m_lpBmpData) GlobalFree(m_lpBmpData);
	m_lpBmpData = NULL;
    m_dwBmpSize = 0;
    m_dwBmpInfoSize = 0;
}


LPSTR FAR CGetScreenToBitmap::GetImage()
{
	 return m_lpBmpData;
}

DWORD CGetScreenToBitmap::GetImageSize()
{
	 return m_dwBmpSize;
}

BOOL CGetScreenToBitmap::GetScreen(CRect rcArea,int nBits, int nArea)
{
	HDC	hScrDC, hMemDC;		// ��Ļ���ڴ��豸������
	HBITMAP	hBitmap, hOldBitmap;		// λͼ���
	HDC	hDC;				//�豸������
	int	iBits;				//��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���
	WORD wBitCount;			//λͼ��ÿ��������ռ�ֽ���
	//�����ɫ���С�� λͼ�������ֽڴ�С ��λͼ�ļ���С �� д���ļ��ֽ���
	DWORD dwPaletteSize=0, dwBmBitsSize;
	BITMAP Bitmap;			//λͼ���Խṹ
	BITMAPINFOHEADER bi;		//λͼ��Ϣͷ�ṹ 
	LPBITMAPINFOHEADER lpbi;		//ָ��λͼ��Ϣͷ�ṹ

	//�����ļ��������ڴ�������ɫ����  
	HPALETTE hPal,hOldPal=NULL;
    //�ͷ�ԭ����Դ
	ResetVariable();
	// �����Ļ�ֱ���
	SetArea(rcArea, nArea); //ȡͼ������
	//Ϊ��Ļ�����豸������
	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	//Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
	hMemDC = CreateCompatibleDC(hScrDC);
	// ����һ������Ļ�豸��������ݵ�λͼ
	hBitmap = CreateCompatibleBitmap (hScrDC, 
		      m_rcArea.Width(), m_rcArea.Height());
	// ����λͼѡ���ڴ��豸��������
	hOldBitmap = (HBITMAP) SelectObject(hMemDC, 
		          hBitmap);
	// ����Ļ�豸�����������ڴ��豸��������
	BitBlt(hMemDC, 0, 0, m_rcArea.Width(), 
		   m_rcArea.Height(), hScrDC, 
		   m_rcArea.left, m_rcArea.top, SRCCOPY);
	//�õ���Ļλͼ�ľ��
	hBitmap = (HBITMAP) SelectObject(hMemDC, hOldBitmap);
	//����λͼÿ��������ռ�ֽ���
	iBits = GetDeviceCaps(hScrDC, BITSPIXEL);
	iBits = iBits * GetDeviceCaps(hScrDC, PLANES);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else// if (iBits <= 24)
		wBitCount = 24;

	if (wBitCount > nBits)
		wBitCount = nBits;
	//�����ɫ���С
	if (wBitCount <= 8)
		dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);

	//����λͼ��Ϣͷ�ṹ
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = dwPaletteSize/sizeof(RGBQUAD);
	bi.biClrImportant = 0;

	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount+31)/32)* 4 *Bitmap.bmHeight ;
	m_dwBmpInfoSize = dwPaletteSize + sizeof(BITMAPINFOHEADER);
	m_dwBmpSize = dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER);
	m_lpBmpData  = (LPSTR)GlobalAlloc(GMEM_FIXED,m_dwBmpSize);
	if (m_lpBmpData == NULL) goto Exit01;

    lpbi = (LPBITMAPINFOHEADER)m_lpBmpData;
	*lpbi = bi;
	// �����ɫ��   
	hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC (NULL);
		hOldPal = ::SelectPalette(hDC, hPal, FALSE);
		RealizePalette(hDC);
	}
	// ��ȡ�õ�ɫ�����µ�����ֵ
	::GetDIBits(
			hDC,
			hBitmap,
			0, 
			(UINT) Bitmap.bmHeight,
			(LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize,
			//(BITMAPINFOHEADER *)lpbi,
			(BITMAPINFO *)lpbi,
			DIB_RGB_COLORS
		  );
	//�ָ���ɫ��
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}
//DEL	m_lpBmpData = (char *)lpbi;
	//���
	DeleteObject (hBitmap);
	DeleteObject (hOldBitmap);
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	return TRUE;

Exit01:
	ResetVariable();
	DeleteObject (hBitmap);
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	return FALSE;
}

void CGetScreenToBitmap::SetArea(CRect rcArea, int nArea)
{
	//Ϊ��Ļ�����豸������
	HWND hFocus;
	int		xScrn, yScrn;			// ��Ļ�ֱ��� 
	HDC hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	// �����Ļ�ֱ���

	xScrn = GetDeviceCaps(hScrDC, HORZRES);
	yScrn = GetDeviceCaps(hScrDC, VERTRES);
	DeleteDC(hScrDC);
    m_rcArea.left = 0;
    m_rcArea.top = 0;
    m_rcArea.right = xScrn;
    m_rcArea.bottom = yScrn;
    if (nArea == 1 &&  //1)��ǰ����2)ָ������0)ȫ��
        (hFocus = ::GetForegroundWindow()) != NULL)
	{
	    GetWindowRect(hFocus, &m_rcArea);
	}
	else if(nArea == 2)
	{
    m_rcArea =  rcArea;
	}
	if (m_rcArea.left < 0)
		m_rcArea.left = 0;
	if (m_rcArea.top < 0)
		m_rcArea.top = 0;
	if (m_rcArea.right > xScrn)
		m_rcArea.right = xScrn;
	if (m_rcArea.bottom > yScrn)
		m_rcArea.bottom = yScrn;
	//ȷ��ѡ�������ǿɼ���
}
