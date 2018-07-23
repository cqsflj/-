
// SpiderDlg.h : 头文件
//

#pragma once
#include "SkyChaserHttp.h"
#include "afxcmn.h"
#include <regex>
#include <queue>
#include "URLCode.h"
#include <openssl/md5.h>
using namespace std;
// CSpiderDlg 对话框
class CSpiderDlg : public CDialogEx
{
// 构造
public:
	CSpiderDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SPIDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedLogin();
    afx_msg void OnBnClickedStrat();
    afx_msg void OnBnClickedButton1();
    afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDestroy();
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    static unsigned __stdcall ThreadProc(LPVOID lpParam);
    BOOL ReadList(string& strURL);

    CSkyChaserHttp m_HttpPost;
    CStatusBar m_StatusBar;
    string m_csCookie;
    HANDLE m_hThread;
    int m_nCount;
    CListCtrl m_List;
    BOOL m_IsLogin;
    queue<CString> m_q1;
    queue<CString> m_q2;
    CCriticalSection m_cs;

protected:
    afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
};
