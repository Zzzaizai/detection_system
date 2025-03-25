#pragma once
#include "afxdialogex.h"
#include "string"


// TCP_Dialog 对话框

class TCP_Dialog : public CDialogEx
{
	DECLARE_DYNAMIC(TCP_Dialog)

public:
	TCP_Dialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~TCP_Dialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();

	// 初始化函数
	virtual BOOL OnInitDialog();

	// 判断字符串是否符合ip地址格式
	bool isIP(std::string ip);
	// 用户输入的ip
	CString ip;
	// 用户输入的端口
	int port;
	
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
