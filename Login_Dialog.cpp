// Login_Dialog.cpp: 实现文件
//

#include "pch.h"
#include "Application1.h"
#include "afxdialogex.h"
#include "Login_Dialog.h"
#include "Application1Dlg.h"


// Login_Dialog 对话框

IMPLEMENT_DYNAMIC(Login_Dialog, CDialogEx)

Login_Dialog::Login_Dialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent)
	, admin(_T("admin"))
	, word(_T(""))
{

}

Login_Dialog::~Login_Dialog()
{
}

void Login_Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, admin);
	DDX_Text(pDX, IDC_EDIT2, word);
}


BEGIN_MESSAGE_MAP(Login_Dialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &Login_Dialog::OnBnClickedOk)
END_MESSAGE_MAP()


// Login_Dialog 消息处理程序


void Login_Dialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (admin == "admin" && word == "zju123")
	{
		CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
		if (MainDialog)
		{
			MessageBox("log in successfully!");
			MainDialog->Loginsign = TRUE;
			PostMessage(WM_CLOSE);
		}
		else
			MessageBox("log in error, please try again");
	}
	else
	{
		MessageBox("cannot log in");
	}
}
