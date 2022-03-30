#include "stdafx.h"
#include "Circle.h"

namespace SOUI
{

	CCircle::CCircle(void)
	{

	}

	CCircle::CCircle(SPoint pt1,double radious)
		:m_ptCenter(pt1),m_radious(radious)
	{

	}

	CCircle::~CCircle(void)
	{

	}

	bool CCircle::SetAttribute(SStringW strName,SStringW strValue)
	{
		bool bRet = CCadObj::SetAttribute(strName,strValue);
		if(bRet) return true;
		bRet = true;
		if(strName == L"x")
			m_ptCenter.fX = _wtof(strValue);
		else if(strName == L"y")
			m_ptCenter.fY = _wtof(strValue);
		else if(strName == L"radius")
			m_radious = _wtof(strValue);
		else
			bRet = false;
		return bRet;
	}

	void CCircle::OnDraw(IRenderTarget *pRT)
	{
		SAutoRefPtr<IPenS> pen,oldPen;
		pRT->CreatePen(PS_SOLID,IsSelected()?m_crHighlight:m_crNormal,1,&pen);
		pRT->SelectObject(pen,(IRenderObj**)&oldPen);
		
		pRT->DrawEllipse(&m_rcBound);

		pRT->SelectObject(oldPen,NULL);
	}

	bool CCircle::HitTest(CPoint pt) const
	{
		//SQuad quad;
		//quad.fPts[0] = m_centerPt.AddXYZ(-m_radious,-m_radious).toSPoint();//SPoint::IMake(m_pt1.fX-1,m_pt1.fY);
		//quad.fPts[1] = SPoint::IMake(m_pt1.fX+1,m_pt1.fY);

		//quad.fPts[2] = SPoint::IMake(m_pt2.fX-1,m_pt2.fY);
		//quad.fPts[3] = SPoint::IMake(m_pt2.fX+1,m_pt2.fY);

		//SPoint fpt = SPoint::IMake(pt);
		//return quad.contains(fpt);
		return false;
	}

	void CCircle::OnContentChanged()
	{
		CSize sz(m_radious,m_radious);
		CPoint pt = m_ptCenter.toPoint();
		m_rcBound = CRect(pt,sz*2);
		m_rcBound.OffsetRect(-sz);
	}

}