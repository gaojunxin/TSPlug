// SelTransparentColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CreateGIF.h"
#include "SelTransparentColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelTransparentColorDlg dialog


CSelTransparentColorDlg::CSelTransparentColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelTransparentColorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelTransparentColorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_transparentColorIndex = 0;
}


void CSelTransparentColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelTransparentColorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelTransparentColorDlg, CDialog)
	//{{AFX_MSG_MAP(CSelTransparentColorDlg)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelTransparentColorDlg message handlers
void CSelTransparentColorDlg::SetBitmap(BYTE *pData,BYTE*palette,int nWidth,int nHeight)
{
	m_pData = pData;
	m_palette = palette;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}

HBRUSH CSelTransparentColorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_BMP)
	{
		for(int i=0; i<m_nHeight; i++)
		{
			for(int j=0; j<m_nWidth; j++)
			{
				int nIndex = m_pData[i*m_nWidth+j]*3;
				pDC->SetPixel(j,i,RGB(m_palette[nIndex],m_palette[nIndex+1],m_palette[nIndex+2]));
			}
		}
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CSelTransparentColorDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint pt;
	GetCursorPos(&pt);
	GetDlgItem(IDC_STATIC_BMP)->ScreenToClient(&pt);

	m_transparentColorIndex = m_pData[pt.y*m_nWidth+pt.x];

	CDialog::OnLButtonDown(nFlags, point);
}

void CSelTransparentColorDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
