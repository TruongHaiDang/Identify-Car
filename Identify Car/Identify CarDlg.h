
// Identify CarDlg.h : header file
//

#pragma once


// CIdentifyCarDlg dialog
class CIdentifyCarDlg : public CDialogEx
{
// Construction
public:
	CIdentifyCarDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IDENTIFYCAR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton var_bienSo;
	afx_msg
		void loadImages();
	void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	void anyLizeImages();
	afx_msg void OnBnClickedButton3();
	CComboBox var_comboBoxMorph;
	CEdit var_morphValue;
	CStatic pictureBox1;
	CEdit var_Car_Number;
	CStatic pictureBox2;
};
