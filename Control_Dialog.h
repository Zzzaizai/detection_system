#pragma once
#include "afxdialogex.h"


// Control_Dialog 对话框

class Control_Dialog : public CDialogEx
{
	DECLARE_DYNAMIC(Control_Dialog)

public:
	Control_Dialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Control_Dialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	// 世界坐标J1
	float J1_world;
	// J2
	float J2_world;
	// J3
	float J3_world;
	// J4
	float J4_world;
	// J5
	float J5_world;
	// J6
	float J6_world;
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
};
