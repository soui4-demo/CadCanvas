// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

class CMainDlg : public SHostWnd
{
public:
	CMainDlg();
	~CMainDlg();

	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	void OnSize(UINT nType, CSize size);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

protected:
	void OnBtnLoadFile();
	void OnBtnSaveFile();
	void OnBtnZoomin();
	void OnBtnZoomout();
	void OnBtnZoomfit();

	void OnCanvasRectSel(IEvtArgs *e);
	void OnLoadDxf();

	//绘制命令-----------------------------------------------------------------------------
	void OnDraw_Line();
	//-------------------------------------------------------------------------------------

	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
		EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
		EVENT_NAME_COMMAND(L"btn_restore", OnRestore)
		EVENT_NAME_COMMAND(L"btn_zoomin", OnBtnZoomin)
		EVENT_NAME_COMMAND(L"btn_zoomout", OnBtnZoomout)
		EVENT_NAME_COMMAND(L"btn_zoomfit",OnBtnZoomfit)
		EVENT_NAME_COMMAND(L"btn_load_file",OnBtnLoadFile)
		EVENT_NAME_COMMAND(L"btn_save_file", OnBtnSaveFile)
		EVENT_NAME_HANDLER(L"cc_test",EventCanvasRectSel::EventID, OnCanvasRectSel)

		EVENT_NAME_COMMAND(L"btn_loaddxf", OnLoadDxf)

		//绘制命令-----------------------------------------------------------------------------
		EVENT_NAME_COMMAND(L"btn_Draw_Line",OnDraw_Line)
		//-------------------------------------------------------------------------------------

	EVENT_MAP_END()
		
	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
private:
	BOOL			m_bLayoutInited;	

	SCadCanvas	* m_pCadCanvas;
};
