// TCP_Dialog.cpp: 实现文件
//

#include "pch.h"
#include "Application1.h"
#include "Application1Dlg.h"
#include "afxdialogex.h"
#include "iostream"
#include <regex>

// TCP_Dialog 对话框

IMPLEMENT_DYNAMIC(TCP_Dialog, CDialogEx)

TCP_Dialog::TCP_Dialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, ip(_T("192.168.10.10"))
	, port(502)
{

}

TCP_Dialog::~TCP_Dialog()
{
}

void TCP_Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, ip);
	DDX_Text(pDX, IDC_EDIT2, port);
}


BEGIN_MESSAGE_MAP(TCP_Dialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &TCP_Dialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &TCP_Dialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &TCP_Dialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &TCP_Dialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &TCP_Dialog::OnBnClickedButton3)
END_MESSAGE_MAP()


// TCP_Dialog 消息处理程序

BOOL TCP_Dialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化

	return TRUE;		// return TRUE unless you set the focus to a control
						// 异常: OCX 属性页应返回 FALSE
}


void TCP_Dialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
	if (MainDialog)
	{
		MainDialog->SendMessage(WM_TCP_Dialog_MESSAGE, 1, 0);
	}
	UpdateData(TRUE);
	if (!isIP(ip.GetBuffer(0)))			// CString转string
	{
		MessageBox("IP地址格式错误！", "警告", MB_ICONHAND);
	}
	else
	{
		MainDialog->tcp->cleanTCP();
		std::cout << ip << " " << port << std::endl;
		MainDialog->tcp->setIPandPort(ip.GetBuffer(0), port);
		MessageBox("设置成功！", "成功", MB_OK);
	}
}


void TCP_Dialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


bool TCP_Dialog::isIP(std::string ip)
{
	std::regex ipPattern(R"((\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3}))");

	// 检查格式是否匹配
	if (!std::regex_match(ip, ipPattern)) {
		return false;
	}

	// 进一步检查每个数字是否在0-255之间
	std::smatch match;
	std::regex_match(ip, match, ipPattern);
	for (int i = 1; i <= 4; ++i) {
		int num = std::stoi(match[i]);
		if (num < 0 || num > 255) {
			return false;
		}
	}

	return true;
}


// sendmsg
void TCP_Dialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
	if (MainDialog)
	{
		MainDialog->SendMessage(WM_TCP_Dialog_MESSAGE, 2, 0);
	}
}


// clean
void TCP_Dialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
	MainDialog->tcp->cleanTCP();
}


// receive
void TCP_Dialog::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
	std::string msg = MainDialog->tcp->recvMsg();
	if (msg == "1" || msg == "2")
		std::cout << "error receive" << std::endl;
	else
		std::cout << msg << std::endl;
}
