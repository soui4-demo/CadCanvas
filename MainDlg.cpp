// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"	
#include "FileHelper.h"
#include "probeseldlg.h"

CMainDlg::CMainDlg() 
	: SHostWnd(_T("LAYOUT:XML_MAINWND"))
	, m_pCadCanvas(NULL)
{
	m_bLayoutInited = FALSE;
}

CMainDlg::~CMainDlg()
{
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetMsgHandled(FALSE);
	return 0;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	m_pCadCanvas = FindChildByName2<SCadCanvas>(L"cc_test");
	m_pCadCanvas->LoadFile(L"d:\\test.xml");
	m_pCadCanvas->SetFocus();
	return 0;
}
//TODO:消息映射
void CMainDlg::OnClose()
{
	SNativeWnd::DestroyWindow();
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	if (!m_bLayoutInited) return;
	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}

void CMainDlg::OnBtnZoomin()
{
	m_pCadCanvas->SetZoom(m_pCadCanvas->GetZoom()/1.1f);
}

void CMainDlg::OnBtnZoomout()
{
	m_pCadCanvas->SetZoom(m_pCadCanvas->GetZoom()*1.1f);
}

void CMainDlg::OnBtnZoomfit()
{
	m_pCadCanvas->Zoomfit();
}

void CMainDlg::OnCanvasRectSel(IEvtArgs * e)
{
	SCadCanvas *cadCanvas = sobj_cast<SCadCanvas>(e->Sender());
	//if (cadCanvas->GetSelectedCount() > 0)
	//{
	//	CProbeSelDlg selDlg;
	//	selDlg.DoModal();
	//}
}

void CMainDlg::OnLoadDxf()
{
	CFileDialogEx openDlg (TRUE,_T("dxf"),0,6,_T("dxf file(*.dxf)\0*.dxf\0\0"));
	if (openDlg.DoModal() == IDOK)
	{
		m_pCadCanvas->LoadDXFFile(openDlg.m_szFileName);
	}
}

void CMainDlg::OnDraw_Line()
{
	m_pCadCanvas->SetFocus();
	m_pCadCanvas->m_IsDrawLine = true;
}

void CMainDlg::OnBtnLoadFile()
{
	CFileDialogEx openDlg (TRUE,_T("xml"),0,6,_T("xml file(*.xml)\0*.xml\0\0"));
	if (openDlg.DoModal() == IDOK)
	{
		m_pCadCanvas->LoadFile(openDlg.m_szFileName);
	}
}

void CMainDlg::OnBtnSaveFile()
{
	CFileDialogEx saveDlg(FALSE, _T("xml"), 0, 6, _T("xml file(*.xml)\0*.xml\0\0"));
	if (saveDlg.DoModal() == IDOK)
	{
		m_pCadCanvas->SaveFile(saveDlg.m_szFileName);
	}

}

