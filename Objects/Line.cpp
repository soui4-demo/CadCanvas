#include "StdAfx.h"
#include "Line.h"
#pragma warning(disable:4244)

namespace SOUI
{


	CLine::CLine(void)
	{
	}

	CLine::CLine(YJPoint pt1,YJPoint pt2)
		:m_pt1(pt1.toSPoint()),m_pt2(pt2.toSPoint()),m_startPt(pt1),m_endPt(pt2)
	{

	}

	CLine::~CLine(void)
	{
	}

	void CLine::OnDraw(IRenderTarget *pRT)
	{
		SAutoRefPtr<IPenS> pen,oldPen;
		pRT->CreatePen(PS_SOLID,IsSelected()?m_crHighlight:m_crNormal,1,&pen);
		pRT->SelectObject(pen,(IRenderObj**)&oldPen);
		CPoint pts[2];
		pts[0] = m_pt1.toPoint();
		pts[1] = m_pt2.toPoint();
		pRT->DrawLines(pts,2);
		pRT->SelectObject(oldPen,NULL);
	}

	bool CLine::HitTest(CPoint pt) const
	{
		SPoint fpt = SPoint::IMake(pt);
		return m_quadBound.contains(fpt);
	}

	void CLine::OnContentChanged()
	{
		m_rcBound.SetRect(m_pt1.toPoint(),m_pt2.toPoint());
		m_rcBound.NormalizeRect();
		if(IsRectangle())
		{
			if(fEqual(m_pt1.fX , m_pt2.fX))
			{
				m_quadBound.fPts[0]= SPoint::Make(m_pt1.fX-1,m_pt1.fY);
				m_quadBound.fPts[1]= SPoint::Make(m_pt1.fX+1,m_pt1.fY);
				m_quadBound.fPts[2]= SPoint::Make(m_pt1.fX+1,m_pt2.fY);
				m_quadBound.fPts[3]= SPoint::Make(m_pt1.fX-1,m_pt2.fY);
			}else
			{
				m_quadBound.fPts[0]= SPoint::Make(m_pt1.fX,m_pt1.fY-1);
				m_quadBound.fPts[1]= SPoint::Make(m_pt1.fX,m_pt1.fY+1);
				m_quadBound.fPts[2]= SPoint::Make(m_pt2.fX,m_pt2.fY+1);
				m_quadBound.fPts[3]= SPoint::Make(m_pt2.fX,m_pt2.fY-1);
			}
		}else
		{
			m_quadBound.fPts[0]= SPoint::Make(m_pt1.fX-1,m_pt1.fY-1);
			m_quadBound.fPts[1]= SPoint::Make(m_pt1.fX-1,m_pt1.fY+1);
			m_quadBound.fPts[2]= SPoint::Make(m_pt1.fX+1,m_pt2.fY+1);
			m_quadBound.fPts[3]= SPoint::Make(m_pt1.fX+1,m_pt2.fY-1);			
		}
	}

	bool CLine::SetAttribute(SStringW strName,SStringW strValue)
	{
		bool bRet = CCadObj::SetAttribute(strName,strValue);
		if(bRet) return true;
		bRet = true;
		if(strName==L"x1") m_pt1.fX = _wtof(strValue);
		else if(strName ==L"y1") m_pt1.fY = _wtof(strValue);
		else if(strName == L"x2") m_pt2.fX = _wtof(strValue);
		else if(strName == L"y2") m_pt2.fY = _wtof(strValue);
		else bRet = false;
		return bRet;
	}

	bool CLine::IsRectangle() const
	{
		return fEqual(m_pt1.fX , m_pt2.fX )|| fEqual(m_pt1.fY,m_pt2.fY);
	}

}