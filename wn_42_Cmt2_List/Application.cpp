// Implementation of the CApplication class.
//
////////////////////////////////////////////////////////////////////////////////

// original source http://soen.kr/


#if _MSC_VER >= 1200
#pragma warning(push)
#pragma warning(disable: 4996)
#endif


#include "_StdAfx.h"
#include <string>
#include <tuple>
#include <vector>


static CApplication*	g_pD3DApp;


CApplication::CApplication()
{
	g_pD3DApp	= this;

	strcpy(m_sCls, "McApi Tutorial");

	m_hInst		= NULL;
	m_hWnd		= NULL;
	m_dWinStyle	= WS_OVERLAPPEDWINDOW| WS_VISIBLE;
	m_dScnX		= 1024;
	m_dScnY		= 768;

	m_bShowCursor= true;
}


LRESULT WINAPI CApplication::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return g_pD3DApp->MsgProc(hWnd, msg, wParam, lParam);
}

INT CApplication::Create( HINSTANCE hInst)
{
	ATOM hr = GlobalFindAtom("edit");

	hr = GlobalFindAtom("datagrid");

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
		, MAKEINTRESOURCE(IDR_MENU1)
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



	#define IDC_NAME		(WM_USER+1)
	#define IDC_TEL			(WM_USER+2)
	#define IDC_ADDR		(WM_USER+3)
	#define IDC_MALE		(WM_USER+4)
	#define IDC_ADD			(WM_USER+5)
	#define IDC_DEL			(WM_USER+6)
	#define IDC_EDIT		(WM_USER+7)
	#define IDC_FIND		(WM_USER+8)

	// 리스트 컨트롤을 자세히 보기로 만든다.
	m_hList=CreateWindow(WC_LISTVIEW,NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_EDITLABELS, 10,10,600,150,m_hWnd,NULL,m_hInst,NULL);

	// 정보를 입력받기 위한 컨트롤들을 만든다.
	CreateWindow("static"	, "이름",WS_CHILD | WS_VISIBLE								,  10,170,50,25,m_hWnd,(HMENU)-1,m_hInst,NULL);
	CreateWindow("edit"		, NULL	,WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL	,  60,170,90,25,m_hWnd,(HMENU)IDC_NAME,m_hInst,NULL);
	CreateWindow("static"	, "전화",WS_CHILD | WS_VISIBLE								, 160,170,50,25,m_hWnd,(HMENU)-1,m_hInst,NULL);
	CreateWindow("edit"		, NULL	,WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL	, 210,170,90,25,m_hWnd,(HMENU)IDC_TEL,m_hInst,NULL);
	CreateWindow("static"	, "주소",WS_CHILD | WS_VISIBLE								, 310,170,50,25,m_hWnd,(HMENU)-1,m_hInst,NULL);
	CreateWindow("edit"		, NULL	,WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL	, 360,170,190,25,m_hWnd,(HMENU)IDC_ADDR,m_hInst,NULL);

	m_hMale=CreateWindow("button","남자",WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,560,170,100,25,m_hWnd,(HMENU)IDC_MALE,m_hInst,NULL);

	// 명령 버튼
	CreateWindow("button","추가",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 110,200,90,25,m_hWnd,(HMENU)IDC_ADD,m_hInst,NULL);
	CreateWindow("button","삭제",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 210,200,90,25,m_hWnd,(HMENU)IDC_DEL,m_hInst,NULL);
	CreateWindow("button","수정",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 310,200,90,25,m_hWnd,(HMENU)IDC_EDIT,m_hInst,NULL);
	CreateWindow("button","검색",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 410,200,90,25,m_hWnd,(HMENU)IDC_FIND,m_hInst,NULL);

	// 크기별로 두 종류의 이미지 리스트를 만들고 리스트 컨트롤에 연결한다.
	m_hImgSm=ImageList_LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_BITMAP1), 16, 1, RGB(255,255,255));
	m_hImgLa=ImageList_LoadBitmap(m_hInst, MAKEINTRESOURCE(IDB_BITMAP2), 32, 1, RGB(255,255,255));
	ListView_SetImageList(m_hList, m_hImgSm, LVSIL_SMALL);
	ListView_SetImageList(m_hList, m_hImgLa, LVSIL_NORMAL);

	// 헤더를 추가한다.
	LVCOLUMN	COL;

	COL.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT |LVCF_SUBITEM;
	COL.fmt=LVCFMT_LEFT;
	COL.cx=150;
	COL.pszText="이름";			// 첫번째 헤더
	COL.iSubItem=0;
	ListView_InsertColumn(m_hList,0,&COL);

	COL.pszText="전화번호";		// 두번째 헤더
	COL.iSubItem=1;
	ListView_InsertColumn(m_hList,1,&COL);

	COL.cx=250;
	COL.pszText="주소";			// 세번째 헤더
	COL.iSubItem=2;
	ListView_InsertColumn(m_hList,2,&COL);


	ListView_SetExtendedListViewStyle(m_hList, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES |LVS_EX_HEADERDRAGDROP |LVS_EX_FLATSB);

	std::vector<std::tuple<std::string, std::string, std::string, int> > people
	{
		std::make_tuple("김상형","123-4567","서울시 강남구 논현동",TRUE),
		std::make_tuple("이송우","543-9876","서울시 구의동",TRUE),
		std::make_tuple("박다희","111-3333","경기도 광명시",FALSE),
		std::make_tuple("오궁섭","236-1818","서울시 강남구 반포동",TRUE),
		std::make_tuple("조기순","358-2277","서울시 압구정동",FALSE),
		std::make_tuple("오뱅훈","548-1109","서울시 신사동",TRUE),
	};

	// 텍스트와 이미지를 가진 아이템들을 등록한다.
	LVITEM		LI;
	auto it = people.begin();
	for (int i=0; i<10 && it != people.end();i++, ++it)
	{
		LI.mask=LVIF_TEXT | LVIF_IMAGE;
		LI.state=0;
		LI.stateMask=0;
		LI.iImage= std::get<3>(*it) ? 0 : 1;
		LI.iSubItem=0;
		LI.iItem=i;
		LI.pszText=(LPSTR)std::get<0>(*it).c_str();
		ListView_InsertItem(m_hList,&LI);
		ListView_SetItemText(m_hList,i,1, (LPSTR)std::get<1>(*it).c_str());
		ListView_SetItemText(m_hList,i,2, (LPSTR)std::get<2>(*it).c_str());
	}






	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );
	::ShowCursor(m_bShowCursor);

	return S_OK;
}


void CApplication::Cleanup()
{
	SAFE_DESTROY_WINDOW(m_hList);

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




void SetListViewStyle(HWND m_hList, DWORD dwView)
{
	DWORD dwStyle;

	dwStyle=GetWindowLong(m_hList, GWL_STYLE);

	if ((dwStyle & LVS_TYPEMASK) != dwView)
	{
		SetWindowLong(m_hList, GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | dwView);
	}
}

LRESULT CApplication::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDM_ICON:
					SetListViewStyle(m_hList, LVS_ICON);
					break;

				case IDM_SMALLICON:
					SetListViewStyle(m_hList, LVS_SMALLICON);
					break;

				case IDM_LIST:
					SetListViewStyle(m_hList, LVS_LIST);
					break;

				case IDM_REPORT:
					SetListViewStyle(m_hList, LVS_REPORT);
					break;

				// 새 항목을 추가한다.
				case IDC_ADD:
				{
					LVITEM	LI;
					int		idx;
					char	sName[256];
					char	sTel[256];
					char	sAddr[256];


					idx=ListView_GetItemCount(m_hList);
					GetDlgItemText(hWnd,IDC_NAME,sName,	sizeof(sName));
					GetDlgItemText(hWnd,IDC_TEL,sTel,	sizeof(sTel));
					GetDlgItemText(hWnd,IDC_ADDR,sAddr,	sizeof(sAddr));

					LI.mask=LVIF_TEXT | LVIF_IMAGE;
					LI.state=0;
					LI.stateMask=0;

					if (SendMessage(m_hMale,BM_GETCHECK,0,0)==BST_CHECKED)
						LI.iImage=0;
					else
						LI.iImage=1;

					LI.iSubItem=0;
					LI.iItem=idx;
					LI.pszText=sName;
					ListView_InsertItem(m_hList,&LI);
					ListView_SetItemText(m_hList,idx,1,sTel);
					ListView_SetItemText(m_hList,idx,2,sAddr);
					break;
				}

				// 선택한 항목을 삭제한다.
				case IDC_DEL:
				{
					int		idx;

					idx=ListView_GetNextItem(m_hList,-1,LVNI_ALL | LVNI_SELECTED);

					if (idx == -1)
						MessageBox(hWnd,"삭제할 항목을 먼저 선택하십시요","알림",MB_OK);
					else
						ListView_DeleteItem(m_hList, idx);

					break;
				}

				// 선택한 항목을 수정한다.
				case IDC_EDIT:
				{
					LVITEM	LI;
					int		idx;
					char	sName[256];
					char	sTel[256];
					char	sAddr[256];

					idx = ListView_GetNextItem(m_hList,-1,LVNI_ALL | LVNI_SELECTED);

					GetDlgItemText(hWnd,IDC_NAME,sName,	sizeof(sName));
					GetDlgItemText(hWnd,IDC_TEL,sTel,	sizeof(sTel));
					GetDlgItemText(hWnd,IDC_ADDR,sAddr,	sizeof(sAddr));

					if (idx == -1)
					{
						MessageBox(hWnd,"수정할 항목을 먼저 선택하십시요","알림",MB_OK);
					}

					else
					{
						LI.mask=LVIF_IMAGE;
						LI.iItem=idx;
						LI.iSubItem=0;

						if (SendMessage(m_hMale,BM_GETCHECK,0,0)==BST_CHECKED)
							LI.iImage=0;
						else
							LI.iImage=1;

						ListView_SetItem(m_hList, &LI);
						ListView_SetItemText(m_hList,idx,0,sName);
						ListView_SetItemText(m_hList,idx,1,sTel);
						ListView_SetItemText(m_hList,idx,2,sAddr);
					}

					break;
				}

				// 이름으로 항목을 검색한다.
				case IDC_FIND:
				{
					LVFINDINFO	fi;
					int			idx;
					char		sName[256];

					GetDlgItemText(hWnd,IDC_NAME,sName,sizeof(sName));

					fi.flags=LVFI_STRING;
					fi.psz=sName;
					fi.vkDirection=VK_DOWN;
					idx=ListView_FindItem(m_hList,-1,&fi);

					if (idx==-1)
					{
						MessageBox(hWnd,"지정한 이름의 항목은 없습니다","알림",MB_OK);
					}
					else
					{
						ListView_SetItemState(m_hList,-1,0,LVIS_FOCUSED | LVIS_SELECTED);
						ListView_SetItemState(m_hList,idx,LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);
						ListView_EnsureVisible(m_hList,idx,FALSE);
					}

					break;
				}

			}

			break;
		}

		case WM_NOTIFY:
		{
			LPNMHDR hdr;
			LPNMLISTVIEW nlv;
			hdr=(LPNMHDR)lParam;
			nlv=(LPNMLISTVIEW)lParam;

			if (hdr->hwndFrom == m_hList)
			{
				switch (hdr->code)
				{
					// 선택된 항목을 에디트에 보여준다.
					case LVN_ITEMCHANGED:
					{
						if (nlv->uChanged == LVIF_STATE && nlv->uNewState == (LVIS_SELECTED | LVIS_FOCUSED))
						{
							char szName[255], szTel[255], szAddr[255];
							ListView_GetItemText(m_hList,nlv->iItem,0,szName,255);
							SetDlgItemText(hWnd,IDC_NAME,szName);
							ListView_GetItemText(m_hList,nlv->iItem,1,szTel,255);
							SetDlgItemText(hWnd,IDC_TEL,szTel);
							ListView_GetItemText(m_hList,nlv->iItem,2,szAddr,255);
							SetDlgItemText(hWnd,IDC_ADDR,szAddr);
						}

						break;
					}
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