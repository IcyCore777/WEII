// Supported with union (c) 2020 Union team
// Union HEADER file

using namespace GOTHIC_ENGINE;
	class  CInputDialog : public CDialogEx
	{
		DECLARE_DYNAMIC(CInputDialog)

	public:
		CInputDialog(CWnd* pParent = nullptr);
		virtual ~CInputDialog();

#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_INPUT };
#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);

		DECLARE_MESSAGE_MAP()
	public:
		CString ctrl_input;
		void SetInput(CString input);
		CString GetInput();
		CString name;
	};
