#pragma once
#include "afxdialogex.h"


// Setting_Dialog 对话框

class Setting_Dialog : public CDialogEx
{
	DECLARE_DYNAMIC(Setting_Dialog)

public:
	Setting_Dialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Setting_Dialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio2();
	int SetTriggerMode1();
	int SetTriggerMode2();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedButton3();

	// 相机1曝光
	float exposure1;
	// 相机2曝光
	float exposure2;
	// 相机1增益
	float gain1;
	// 相机2增益
	float gain2;
	// 相机1帧率
	float frame1;
	// 相机2帧率
	float frame2;
	// 相机触发模式
	int trigger[2];
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	void getTriggerMode();
};
