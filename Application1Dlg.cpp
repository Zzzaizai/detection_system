
// Application1Dlg.cpp: 实现文件
//
#include "pch.h"
#include "framework.h"
#include "Application1.h"
#include "Application1Dlg.h"
#include "afxdialogex.h"

#include "MvCameraControl.h"
#include "MvCamera.h"
#include "iostream"
#include "fstream"
#include <windows.h>
#include <stdio.h>
#include <string>
#include <ctime>

#include "opencv2/opencv.hpp"
#include "zbar_include/zbar.h"

using json = nlohmann::json;

#pragma warning( disable : 4996 )

//int globleVar = 0;
//bool isStop = false;
//std::string log_message = "";

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CApplication1Dlg 对话框



CApplication1Dlg::CApplication1Dlg(CWnd* pParent /*=nullptr*/)		// 成员变量初始化
	: CDialogEx(IDD_APPLICATION1_DIALOG, pParent)
	, log_message(_T(""))
	, isStop(FALSE)
	, globleVar(0)
	, m_pSaveImageBuf(NULL)
	, m_nSaveImageBufSize(0)
	, isGrab(FALSE)
	, m_bThreadState1(FALSE)
	, m_bThreadState2(FALSE)
	, m_bStartGrabbing1(FALSE)
	, m_bStartGrabbing2(FALSE)
	, m_bOpenDevice1(FALSE)
	, m_bOpenDevice2(FALSE)
	, m_nTriggerMode1(MV_TRIGGER_MODE_ON)
	, m_nTriggerMode2(MV_TRIGGER_MODE_ON)
	, Loginsign(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CApplication1Dlg::DoDataExchange(CDataExchange* pDX)			// 变量与控件关联
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, log_message);					// 日志消息关联IDC_EDIT3
	DDV_MaxChars(pDX, log_message, 1024);
	DDX_Control(pDX, IDC_BUTTON_START, button_start);		// 开始检测按钮关联IDC_BUTTON_START
	DDX_Control(pDX, IDC_BUTTON_STOP, button_stop);
}

BEGIN_MESSAGE_MAP(CApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_START, &CApplication1Dlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CApplication1Dlg::OnBnClickedButtonStop)
	ON_MESSAGE(WM_UPDATEDATA, &CApplication1Dlg::OnUpdateData)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(ID_TEST2_1, &CApplication1Dlg::OnTCP)
	ON_BN_CLICKED(IDC_BUTTON1, &CApplication1Dlg::OnBnClickedButton1)
	ON_COMMAND(ID_TEST2_2, &CApplication1Dlg::OnControl)
	ON_COMMAND(ID_TEST2_3, &CApplication1Dlg::OnSetting)
	ON_MESSAGE(WM_Setting_Dialog_MESSAGE, OnSettingMessage)
	ON_MESSAGE(WM_Control_Dialog_MESSAGE, OnControlMessage)
	ON_MESSAGE(WM_TCP_Dialog_MESSAGE, OnTCPMessage)
	ON_COMMAND(ID_TEST1_1, &CApplication1Dlg::OnTestLogin)
END_MESSAGE_MAP()


// CApplication1Dlg 消息处理程序


// 初始化主界面
BOOL CApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	login_window.DoModal();
	if (Loginsign)
	{
		InitCamera();									// 初始化相机
		UpdateData(FALSE);								// 更新初始化相机时的日志
		InitializeCriticalSection(&m_hSaveImageMux);	// 初始化临界区
		tcp = new TCPClient();							// 定义TCP对象，用于打开网口进行指令传输
	}
	else
	{
		OnBnClickedButton1();
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();					// 调用父类的OnPaint方法
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 子线程1，用于拍摄、裁剪、保存、显示图片
static UINT WorkerThread(LPVOID pParam)
{
	// 在这里执行耗时操作
	CApplication1Dlg* pThis = (CApplication1Dlg*)pParam;  // 获取传入的类指针
	int imgNum = 0;		// 计数

	// 用pThis调用类的成员函数
	pThis->NewSampleInit();
	pThis->PostMessage(WM_UPDATEDATA, FALSE);
	pThis->SendDlgItemMessage(IDC_EDIT3, WM_VSCROLL, SB_BOTTOM, 0);
	Sleep(500);

	while (true)
	{
		Sleep(200);
		if (pThis->isStop)
		{
			std::string str = "检测停止";
			pThis->SetDlgItemText(IDC_EDIT1, str.c_str());
			return 0;		// 提前结束线程，不计数
		}
		if (pThis->ReceiveGrabCommand())
		{
  			pThis->isGrab = false;
			if (imgNum == 0 || imgNum == 1)
				int ret = pThis->GrabImg(imgNum, 1);
			else
				int ret = pThis->GrabImg(imgNum, 2);
			Sleep(500);
			if (imgNum == 0)
			{
				int type = pThis->QRRecognition();		// 从未裁剪照片中读取二维码
				std::string DisplayType;
				switch (type)
				{
				case 1:
					DisplayType = "Type 00136: 1156*1254.7mm";
					break;
				case 2:
					DisplayType = "Type 00146: 1847*1358mm";
					break;
				case 3:
					DisplayType = "Type 00237: 1430*867mm";
					break;
				case 4:
					DisplayType = "Type 00242: 1995*1058mm";
					break;
				case 5:
					DisplayType = "Type 00399: 1646.5*1314mm";
					break;
				case 0:
					DisplayType = "unrecognizable or not detect QR code";
					break;
				default:
					break;
				}
				pThis->SetDlgItemText(IDC_EDIT4, DisplayType.c_str());
			}
			pThis->CropImg(imgNum);
			pThis->ShowImgfromfile(imgNum);
			pThis->PostMessage(WM_UPDATEDATA, FALSE);						// 在消息框更新日志消息，DoDataExchenge中已关联日志变量至消息框
			pThis->SendDlgItemMessage(IDC_EDIT3, WM_VSCROLL, SB_BOTTOM, 0);
			Sleep(500);
			imgNum++;
		}
		if (imgNum >= 8)	// 检测完成
		{
			std::string str = "检测完成";
			pThis->SetDlgItemText(IDC_EDIT1, str.c_str());
			pThis->globleVar += 1;
			pThis->stop();
			return 0;
		}
	}
	return 0;  // 线程结束
}


// 子线程2，用于接受机械臂消息
static UINT WorkerThreadReceive(LPVOID pParam)
{
	CApplication1Dlg* pThis = (CApplication1Dlg*)pParam;  // 获取传入的类指针
	std::string msg = "";
	while (TRUE)
	{
		msg = pThis->tcp->recvMsg();
		Sleep(500);
		if (msg == "1" || msg == "2")
		{
			pThis->stop();
			pThis->MessageBox("TCP通信未设置！", "错误", MB_ICONHAND);
			break;
		}
		else if (msg != "")
		{
			std::cout << msg << std::endl;
			pThis->m = pThis->SolveMsg(msg);
			if (pThis->m.state == TRUE && pThis->m.packID == "1")		// 查询版本与轴数
			{
				pThis->m.state = FALSE;
				pThis->isGrab = FALSE;
				std::cout << "packID: " << pThis->m.packID << std::endl;
				std::cout << "version: " << pThis->m.queryData[0] << std::endl;
				std::cout << "axisNum: " << pThis->m.queryData[1] << std::endl;
			}
			else if (pThis->m.state == TRUE && pThis->m.packID == "2")	// 查询世界坐标
			{
				pThis->m.state = FALSE;
				pThis->isGrab = FALSE;
				std::cout << "packID: " << pThis->m.packID << std::endl;
				std::cout << "世界坐标：" << std::endl;
				std::cout << pThis->m.queryData[0] << "" << pThis->m.queryData[1] << "" << pThis->m.queryData[2] << std::endl;
				std::cout << pThis->m.queryData[3] << "" << pThis->m.queryData[4] << "" << pThis->m.queryData[5] << std::endl;
			}
			else if (pThis->m.state == TRUE && pThis->m.packID == "3")	// 查询轴坐标
			{
				pThis->m.state = FALSE;
				pThis->isGrab = FALSE;
				std::cout << "packID: " << pThis->m.packID << std::endl;
				std::cout << "轴坐标：" << std::endl;
				std::cout << pThis->m.queryData[0] << "" << pThis->m.queryData[1] << "" << pThis->m.queryData[2] << std::endl;
				std::cout << pThis->m.queryData[3] << "" << pThis->m.queryData[4] << "" << pThis->m.queryData[5] << std::endl;
			}
			else if (pThis->m.state == TRUE && pThis->m.packID == "4")	// 立即停止
			{
				pThis->m.state = FALSE;
				pThis->isGrab = FALSE;
				pThis->stop();
				std::cout << "packID: " << pThis->m.packID << std::endl;
				std::cout << "立即停止"<< std::endl;
			}
			else if (pThis->m.state == TRUE && pThis->m.packID == "5")	// 成功设置复位标识为0，机械臂开始操作
			{
				pThis->m.state = FALSE;
				pThis->isGrab = FALSE;
				pThis->isStop = FALSE;
				std::cout << "packID: " << pThis->m.packID << std::endl;
				std::cout << "复位标识置0成功" << std::endl;
			}
			else if (pThis->m.state == TRUE && pThis->m.packID == "6")	// 成功设置复位标识为1，机械臂停止操作
			{
				pThis->m.state = FALSE;
				pThis->isGrab = FALSE;
				pThis->isStop = TRUE;
				std::cout << "packID: " << pThis->m.packID << std::endl;
				std::cout << "复位标识置1成功" << std::endl;
			}
			else if (pThis->m.state == TRUE && pThis->m.packID == "7")	// 读取复位标识
			{
				std::string sign = pThis->m.queryData[0];
				std::cout << "packID: " << pThis->m.packID << std::endl;
				std::cout << "读取复位标识成功" << std::endl;
				if (sign == "0")
				{
					pThis->m.state = FALSE;
					pThis->isGrab = FALSE;
					pThis->isStop = FALSE;
					std::cout << "复位标识为0" << std::endl;
				}
				else if (sign == "1")
				{
					pThis->m.state = FALSE;
					pThis->isGrab = FALSE;
					pThis->isStop = TRUE;
					std::cout << "复位标识为1" << std::endl;
				}
				else
					pThis->m.state = FALSE;
			}
			else if (pThis->m.state == TRUE && pThis->m.packID == "8")	// 成功设置拍摄标识为0，相机停止拍摄
			{
				pThis->m.state = FALSE;
				pThis->isGrab = FALSE;
				pThis->isStop = FALSE;
				std::cout << "packID: " << pThis->m.packID << std::endl;
				std::cout << "拍摄标识置0成功" << std::endl;
			}
			else if (pThis->m.state == TRUE && pThis->m.packID == "9")	// 成功设置拍摄标识为1，相机可以拍摄
			{
				pThis->m.state = FALSE;
				pThis->isGrab = TRUE;
				pThis->isStop = FALSE;
				std::cout << "packID: " << pThis->m.packID << std::endl;
				std::cout << "拍摄标识置1成功" << std::endl;
			}
			else if (pThis->m.state == TRUE && pThis->m.packID == "10")	// 读取相机拍摄标识
			{
				std::string sign = pThis->m.queryData[0];
				std::cout << "packID: " << pThis->m.packID << std::endl;
				std::cout << "读取拍摄标识成功" << std::endl;
				if (sign == "0")
				{
					pThis->m.state = FALSE;
					pThis->isGrab = FALSE;
					pThis->isStop = FALSE;
					std::cout << "拍摄标识为0" << std::endl;
				}
				else if (sign == "1")
				{
					pThis->m.state = FALSE;
					pThis->isGrab = TRUE;
					pThis->isStop = FALSE;
					std::cout << "拍摄标识为1" << std::endl;
				}
				else
					pThis->m.state = FALSE;
			}
		}
		msg = "";
	}
	return 0;
}


// 开始检测按钮事件处理函数
void CApplication1Dlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	// 启动计时器，每秒触发一次
	button_start.EnableWindow(FALSE);
	m_nTimeCounter = 0;  // 初始化计时器
	m_nTimerID = SetTimer(1, 100, NULL);  // 每100ms触发一次，ID为1
	isStop = false;
	isGrab = false;
	
	AfxBeginThread(WorkerThread, this);				// 启动拍照线程，传递类的 this 指针
	AfxBeginThread(WorkerThreadReceive, this);		// 启动接受机械臂指令线程
}


// 停止检测按钮事件处理函数
void CApplication1Dlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	SendCommand("setStop");
	stop();
}


// 停止，停止标识置true，停止计时器，按钮恢复
void CApplication1Dlg::stop()
{
	isStop = true;
	KillTimer(m_nTimerID);  // 停止计时器
	button_start.EnableWindow(TRUE);
}


// 计时器函数
void CApplication1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)  // 如果是计时器1触发
	{
		m_nTimeCounter += 0.1;  // 增加计时器计数

		// 格式化时间并更新 Edit Control
		CString strTime;
		strTime.Format(_T("%.2f 秒"), m_nTimeCounter);  // 格式化时间
		SetDlgItemText(IDC_EDIT2, strTime);  // 更新 Edit Control 显示
	}

	// 调用父类的OnTimer函数
	CDialogEx::OnTimer(nIDEvent);
}


// 拍摄开始前的准备工作
void CApplication1Dlg::NewSampleInit()
{
	std::string str = "检测中......";
	SetDlgItemText(IDC_EDIT1, str.c_str());
	SetDlgItemText(IDC_EDIT4, "");
	for (int i = 0; i < 8; i++)
	{
		GetDlgItem(IDC_IMG1 + i)->ShowWindow(FALSE);		// 取消所有图片显示
		GetDlgItem(IDC_IMG1 + i)->ShowWindow(TRUE);
	}
	CString message;
	message.Format(_T("第%d张水冷板"), globleVar + 1);
	tip(message);
}


// 初始化并打开设备，分别对应camera1和camera2两个对象
void CApplication1Dlg::InitCamera()
{
	// 1. 初始化SDK
	int ret = CMvCamera::InitSDK();
	if (ret != MV_OK)
	{
		std::cerr << "Failed to initialize SDK!" << std::endl;
		return;
	}
	std::cout << "SDK initialized successfully!" << std::endl;
	tip("初始化成功");
	
	// 2. 枚举设备
	MV_CC_DEVICE_INFO_LIST devList;
	memset(&devList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
	ret = CMvCamera::EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &devList);
	if (ret != MV_OK || devList.nDeviceNum == 0)
	{
		std::cerr << "No devices found!" << std::endl;
		CMvCamera::FinalizeSDK();
		return;
	}
	std::cout << "Found " << devList.nDeviceNum << " devices!" << std::endl;

	char* device1 = { "MV-CS200-10UC" };
	char* device2 = { "MV-CS060-10UM-PRO" };
	int devNum = 0;

	// 3. 打开第一个设备
	for (int num = 0; num < devList.nDeviceNum; num++)
	{
		if (strcmp((const char*)devList.pDeviceInfo[num]->SpecialInfo.stUsb3VInfo.chModelName, device1) == 0)
		{
			devNum = num;
			std::cout << "相机1型号：" << devList.pDeviceInfo[num]->SpecialInfo.stUsb3VInfo.chModelName << std::endl;
			break;
		}
	}

	MV_CC_DEVICE_INFO* pDeviceInfo1 = devList.pDeviceInfo[devNum];
	camera1 = new CMvCamera();
	ret = camera1->Open(pDeviceInfo1);
	if (ret != MV_OK)
	{
		std::cerr << "Failed to open camera1!" << std::endl;
		return;
	}
	m_bOpenDevice1 = TRUE;
	std::cout << "Camera1 opened successfully!" << std::endl;

	ret = camera1->StartGrabbing();
	if (ret != MV_OK)
		std::cout << "failed to start grabbing1" << std::endl;
	else
	{
		m_bStartGrabbing1 = TRUE;
		std::cout << "start grabbing1 successfully" << std::endl;
		tip("相机1打开成功，开始取流");
	}

	// 4. 打开第二个设备
	for (int num = 0; num < devList.nDeviceNum; num++)
	{
		if (strcmp((const char*)devList.pDeviceInfo[num]->SpecialInfo.stUsb3VInfo.chModelName, device2) == 0)
		{
			devNum = num;
			std::cout << "相机2型号：" << devList.pDeviceInfo[num]->SpecialInfo.stUsb3VInfo.chModelName << std::endl;
			break;
		}
	}

	MV_CC_DEVICE_INFO* pDeviceInfo2 = devList.pDeviceInfo[devNum];
	camera2 = new CMvCamera();
	ret = camera2->Open(pDeviceInfo2);
	if (ret != MV_OK)
	{
		std::cerr << "Failed to open camera2!" << std::endl;
		return;
	}
	m_bOpenDevice2 = TRUE;
	std::cout << "Camera2 opened successfully!" << std::endl;

	ret = camera2->StartGrabbing();
	if (ret != MV_OK)
		std::cout << "failed to start grabbing2" << std::endl;
	else
	{
		m_bStartGrabbing2 = TRUE;
		std::cout << "start grabbing2 successfully" << std::endl;
		tip("相机2打开成功，开始取流");
	}
}


// 获取1帧图片并保存本地
int CApplication1Dlg::GrabImg(int i, int cameraNum)
{
	// 具体流程：
	// 相机获取一帧stImageInfo---->数据缓冲区m_pSaveImageBuf---->待保存图像stImage------->保存成功
	//                |												|					|
	//                --------->信息缓冲区m_stImageInfo------>--------					|
	//																				  保存参数stSaveImageParam
	if (cameraNum == 1)
	{
		MV_FRAME_OUT stImageInfo1 = { 0 };
		int nRet = camera1->GetImageBuffer(&stImageInfo1, 1000);	// 从相机获取图像信息
		std::cout << nRet << std::endl;
		if (nRet == MV_OK)
		{
			std::cout << "successfully grab an image" << std::endl;

			//用于保存图片
			EnterCriticalSection(&m_hSaveImageMux);					// 使用m_hSaveImageMux进入临界区，保证多线程访问不会冲突
			if (NULL == m_pSaveImageBuf || stImageInfo1.stFrameInfo.nFrameLenEx > m_nSaveImageBufSize)	// 判断缓冲区是否足够，若足够不执行if内语句
			{
				if (m_pSaveImageBuf)
				{
					free(m_pSaveImageBuf);
					m_pSaveImageBuf = NULL;
				}

				m_pSaveImageBuf = (unsigned char*)malloc(sizeof(unsigned char) * stImageInfo1.stFrameInfo.nFrameLenEx);
				if (m_pSaveImageBuf == NULL)
				{
					LeaveCriticalSection(&m_hSaveImageMux);
					return 0;
				}
				m_nSaveImageBufSize = stImageInfo1.stFrameInfo.nFrameLenEx;
			}
			memcpy(m_pSaveImageBuf, stImageInfo1.pBufAddr, stImageInfo1.stFrameInfo.nFrameLenEx);	// 拷贝图像数据
			memcpy(&m_stImageInfo, &(stImageInfo1.stFrameInfo), sizeof(MV_FRAME_OUT_INFO_EX));		// 拷贝图像信息
			LeaveCriticalSection(&m_hSaveImageMux);

			memset(&stImage, 0, sizeof(MV_CC_IMAGE));							// 待保存图像stImage = 数据 + 信息
			MV_CC_SAVE_IMAGE_PARAM  stSaveImageParam;
			memset(&stSaveImageParam, 0, sizeof(MV_CC_SAVE_IMAGE_PARAM));		// 保存参数stSaveImageParam

			EnterCriticalSection(&m_hSaveImageMux);
			if (m_pSaveImageBuf == NULL || m_stImageInfo.enPixelType == 0)
			{
				LeaveCriticalSection(&m_hSaveImageMux);
				return MV_E_NODATA;
			}

			stImage.nWidth = m_stImageInfo.nExtendWidth;						// 为图像赋值
			stImage.nHeight = m_stImageInfo.nExtendHeight;
			stImage.enPixelType = m_stImageInfo.enPixelType;
			stImage.pImageBuf = m_pSaveImageBuf;
			stImage.nImageBufLen = m_stImageInfo.nFrameLenEx;

			stSaveImageParam.enImageType = MV_Image_Bmp;						// 为参数赋值
			stSaveImageParam.iMethodValue = 1;
			stSaveImageParam.nQuality = 99;

			char chImagePath[256] = { 0 };		// 相对路径
			std::string folder = getNow();
			std::string folderpath = folder + "\\" + std::to_string(globleVar);
			if (i == 0)
			{
				bool flag1 = CreateDirectory(folder.c_str(), NULL);			// 父文件夹
				bool flag2 = CreateDirectory(folderpath.c_str(), NULL);		// 子文件夹
			}			

			// ch:jpg图像质量范围为(50-99], png图像质量范围为[0-9] | en:jpg image nQuality range is (50-99], png image nQuality range is [0-9]
			if (MV_Image_Bmp == stSaveImageParam.enImageType)
			{
				sprintf_s(chImagePath, 256, "%s\\frame%d.bmp", folderpath.c_str(), i);
			}
			else if (MV_Image_Jpeg == stSaveImageParam.enImageType)
			{
				sprintf_s(chImagePath, 256, "%s\\frame%d.jpg", folderpath.c_str(), i);
			}
			else if (MV_Image_Tif == stSaveImageParam.enImageType)
			{
				sprintf_s(chImagePath, 256, "%s\\frame%d.tif", folderpath.c_str(), i);
			}
			else if (MV_Image_Png == stSaveImageParam.enImageType)
			{
				sprintf_s(chImagePath, 256, "%s\\frame%d.png", folderpath.c_str(), i);
			}

			int nRet = camera1->SaveImageToFile(&stImage, &stSaveImageParam, chImagePath);
			if (nRet == MV_OK)
			{
				std::cout << "save successfully" << std::endl;
				std::cout << m_stImageInfo.nExtendWidth << " x " << m_stImageInfo.nExtendHeight << std::endl;
				CString message;
				message.Format(_T("第%d幅图拍摄完成"), i + 1);
				tip(message);
			}
			LeaveCriticalSection(&m_hSaveImageMux);
			camera1->FreeImageBuffer(&stImageInfo1);

			return nRet;
		}
	}
	else if (cameraNum == 2)
	{
		MV_FRAME_OUT stImageInfo2 = { 0 };
		int nRet = camera2->GetImageBuffer(&stImageInfo2, 1000);
		if (nRet == MV_OK)
		{
			std::cout << "successfully grab an image" << std::endl;

			//用于保存图片
			EnterCriticalSection(&m_hSaveImageMux);
			if (NULL == m_pSaveImageBuf || stImageInfo2.stFrameInfo.nFrameLenEx > m_nSaveImageBufSize)
			{
				if (m_pSaveImageBuf)
				{
					free(m_pSaveImageBuf);
					m_pSaveImageBuf = NULL;
				}

				m_pSaveImageBuf = (unsigned char*)malloc(sizeof(unsigned char) * stImageInfo2.stFrameInfo.nFrameLenEx);
				if (m_pSaveImageBuf == NULL)
				{
					LeaveCriticalSection(&m_hSaveImageMux);
					return 0;
				}
				m_nSaveImageBufSize = stImageInfo2.stFrameInfo.nFrameLenEx;
			}
			memcpy(m_pSaveImageBuf, stImageInfo2.pBufAddr, stImageInfo2.stFrameInfo.nFrameLenEx);
			memcpy(&m_stImageInfo, &(stImageInfo2.stFrameInfo), sizeof(MV_FRAME_OUT_INFO_EX));
			LeaveCriticalSection(&m_hSaveImageMux);


			memset(&stImage, 0, sizeof(MV_CC_IMAGE));
			MV_CC_SAVE_IMAGE_PARAM  stSaveImageParam;
			memset(&stSaveImageParam, 0, sizeof(MV_CC_SAVE_IMAGE_PARAM));

			EnterCriticalSection(&m_hSaveImageMux);
			if (m_pSaveImageBuf == NULL || m_stImageInfo.enPixelType == 0)
			{
				LeaveCriticalSection(&m_hSaveImageMux);
				return MV_E_NODATA;
			}

			stImage.nWidth = m_stImageInfo.nExtendWidth;
			stImage.nHeight = m_stImageInfo.nExtendHeight;
			stImage.enPixelType = m_stImageInfo.enPixelType;
			stImage.pImageBuf = m_pSaveImageBuf;
			stImage.nImageBufLen = m_stImageInfo.nFrameLenEx;

			stSaveImageParam.enImageType = MV_Image_Bmp;
			stSaveImageParam.iMethodValue = 1;
			stSaveImageParam.nQuality = 99;

			char chImagePath[256] = { 0 };
			std::string folder = getNow();
			std::string folderpath = folder + "\\" + std::to_string(globleVar);
			if (i == 0)
			{
				bool flag1 = CreateDirectory(folder.c_str(), NULL);
				bool flag2 = CreateDirectory(folderpath.c_str(), NULL);
			}
				
			if (MV_Image_Bmp == stSaveImageParam.enImageType)
			{
				sprintf_s(chImagePath, 256, "%s\\frame%d.bmp", folderpath.c_str(), i);
			}
			else if (MV_Image_Jpeg == stSaveImageParam.enImageType)
			{
				sprintf_s(chImagePath, 256, "%s\\frame%d.jpg", folderpath.c_str(), i);
			}
			else if (MV_Image_Tif == stSaveImageParam.enImageType)
			{
				sprintf_s(chImagePath, 256, "%s\\frame%d.tif", folderpath.c_str(), i);
			}
			else if (MV_Image_Png == stSaveImageParam.enImageType)
			{
				sprintf_s(chImagePath, 256, "%s\\frame%d.png", folderpath.c_str(), i);
			}

			int nRet = camera2->SaveImageToFile(&stImage, &stSaveImageParam, chImagePath);
			if (nRet == MV_OK)
			{
				std::cout << "save successfully" << std::endl;
				std::cout << m_stImageInfo.nExtendWidth << " x " << m_stImageInfo.nExtendHeight << std::endl;
				CString message;
				message.Format(_T("第%d幅图拍摄完成"), i + 1);
				tip(message);
			}
			LeaveCriticalSection(&m_hSaveImageMux);
			camera2->FreeImageBuffer(&stImageInfo2);

			return nRet;
		}
	}
}


// 从缓存中读取图片显示
/*
void CApplication1Dlg::ShowImg()
{
	if (stImage.pImageBuf == nullptr)
	{
		AfxMessageBox(_T("No image data to display"));
		return;
	}

	// 创建 CImage 对象并准备显示图像
	CImage image;
	HRESULT hr = image.Create(stImage.nWidth, stImage.nHeight, 8);  // 24-bit RGB format
	byte* pS;
	byte* pImg = (byte*)image.GetBits();
	int step = image.GetPitch();
	int height = image.GetHeight();
	int width = image.GetWidth();

	// 将图像数据复制到 CImage 对象
	for (int i = 0; i < image.GetHeight(); ++i)
	{
		pS = stImage.pImageBuf + i * width;
		for (int j = 0; j < image.GetWidth(); ++j)
		{
			*(pImg + i * step + j) = pS[j];
		}
	}

	// 获取控件的大小
	CWnd* pWnd = GetDlgItem(IDC_IMG3); // 假设 IDC_IMG1 是你的图像控件 ID
	CRect rect;
	pWnd->GetClientRect(&rect);  // 获取控件的客户区域大小

	// 计算控件的宽度和高度
	int nCtrlWidth = rect.Width();
	int nCtrlHeight = rect.Height();

	// 计算按比例缩放后的宽高
	float fScaleX = (float)nCtrlWidth / (float)stImage.nWidth;
	float fScaleY = (float)nCtrlHeight / (float)stImage.nHeight;
	float fScale = min(fScaleX, fScaleY); // 按最小比例缩放，避免图像变形

	int nNewWidth = (int)(stImage.nWidth * fScale);  // 按比例计算缩放后的宽度
	int nNewHeight = (int)(stImage.nHeight * fScale);  // 按比例计算缩放后的高度

	// 创建一个内存设备上下文 (CDC) 用于绘制
	CDC* pDC = pWnd->GetDC();

	// 使用 StretchBlt 缩放图像
	image.StretchBlt(pDC->m_hDC, 0, 0, nNewWidth, nNewHeight, 0, 0, stImage.nWidth, stImage.nHeight, SRCCOPY);

	// 释放 DC
	pWnd->ReleaseDC(pDC);
}
*/


// 从文件夹中读取图片显示
void CApplication1Dlg::ShowImgfromfile(int i)
{
	// 第i个图片
	std::string filename = getNow() + "\\" + std::to_string(globleVar) + "\\cropped_frame" + std::to_string(i) + ".bmp";
	std::cout << filename << std::endl;

	// 获取Picture Control控件
	CStatic* pPictureCtrl = (CStatic*)GetDlgItem(IDC_IMG1 + i);

	// 创建CImage对象并加载图片
	CImage image;
	image.Load(filename.c_str());

	// 获取Picture Control的客户区大小
	CRect rect;
	pPictureCtrl->GetClientRect(&rect);

	// 调整图片大小以适应控件
	image.StretchBlt(pPictureCtrl->GetDC()->GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	// 释放设备上下文
	pPictureCtrl->ReleaseDC(pPictureCtrl->GetDC());
	
}


// 判断机械臂是否到位，到位返回true
bool CApplication1Dlg::ReceiveGrabCommand()
{
	if (isGrab)
		return true;
	else
		return false;
}


// 显示日志
/*
void CApplication1Dlg::tip(std::string s)
{
	log_message += s;							//原有日志加上新日志（提示）信息
	log_message += _T("\r\n");					//添加换行符
	
	SetDlgItemText(IDC_EDIT3, log_message.c_str());
	
	//日志容器过大时清空一次。此时可选择先写入进txt或log日志文件后在清空。
	int size = sizeof(log_message);
	if (size > 1024)
		log_message = "";
}
*/


// 显示日志，使用CString类型
void CApplication1Dlg::tip(CString s)
{
	log_message.Append(s);
	log_message.Append(_T("\r\n"));
}


// 用于子线程向主线程发送UpdateData消息
LRESULT CApplication1Dlg::OnUpdateData(WPARAM wParam, LPARAM lParam) {
	UpdateData(wParam);  // wParam为TRUE/FALSE控制数据交换方向
	return 0;
}


// 获取当前时间
std::string CApplication1Dlg::getNow()
{
	// 获取当前时间戳
	std::time_t now = std::time(nullptr);

	// 转换为本地时间结构体
	std::tm* local_time = localtime(&now);

	// 格式化输出
	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "%Y%m%d", local_time);
	return buffer;
}


// 向机械臂发送指令
bool CApplication1Dlg::SendCommand(std::string command)
{
	std::string msgData = readJson(command);
	tcp->sendMsg(msgData.c_str());
	return TRUE;
}


// 接收信息函数
CApplication1Dlg::Msg CApplication1Dlg::SolveMsg(std::string msg)
{
	Msg solvedMsg;
	try {
		// 解析传入的JSON字符串
		json j = json::parse(msg); 
		// 遍历每一个键值对
		for (auto& el : j.items()) {
			// el.key() 获取键，el.value() 获取值
			if (el.key() == "dsID")
				solvedMsg.dsID = el.value();
			else if (el.key() == "packID")
				solvedMsg.packID = el.value();
			else if (el.key() == "reqType")
				solvedMsg.reqType = el.value();
			else if (el.key() == "queryData")
				solvedMsg.queryData = el.value().get<std::vector<std::string>>();
			else if (el.key() == "cmdReply")
				solvedMsg.cmdReply = el.value().get<std::vector<std::string>>();
			solvedMsg.state = TRUE;
		}
		std::cout << solvedMsg.reqType << std::endl;
	}
	catch (const nlohmann::json::parse_error& e) {
		// 捕获JSON解析错误
		std::cout << "JSON解析错误: " << e.what() << std::endl;
		solvedMsg.state = FALSE;
	}
	return solvedMsg;
}


// 传入指令id，返回格式化指令
std::string CApplication1Dlg::readJson(std::string targetId)
{
	// 读取 JSON 文件
	std::ifstream inputFile("command.json");
	if (!inputFile.is_open()) {
		std::cerr << "Failed to open the file!" << std::endl;
		return "error";
	}

	// 解析 JSON 数据
	json jsonData;
	try {
		inputFile >> jsonData;
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
		return "error";
	}

	// 查找并处理特定 "id" 的 JSON 对象
	for (auto& item : jsonData) {
		if (item.contains("id") && item["id"] == targetId) {
			// 删除 "id" 键值对
			item.erase("id");

			// 输出处理后的 JSON 数据
			std::cout << item.dump() << std::endl;
			return item.dump();
			break;
		}
	}

	return "error";
}


// 裁剪图片
void CApplication1Dlg::CropImg(int i)
{
	std::string folder = getNow();
	std::string folderpath = folder + "\\" + std::to_string(globleVar);
	std::string imgPath = folderpath + "\\frame" + std::to_string(i) + ".bmp";
	std::string cropImgPath = folderpath + "\\cropped_frame" + std::to_string(i) + ".bmp";
	if (i == 0 || i == 1)
	{
		cv::Mat originalImg = cv::imread(imgPath);
		cv::imwrite(cropImgPath, originalImg);

	}
	else
	{
		cv::Mat originalImg = cv::imread(imgPath);
		int imageWidth = originalImg.cols;
		int imageHeight = originalImg.rows;
		int x, y, w, h;
		w = 1000;
		h = 1500;
		x = (imageWidth - w) / 2;
		y = (imageHeight - h) / 2;
		cv::Rect roi(x, y, w, h);
		cv::Mat croppedImage = originalImg(roi);
		cv::imwrite(cropImgPath, croppedImage);
	}
}


// 识别QR，默认第一张图为二维码图，返回值为样品种类
int CApplication1Dlg::QRRecognition()
{
	std::string folderpath = getNow() + "\\" + std::to_string(globleVar) + "\\frame0" + ".bmp";
	zbar::ImageScanner scanner;
	scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
	cv::Mat image = cv::imread(folderpath);			// 二维码和条形码图片
	cv::Mat imageGray;
	cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
	int width = image.cols;
	int height = image.rows;
	uchar* raw = (uchar*)image.data;
	zbar::Image imageZbar(width, height, "Y800", raw, width * height);
	scanner.scan(imageZbar); //扫描条码  

	zbar::Image::SymbolIterator symbol = imageZbar.symbol_begin();
	if (imageZbar.symbol_begin() == imageZbar.symbol_end())
	{
		std::cout << "查询条码失败，请检查图片！" << std::endl;
	}
	for (; symbol != imageZbar.symbol_end(); ++symbol)
	{
		std::cout << "类型：" << symbol->get_type_name() << std::endl;
		std::cout << "条码：" << symbol->get_data() << std::endl;
		if (symbol->get_data() == "type1")
			return 1;
		else if (symbol->get_data() == "type2")
			return 2;
		else if (symbol->get_data() == "type3")
			return 3;
		else if (symbol->get_data() == "type4")
			return 4;
		else if (symbol->get_data() == "type5")
			return 5;
	}
	return 0;
}


// 测试按钮
void CApplication1Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

}


// 登陆后初始化
void CApplication1Dlg::OnTestLogin()
{
	// TODO: 在此添加命令处理程序代码
	login_window.DoModal();
	if (Loginsign)
	{
		InitCamera();									// 初始化相机
		UpdateData(FALSE);								// 更新初始化相机时的日志
		InitializeCriticalSection(&m_hSaveImageMux);	// 初始化临界区
		tcp = new TCPClient();							// 定义TCP对象，用于打开网口进行指令传输
		CMenu* menu = GetMenu();
		menu->EnableMenuItem(ID_TEST1_1, MF_DISABLED);
		menu->EnableMenuItem(ID_TEST1_2, MF_ENABLED);
		menu->EnableMenuItem(ID_TEST1_3, MF_ENABLED);
		menu->EnableMenuItem(ID_TEST2_1, MF_ENABLED);
		menu->EnableMenuItem(ID_TEST2_2, MF_ENABLED);
		menu->EnableMenuItem(ID_TEST2_3, MF_ENABLED);

		button_start.EnableWindow(TRUE);
		button_stop.EnableWindow(TRUE);

		UpdateWindow();
	}
	else
	{
		OnBnClickedButton1();
	}
}



#pragma region TCP_Dialog对话框消息处理
// 点击菜单栏TCP
void CApplication1Dlg::OnTCP()
{
	// TODO: 在此添加命令处理程序代码
	tcp_window.DoModal();
}

LRESULT CApplication1Dlg::OnTCPMessage(WPARAM wParam, LPARAM lParam)
{
	int msgType = (int)wParam;
	std::string msgData;
	switch (msgType)
	{
	case 1:
		std::cout << "设置ip与端口" << std::endl;
		break;
	case 2:
		msgData = readJson("get");
		tcp->sendMsg(msgData.c_str());
		break;
	default:
		break;
	}
	return 0;
}
#pragma endregion


#pragma region Control_Dialog对话框消息处理
void CApplication1Dlg::OnControl()
{
	// TODO: 在此添加命令处理程序代码
	control_window.DoModal();
}

LRESULT CApplication1Dlg::OnControlMessage(WPARAM wParam, LPARAM lParam)
{
	int msgType = (int)wParam;
	std::string msgData;
	switch (msgType)
	{
	case 1:
		msgData = readJson("getworld");		// pcakID:1
		tcp->sendMsg(msgData.c_str());
		break;
	case 2:
		msgData = readJson("getJ");		// packID:1
		tcp->sendMsg(msgData.c_str());
		break;
	default:
		break;
	}
	return 0;
}
#pragma endregion


#pragma region Setting_Dialog对话框消息处理
void CApplication1Dlg::OnSetting()
{
	// TODO: 在此添加命令处理程序代码
	setting_window.DoModal();
}

// Setting_Dialog对话框向主对话框发送的消息
// wParam传递消息内容（int）：
//			1：camera1开始采集并显示
//			2：camera2开始采集并显示
//			3：camera1读取参数
//			4：camera2读取参数
//			5：camera1写入参数，lParam为待写入参数
//			6：camera2写入参数，lParam为待写入参数
//			7：获取两个相机触发模式
LRESULT CApplication1Dlg::OnSettingMessage(WPARAM wParam, LPARAM lParam) {
	// 处理消息
	switch ((int)wParam)
	{
	case 1:
		showVideo1();
		break;
	case 2:
		showVideo2();
		break;
	case 3:
		getValue1();
		break;
	case 4:
		getValue2();
		break;
	case 5:
		setValue1(lParam);
		break;
	case 6:
		setValue2(lParam);
		break;
	case 7 :
		getTrigger();
		break;
	default:
		break;
	}
	return 0;
}

// 子线程3，在Setting_Dialog界面获取并显示视频流camera1
static UINT grabThread1(LPVOID pParam)
{
	CApplication1Dlg* pThis = (CApplication1Dlg*)pParam;  // 获取传入的类指针
	pThis->GrabThreadProcess1();
	return 0;
}

// 子线程4，在Setting_Dialog界面获取并显示视频流camera2
static UINT grabThread2(LPVOID pParam)
{
	CApplication1Dlg* pThis = (CApplication1Dlg*)pParam;  // 获取传入的类指针
	pThis->GrabThreadProcess2();
	return 0;
}

void CApplication1Dlg::showVideo1()
{
	if (FALSE == m_bOpenDevice1 || NULL == camera1)
	{
		MessageBox("相机1未打开！", "错误", MB_ICONHAND);
		return;
	}

	//memset(&m_stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
	m_bThreadState1 = TRUE;
	AfxBeginThread(grabThread1, this);

	if (m_bStartGrabbing1 != TRUE)
	{
		int nRet = camera1->StartGrabbing();
		if (MV_OK != nRet)
		{
			m_bThreadState1 = FALSE;
			std::cout << "Start grabbing fail" << std::endl;
			return;
		}
		m_bStartGrabbing1 = TRUE;
	}
}

int CApplication1Dlg::GrabThreadProcess1()
{
	MV_FRAME_OUT stImageInfo = { 0 };
	MV_CC_IMAGE   stImageData = { 0 };
	int nRet = MV_OK;

	while (m_bThreadState1)
	{
		if (!m_bStartGrabbing1)
		{
			Sleep(10);
			continue;
		}
		nRet = camera1->GetImageBuffer(&stImageInfo, 1000);
		if (nRet == MV_OK)
		{
			stImageData.nWidth = stImageInfo.stFrameInfo.nExtendWidth;
			stImageData.nHeight = stImageInfo.stFrameInfo.nExtendHeight;
			stImageData.enPixelType = stImageInfo.stFrameInfo.enPixelType;
			stImageData.nImageBufLen = stImageInfo.stFrameInfo.nFrameLenEx;
			stImageData.pImageBuf = stImageInfo.pBufAddr;
			camera1->DisplayOneFrame((void*)m_hwndDisplay1, &stImageData);
			camera1->FreeImageBuffer(&stImageInfo);
		}
		else
		{
			if (MV_TRIGGER_MODE_ON == m_nTriggerMode1)
			{
				Sleep(5);
			}
		}
	}

	return MV_OK;
}

void CApplication1Dlg::showVideo2()
{
	if (FALSE == m_bOpenDevice2 || NULL == camera2)
	{
		MessageBox("相机2未打开！", "错误", MB_ICONHAND);
		return;
	}

	//memset(&m_stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
	m_bThreadState2 = TRUE;
	AfxBeginThread(grabThread2, this);

	if (m_bStartGrabbing2 != TRUE)
	{
		int nRet = camera2->StartGrabbing();
		if (MV_OK != nRet)
		{
			m_bThreadState2 = FALSE;
			std::cout << "Start grabbing fail" << std::endl;
			return;
		}
		m_bStartGrabbing2 = TRUE;
	}
}

int CApplication1Dlg::GrabThreadProcess2()
{
	MV_FRAME_OUT stImageInfo = { 0 };
	MV_CC_IMAGE   stImageData = { 0 };
	int nRet = MV_OK;

	while (m_bThreadState2)
	{
		if (!m_bStartGrabbing2)
		{
			Sleep(10);
			continue;
		}
		nRet = camera2->GetImageBuffer(&stImageInfo, 1000);
		if (nRet == MV_OK)
		{
			stImageData.nWidth = stImageInfo.stFrameInfo.nExtendWidth;
			stImageData.nHeight = stImageInfo.stFrameInfo.nExtendHeight;
			stImageData.enPixelType = stImageInfo.stFrameInfo.enPixelType;
			stImageData.nImageBufLen = stImageInfo.stFrameInfo.nFrameLenEx;
			stImageData.pImageBuf = stImageInfo.pBufAddr;
			camera2->DisplayOneFrame((void*)m_hwndDisplay2, &stImageData);
			camera2->FreeImageBuffer(&stImageInfo);
		}
		else
		{
			if (MV_TRIGGER_MODE_ON == m_nTriggerMode2)
			{
				Sleep(5);
			}
		}
	}

	return MV_OK;
}

void CApplication1Dlg::getValue1()
{
	MVCC_FLOATVALUE ExposureFloatValue = { 0 };
	int nRet = camera1->GetFloatValue("ExposureTime", &ExposureFloatValue);
	if (MV_OK != nRet)
	{
		std::cout << "曝光获取失败" << std::endl;
	}
	MVCC_FLOATVALUE GainFloatValue = { 0 };
	nRet = camera1->GetFloatValue("Gain", &GainFloatValue);
	if (MV_OK != nRet)
	{
		std::cout << "增益获取失败" << std::endl;
	}
	MVCC_FLOATVALUE FrameFloatValue = { 0 };
	nRet = camera1->GetFloatValue("ResultingFrameRate", &FrameFloatValue);
	if (MV_OK != nRet)
	{
		std::cout << "帧率获取失败" << std::endl;
	}
	cameraParam[0] = ExposureFloatValue.fCurValue;
	cameraParam[1] = GainFloatValue.fCurValue;
	cameraParam[2] = FrameFloatValue.fCurValue;

}

void CApplication1Dlg::getValue2()
{
	MVCC_FLOATVALUE ExposureFloatValue = { 0 };
	int nRet = camera2->GetFloatValue("ExposureTime", &ExposureFloatValue);
	if (MV_OK != nRet)
	{
		std::cout << "曝光获取失败" << std::endl;
	}
	MVCC_FLOATVALUE GainFloatValue = { 0 };
	nRet = camera2->GetFloatValue("Gain", &GainFloatValue);
	if (MV_OK != nRet)
	{
		std::cout << "增益获取失败" << std::endl;
	}
	MVCC_FLOATVALUE FrameFloatValue = { 0 };
	nRet = camera2->GetFloatValue("ResultingFrameRate", &FrameFloatValue);
	if (MV_OK != nRet)
	{
		std::cout << "帧率获取失败" << std::endl;
	}
	cameraParam[3] = ExposureFloatValue.fCurValue;
	cameraParam[4] = GainFloatValue.fCurValue;
	cameraParam[5] = FrameFloatValue.fCurValue;
}

void CApplication1Dlg::setValue1(LPARAM lParam)
{
	float* param = reinterpret_cast<float*>(lParam);

	int nRet = camera1->SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);
	nRet = camera1->SetFloatValue("ExposureTime", param[0]);
	nRet = camera1->SetEnumValue("GainAuto", 0);
	nRet = camera1->SetFloatValue("Gain", param[1]);
	nRet = camera1->SetBoolValue("AcquisitionFrameRateEnable", true);
	nRet = camera1->SetFloatValue("AcquisitionFrameRate", param[2]);
}

void CApplication1Dlg::setValue2(LPARAM lParam)
{
	float* param = reinterpret_cast<float*>(lParam);

	int nRet = camera2->SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);
	nRet = camera2->SetFloatValue("ExposureTime", param[0]);
	nRet = camera2->SetEnumValue("GainAuto", 0);
	nRet = camera2->SetFloatValue("Gain", param[1]);
	nRet = camera2->SetBoolValue("AcquisitionFrameRateEnable", true);
	nRet = camera2->SetFloatValue("AcquisitionFrameRate", param[2]);
}

void CApplication1Dlg::getTrigger()
{
	MVCC_ENUMVALUE stEnumValue1 = { 0 };
	int nRet = camera1->GetEnumValue("TriggerMode", &stEnumValue1);
	if (MV_OK != nRet)
	{
		std::cout << "相机1触发模式获取失败" << std::endl;
		return;
	}
	MVCC_ENUMVALUE stEnumValue2 = { 0 };
	nRet = camera2->GetEnumValue("TriggerMode", &stEnumValue2);
	if (MV_OK != nRet)
	{
		std::cout << "相机2触发模式获取失败" << std::endl;
		return;
	}
	m_TriggerMode[0] = stEnumValue1.nCurValue;
	m_TriggerMode[1] = stEnumValue2.nCurValue;
}
#pragma endregion


