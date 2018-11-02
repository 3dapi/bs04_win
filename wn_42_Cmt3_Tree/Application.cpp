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

// 우편 번호 구조체
std::vector< std::tuple<int, std::string, std::string, int> > arPost
{
	std::make_tuple(-1,"우편번호부","000-000",0),
	std::make_tuple(0,"서울특별시","000-000",1),
	std::make_tuple(0,"경기도","000-000",1),
	std::make_tuple(0,"강원도","000-000",1),
	std::make_tuple(0,"충청남도","000-000",1),
	std::make_tuple(0,"충청북도","000-000",1),
	std::make_tuple(1,"강남구","000-000",2),
	std::make_tuple(1,"강동구","000-000",2),
	std::make_tuple(1,"강북구","000-000",2),
	std::make_tuple(1,"강서구","000-000",2),
	std::make_tuple(1,"관악구","000-000",2),
	std::make_tuple(1,"광진구","000-000",2),
	std::make_tuple(1,"구로구","000-000",2),
	std::make_tuple(1,"금천구","000-000",2),
	std::make_tuple(1,"노원구","000-000",2),
	std::make_tuple(1,"도봉구","000-000",2),
	std::make_tuple(1,"동대문구","000-000",2),
	std::make_tuple(1,"동작구","000-000",2),
	std::make_tuple(2,"고양시","000-000",2),
	std::make_tuple(2,"일산시","000-000",2),
	std::make_tuple(2,"과천시","000-000",2),
	std::make_tuple(2,"광명시","000-000",2),
	std::make_tuple(2,"구리시","000-000",2),
	std::make_tuple(2,"군포시","000-000",2),
	std::make_tuple(2,"남양주시","000-000",2),
	std::make_tuple(2,"동두천시","000-000",2),
	std::make_tuple(2,"부천시","000-000",2),
	std::make_tuple(3,"강릉시","000-000",2),
	std::make_tuple(3,"삼척시","000-000",2),
	std::make_tuple(6,"개포동","135-240",3),
	std::make_tuple(6,"논현동","135-010",3),
	std::make_tuple(6,"대치동","135-280",3),
	std::make_tuple(6,"도곡동","135-270",3),
	std::make_tuple(6,"삼성동","135-090",3),
	std::make_tuple(6,"세곡동","135-190",3),
	std::make_tuple(6,"수서동","135-220",3),
	std::make_tuple(6,"신사동","135-120",3),
	std::make_tuple(6,"압구정동","135-110",3),
	std::make_tuple(6,"역삼동","135-080",3),
	std::make_tuple(6,"율현동","135-210",3),
	std::make_tuple(7,"고덕동","134-080",3),
	std::make_tuple(8,"미아동","142-100",3),
	std::make_tuple(9,"가양동","157-200",3),
	std::make_tuple(10,"남현동","151-080",3),
	std::make_tuple(10,"봉천동","151-050",3),
	std::make_tuple(10,"신림동","151-010",3),
	std::make_tuple(11,"광장동","143-210",3),
	std::make_tuple(12,"가리봉동","152-020",3),
	std::make_tuple(13,"가산동","153-023",3),
	std::make_tuple(14,"공릉동","139-240",3),
	std::make_tuple(15,"도봉동","132-010",3),
	std::make_tuple(16,"답십리동","130-030",3),
	std::make_tuple(17,"노량진동","156-050",3),
	std::make_tuple(18,"강매동","412-290",3),
	std::make_tuple(18,"고양동","412-500",3),
	std::make_tuple(18,"관산동","412-470",3),
	std::make_tuple(18,"내곡동","412-260",3),
	std::make_tuple(18,"내유동","412-520",3),
	std::make_tuple(18,"대자동","412-480",3),
	std::make_tuple(18,"대장동","412-250",3),
	std::make_tuple(18,"덕은동","412-170",3),
	std::make_tuple(18,"도내동","412-060",3),
	std::make_tuple(18,"동산동","412-090",3),
	std::make_tuple(19,"가좌동","411-440",3),
	std::make_tuple(19,"구산동","411-430",3),
	std::make_tuple(20,"갈현동","427-100",3),
	std::make_tuple(20,"과천동","427-060",3),
	std::make_tuple(20,"관문동","427-020",3),
	std::make_tuple(20,"막계동","427-080",3),
	std::make_tuple(20,"문원동","427-090",3),
	std::make_tuple(20,"별양동","427-040",3),
	std::make_tuple(21,"가학동","423-070",3),
	std::make_tuple(22,"갈매동","471-080",3),
	std::make_tuple(23,"광정동","435-045",3),
	std::make_tuple(24,"가운동","472-060",3),
	std::make_tuple(25,"걸산동","483-070",3),
	std::make_tuple(26,"계수동","422-070",3),
};



// Parent 노드의 차일드를 모두 찾아 트리에 추가한다. 각 차일드에 대해서도 이
// 함수를 재귀적으로 호출함으로써 모든 트리를 다 입력한다.
// pNode:부모의 노드
// pid:부모의 인덱스 번호
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
			// 파라미터에 구조체의 포인터를 저장한다.
			TI.item.lParam=(LPARAM)pParam;
			Node=TreeView_InsertItem(m_hTree, &TI);
			// 차일드의 차일드를 채워 넣는다.
			InsertChild(Node,i++);
		}
	}
}



// 입력된 정보대로 항목을 추가한다.
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
		MessageBox(m_hWnd,"추가할 부모 노드를 먼저 선택해 주십시요.","알림",MB_OK);
	}
	else
	{
		// 부모 노드의 이미지 번호를 구해 항목 추가가 가능한지 조사한다.
		TvEx.mask=TVIF_IMAGE;
		TvEx.hItem=hNow;
		TreeView_GetItem(m_hTree,&TvEx);

		if (TvEx.iImage == 3)
		{
			MessageBox(m_hWnd,"동 아래에는 항목을 추가할 수 없습니다.","알림",MB_OK);
			return;
		}

		// 에디트에 입력된 이름,우편번호를 읽어온다.
		pParam=(char*)malloc(32);
		memset(pParam, 0, 32);
		GetDlgItemText(m_hWnd,IDC_NAME,Title,30);
		GetDlgItemText(m_hWnd,IDC_POST,pParam,32);

		// 항목을 삽입한다.
		TI.hParent=hNow;
		TI.hInsertAfter=TVI_LAST;
		TI.item.mask=TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
		TI.item.iImage=TvEx.iImage+1;
		TI.item.iSelectedImage=TvEx.iImage+1+4;
		TI.item.pszText=Title;
		TI.item.lParam=(LPARAM)pParam;
		TreeView_InsertItem(m_hTree, &TI);

		// 항목을 추가한 후 부모 노드를 확장하여 추가한 결과를 보여준다.
		TreeView_Expand(m_hTree,hNow,TVE_EXPAND);
	}
}

// 현재 선택된 항목을 삭제한다.
void CApplication::DelPost()
{
	HTREEITEM hNow;

	hNow=TreeView_GetSelection(m_hTree);

	if (hNow!=NULL)
	{
		TreeView_DeleteItem(m_hTree,hNow);
	}
}

// 현재 선택된 항목을 수정한다.
void CApplication::EditPost()
{
	HTREEITEM	hNow;
	TVITEMEX	TvEx;
	char*		pParam;
	char		Title[30];

	hNow=TreeView_GetSelection(m_hTree);

	if (hNow==NULL)
	{
		MessageBox(m_hWnd,"수정할 노드를 먼저 선택해 주십시요.","알림",MB_OK);
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


		// 트리 뷰 컨트롤을 만든다.
		m_hTree=CreateWindow(WC_TREEVIEW, ""
			, WS_CHILD | WS_VISIBLE | WS_BORDER | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_SHOWSELALWAYS
			, 10,10,200,300
			, m_hWnd , NULL
			,m_hInst, NULL);

		// 트리뷰에 사용할 이미지 리스트를 만들어 트리뷰에 연결한다.
		m_Image=ImageList_LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_BITMAP1), 16, 1, RGB(255,255,255));

		TreeView_SetImageList(m_hTree, m_Image, TVSIL_NORMAL);

		// 트리에 데이터를 입력한다.
		InsertChild((HTREEITEM)0,-1);

		// 정보를 입력받기 위한 컨트롤들을 만든다.
		CreateWindow("static","이름"
			, WS_CHILD | WS_VISIBLE
			, 220,70,80,25
			, m_hWnd,(HMENU)-1
			, m_hInst,NULL);

		CreateWindow("edit",NULL
			, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL
			, 300,70,90,25
			, m_hWnd, (HMENU)IDC_NAME
			, m_hInst,NULL);

		CreateWindow("static","우편번호"
			, WS_CHILD | WS_VISIBLE
			, 220,100,80,25
			, m_hWnd, (HMENU)-1
			, m_hInst,NULL);

		CreateWindow("edit",NULL
			, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL
			, 300,100,90,25
			, m_hWnd, (HMENU)IDC_POST
			, m_hInst, NULL);

		// 명령 버튼
		CreateWindow("button","추가"
			, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 220,150,90,25
			, m_hWnd, (HMENU)IDC_ADD
			, m_hInst,NULL);

		CreateWindow("button","삭제"
			, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 320,150,90,25
			, m_hWnd, (HMENU)IDC_DEL
			, m_hInst,NULL);

		CreateWindow("button","수정"
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

					// 선택된 항목을 보여준다.
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