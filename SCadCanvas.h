#pragma once

#include <iostream>
#include <matrix/SPoint.h>
#include <vector>
#include "Objects/CadObj.h"

#include ".\dxflib3170\dl_creationadapter.h"
namespace SOUI
{
	DEF_EVT_EXT(EventCanvasRectSel,EVT_EXTERNAL_BEGIN+100,{int dummy;})

	class SCadCanvas : public SScrollView, public DL_CreationAdapter
	{
		DEF_SOBJECT(SScrollView,L"cadCanvas")
	public:
		SCadCanvas(void);
		~SCadCanvas(void);

		void SetCanvasSize(CSize szCanvas);

		bool SetZoom(float fZoom);

		float GetZoom() const{return m_fZoom;}

		void Zoomfit();

		void LoadFile(const SStringT & strFileName);
		bool SaveFile(const SStringT & strFileName);

		void LoadDXFFile(const SStringT & strFileName);

	protected:
		//dxflib
		virtual void addLine(const DL_LineData& data);
		virtual void addMText(const DL_MTextData& data);
		virtual void addText(const DL_TextData& data);
		virtual void addArc(const DL_ArcData& data);
		virtual void addCircle(const DL_CircleData& data);
		virtual void addEllipse(const DL_EllipseData& data);
		virtual void addPolyline(const DL_PolylineData& data);
	protected:
		CCadObj * NewObjectFromXml(pugi::xml_node node);

		virtual UINT OnGetDlgCode();
		void RectSelect();
		void OnPaint(IRenderTarget *pRT);
		
		void OnMouseMove(UINT nFlags,CPoint pt);

		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags, CPoint point);

		void OnRButtonDown(UINT nFlags, CPoint point);
		void OnRButtonUp(UINT nFlags, CPoint point);

		BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

		void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
		SOUI_MSG_MAP_BEGIN()
			MSG_WM_PAINT_EX(OnPaint)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_RBUTTONDOWN(OnRButtonDown)
			MSG_WM_RBUTTONUP(OnRButtonUp)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_MOUSEWHEEL(OnMouseWheel)
			MSG_WM_KEYDOWN(OnKeyDown)
		SOUI_MSG_MAP_END()
	public:
		SOUI_ATTRS_BEGIN()
			ATTR_COLOR(L"normalColor",m_crNormal,TRUE)
			ATTR_COLOR(L"highlightColor",m_crHighlight,TRUE)
			ATTR_COLOR(L"rectColor1",m_crSelLeft2Right,TRUE)
			ATTR_COLOR(L"rectColor2", m_crSelRight2Left, TRUE)
			ATTR_COLOR(L"selRectBorderColor", m_crSelBorder,TRUE)
		SOUI_ATTRS_END()
	private:
		float	m_fZoom;

		CRect	m_rcCanvas;

		COLORREF		m_crNormal;
		COLORREF		m_crHighlight;
		COLORREF		m_crSelLeft2Right;
		COLORREF		m_crSelRight2Left;
		COLORREF		m_crSelBorder;
		bool			m_bMoving;
		CPoint			m_ptClick;
		CPoint			m_ptOriClick;

		CPoint			m_ptSel1;
		CPoint			m_ptSel2;

		typedef std::vector<SAutoRefPtr<CCadObj>> OBJLIST;
		OBJLIST m_objects;

	public:
		bool			m_IsDrawLine;		//是否在画线状态
		bool			m_IsLBtnDown;		//左键是否按下
		CPoint			m_StartPt;			//画线的起点

	};

}
