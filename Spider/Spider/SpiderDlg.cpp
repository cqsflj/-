
// SpiderDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Spider.h"
#include "SpiderDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MD5LENTH 16
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

#define MY_MESSAGE WM_USER+1

string g_strURL = "https://bbs.pediy.com/new-digest.htm";
string g_strURL2 = "https://bbs.pediy.com/new-digest-";

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSpiderDlg �Ի���



CSpiderDlg::CSpiderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSpiderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSpiderDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_List);
}

BEGIN_MESSAGE_MAP(CSpiderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CSpiderDlg::OnBnClickedOk)
    ON_BN_CLICKED(BN_LOGIN, &CSpiderDlg::OnBnClickedLogin)
    ON_BN_CLICKED(BN_STRAT, &CSpiderDlg::OnBnClickedStrat)
    ON_BN_CLICKED(IDC_BUTTON1, &CSpiderDlg::OnBnClickedButton1)
    ON_MESSAGE(MY_MESSAGE, &CSpiderDlg::OnMyMessage)
    ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CSpiderDlg::OnNMRClickList1)
    ON_WM_DESTROY()
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CSpiderDlg ��Ϣ�������

BOOL CSpiderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
    //״̬��
    static UINT indicators[] =
    {
        ID_STAUTSTIP,           // status line indicator
        ID_STAUTSSPEED,
        ID_STAUTSPORT,
        ID_STAUTSCOUNT
    };
    //״̬��
    m_StatusBar.Create(this);
    m_StatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));
    m_StatusBar.SetPaneInfo(0, m_StatusBar.GetItemID(0), SBPS_STRETCH, NULL);
    m_StatusBar.SetPaneInfo(1, m_StatusBar.GetItemID(1), SBPS_NORMAL, 160);
    m_StatusBar.SetPaneInfo(2, m_StatusBar.GetItemID(2), SBPS_NORMAL, 100);
    m_StatusBar.SetPaneInfo(3, m_StatusBar.GetItemID(3), SBPS_NORMAL, 100);
    m_StatusBar.SetPaneText(1, CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")));
    m_StatusBar.SetPaneText(2, _T("״̬"));
    
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0); //��ʾ״̬��

    SetDlgItemText(EDIT_USER, "15310872267");
    SetDlgItemText(EDIT_PASS, "admin1235");
    SetDlgItemInt(IDC_EDIT1, 10);
    m_IsLogin = FALSE;
    m_List.ModifyStyle(0, LVS_REPORT | LVS_EDITLABELS);
    m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
    m_List.InsertColumn(0, _T("����"), LVCFMT_LEFT,250);
    m_List.InsertColumn(1, _T("����"), LVCFMT_LEFT, 400);
    m_hThread = INVALID_HANDLE_VALUE;

    SetTimer(1, 500, NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSpiderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSpiderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


void CSpiderDlg::OnBnClickedOk()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    //CDialogEx::OnOK();
}


void CSpiderDlg::OnBnClickedLogin()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    unsigned char MD5result[MD5LENTH] = { 0 };
    CString csUserName;
    CString csPassWord;
    GetDlgItemText(EDIT_USER, csUserName);
    GetDlgItemText(EDIT_PASS, csPassWord);
    //���뱾�ؼ���MD5
    string strPassWord = csPassWord.GetString();
    MD5((const unsigned char*)strPassWord.c_str(), strPassWord.length(), MD5result);
    string strEncodePwd;
    CStringA str;

    for (int i = 0; i < MD5LENTH; i++)
    {
        str.Format("%02x", MD5result[i]);
        strEncodePwd += str.GetBuffer(0);
    }

    //post��ַ
    string strURL = "http://passport.kanxue.com/user-login.htm";
    m_StatusBar.SetPaneText(0, strURL.c_str());
    //���
    CString strPostData;
    //account=15310872267&password=905f0a2a11eef8797c1dcd15f9fa6506
    strPostData.Format("account=%s&password=%s", csUserName, strEncodePwd.c_str());
    //���ô���
    //m_HttpPost.sc_setProxy("192.168.1.135:8080");
    //ƴ��HTTPͷ
    m_HttpPost.sc_appendHeader("Proxy-Connection: keep-alive");
    m_HttpPost.sc_appendHeader("Accept: text/plain, */*; q=0.01");
    m_HttpPost.sc_appendHeader("Origin: http://passport.kanxue.com");
    m_HttpPost.sc_appendHeader("X-Requested-With: XMLHttpRequest");
    m_HttpPost.sc_appendHeader("User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36");
    m_HttpPost.sc_appendHeader("Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
    m_HttpPost.sc_appendHeader("Accept-Encoding: gzip, deflate, br");
    m_HttpPost.sc_appendHeader("Accept-Language: zh-CN,zh;q=0.9");
    string strRet;
    CURLcode urlCodeRet = m_HttpPost.sc_post(strURL.c_str(), strPostData.GetBuffer(), strRet);
    if (urlCodeRet == CURLE_OK)
    {
        //string str2 = strRet.substr(3, strRet.length()); //����UTF8��־λ
        string str3 = CURLCode::UTF8ToString(strRet.data());
        std::regex re1("\"code\":[\\s](.*),\n[\\s]*\"message\": \"(.*)\"");
        std::match_results<std::string::const_iterator> mr2;
        string strIsLogin;
        regex_search(str3, mr2, re1);
        strIsLogin = mr2[1].str();
        m_csCookie = mr2[2].str();
        if (strIsLogin == "0")
        {
            char szbuf[] = "passport_token";
            m_HttpPost.sc_setCookie(szbuf, m_csCookie.data());
            m_StatusBar.SetPaneText(3, _T("��¼�ɹ�"));
            m_IsLogin = TRUE;
        }
        else
        {
            MessageBox(str3.c_str());
        }
    }
    else
    {
        m_StatusBar.SetPaneText(3, _T("����ʧ��"));
    }
}
//����html��ǩ
string Guolv(string& str)
{
    int nLength = str.length();
    string str2;
    BOOL is = FALSE;
    int nFlag = 0;
    for (int i = 0; i < nLength; i++)
    {
        if (str.at(i) == '<')
        {
            if (is)
            {
                if (nFlag==0)
                {
                    str2 += "|";
                    nFlag++;
                }
            }
            is = FALSE;
        }
        else if (str.at(i) == '>')
        {
            is = TRUE;
            continue;
        }
        if (is)
        {
            if (str.at(i)!=' ') //���˿ո�
            {
                if (str.at(i) != 10 && str.at(i)!=13)
                {
                    str2 += str.at(i);
                    nFlag = 0;
                }
            }
        }
    }
    return str2;
}


void CSpiderDlg::OnBnClickedStrat()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    string strURL = "https://passport.kanxue.com/my.htm";
    string strRet;
    if (!m_IsLogin)
    {
        MessageBox("�ȵ�¼!");
        return;
    }
    CURLcode urlCodeRet = m_HttpPost.sc_get(strURL.c_str(), strRet);
    if (urlCodeRet == CURLE_OK)
    {
        string str3 = CURLCode::UTF8ToString(strRet.data());
        std::regex re("<label class=\"(.*)\">(.*)</label>[\\s]*<div class=\"(.*)\">(.*)</div>");
        smatch result;
        auto curIter = str3.cbegin();
        auto endIter = str3.cend();
        while (regex_search(curIter, endIter, result, re))
        {
            string str1 = regex_replace(result[0].str(), regex("<label class=\"(.*)\">(.*)</label>[\\s]*<div class=\"(.*)\">(.*)</div>"), \
                "$2");
            string str2;
            if (!str1.compare("��֤״̬��"))
            {
                str2 = regex_replace(result[3].str(), regex("(.*)</i>(.*)</span>(.*)<a(.*)"), "$2$3");
            }
            else
            {
                str2 = regex_replace(result[0].str(), regex("<label class=\"(.*)\">(.*)</label>[\\s]*<div class=\"(.*)\">(.*)</div>"), \
                    "$4");
            }
            curIter = result[0].second;
            CString csTemp1 = str1.c_str();
            CString csTemp2 = str2.c_str();
            m_cs.Lock();
            m_q1.push(csTemp1.Trim());
            m_q2.push(csTemp2.Trim());
            m_cs.Unlock();
            PostMessage(MY_MESSAGE, 1, NULL);
        }
    }

}
unsigned CSpiderDlg::ThreadProc(LPVOID lpParam)
{
    CSpiderDlg* pDlg = (CSpiderDlg*)lpParam;
    pDlg->ReadList(g_strURL);
    for (int i = 2; i <= pDlg->m_nCount; i++)
    {
        char szbuf[MAXBYTE] = { 0 };
        sprintf_s(szbuf, "%s%d.htm", g_strURL2.data(), i);
        string str(szbuf);
        pDlg->ReadList(str);
    }
    pDlg->PostMessage(MY_MESSAGE, 2, NULL);
    return 1;
}

void CSpiderDlg::OnBnClickedButton1()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    m_nCount = GetDlgItemInt(IDC_EDIT1, FALSE);
    m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, this, NULL, NULL);
    if (m_hThread!=INVALID_HANDLE_VALUE)
    {
        m_StatusBar.SetPaneText(0, g_strURL.c_str());
        m_StatusBar.SetPaneText(3, _T("С�����ҿ�ʼ��"));
    }
}

BOOL CSpiderDlg::ReadList(string& strURL)
{
    string strRet;
    CURLcode urlCodeRet = m_HttpPost.sc_get(strURL.c_str(), strRet);
    if (urlCodeRet != CURLE_OK)
    {
        return FALSE;
    }
    string str3 = CURLCode::UTF8ToString(strRet.data());
    std::regex re1("<a href=\"thread(.*)\">������</a>(.*)");
    string strText = regex_replace(str3, re1, "$3");
    std::regex re("<a href=\"thread(.*)\">(.*)</a>");
    smatch result;
    auto curIter = strText.cbegin();
    auto endIter = strText.cend();
    while (regex_search(curIter, endIter, result, re))
    {
        string strHost = "https://bbs.pediy.com/";
        string str1 = regex_replace(result[0].str(), regex("<a href=\"(.*)\">(.*)</a>"), "$1");
        strHost += str1;
        string str2 = regex_replace(result[0].str(), regex("<a href=\"(.*)\">(.*)</a>"), "$2");
        curIter = result[0].second;
        CString csTemp1 = strHost.c_str();
        CString csTemp2 = str2.c_str();
        m_cs.Lock();
        m_q1.push(csTemp1.Trim());
        m_q2.push(csTemp2.Trim());
        m_cs.Unlock();
        PostMessage(MY_MESSAGE, 1, NULL);
    }
    return TRUE;
}

afx_msg LRESULT CSpiderDlg::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
    int n = (int)wParam;
    switch (n)
    {
    case 1:
    {
        if (!m_q1.empty())
        {
            m_cs.Lock();
            CString csTemp1 = m_q1.front();
            m_q1.pop();
            CString csTemp2 = m_q2.front();
            m_q2.pop();
            m_cs.Unlock();
            int n = m_List.GetItemCount();
            m_List.InsertItem(n, _T(""));
            m_List.SetItemText(n, 0, csTemp1.Trim());
            m_List.SetItemText(n, 1, csTemp2.Trim());
        }
    }
    break;
    case 2:
    {
        m_StatusBar.SetPaneText(3, _T("С������������"));
        AfxMessageBox("�Ҽ�ѡ�и���");
        break;
    }
    default:
        break;
    }

    return 0;
}

void CSpiderDlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    if (pNMItemActivate->iItem !=-1)//ѡ����
    {
        CString csText;
        csText = m_List.GetItemText(pNMItemActivate->iItem, 0);
        csText += " ";
        csText += m_List.GetItemText(pNMItemActivate->iItem, 1);
        if (OpenClipboard())//�򿪼��а�  
        {
            EmptyClipboard();//��ռ��а�,�õ�ǰ���ڽ���ӵ�м��а�  
            HANDLE hClip;
            hClip = GlobalAlloc(GMEM_MOVEABLE, csText.GetLength() + 1);//�����ڴ���󷵻ص�ַ(�ú�����ϵͳ��ȫ�ֶ��з����ڴ�)  
            char *pBuf;
            pBuf = (char *)GlobalLock(hClip);//����ȫ���ڴ���ָ�����ڴ�飬������һ����ֵַ������ָ���ڴ�����ʼ��  
            strcpy(pBuf, csText);//��Str�����е�����Copy���ڴ�ռ���  
            GlobalUnlock(hClip);//����ȫ���ڴ��  
            SetClipboardData(CF_TEXT, hClip);//���ü���������  
            CloseClipboard();//�ر�  
        }
        AfxMessageBox("�Ѿ����Ƶ�������");
    }
    *pResult = 0;
}


void CSpiderDlg::OnDestroy()
{
    CDialogEx::OnDestroy();
    KillTimer(1);
    if (m_hThread!=INVALID_HANDLE_VALUE)
    {
        WaitForSingleObject(m_hThread, INFINITE);
        CloseHandle(m_hThread);
    }
    // TODO:  �ڴ˴������Ϣ����������
}


void CSpiderDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if (nIDEvent==1)
    {
        m_StatusBar.SetPaneText(1, CTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")));
    }
    CDialogEx::OnTimer(nIDEvent);
}
