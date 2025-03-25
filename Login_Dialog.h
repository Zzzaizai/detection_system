#pragma once
#include "afxdialogex.h"


// Login_Dialog 对话框

class Login_Dialog : public CDialogEx
{
	DECLARE_DYNAMIC(Login_Dialog)

public:
	Login_Dialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Login_Dialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// 用户名
	CString admin;
	// 密码
	CString word;
public:
	afx_msg void OnBnClickedOk();
};
