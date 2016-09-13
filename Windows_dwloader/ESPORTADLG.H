// EsportaDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEsportaDlg dialog

class CEsportaDlg : public CDialog
{
// Construction
public:
	CEsportaDlg(CWnd* pParent = NULL);   // standard constructor
	virtual BOOL Create(UINT ,CWnd *,CWnd *);
// Dialog Data
	//{{AFX_DATA(CEsportaDlg)
	enum { IDD = IDD_ESPORTA };
	CProgressCtrl	m_Pos;
	CEdit	m_Stop;
	CEdit	m_Start;
	CButton	m_Range;
	CButton	m_All;
	CEdit	m_Sep;
	CButton	m_Altro;
	CButton	m_Tab;
	CComboBox	m_Combo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEsportaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd * m_Frame;
	BOOL addr;
	BOOL Separ;
	// Generated message map functions
	//{{AFX_MSG(CEsportaDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTab();
	afx_msg void OnExtab();
	afx_msg void OnAddr();
	afx_msg void OnRange();
	afx_msg void OnFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
