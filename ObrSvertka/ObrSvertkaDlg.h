
// ObrSvertkaDlg.h: файл заголовка
//

#pragma once
#include "Drawer.h"
#include "LineSys.h"

// Диалоговое окно CObrSvertkaDlg
class CObrSvertkaDlg : public CDialogEx
{
// Создание
public:
	CObrSvertkaDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBRSVERTKA_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	double A1;
	double stok1;
	double mat1;
	double A2;
	double stok2;
	double mat2;
	double A3;
	double stok3;
	double mat3;
	double Ah;
	double stokh;
	double fd;
	int N;
	Drawer drwx;
	Drawer drwh;
	Drawer drwy;
	afx_msg void OnBnClickedOk();
	float h;
	float TAU;
	afx_msg void OnBnClickedButton1();
	CString otkl;
	bool canDraw;
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedContinue();
	bool pause;
};
