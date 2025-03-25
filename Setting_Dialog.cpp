// Setting_Dialog.cpp: 实现文件
//

#include "pch.h"
#include "Application1.h"
#include "afxdialogex.h"
#include "Application1Dlg.h"


// Setting_Dialog 对话框

IMPLEMENT_DYNAMIC(Setting_Dialog, CDialogEx)

Setting_Dialog::Setting_Dialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
    , exposure1(0)
    , exposure2(0)
    , gain1(0)
    , gain2(0)
    , frame1(0)
    , frame2(0)
{

}

Setting_Dialog::~Setting_Dialog()
{
}

void Setting_Dialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, exposure1);
    DDX_Text(pDX, IDC_EDIT2, exposure2);
    DDX_Text(pDX, IDC_EDIT8, gain1);
    DDX_Text(pDX, IDC_EDIT10, gain2);
    DDX_Text(pDX, IDC_EDIT9, frame1);
    DDX_Text(pDX, IDC_EDIT11, frame2);
}


BEGIN_MESSAGE_MAP(Setting_Dialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Setting_Dialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO3, &Setting_Dialog::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO2, &Setting_Dialog::OnBnClickedRadio2)
    ON_BN_CLICKED(IDC_BUTTON2, &Setting_Dialog::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_RADIO4, &Setting_Dialog::OnBnClickedRadio4)
    ON_BN_CLICKED(IDC_RADIO5, &Setting_Dialog::OnBnClickedRadio5)
    ON_BN_CLICKED(IDC_BUTTON3, &Setting_Dialog::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON5, &Setting_Dialog::OnBnClickedButton5)
    ON_BN_CLICKED(IDC_BUTTON4, &Setting_Dialog::OnBnClickedButton4)
    ON_BN_CLICKED(IDC_BUTTON6, &Setting_Dialog::OnBnClickedButton6)
END_MESSAGE_MAP()


// 初始化
BOOL Setting_Dialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
    CWnd* pWnd1 = GetDlgItem(IDC_STATIC1);
    if (pWnd1)
    {
        std::cout << "显示句柄1获取成功" << std::endl;
        MainDialog->m_hwndDisplay1 = pWnd1->GetSafeHwnd();
    }
    CWnd* pWnd2 = GetDlgItem(IDC_STATIC2);
    if (pWnd2)
    {
        std::cout << "显示句柄2获取成功" << std::endl;
        MainDialog->m_hwndDisplay2 = pWnd2->GetSafeHwnd();
    }

    getTriggerMode();
    if (trigger[0] == MV_TRIGGER_MODE_OFF)
        OnBnClickedRadio3();
    else if (trigger[0] == MV_TRIGGER_MODE_ON)
        OnBnClickedRadio2();
    if (trigger[1] == MV_TRIGGER_MODE_OFF)
        OnBnClickedRadio5();
    else if (trigger[1] == MV_TRIGGER_MODE_ON)
        OnBnClickedRadio4();
    OnBnClickedButton3();
    OnBnClickedButton5();



    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


// Setting_Dialog 消息处理程序
// camera1开始采集按钮
void Setting_Dialog::OnBnClickedButton1()
{
    // TODO: 在此添加控件通知处理程序代码
    CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
    if (MainDialog) {
        MainDialog->SendMessage(WM_Setting_Dialog_MESSAGE, 1, 0);
    }
}

// camera1连续模式
void Setting_Dialog::OnBnClickedRadio3()
{
    // TODO: 在此添加控件通知处理程序代码
    CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
    ((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(TRUE);
    MainDialog->m_nTriggerMode1 = MV_TRIGGER_MODE_OFF;
    int nRet = SetTriggerMode1();
    if (MV_OK != nRet)
    {
        std::cout << "Set Trigger Mode Fail" << nRet << std::endl;
        return;
    }
}

// camera1软触发模式
void Setting_Dialog::OnBnClickedRadio2()
{
    // TODO: 在此添加控件通知处理程序代码
    CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
    ((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);
    MainDialog->m_nTriggerMode1 = MV_TRIGGER_MODE_ON;
    int nRet = SetTriggerMode1();
    if (MV_OK != nRet)
    {
        std::cout << "Set Trigger Mode Fail" << nRet << std::endl;
        return;
    }
}

// camera1设置触发模式
int Setting_Dialog::SetTriggerMode1()
{
    CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
    return MainDialog->camera1->SetEnumValue("TriggerMode", MainDialog->m_nTriggerMode1);
}

// camera2开始采集按钮
void Setting_Dialog::OnBnClickedButton2()
{
    // TODO: 在此添加控件通知处理程序代码
    CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
    if (MainDialog) {
        MainDialog->SendMessage(WM_Setting_Dialog_MESSAGE, 2, 0);
    }
}

// camera2软触发模式
void Setting_Dialog::OnBnClickedRadio4()
{
    // TODO: 在此添加控件通知处理程序代码
    CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
    ((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(TRUE);
    MainDialog->m_nTriggerMode2 = MV_TRIGGER_MODE_ON;
    int nRet = SetTriggerMode2();
    if (MV_OK != nRet)
    {
        std::cout << "Set Trigger Mode Fail" << nRet << std::endl;
        return;
    }
}

// camera2连续采集模式
void Setting_Dialog::OnBnClickedRadio5()
{
    // TODO: 在此添加控件通知处理程序代码
    CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
    ((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);
    ((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(TRUE);
    MainDialog->m_nTriggerMode2 = MV_TRIGGER_MODE_OFF;
    int nRet = SetTriggerMode2();
    if (MV_OK != nRet)
    {
        std::cout << "Set Trigger Mode Fail" << nRet << std::endl;
        return;
    }
}

// camera2设置触发模式
int Setting_Dialog::SetTriggerMode2()
{
    CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
    return MainDialog->camera2->SetEnumValue("TriggerMode", MainDialog->m_nTriggerMode2);
}

// camera1获取参数
void Setting_Dialog::OnBnClickedButton3()
{
    // TODO: 在此添加控件通知处理程序代码
    CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
    if (MainDialog) {
        MainDialog->SendMessage(WM_Setting_Dialog_MESSAGE, 3, 0);
    }
    Sleep(500);
    float* param = MainDialog->cameraParam;
    exposure1 = param[0];
    gain1 = param[1];
    frame1 = param[2];
    UpdateData(FALSE);
}

// camera2获取参数
void Setting_Dialog::OnBnClickedButton5()
{
    // TODO: 在此添加控件通知处理程序代码
    CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
    if (MainDialog) {
        MainDialog->SendMessage(WM_Setting_Dialog_MESSAGE, 4, 0);
    }
    Sleep(500);
    float* param = MainDialog->cameraParam;
    exposure2 = param[3];
    gain2 = param[4];
    frame2 = param[5];
    UpdateData(FALSE);
}

// camera1设置参数
void Setting_Dialog::OnBnClickedButton4()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
    float param[3];
    param[0] = exposure1;
    param[1] = gain1;
    param[2] = frame1;
    LPARAM lParam = reinterpret_cast<LPARAM>(param);
    if (MainDialog) {
        MainDialog->SendMessage(WM_Setting_Dialog_MESSAGE, 5, lParam);
    }
}

// camera2设置参数
void Setting_Dialog::OnBnClickedButton6()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
    float param[3];
    param[0] = exposure2;
    param[1] = gain2;
    param[2] = frame2;
    LPARAM lParam = reinterpret_cast<LPARAM>(param);
    if (MainDialog) {
        MainDialog->SendMessage(WM_Setting_Dialog_MESSAGE, 6, lParam);
    }
}

// 获取两个相机触发模式
void Setting_Dialog::getTriggerMode()
{
    CApplication1Dlg* MainDialog = (CApplication1Dlg*)AfxGetMainWnd();
    if (MainDialog) {
        MainDialog->SendMessage(WM_Setting_Dialog_MESSAGE, 7, 0);
    }
    trigger[0] = MainDialog->m_TriggerMode[0];
    trigger[1] = MainDialog->m_TriggerMode[1];
}