// FileDlg.cpp : implementation file
//


#include "stdafx.h"
#include "Comser.h"
#include "dwload.h"
#include "FileDlg.h"


extern CDwloadApp theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileDlg dialog


CFileDlg::CFileDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CFileDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CFileDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    file = NULL;
    filesize = 0;
    div = 1;
}


void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFileDlg)
    DDX_Control(pDX, IDC_POS, m_File);
    DDX_Control(pDX, IDC_POS_2, m_Pos);
    DDX_Control(pDX, IDC_PROGRESS1, m_Bar);
    DDX_Control(pDX, IDC_COM, m_Port);
    DDX_Control(pDX, IDC_VEL, m_Vel);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileDlg, CDialog)
    //{{AFX_MSG_MAP(CFileDlg)
    ON_BN_CLICKED(IDC_FILE, OnStart)
    ON_BN_CLICKED(IDC_START, OnDownload)
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileDlg message handlers

BOOL CFileDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();

    autoclose = false;
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon
    m_Vel.AddString ("2400 ");
    m_Vel.AddString ("4800 ");
    m_Vel.AddString ("9600 ");
    m_Vel.AddString ("19200 ");
    m_Vel.AddString ("39400 ");
    m_Vel.AddString ("57600 ");
    m_Vel.AddString ("115200 ");
    m_Vel.SetCurSel(0);
    m_Port.AddString("COM 1");
    m_Port.AddString("COM 2");
    m_Port.AddString("COM 3");
    m_Port.AddString("COM 4");
    m_Port.AddString("COM 5");
    m_Port.AddString("COM 6");
    m_Port.AddString("COM 7");
    m_Port.AddString("COM 8");
    m_Port.AddString("COM 9");
    m_Port.SetCurSel(2);
    GetDlgItem(IDC_START)->EnableWindow(FALSE);		
    CString param = theApp.param;
    if (param.IsEmpty()==FALSE)
    {
        m_File.SetWindowText(param);
        file = fopen((const char *)param, "r+");
        if (file == NULL)
        {
            CString msg = _T("Cannot Open file :\r\n");
            msg += param;
            AfxMessageBox(msg);
        }
        else
        {
            autoclose = true;
            OnDownload();
        }
    }
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CFileDlg::Create(UINT nID,CWnd *pParentWnd) 
{
    // TODO: Add your specialized code here and/or call the base class
    return CDialog::Create(IDD, pParentWnd);
}

void CFileDlg::OnStart()
{
    // szFilters is a text string that includes two file name filters:
    // "*.my" for "MyType Files" and "*.*' for "All Files."
    TCHAR szFilters[] = _T("IHEX Files (*.hex)|*.hex|All Files (*.*)|*.*||");

    // Create an Open dialog; the default file name extension is ".my".
    CFileDialog fileDlg(TRUE, _T("hex"), _T("*.hex"),
        OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

    // Display the file dialog. When user clicks OK, fileDlg.DoModal() 
    // returns IDOK.
    if (fileDlg.DoModal() == IDOK)
    {
        CString pathName = fileDlg.GetPathName();
        m_File.SetWindowText(pathName);
        file = fopen((const char *)pathName, "r+");
        if (file == NULL)
        {
            CString msg = _T("Cannot Open file :\r\n");
            msg += pathName;
            AfxMessageBox(msg);
        }
        else
        {
            GetDlgItem(IDC_START)->EnableWindow(TRUE);
        }
        
    }
}

void CFileDlg::OnDownload() 
{
    int sel=m_Vel.GetCurSel();
    int vel=115200;
    switch(sel)
        {
        default:
        case 0: vel=115200;break;
        case 1: vel=19200;break; 
        case 2: vel=2400;break;
        case 3: vel=38400;break; 
        case 4: vel=4800;break; 
        case 5: vel=57600;break; 
        case 6: vel=9600;break; 
        }
    int com=2;
    sel=m_Port.GetCurSel();
    switch(sel)
        {
        case 0: com=1;break;
        case 1: com=2;break;
        case 2: com=3;break;
        case 3: com=4;break;
        case 4: com = 5; break;
        case 5: com = 6; break;
        case 6: com = 7; break;
        case 7: com = 8; break;
        case 8: com = 9; break;
    }
    if(!COM.OpenComm(com,vel,4096,4096))
    {
        AfxMessageBox("Non si Apre la porta di\n Comunicazione Seriale",MB_OK|MB_ICONQUESTION);
        AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
    }
    else
    {
        fseek(file, 0L, SEEK_END);
        filesize = ftell(file);
        fseek(file, 0L, SEEK_SET);
        if (filesize > 1000)
            div = filesize / 100;
        else
            div = 1;
        if (COM.TxComm(file) != 0)
        {
            KillTimer(0);
            COM.CloseComm();
            AfxMessageBox("Tx error : stop");
            AfxGetMainWnd()->PostMessage(WM_CLOSE, 0, 0);
        }
        else
        {
            m_timer = SetTimer(0, 5, NULL);
            m_Bar.SetRange(0, 100);
            GetDlgItem(IDC_START)->EnableWindow(FALSE);
            GetDlgItem(IDC_FILE)->EnableWindow(FALSE);
            GetDlgItem(IDC_VEL)->EnableWindow(FALSE);
            GetDlgItem(IDC_COM)->EnableWindow(FALSE);
        }
    }	
}

void CFileDlg::OnTimer(UINT nIDEvent) 
{
    static int nErr = 0;
    size_t  pos=ftell(file);
    m_Bar.SetPos(pos/div);
    CString s_pos;
    s_pos.Format("%ld of %ld total bytes..", pos, filesize);
    m_Pos.SetWindowText((LPCSTR)s_pos);
    if (pos < filesize)
    {
        int res = COM.RxComm();
        if (res == 0)
        {
            nErr = 0;
            if (COM.TxComm(file) != 0)
            {
                KillTimer(0);
                COM.CloseComm();
                AfxMessageBox("Tx error : stop");
                AfxGetMainWnd()->PostMessage(WM_CLOSE, 0, 0);
            }
        }
        else  if (res == -1000)
        {
            if (COM.Ritrasmetti() != 0)
            {
                nErr++;
                if (nErr == 5)
                {
                    KillTimer(0);
                    COM.CloseComm();
                    AfxMessageBox("Rx error : stop");
                    AfxGetMainWnd()->PostMessage(WM_CLOSE, 0, 0);
                }
            }
            else
                nErr = 0;
        }
    }
    else
    {
        KillTimer(0);
        COM.CloseComm();
        if (autoclose == false)
            AfxMessageBox("Download complete!");
        AfxGetMainWnd()->PostMessage(WM_CLOSE, 0, 0);

    }
    CDialog::OnTimer(nIDEvent);
}

