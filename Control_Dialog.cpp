// Control_Dialog.cpp: 实现文件
//

#include "pch.h"
#include "Application1.h"
#include "afxdialogex.h"
#include "Control_Dialog.h"
#include "Application1Dlg.h"


// Control_Dialog 对话框

IMPLEMENT_DYNAMIC(Control_Dialog, CDialogEx)

Control_Dialog::Control_Dialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, J1_world(0)
	, J2_world(0)
	, J3_world(0)
	, J4_world(0)
	, J5_world(0)
	, J6_world(0)
{

}

Control_Dialog::~Control_Dialog()
{
}

void Control_Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, J1_world);
	DDX_Text(pDX, IDC_EDIT2, J2_world);
	DDX_Text(pDX, IDC_EDIT7, J3_world);
	DDX_Text(pDX, IDC_EDIT8, J4_world);
	DDX_Text(pDX, IDC_EDIT9, J5_world);
	DDX_Text(pDX, IDC_EDIT10, J6_world);
}


BEGIN_MESSAGE_MAP(Control_Dialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &Control_Dialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON13, &Control_Dialog::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &Control_Dialog::OnBnClickedButton14)
END_MESSAGE_MAP()


// Control_Dialog 消息处理程序


void Control_Dialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
}


// getWorld
void Control_Dialog::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
	if (MainDialog)
	{
		MainDialog->SendMessage(WM_Control_Dialog_MESSAGE, 1, 0);
	}
}


// getJ
void Control_Dialog::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
	CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
	if (MainDialog)
	{
		MainDialog->SendMessage(WM_Control_Dialog_MESSAGE, 2, 0);
	}
}
