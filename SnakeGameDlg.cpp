﻿
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
END_MESSAGE_MAP()


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
	for (int i = 0; i < 300; i++)
	{
		x = rand() % 80;
		y = rand() % 60;
		if (m_table[y][x] == 0) m_table[y][x] = RGB(255, 0, 0);
		else i--;
	}

	m_draw_dc.SelectStockObject(DC_BRUSH);
	m_draw_dc.SelectStockObject(DC_PEN);
	
	m_draw_dc.SetDCPenColor(RGB(48, 48, 12));

	for (int y = 0; y < 60; y++)
	{
		for (int x = 0; x < 80; x++)
		{
			m_draw_dc.SetDCBrushColor(m_table[y][x]);
			m_draw_dc.Rectangle(x * 10, y * 10, x * 10 + 11, y * 10 + 11);
		}
	}

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

	m_draw_dc.Detach();
	m_draw_image.ReleaseDC();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
