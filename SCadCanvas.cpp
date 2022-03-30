#include "StdAfx.h"
#include "SCadCanvas.h"
#include <helper/SplitString.h>
#include <fstream>
#include <math.h>
using namespace std;
#include <matrix/SMatrix.h>
#ifndef INT32_MAX
#define INT32_MAX        2147483647i32
#endif

#include "objects/Line.h"
#include "Objects/Text.h"
#include "Objects/Composite.h"

#include ".\dxflib3170\dl_dxf.h"
#include ".\dxflib3170\dl_creationadapter.h"
#include "Objects/YJPoint.h"
#include "Objects/Circle.h"
#include "Objects/ARC.h"
namespace SOUI
{
	static const int kMinVisibleTextSize=10;
	static const float kLineHitExt = 5.0f;
	static const float kPI = 3.1415926f;

	SCadCanvas::SCadCanvas(void)
		:m_fZoom(1.0f)
		, m_crNormal(RGBA(0,0,0,255))
		,m_crHighlight(RGBA(255,0,0,255))
		, m_crSelLeft2Right(RGBA(205,232,255,128))
		, m_crSelRight2Left(RGBA(92, 192, 255, 128))
		, m_crSelBorder(RGBA(0,120,215,128))
		, m_bMoving(false)
		, m_IsDrawLine(false),m_IsLBtnDown(false)
	{
		m_bFocusable = TRUE;
		GetEventSet()->addEvent(EVENTID(EventCanvasRectSel));
	}

	SCadCanvas::~SCadCanvas(void)
	{
	}

	void SCadCanvas::SetCanvasSize(CSize szCanvas)
	{
		SIZE szView = szCanvas;
		szView.cx *= m_fZoom;
		szView.cy *= m_fZoom;
		SetViewSize(szView);
	}

	bool SCadCanvas::SetZoom(float fZoom)
	{
		if (fZoom < 1e-6)
			return false;
		m_fZoom = fZoom;
		SIZE szView = m_rcCanvas.Size();
		szView.cx *= m_fZoom;
		szView.cy *= m_fZoom;
		SetViewSize(szView);
		return true;
	}

	void SCadCanvas::Zoomfit()
	{
		CRect rcWnd = SWindow::GetClientRect();
		SIZE szCanvas = m_rcCanvas.Size();
		float zoomX = rcWnd.Width()*1.0f/ szCanvas.cx;
		float zoomY = rcWnd.Height()*1.0f/ szCanvas.cy;
		SetZoom(smin(zoomX,zoomY));
	}


	void SCadCanvas::OnPaint(IRenderTarget *pRT)
	{
		SPainter painter;
		BeforePaint(pRT,painter);

		CRect rcClient = GetClientRect();
		pRT->FillSolidRect(&rcClient,RGBA(255,255,255,255));
		CRect rcView = m_rcCanvas;
		rcView.left *= m_fZoom;
		rcView.top *= m_fZoom;
		rcView.right *= m_fZoom;
		rcView.bottom *= m_fZoom;

		SAutoRefPtr<IPenS> pen,oldPen;
		pRT->OffsetViewportOrg(rcClient.left - rcView.left - m_ptOrigin.x, rcClient.top - rcView.top - m_ptOrigin.y, NULL);
		CRect rcClip = rcClient;
		rcClip.MoveToXY(CPoint(m_ptOrigin)+rcView.TopLeft());
		rcClip.left /= m_fZoom;
		rcClip.top /= m_fZoom;
		rcClip.right /= m_fZoom;
		rcClip.bottom /= m_fZoom;
		SAutoRefPtr<IRegionS> rgn;
		GETRENDERFACTORY->CreateRegion(&rgn);
		rgn->CombineRect(rcClip, RGN_COPY);

		pRT->CreatePen(PS_DASH, RGBA(0, 0, 0, 255), 1, &pen);
		pRT->SelectObject(pen, (IRenderObj**)&oldPen);
		pRT->DrawRectangle(rcView);
		if (rcView.PtInRect(CPoint(0, 0)))
		{
			{
				CPoint pt[2] = { rcView.TopLeft(),rcView.BottomRight() };
				pt[0].y = pt[1].y = 0;
				pRT->DrawLines(pt, 2);
			}
			{
				CPoint pt[2] = { rcView.TopLeft(),rcView.BottomRight() };
				pt[0].x = pt[1].x = 0;
				pRT->DrawLines(pt, 2);
			}
		}

		//draw objects
		OBJLIST::iterator it = m_objects.begin();
		while(it != m_objects.end())
		{
			if( (*it)->IsDeleted() )
			{
				it++;
				continue;
			}

			SMatrix mtx = (*it)->GetMatrix();
			if(mtx.isIdentity())
			{
				(*it)->OnDraw(pRT);
			}else
			{
				float oldMtx[9];
				pRT->SetTransform(mtx.Data()->fMat,oldMtx);
				(*it)->OnDraw(pRT);
				pRT->SetTransform(oldMtx,NULL);
			}
			it++;
		}

		//draw select rect
		if (m_ptSel1 != m_ptSel2)
		{
			COLORREF cr = m_ptSel1.x < m_ptSel2.x ? m_crSelLeft2Right : m_crSelRight2Left;
			SAutoRefPtr<IPenS> pen;
			pRT->CreatePen(PS_SOLID, m_crSelBorder, 1, &pen);
			pRT->SelectObject(pen,NULL);

			CRect rcSel(m_ptSel1, m_ptSel2);
			rcSel.OffsetRect(rcView.TopLeft());
			rcSel.NormalizeRect();
			pRT->FillSolidRect(rcSel, cr);
			rcSel.InflateRect(1, 1);
			pRT->DrawRectangle(rcSel);
		}

		//pRT->SelectObject(curFont);
		pRT->SelectObject(oldPen,NULL);
		pRT->OffsetViewportOrg(-(rcClient.left-rcView.left -m_ptOrigin.x),-(rcClient.top-rcView.top -m_ptOrigin.y),NULL);
		AfterPaint(pRT,painter);
	}

	void SCadCanvas::LoadFile(const SStringT & strFileName)
	{
		m_rcCanvas = CRect();
		CRect rcIndex;
		int i = 0;

		m_objects.clear();

		pugi::xml_document xmlDoc;
		if(xmlDoc.load_file(strFileName))
		{
			pugi::xml_node ele = xmlDoc.first_child().first_child();
			while(ele)
			{
				CCadObj *pObj = NewObjectFromXml(ele);
				if(pObj)
				{
					m_objects.push_back(pObj);
					pObj->Release();
				}

				ele = ele.next_sibling();
			}
		}

		OBJLIST::iterator it = m_objects.begin();
		while(it!=m_objects.end())
		{
			CRect rcBound = (*it)->GetBoundRect();
			m_rcCanvas.UnionRect(m_rcCanvas,rcBound);
			it ++;
		}
		m_rcCanvas.InflateRect(100, 100);
		SetViewSize(m_rcCanvas.Size());
	}


	void SCadCanvas::OnMouseMove(UINT nFlags,CPoint pt)
	{
		if (m_bMoving)
		{
			CRect rcClient = GetClientRect();
			m_ptSel2 = pt - rcClient.TopLeft() + GetViewOrigin();
			if (nFlags & MK_RBUTTON)
			{
				Invalidate();
			}
			else
			{
				RectSelect();
			}
		}
		else
		{
			if(m_IsDrawLine && m_IsLBtnDown)
			{
				//删除所有临时对象
				OBJLIST::iterator it = m_objects.begin();
				while(it!=m_objects.end())
				{
					if( (*it)->IsTmp() )
						it = m_objects.erase(it);
					else
						it ++;
				}
				//增加一个临时线
				CRect rcClient = GetClientRect();
				CPoint endPt = pt - rcClient.TopLeft() + GetViewOrigin();
				CCadObj *pObj = new CLine(YJPoint(m_StartPt.x,-m_StartPt.y,0),YJPoint(endPt.x,-endPt.y,0));
				if(pObj)
				{
					pObj->SetTmp(true);
					m_objects.push_back(pObj);
					pObj->Release();
				}
				pObj->UpdateBoundRect();
			}
		}
	}

	//delete hit object.
	void SCadCanvas::OnRButtonDown(UINT nFlags, CPoint point)
	{
		m_ptClick = point;
		m_ptOriClick = GetViewOrigin();
		m_bMoving = true;

		CRect rcClient = GetClientRect();
		m_ptSel1 = m_ptSel2 = point - rcClient.TopLeft() + GetViewOrigin();
		SetCapture();
	}

#if _MSC_VER <= 1600
	double round(double r)
	{
		return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
	}
#endif

	void SCadCanvas::OnRButtonUp(UINT nFlags, CPoint pt)
	{
		ReleaseCapture();
		m_bMoving = false;
		bool bZoom = m_ptSel1 != m_ptSel2;

		if (!bZoom)
		{
		}
		else
		{//do zoom
			CRect rcView(m_ptSel1, m_ptSel2);
			rcView.NormalizeRect();
			CRect rcClient = GetClientRect();
			double fRatioView = rcView.Width()*1.0 / rcView.Height();
			double fRadioClient = rcClient.Width()*1.0 / rcClient.Height();
			if (fRatioView < fRadioClient)
			{
				int wid = round(fRadioClient * rcView.Height());
				rcView.InflateRect((wid - rcView.Width()) / 2, 0);
			}
			else
			{
				int hei = round(rcView.Width() / fRadioClient);
				rcView.InflateRect(0, (hei - rcView.Height()) / 2);
			}

			float fZoom = rcClient.Width()*1.0f / rcView.Width();
			CPoint ptOrg = rcView.TopLeft();
			ptOrg.x /= m_fZoom;
			ptOrg.y /= m_fZoom;
			SetZoom(fZoom*m_fZoom);
			ptOrg.x *= m_fZoom;
			ptOrg.y *= m_fZoom;
			SetViewOrigin(ptOrg);
		}
		m_ptSel1 = m_ptSel2 = CPoint();
		Invalidate();
	}


	bool SCadCanvas::SaveFile(const SStringT & strFileName)
	{
		return true;
	}


	void SCadCanvas::LoadDXFFile(const SStringT & strFileName)
	{
		m_rcCanvas = CRect();
		CRect rcIndex;
		int i = 0;

		m_objects.clear();

		// Load DXF file into memory:
		DL_Dxf* dxf = new DL_Dxf();
		if (!dxf->in(std::string(S_CT2A(strFileName)), this)) { // if file open failed
			//std::cerr << file << " could not be opened.\n";
			return;
		}
		delete dxf;

		OBJLIST::iterator it = m_objects.begin();
		while(it!=m_objects.end())
		{
			CRect rcBound = (*it)->GetBoundRect();
			m_rcCanvas.UnionRect(m_rcCanvas,rcBound);
			it ++;
		}
		m_rcCanvas.InflateRect(100, 100);
		SetViewSize(m_rcCanvas.Size());
	}

	void SCadCanvas::OnLButtonDown(UINT nFlags, CPoint point)
	{
		__super::OnLButtonDown(nFlags,point);
		m_ptClick = point;
		m_ptOriClick = GetViewOrigin();

		CRect rcClient = GetClientRect();
		m_ptSel1 = m_ptSel2 = point - rcClient.TopLeft() +  GetViewOrigin();
		if(m_IsDrawLine)
		{
			m_bMoving = false;
			m_IsLBtnDown = true;
			m_StartPt = m_ptSel1;
		}
		else
		{
			m_bMoving = true;
		}
	}

	void SCadCanvas::OnLButtonUp(UINT nFlags, CPoint point)
	{
		__super::OnLButtonUp(nFlags,point);
		m_bMoving = false;
		bool bRectSel = m_ptSel1 != m_ptSel2;
		if (!bRectSel)
		{//clear selection.
			CRect rcClient = GetClientRect();
			m_ptSel1 = m_ptSel2 = point - rcClient.TopLeft() +  GetViewOrigin();
			m_ptSel1.x -= 5;
			m_ptSel1.y -= 5;
			m_ptSel2.x += 5;
			m_ptSel2.y += 5;
			RectSelect();
		}
		m_ptSel1 = m_ptSel2 = CPoint();
		Invalidate();
		if (bRectSel)
		{
			EventCanvasRectSel evt(this);
			FireEvent(evt);
		}
	}

	static const float kZoomRate = 1.1f;
	BOOL SCadCanvas::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		if (GetKeyState(VK_CONTROL) & 0x80)
		{//zoom 
			m_zDelta += zDelta;
			short delta = m_zDelta / WHEEL_DELTA;
			m_zDelta %= WHEEL_DELTA;

			float fZoom = GetZoom();
			if (delta > 0)
			{//zoomout
				fZoom *= delta * kZoomRate;
			}
			else
			{
				fZoom /= -delta * kZoomRate;
			}
			SetZoom(fZoom);
		}
		else
		{
			SetMsgHandled(FALSE);
		}
		return TRUE;
	}


	void SCadCanvas::addLine(const DL_LineData& data)
	{
		CCadObj *pObj = new CLine(YJPoint(data.x1,data.y1,data.z1),YJPoint(data.x2,data.y2,data.z1));
		if(pObj)
		{
			m_objects.push_back(pObj);
			pObj->Release();
		}
		pObj->UpdateBoundRect();
	}

	void SCadCanvas::addMText(const DL_MTextData& data)
	{

	}

	void SCadCanvas::addText(const DL_TextData& data)
	{
		CCadObj *pObj = new CText(S_CA2W(SStringA(data.text.c_str())),data.angle,
			YJPoint(data.ipx,data.ipy,data.ipz),S_CA2W(SStringA(data.style.c_str())) );
		if(pObj)
		{
			m_objects.push_back(pObj);
			pObj->Release();
		}
		pObj->UpdateBoundRect();
	}

	void SCadCanvas::addArc(const DL_ArcData& data)
	{
		CCadObj *pObj = new CArc(SPoint::Make(data.cx,data.cy),data.radius,data.angle1,data.angle2,true );
		if(pObj)
		{
			m_objects.push_back(pObj);
			pObj->Release();
		}
		pObj->UpdateBoundRect();
	}

	void SCadCanvas::addCircle(const DL_CircleData& data)
	{
		CCadObj *pObj = new CCircle(SPoint::Make(data.cx,data.cy),data.radius );
		if(pObj)
		{
			m_objects.push_back(pObj);
			pObj->Release();
		}
		pObj->UpdateBoundRect();
	}

	void SCadCanvas::addEllipse(const DL_EllipseData& data)
	{

	}

	void SCadCanvas::addPolyline(const DL_PolylineData& data)
	{

	}

	void SCadCanvas::RectSelect()
	{
		bool bPartSel = m_ptSel1.x < m_ptSel2.x;
		CRect rcSel(m_ptSel1, m_ptSel2);
		CRect rcView = m_rcCanvas;
		rcView.left *= m_fZoom;
		rcView.top *= m_fZoom;
		rcView.right *= m_fZoom;
		rcView.bottom *= m_fZoom;
		rcSel.OffsetRect(rcView.TopLeft());
		rcSel.left /= m_fZoom;
		rcSel.top /= m_fZoom;
		rcSel.right /= m_fZoom;
		rcSel.bottom /= m_fZoom;
		rcSel.NormalizeRect();


		OBJLIST::iterator it = m_objects.begin();
		while(it!=m_objects.end())
		{
			bool bInSel = (*it)->ObjInRect(rcSel);
			(*it)->SetSel(bInSel);
			it ++;
		}

		Invalidate();
	}

	CCadObj * SCadCanvas::NewObjectFromXml(pugi::xml_node ele)
	{
		CCadObj *pObj = NULL;
		if(wcscmp(ele.name(),CLine::GetClassName())==0)
		{
			pObj = new CLine;
		}else if(wcscmp(ele.name(),CText::GetClassName())==0)
		{
			pObj = new CText;
		}else if(wcscmp(ele.name(),CArc::GetClassName())==0)
		{
			pObj = new CArc;
		}else if(wcscmp(ele.name(),CCircle::GetClassName())==0)
		{
			pObj = new CCircle;
		}else if(wcscmp(ele.name(),CComposite::GetClassName())==0)
		{
			CComposite *pComposite = new CComposite();
			pugi::xml_node child = ele.first_child();
			while(child)
			{
				CCadObj *pChild = NewObjectFromXml(child);
				if(pChild)
				{
					pComposite->AddChild(pChild);
				}
				child = child.next_sibling();
			}
			pObj = pComposite;
		}
		if(pObj)
		{
			pugi::xml_attribute attr = ele.first_attribute();
			while(attr)
			{
				SStringW name = attr.name();
				SStringW value = attr.value();
				pObj->SetAttribute(name,value);
				attr = attr.next_attribute();
			}
		}
		pObj->UpdateBoundRect();
		return pObj;
	}

	UINT SCadCanvas::OnGetDlgCode()
	{
		return SC_WANTALLKEYS;
	}

	void SCadCanvas::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		SWindow::OnGetDlgCode();
		SLOG_INFO("OnKeyDown, nChar="<<nChar);
		if(nChar == VK_DELETE)
		{//delete
			//遍历当前已选中的对象，将其设置为删除
			OBJLIST::iterator it = m_objects.begin();
			while(it!=m_objects.end())
			{
				if((*it)->IsSelected())
				{
					(*it)->SetSel(false);
					(*it)->SetDeleted(true);
				}
				it ++;
			}

			Invalidate();
		}
	}


}
