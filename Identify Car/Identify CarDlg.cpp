
// Identify CarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Identify Car.h"
#include "Identify CarDlg.h"
#include "afxdialogex.h"
#include "opencv2/opencv_modules.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <vector>
using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CIdentifyCarDlg dialog



CIdentifyCarDlg::CIdentifyCarDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IDENTIFYCAR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIdentifyCarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, var_bienSo);
	DDX_Control(pDX, IDC_EDIT1, var_morphValue);
}

BEGIN_MESSAGE_MAP(CIdentifyCarDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CIdentifyCarDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CIdentifyCarDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CIdentifyCarDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CIdentifyCarDlg message handlers

BOOL CIdentifyCarDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIdentifyCarDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIdentifyCarDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIdentifyCarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Declartion
	CString path, name;
	cv::Mat src, dst, gray, noise_remove, histgr, morph, substr, threshImage, cannyImage, dilateImage, Roi;
	cv::Mat roi_gray, roi_noise_remove, roi_histgr, roi_morph, roi_substr, roi_threshImage, roi_cannyImage, roi_dilateImage, result;

	void CIdentifyCarDlg::loadImages()
	{
		static CString Filter = _T("Image File(*.jpg; *.png; *.bmp)|*.jpg;*.png;*.bmp|All Files(*.*)|*.*||");
		CFileDialog Load(TRUE, _T("*.jpg"), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, Filter, NULL);
		Load.m_ofn.lpstrTitle = _T("Choose Image");
		if (Load.DoModal() == IDOK)
		{
			path = Load.GetPathName();
			name = Load.GetFileName();
			const char *address = (LPCSTR)path;
			const char *fileName = (LPCSTR)name;
			src = cv::imread(address, -1);
			cv::namedWindow(fileName, CV_WINDOW_AUTOSIZE);
			HWND getHandle = (HWND)cvGetWindowHandle(fileName);
			HWND getPa = ::GetParent(getHandle);
			::SetParent(getHandle, GetDlgItem(IDC_PictureBox1)->m_hWnd);
			::ShowWindow(getPa, SW_HIDE);
			cv::resize(src, dst, cv::Size(600, 450), 0, 0, 1);
			cv::imshow(fileName, dst);
		}
	}

	void CIdentifyCarDlg::anyLizeImages()
	{
		// Get morph scan
		var_morphValue.SetWindowTextA("15");
		CString value;
		var_morphValue.GetWindowTextA(value);
		int morphValueScan = atoi(value);
		// Process
		cv::cvtColor(src, gray, CV_BGR2GRAY);
		cv::bilateralFilter(gray, noise_remove, 9, 75, 75);
		cv::equalizeHist(noise_remove, histgr);
		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
		cv::morphologyEx(histgr, morph, cv::MORPH_OPEN, kernel, cv::Point(-1, -1), morphValueScan);
		cv::subtract(histgr, morph, substr);
		cv::adaptiveThreshold(substr, threshImage, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 35, 5);
		cv::Canny(threshImage, cannyImage, 250, 255);
		cv::Mat element = getStructuringElement(MORPH_RECT, cv::Size(3, 3));
		cv::dilate(cannyImage, dilateImage, element);
		// Get the number zone
		std::vector<std::vector<cv::Point>> contours;
		std::vector<cv::Vec4i> hierarchy;
		cv::findContours(dilateImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
		// Draw contours
		for (size_t i = 0; i < contours.size(); i++)
		{
			cv::Rect defineRect = cv::boundingRect(contours[i]);
			if (defineRect.width > 200 && (double)defineRect.width / defineRect.height > 4.1 && (double)defineRect.width / defineRect.height < 4.4)
			{
				// Draw red rectange
				rectangle(src, defineRect, Scalar(0, 0, 255), 2, 8, 0);
				Roi = src(defineRect);
				// Filter inside 
				cv::cvtColor(Roi, roi_gray, CV_BGR2GRAY);
				cv::bilateralFilter(roi_gray, roi_noise_remove, 9, 75, 75);
				cv::equalizeHist(roi_noise_remove, roi_histgr);
				cv::morphologyEx(roi_histgr, roi_morph, cv::MORPH_OPEN, kernel, cv::Point(-1, -1), morphValueScan);
				cv::subtract(roi_histgr, roi_morph, roi_substr);
				cv::adaptiveThreshold(roi_substr, roi_threshImage, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 35, 5);
				cv::Canny(roi_threshImage, roi_cannyImage, 250, 255);
				cv::dilate(roi_cannyImage, roi_dilateImage, element);
				// Get the number zone
				std::vector<std::vector<cv::Point>> sub_contours;
				std::vector<Vec4i> sub_hierarchy;
				cv::findContours(roi_dilateImage, sub_contours, sub_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
				// Draw contours
				for (size_t j = 0; j < sub_contours.size(); j++)
				{
					cv::Rect sub_defineRect = cv::boundingRect(sub_contours[j]);
					if (sub_defineRect.height > defineRect.height / 2 && sub_defineRect.width < defineRect.width / 8 && sub_defineRect.width > 5 && defineRect.width > 15 && sub_defineRect.y < 13 && sub_defineRect.y > 7)
					{
						rectangle(Roi, sub_defineRect, Scalar(0, 0, 255), 2, 8, 0);
					}
				}
			}
		}
		// Filter image
		cv::namedWindow("Result", CV_WINDOW_AUTOSIZE);
		HWND getHandle = (HWND)cvGetWindowHandle("Result");
		HWND getPa = ::GetParent(getHandle);
		::SetParent(getHandle, GetDlgItem(IDC_PictureBox2)->m_hWnd);
		::ShowWindow(getPa, SW_HIDE);
		// Show result
		cv::resize(Roi, Roi, cv::Size(270, 100), 0, 0, 1);
		cv::imshow("Result", Roi);
	}

	void CIdentifyCarDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
		loadImages();
}

void CIdentifyCarDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	// Anylize
	if (!src.empty())
	{
		anyLizeImages();
	}
	else
	{
		MessageBox("Image doesn't exist!");
	}
}

void CIdentifyCarDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	// Traning

}
