
// SnakeGameDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SnakeGame.h"
#include "SnakeGameDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSnakeGameDlg 대화 상자



CSnakeGameDlg::CSnakeGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SNAKEGAME_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSnakeGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSnakeGameDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CSnakeGameDlg::DrawMap()
{
	m_draw_dc.SelectStockObject(DC_BRUSH);
	m_draw_dc.SelectStockObject(DC_PEN);

	m_draw_dc.SetDCPenColor(RGB(48, 48, 12));
	m_draw_dc.SetDCBrushColor(RGB(0, 0, 0));

	// Map 그리기 담당
	for (int y = 0; y < 60; y++)
	{
		for (int x = 0; x < 80; x++)
		{
			/*if (m_count_map[y][x] > 0)
			{
				m_count_map[y][x]--;
				if (m_count_map[y][x] == 0) m_table[y][x] = 0;
			}
			m_draw_dc.SetDCBrushColor(m_table[y][x]);*/
			m_draw_dc.Rectangle(x * 10, y * 10, x * 10 + 11, y * 10 + 11);
		}
	}

	// 먹이 그리기 담당
	MapPoint* p = m_eat_pos;
	m_draw_dc.SetDCBrushColor(RGB(255, 0, 0));
	for (int i = 0; i < m_remain_count; i++, p++) // 먹이가 존재하는 갯수만큼 반복하면서 빨간색 사각형 그리기
	{
		m_draw_dc.Rectangle(p->x * 10, p->y * 10, p->x * 10 + 11, p->y * 10 + 11);
	}

	// 뱀 그리기 담당
	p = m_snake_pos;
	m_draw_dc.SetDCBrushColor(RGB(0, 255, 0));
	for (int i = 0; i < m_eat_count + 1; i++, p++)
	{
		m_draw_dc.Rectangle(p->x * 10, p->y * 10, p->x * 10 + 11, p->y * 10 + 11);
	}
}


// CSnakeGameDlg 메시지 처리기
BOOL CSnakeGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	m_draw_image.Create(801, 601, 32);
	m_draw_dc.Attach(m_draw_image.GetDC());

	srand((unsigned int)time(NULL));
	int x, y;
	MapPoint* p = m_eat_pos, *p_check;
	for (int i = 0; i < MAX_EAT_COUNT; i++, p++)
	{
		p->x = rand() % 80;
		p->y = rand() % 60;

		p_check = m_eat_pos;
		for (int sub_i = 0; sub_i < i; sub_i++, p_check++)
		{
			if (p_check->x == p->x && p_check->y == p->y) // 먹이가 같은 구간에 생성될 경우
			{
				i--;
				p--;
				break;
			}
		}
	}

	/*m_table[m_pos.y][m_pos.x] = RGB(0, 255, 0);
	m_count_map[m_pos.y][m_pos.x] = m_eat_count + 1;*/

	DrawMap();

	SetTimer(1, 50, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSnakeGameDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		m_draw_image.Draw(dc, 0, 0);
		// CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSnakeGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSnakeGameDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);
	m_draw_dc.Detach();
	m_draw_image.ReleaseDC();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CSnakeGameDlg::GameOver()
{
	KillTimer(1);
	MessageBox(L"미션에 실패하였습니다!", L"Game Over", MB_ICONSTOP);
}

void CSnakeGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		// m_table[m_pos.y][m_pos.x] = 0;
		if (m_direction == 0) m_pos.x--;		// left
		else if (m_direction == 1) m_pos.y--;	// up
		else if (m_direction == 2) m_pos.x++;	// right
		else m_pos.y++;	// down

		if (m_pos.x >= 0 && m_pos.x <= 79 && m_pos.y >= 0 && m_pos.y <= 59)
		{
			MapPoint* p = m_eat_pos;
			int eat_flag = 0;
			
			// 먹이 체크 코드
			for (int i = 0; i < m_remain_count; i++, p++)
			{
				if (p->x == m_pos.x && p->y == m_pos.y)
				{
					m_eat_count++;
					m_remain_count--;
					SetDlgItemInt(IDC_EAT_COUNT_EDIT, m_eat_count);
					if (m_remain_count > 0 && i < m_remain_count)
					{
						p->x = m_eat_pos[m_remain_count].x;
						p->y = m_eat_pos[m_remain_count].y;
					}
					eat_flag = 1;
					break;
				}
			}

			// 먹이를 먹지 못한 경우에 자신의 머리랑 꼬리랑 부딪혔는지 체크
			if (m_eat_count != 0)
			{
				if (eat_flag == 0)
				{
					p = m_snake_pos;
					for (int i = 0; i < m_eat_count; i++, p++)
					{
						if (p->x == m_pos.x && p->y == m_pos.y)
						{
							GameOver();
							return;
						}
					}
				}

				// 꼬리를 뒤로 이동
				p = m_snake_pos + m_eat_count;
				for (int i = 0; i < m_eat_count; i++, p--)
				{
					*p = *(p - 1);
				}
			}
			// 머리 위치 대입
			m_snake_pos[0].x = m_pos.x;
			m_snake_pos[0].y = m_pos.y;
			

			//if (m_table[m_pos.y][m_pos.x] == RGB(255, 0, 0))
			//{
			//	m_eat_count++;
			//	SetDlgItemInt(IDC_EAT_COUNT_EDIT, m_eat_count);
			//}
			//else if (m_table[m_pos.y][m_pos.x])
			//{
			//	GameOver();
			//	return;
			//}
			//m_table[m_pos.y][m_pos.x] = RGB(0, 255, 0);
			//m_count_map[m_pos.y][m_pos.x] = m_eat_count + 2;

			DrawMap();
			CClientDC dc(this);
			m_draw_image.Draw(dc, 0, 0);
		}
		else
		{
			GameOver();
		}
	}
	else
	{
		CDialogEx::OnTimer(nIDEvent);
	}
	
}


BOOL CSnakeGameDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam >= VK_LEFT && pMsg->wParam <= VK_DOWN)
		{
			int old_direction = m_direction;
			m_direction = pMsg->wParam - VK_LEFT;
			if (old_direction > m_direction)
			{
				if ((old_direction - m_direction) == 2) m_direction = old_direction;
			}
			else 
			{
				if ((m_direction - old_direction) == 2) m_direction = old_direction;
			}
				
			
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
