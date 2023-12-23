
// ObrSvertkaDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "ObrSvertka.h"
#include "ObrSvertkaDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CObrSvertkaDlg



CObrSvertkaDlg::CObrSvertkaDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OBRSVERTKA_DIALOG, pParent)
	, A1(10)
	, stok1(0.05)
	, mat1(1)
	, A2(7)
	, stok2(0.05)
	, mat2(3)
	, A3(8)
	, stok3(0.05)
	, mat3(4)
	, Ah(1)
	, stokh(0.25)
	, fd(10)
	, N(50)
	, h(0.1)
	, TAU(0.000001)
	, otkl(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CObrSvertkaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, A1);
	DDX_Text(pDX, IDC_EDIT8, stok1);
	DDX_Text(pDX, IDC_EDIT10, mat1);
	DDX_Text(pDX, IDC_EDIT3, A2);
	DDX_Text(pDX, IDC_EDIT5, stok2);
	DDX_Text(pDX, IDC_EDIT7, mat2);
	DDX_Text(pDX, IDC_EDIT9, A3);
	DDX_Text(pDX, IDC_EDIT2, stok3);
	DDX_Text(pDX, IDC_EDIT12, mat3);
	DDX_Text(pDX, IDC_EDIT4, Ah);
	DDX_Text(pDX, IDC_EDIT6, stokh);
	DDX_Text(pDX, IDC_EDIT11, fd);
	DDX_Text(pDX, IDC_EDIT13, N);
	DDX_Text(pDX, IDC_EDIT14, h);
	DDX_Text(pDX, IDC_EDIT15, TAU);
	DDX_Text(pDX, IDC_EDIT16, otkl);
}

BEGIN_MESSAGE_MAP(CObrSvertkaDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CObrSvertkaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CObrSvertkaDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_PAUSE, &CObrSvertkaDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_CONTINUE, &CObrSvertkaDlg::OnBnClickedContinue)
END_MESSAGE_MAP()


// Обработчики сообщений CObrSvertkaDlg

BOOL CObrSvertkaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	drwx.Create(GetDlgItem(IDC_PICX)->GetSafeHwnd());
	drwh.Create(GetDlgItem(IDC_PICH)->GetSafeHwnd());
	drwy.Create(GetDlgItem(IDC_PICY)->GetSafeHwnd());
	canDraw = false;
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CObrSvertkaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		if (canDraw)
		{
			drwx.ReDraw();
			drwh.ReDraw();
			drwy.ReDraw();
		}
		canDraw = true;
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CObrSvertkaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

MSG msg;

void CObrSvertkaDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	double A[] = {A1, A2, A3, Ah};
	double stok[] = { stok1, stok2, stok3, stokh };
	double mat[] = { mat1, mat2, mat3};
	LineS sys(A, stok, mat, N, fd), sys1 = sys;
	vector<double> px, ph, py;
	sys.GetX(px);
	sys.GetH(ph);
	sys.CreateY(fd);
	sys.GetY(py);
	drwx.DrawPoints(px, L"t", L"A", N / fd, 1 / fd, false);
	drwh.DrawPoints(ph, L"t", L"A", N / fd, 1 / fd, false);
	drwy.DrawPoints(py, L"t", L"A", N / fd, 1 / fd, false);
}

vector<float> lyambda;
bool pause2;
void CObrSvertkaDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	pause = false;
	pause2 = false;
	if (lyambda.size() == 0) lyambda.clear();
	UpdateData(TRUE);
	double A[] = { A1, A2, A3, Ah };
	double stok[] = { stok1, stok2, stok3, stokh };
	double mat[] = { mat1, mat2, mat3 };
	LineS sys(A, stok, mat, N, fd), sys1 = sys;
	vector<double> px, ph, py;
	sys.CreateY(fd);
	float* l = new float[N];

	sys.MHJ(l, h, TAU, fd, N, msg, pause, drwx);
	sys1.DekonvSvertka(l);
	if (pause2)
	{
		for (int i = 0; i < N; i++)
		{
			lyambda.push_back(l[i]);
		}
	}
	delete[] l;
	sys.GetX(px);
	sys1.GetSearchX(py);

	drwx.DrawTwoSig(px, py, L"t", L"A", N / fd, 1 / fd);

	vector<double> razn;
	for (int i = 0; i < N; i++)
	{
		razn.push_back(abs(py[i] - px[i]));
	}
	if (!pause2)
	{
		//double ener = abs(E(py) - E(px)) / E(px) * 100;
		double ener = E(razn) / E(px) * 100;
		otkl.Format(_T("%.2f"), ener);
		otkl += " % ";
	}
	UpdateData(FALSE);
}

void CObrSvertkaDlg::OnBnClickedPause()
{
	// TODO: добавьте свой код обработчика уведомлений
	pause = true;
	pause2 = true;
	//PostQuitMessage(0);
	WPARAM F = WPARAM("PAUSE");
	LPARAM O = LPARAM("PAUSE");
	PostThreadMessageA(GetCurrentThreadId(), 12345, F, O);
}


void CObrSvertkaDlg::OnBnClickedContinue()
{
	// TODO: добавьте свой код обработчика уведомлений
	pause2 = false;
	UpdateData(TRUE);
	double A[] = { A1, A2, A3, Ah };
	double stok[] = { stok1, stok2, stok3, stokh };
	double mat[] = { mat1, mat2, mat3 };
	LineS sys(A, stok, mat, N, fd);
	vector<double> px, ph, py;
	sys.CreateY(fd);
	float* l = new float[N];
	for (int i = 0; i < N; i++)
	{
		l[i] = lyambda[i];
	}
	lyambda.clear();
	sys.MHJ(l, h, TAU, fd, N, msg, pause, drwx);
	sys.DekonvSvertka(l);
	if (pause2)
	{
		for (int i = 0; i < N; i++)
		{
			lyambda.push_back(l[i]);
		}
	}
	delete[] l;
	
	sys.GetX(px);
	sys.GetSearchX(py);
	drwx.DrawTwoSig(px, py, L"t", L"A", N / fd, 1 / fd);

	vector<double> razn;
	for (int i = 0; i < N; i++)
	{
		razn.push_back(abs(py[i] - px[i]));
	}
	if (!pause2)
	{
		//double ener = abs(E(py) - E(px)) / E(px) * 100;
		double ener = E(razn) / E(px) * 100;
		otkl.Format(_T("%.2f"), ener);
		otkl += " % ";
	}
	UpdateData(FALSE);
}
