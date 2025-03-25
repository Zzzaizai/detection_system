
// Application1Dlg.h: 头文件
//
#include "MvCameraControl.h"
#include "MvCamera.h"
#include "string"
#include "TCP_Client.h"
#include "TCP_Dialog.h"
#include "Control_Dialog.h"
#include "Setting_Dialog.h"
#include "Login_Dialog.h"

#pragma once

#define WM_UPDATEDATA (WM_USER + 100)				// 用于子线程更新文本框的消息
#define WM_Control_Dialog_MESSAGE (WM_USER + 1)		// Control_Dialog窗口向父窗口发送消息
#define WM_Setting_Dialog_MESSAGE (WM_USER + 2)		// Setting_Dialog窗口向父窗口发送消息
#define WM_TCP_Dialog_MESSAGE (WM_USER + 3)			// TCP_Dialog窗口向父窗口发送消息

// CApplication1Dlg 对话框
class CApplication1Dlg : public CDialogEx
{
// 构造
public:
	CApplication1Dlg(CWnd* pParent = nullptr);	// 标准构造函数

	// 是否登陆成功
	bool Loginsign;

	// 子窗口类
	Login_Dialog login_window;
	TCP_Dialog tcp_window;
	Control_Dialog control_window;
	Setting_Dialog setting_window;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_APPLICATION1_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	//手动添加
public:
	CRITICAL_SECTION        m_hSaveImageMux;		// 用于实现临界区
	unsigned char*			m_pSaveImageBuf;		// 图像数据缓冲区
	unsigned int            m_nSaveImageBufSize;	// 图像数据缓冲区大小
	MV_FRAME_OUT_INFO_EX    m_stImageInfo;			// 图像信息缓冲区

	HWND                    m_hwndDisplay1;			// 显示视频窗口的句柄
	HWND                    m_hwndDisplay2;
	BOOL                    m_bThreadState1;		// 显示视频线程的状态
	BOOL                    m_bThreadState2;
	BOOL                    m_bStartGrabbing1;		// 是否开始抓图
	BOOL                    m_bStartGrabbing2;
	int                     m_nTriggerMode1;		// 触发模式
	int                     m_nTriggerMode2;
	BOOL                    m_bOpenDevice1;			// 是否打开设备
	BOOL                    m_bOpenDevice2;

	UINT_PTR m_nTimerID;						// 计时器ID
	float m_nTimeCounter;						// 用于计时的变量
	CMvCamera* camera1;		// 相机对象 一个相机一个对象
	CMvCamera* camera2;
	MV_CC_IMAGE stImage;

	float cameraParam[6];	// 相机的曝光、增益、帧率
	int m_TriggerMode[2];	// 相机的触发模式


public:
	// 机械臂传回的消息
	struct Msg {
		std::string dsID;
		std::string packID;
		std::string reqType;
		std::vector<std::string> queryData;
		std::vector<std::string> cmdReply;
		bool state = FALSE;		// 读取标志，只允许程序读取一次，然后置FALSE
	}m;

	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);			// 用于子线程调用UpdateData函数
	afx_msg LRESULT OnSettingMessage(WPARAM wParam, LPARAM lParam);		// Setting_Dialog向主窗口发送消息
	afx_msg LRESULT OnControlMessage(WPARAM wParam, LPARAM lParam);		// Control_Dialog向主窗口发送消息
	afx_msg LRESULT OnTCPMessage(WPARAM wParam, LPARAM lParam);		// TCP_Dialog向主窗口发送消息
	afx_msg void OnTCP();

	// 两张水冷板之间的初始化过程
	void NewSampleInit();
	// 初始化相机
	void InitCamera();
	// 拍摄图片
	int GrabImg(int i, int cameraNum);
	//void ShowImg();
	// 在界面上展示图片
	void ShowImgfromfile(int i);
	// 接受是否拍摄指令
	bool ReceiveGrabCommand();
	// 识别二维码
	int QRRecognition();
	// 裁剪图片
	void CropImg(int i);
	// 显示日志
	void tip(CString s);
	// 获取当前时间
	std::string getNow();
	// 向机械臂发送指令
	bool SendCommand(std::string command);
	// 解析机械臂消息
	Msg SolveMsg(std::string msg);
	// 通过指令id获取json指令
	std::string readJson(std::string targetId);
	// 结束或中止检测后的收尾工作
	void stop();
	//
	void showVideo1();
	void showVideo2();
	int GrabThreadProcess1();
	int GrabThreadProcess2();
	void getValue1();
	void getValue2();
	void setValue1(LPARAM lParam);
	void setValue2(LPARAM lParam);
	void getTrigger();



	// 变量声明
	//
	// tcp对象
	TCPClient* tcp;
	// 日志信息
	CString log_message;
	// “开始检测”按钮的控制变量
	CButton button_start;
	// “停止检测”按钮的控制变量
	CButton button_stop;
	// 停止检测标志
	bool isStop;
	// 水冷板计数
	int globleVar;
	// 是否拍摄标志
	bool isGrab;

	
	afx_msg void OnBnClickedButton1();
	afx_msg void OnControl();
	afx_msg void OnSetting();
	afx_msg void OnTestLogin();
	
};
//extern int globleVar;
//extern bool isStop;
//extern std::string log_message;
