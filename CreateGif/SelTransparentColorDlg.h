#if !defined(AFX_SELTRANSPARENTCOLORDLG_H__9A8C06A1_62C4_477B_A811_14D68184E4DB__INCLUDED_)
#define AFX_SELTRANSPARENTCOLORDLG_H__9A8C06A1_62C4_477B_A811_14D68184E4DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelTransparentColorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelTransparentColorDlg dialog

class CSelTransparentColorDlg : public CDialog
{
// Construction
public:
	CSelTransparentColorDlg(CWnd* pParent = NULL);   // standard constructor
	void SetBitmap(BYTE *pData,BYTE*palette,int nWidth,int nHeight);
	BYTE m_transparentColorIndex;
	
// Dialog Data
	//{{AFX_DATA(CSelTransparentColorDlg)
	enum { IDD = IDD_DIALOG_SEL_TRANSPARENT_COLOR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelTransparentColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BYTE *m_pData;
	BYTE *m_palette;
	int m_nWidth;
	int m_nHeight;
	// Generated message map functions
	//{{AFX_MSG(CSelTransparentColorDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELTRANSPARENTCOLORDLG_H__9A8C06A1_62C4_477B_A811_14D68184E4DB__INCLUDED_)
