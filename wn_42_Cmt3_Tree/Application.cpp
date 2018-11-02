// Implementation of the CApplication class.
//
////////////////////////////////////////////////////////////////////////////////

// basic source: http://soen.kr/

#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable: 4996)
#endif


#include "_StdAfx.h"
#include <string>
#include <tuple>
#include <vector>

static CApplication*	g_pD3DApp;


#define IDC_NAME		(WM_USER+1)
#define IDC_POST		(WM_USER+2)
#define IDC_ADD			(WM_USER+3)
#define IDC_DEL			(WM_USER+4)
#define IDC_EDIT		(WM_USER+5)

// ���� ��ȣ ����ü
std::vector< std::tuple<int, std::string, std::string, int> > arPost
{
	std::make_tuple(-1,"�����ȣ��","000-000",0),
	std::make_tuple(0,"����Ư����","000-000",1),
	std::make_tuple(0,"��⵵","000-000",1),
	std::make_tuple(0,"������","000-000",1),
	std::make_tuple(0,"��û����","000-000",1),
	std::make_tuple(0,"��û�ϵ�","000-000",1),
	std::make_tuple(1,"������","000-000",2),
	std::make_tuple(1,"������","000-000",2),
	std::make_tuple(1,"���ϱ�","000-000",2),
	std::make_tuple(1,"������","000-000",2),
	std::make_tuple(1,"���Ǳ�","000-000",2),
	std::make_tuple(1,"������","000-000",2),
	std::make_tuple(1,"���α�","000-000",2),
	std::make_tuple(1,"��õ��","000-000",2),
	std::make_tuple(1,"�����","000-000",2),
	std::make_tuple(1,"������","000-000",2),
	std::make_tuple(1,"���빮��","000-000",2),
	std::make_tuple(1,"���۱�","000-000",2),
	std::make_tuple(2,"����","000-000",2),
	std::make_tuple(2,"�ϻ��","000-000",2),
	std::make_tuple(2,"��õ��","000-000",2),
	std::make_tuple(2,"�����","000-000",2),
	std::make_tuple(2,"������","000-000",2),
	std::make_tuple(2,"������","000-000",2),
	std::make_tuple(2,"�����ֽ�","000-000",2),
	std::make_tuple(2,"����õ��","000-000",2),
	std::make_tuple(2,"��õ��","000-000",2),
	std::make_tuple(3,"������","000-000",2),
	std::make_tuple(3,"��ô��","000-000",2),
	std::make_tuple(6,"������","135-240",3),
	std::make_tuple(6,"������","135-010",3),
	std::make_tuple(6,"��ġ��","135-280",3),
	std::make_tuple(6,"���","135-270",3),
	std::make_tuple(6,"�Ｚ��","135-090",3),
	std::make_tuple(6,"���","135-190",3),
	std::make_tuple(6,"������","135-220",3),
	std::make_tuple(6,"�Ż絿","135-120",3),
	std::make_tuple(6,"�б�����","135-110",3),
	std::make_tuple(6,"���ﵿ","135-080",3),
	std::make_tuple(6,"������","135-210",3),
	std::make_tuple(7,"�����","134-080",3),
	std::make_tuple(8,"�̾Ƶ�","142-100",3),
	std::make_tuple(9,"���絿","157-200",3),
	std::make_tuple(10,"������","151-080",3),
	std::make_tuple(10,"��õ��","151-050",3),
	std::make_tuple(10,"�Ÿ���","151-010",3),
	std::make_tuple(11,"���嵿","143-210",3),
	std::make_tuple(12,"��������","152-020",3),
	std::make_tuple(13,"���굿","153-023",3),
	std::make_tuple(14,"������","139-240",3),
	std::make_tuple(15,"������","132-010",3),
	std::make_tuple(16,"��ʸ���","130-030",3),
	std::make_tuple(17,"�뷮����","156-050",3),
	std::make_tuple(18,"���ŵ�","412-290",3),
	std::make_tuple(18,"��絿","412-500",3),
	std::make_tuple(18,"���굿","412-470",3),
	std::make_tuple(18,"���","412-260",3),
	std::make_tuple(18,"������","412-520",3),
	std::make_tuple(18,"���ڵ�","412-480",3),
	std::make_tuple(18,"���嵿","412-250",3),
	std::make_tuple(18,"������","412-170",3),
	std::make_tuple(18,"������","412-060",3),
	std::make_tuple(18,"���굿","412-090",3),
	std::make_tuple(19,"���µ�","411-440",3),
	std::make_tuple(19,"���굿","411-430",3),
	std::make_tuple(20,"������","427-100",3),
	std::make_tuple(20,"��õ��","427-060",3),
	std::make_tuple(20,"������","427-020",3),
	std::make_tuple(20,"���赿","427-080",3),
	std::make_tuple(20,"������","427-090",3),
	std::make_tuple(20,"���絿","427-040",3),
	std::make_tuple(21,"���е�","423-070",3),
	std::make_tuple(22,"���ŵ�","471-080",3),
	std::make_tuple(23,"������","435-045",3),
	std::make_tuple(24,"���","472-060",3),
	std::make_tuple(25,"�ɻ굿","483-070",3),
	std::make_tuple(26,"�����","422-070",3),
};



// Parent ����� ���ϵ带 ��� ã�� Ʈ���� �߰��Ѵ�. �� ���ϵ忡 ���ؼ��� ��
// �Լ��� ��������� ȣ�������ν� ��� Ʈ���� �� �Է��Ѵ�.
// pNode:�θ��� ���
// pid:�θ��� �ε��� ��ȣ
void CApplication::InsertChild(HTREEITEM pNode,int pid)
{
	TVINSERTSTRUCT TI;
	HTREEITEM Node;
	int i=0;
	for (auto& post=arPost.begin(); post != arPost.end(); ++post, ++i)
	{
		auto parent = std::get<0>(*post);
		if (parent == pid)
		{
			TI.hParent=pNode;
			TI.hInsertAfter=TVI_LAST;
			TI.item.mask=TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
			TI.item.iImage=std::get<3>(*post);
			TI.item.iSelectedImage=std::get<3>(*post)+4;
			TI.item.pszText=(LPSTR)std::get<1>(*post).c_str();

			char* pParam=(char*)malloc(32);
			memset(pParam, 0, 32);

			strcpy(pParam, std::get<2>(*post).c_str());
			// �Ķ���Ϳ� ����ü�� �����͸� �����Ѵ�.
			TI.item.lParam=(LPARAM)pParam;
			Node=TreeView_InsertItem(m_hTree, &TI);
			// ���ϵ��� ���ϵ带 ä�� �ִ´�.
			InsertChild(Node,i++);
		}
	}
}



// �Էµ� ������� �׸��� �߰��Ѵ�.
void CApplication::AddPost()
{
	HTREEITEM hNow;
	TVINSERTSTRUCT TI;
	TVITEMEX	TvEx;
	char		Title[30];
	char*		pParam;

	hNow=TreeView_GetSelection(m_hTree);

	if (hNow==NULL)
	{
		MessageBox(m_hWnd,"�߰��� �θ� ��带 ���� ������ �ֽʽÿ�.","�˸�",MB_OK);
	}
	else
	{
		// �θ� ����� �̹��� ��ȣ�� ���� �׸� �߰��� �������� �����Ѵ�.
		TvEx.mask=TVIF_IMAGE;
		TvEx.hItem=hNow;
		TreeView_GetItem(m_hTree,&TvEx);

		if (TvEx.iImage == 3)
		{
			MessageBox(m_hWnd,"�� �Ʒ����� �׸��� �߰��� �� �����ϴ�.","�˸�",MB_OK);
			return;
		}

		// ����Ʈ�� �Էµ� �̸�,�����ȣ�� �о�´�.
		pParam=(char*)malloc(32);
		memset(pParam, 0, 32);
		GetDlgItemText(m_hWnd,IDC_NAME,Title,30);
		GetDlgItemText(m_hWnd,IDC_POST,pParam,32);

		// �׸��� �����Ѵ�.
		TI.hParent=hNow;
		TI.hInsertAfter=TVI_LAST;
		TI.item.mask=TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
		TI.item.iImage=TvEx.iImage+1;
		TI.item.iSelectedImage=TvEx.iImage+1+4;
		TI.item.pszText=Title;
		TI.item.lParam=(LPARAM)pParam;
		TreeView_InsertItem(m_hTree, &TI);

		// �׸��� �߰��� �� �θ� ��带 Ȯ���Ͽ� �߰��� ����� �����ش�.
		TreeView_Expand(m_hTree,hNow,TVE_EXPAND);
	}
}

// ���� ���õ� �׸��� �����Ѵ�.
void CApplication::DelPost()
{
	HTREEITEM hNow;

	hNow=TreeView_GetSelection(m_hTree);

	if (hNow!=NULL)
	{
		TreeView_DeleteItem(m_hTree,hNow);
	}
}

// ���� ���õ� �׸��� �����Ѵ�.
void CApplication::EditPost()
{
	HTREEITEM	hNow;
	TVITEMEX	TvEx;
	char*		pParam;
	char		Title[30];

	hNow=TreeView_GetSelection(m_hTree);

	if (hNow==NULL)
	{
		MessageBox(m_hWnd,"������ ��带 ���� ������ �ֽʽÿ�.","�˸�",MB_OK);
	}
	else
	{
		TvEx.hItem=hNow;
		TvEx.mask=TVIF_PARAM;
		TreeView_GetItem(m_hTree,&TvEx);
		pParam=(char*)TvEx.lParam;

		GetDlgItemText(m_hWnd,IDC_NAME,Title,30);
		GetDlgItemText(m_hWnd,IDC_POST,pParam,10);

		TvEx.mask=TVIF_TEXT;
		TvEx.pszText=Title;
		TreeView_SetItem(m_hTree,&TvEx);
	}
}


CApplication::CApplication()
{
	g_pD3DApp	= this;

	strcpy(m_sCls, "McApi Tutorial");

	m_hInst		= NULL;
	m_hWnd		= NULL;
	m_dWinStyle	= WS_OVERLAPPEDWINDOW| WS_VISIBLE;
	m_dScnX		= 1024;
	m_dScnY		= 768;

	m_bShowCusor= true;
}


LRESULT WINAPI CApplication::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return g_pD3DApp->MsgProc(hWnd, msg, wParam, lParam);
}

INT CApplication::Create( HINSTANCE hInst)
{
	m_hInst	= hInst;


	WNDCLASS wc =								// Register the window class
	{
		CS_CLASSDC
		, WndProc
		, 0L
		, 0L
		, m_hInst
		, LoadIcon(NULL, IDI_APPLICATION)
		, LoadCursor(NULL, IDC_ARROW)
		, (HBRUSH)(COLOR_BTNFACE + 1)
		, NULL
		, m_sCls
	};

	RegisterClass( &wc );

	RECT rc;									//Create the application's window

	SetRect( &rc, 0, 0, m_dScnX, m_dScnY);
	AdjustWindowRect( &rc, m_dWinStyle, false );

	int iScnSysW = ::GetSystemMetrics(SM_CXSCREEN);
	int iScnSysH = ::GetSystemMetrics(SM_CYSCREEN);

	m_hWnd = CreateWindow( m_sCls
		, m_sCls
		, m_dWinStyle
		, (iScnSysW - (rc.right-rc.left))/2
		, (iScnSysH - (rc.bottom-rc.top))/2
		, (rc.right-rc.left)
		, (rc.bottom-rc.top)
		, NULL
		, NULL
		, m_hInst
		, NULL );



	InitCommonControls();


		// Ʈ�� �� ��Ʈ���� �����.
		m_hTree=CreateWindow(WC_TREEVIEW, ""
			, WS_CHILD | WS_VISIBLE | WS_BORDER | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_SHOWSELALWAYS
			, 10,10,200,300
			, m_hWnd , NULL
			,m_hInst, NULL);

		// Ʈ���信 ����� �̹��� ����Ʈ�� ����� Ʈ���信 �����Ѵ�.
		m_Image=ImageList_LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_BITMAP1), 16, 1, RGB(255,255,255));

		TreeView_SetImageList(m_hTree, m_Image, TVSIL_NORMAL);

		// Ʈ���� �����͸� �Է��Ѵ�.
		InsertChild((HTREEITEM)0,-1);

		// ������ �Է¹ޱ� ���� ��Ʈ�ѵ��� �����.
		CreateWindow("static","�̸�"
			, WS_CHILD | WS_VISIBLE
			, 220,70,80,25
			, m_hWnd,(HMENU)-1
			, m_hInst,NULL);

		CreateWindow("edit",NULL
			, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL
			, 300,70,90,25
			, m_hWnd, (HMENU)IDC_NAME
			, m_hInst,NULL);

		CreateWindow("static","�����ȣ"
			, WS_CHILD | WS_VISIBLE
			, 220,100,80,25
			, m_hWnd, (HMENU)-1
			, m_hInst,NULL);

		CreateWindow("edit",NULL
			, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL
			, 300,100,90,25
			, m_hWnd, (HMENU)IDC_POST
			, m_hInst, NULL);

		// ��� ��ư
		CreateWindow("button","�߰�"
			, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 220,150,90,25
			, m_hWnd, (HMENU)IDC_ADD
			, m_hInst,NULL);

		CreateWindow("button","����"
			, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 320,150,90,25
			, m_hWnd, (HMENU)IDC_DEL
			, m_hInst,NULL);

		CreateWindow("button","����"
			, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 420,150,90,25
			, m_hWnd, (HMENU)IDC_EDIT
			, m_hInst,NULL);



	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );
	::ShowCursor(m_bShowCusor);

	return S_OK;
}


void CApplication::Cleanup()
{
	ImageList_Destroy(m_Image);

	SAFE_DESTROY_WINDOW(m_hTree);

	if(m_hInst)
		UnregisterClass( m_sCls, m_hInst);
}


INT CApplication::Run()
{
	MSG msg;
	memset( &msg, 0, sizeof(msg) );

	while( msg.message!=WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
		}
	}

	UnregisterClass( m_sCls, m_hInst);

	return 1;
}



LRESULT CApplication::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDC_ADD:
					AddPost();
					break;

				case IDC_DEL:
					DelPost();
					break;

				case IDC_EDIT:
					EditPost();
					break;

			}

			break;
		}

		case WM_NOTIFY:
		{
			LPNMHDR			hdr;
			LPNMTREEVIEW	ntv;
			TVITEMEX		TvEx;
			char			Cap[30];
			char			pParam[10];

			hdr=(LPNMHDR)lParam;
			ntv=(LPNMTREEVIEW)lParam;

			if (hdr->hwndFrom == m_hTree)
			{
				switch (hdr->code)
				{

					// ���õ� �׸��� �����ش�.
					case TVN_SELCHANGED:
						TvEx.mask=TVIF_PARAM | TVIF_TEXT;
						TvEx.hItem=ntv->itemNew.hItem;
						TvEx.pszText=Cap;
						TvEx.cchTextMax=30;
						TreeView_GetItem(m_hTree,&TvEx);

						strcpy(pParam, (LPCTSTR)TvEx.lParam);

						SetDlgItemText(hWnd,IDC_NAME,Cap);
						SetDlgItemText(hWnd,IDC_POST,pParam);

						break;
				}
			}

			break;
		}


		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;

			hdc=BeginPaint(hWnd, &ps);

			EndPaint(hWnd, &ps);

			break;
		}

		case WM_KEYDOWN:
		{
			switch(wParam)
			{
				case VK_ESCAPE:
				{
					SendMessage(hWnd, WM_DESTROY, 0,0);
					break;
				}
			}

			return 0;
		}

		case WM_DESTROY:
		{
			Cleanup();
			PostQuitMessage( 0 );
			return 0;
		}
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}






#if _MSC_VER >= 1200
#pragma warning(pop)
#pragma warning(default: 4996)
#endif