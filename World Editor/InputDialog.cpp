// Supported with union (c) 2020 Union team
// Union SOURCE file

using namespace GOTHIC_ENGINE;
	IMPLEMENT_DYNAMIC(CInputDialog, CDialogEx)

		CInputDialog::CInputDialog(CWnd* pParent /*=nullptr*/)
		: CDialogEx(IDD_INPUT, pParent)
		, ctrl_input(_T(""))
		, name(_T(""))
	{
		
	}

	CInputDialog::~CInputDialog()
	{
		
	}

	void CInputDialog::DoDataExchange(CDataExchange* pDX)
	{
		
		CDialogEx::DoDataExchange(pDX);
		DDX_Text(pDX, IDC_VOBNAME, ctrl_input);
		DDX_Text(pDX, IDC_NAME, name);
	}


	BEGIN_MESSAGE_MAP(CInputDialog, CDialogEx)
	END_MESSAGE_MAP()


	// Обработчики сообщений CInputDialog

	void CInputDialog::SetInput(CString input)
	{
		
		ctrl_input = input;
	};


	CString CInputDialog::GetInput()
	{
		
		CString s = ctrl_input;
		s.MakeUpper();
		return s;
	}

