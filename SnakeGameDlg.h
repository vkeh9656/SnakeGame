
// SnakeGameDlg.h: 헤더 파일
//

#pragma once

#define MAX_EAT_COUNT		300
#define MAX_SNAKE_LENGTH	250

struct MapPoint 
{
	char x;
	char y;
};

// CSnakeGameDlg 대화 상자
class CSnakeGameDlg : public CDialogEx
{
private:
	CImage m_draw_image;
	CDC m_draw_dc;

	MapPoint m_eat_pos[MAX_EAT_COUNT];		// 2 byte * 300 = 600	-> 아래 주석보다 32배 차이
	MapPoint m_snake_pos[MAX_SNAKE_LENGTH]; 
	//COLORREF m_table[60][80] = { 0, };	// 60 * 80 * 4 byte = 19200 (18.75K)
	//int m_count_map[60][80] = { 0, };

	MapPoint m_pos = { 40, 30 };
	int m_direction = 0;	 // 0: Left, 1: Up, 2: Right, 3: Down
	int m_eat_count = 0;
	int m_remain_count = MAX_EAT_COUNT;

// 생성입니다.
public:
	CSnakeGameDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	void DrawMap();
	void GameOver();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SNAKEGAME_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
