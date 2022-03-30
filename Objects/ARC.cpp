#include "stdafx.h"
#include "ARC.h"

namespace SOUI
{

	CArc::CArc(void)
	{

	}

	CArc::CArc(SPoint pt1,double radious,float startAngle,float sweepAngle,bool useCenter)
		:m_ptCenter(pt1),m_radious(radious),m_startAng(startAngle),m_endAng(sweepAngle),m_useCenter(useCenter)
	{
		//内部会转化为绘制多少度，比如画90度~270度时，m_startAng=90，m_endAng=270-90=180度
		//1、坐标的Y值自动变成-y
		//2、起始角度及终止角度的计算
		m_startAng = fmod(m_startAng,360.0);
		m_endAng = fmod(m_endAng,360.0);
		double ang;
		if(m_endAng<m_startAng)//跨360的情况
			ang = m_endAng + (360.0 - m_startAng);
		else
			ang = m_endAng - m_startAng;
		m_startAng = 360.0-m_endAng;
		m_endAng = ang;
	}

	CArc::~CArc(void)
	{

	}

	bool CArc::SetAttribute(SStringW strName,SStringW strValue)
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
		else if(strName == L"startAngle")
			m_startAng = _wtof(strValue);
		else if(strName == L"endAngle")
			m_endAng = _wtof(strValue);
		else if(strName == L"userCenter")
			m_useCenter = _wtoi(strValue)!=0;
		else
			bRet = false;
		return bRet;
	}

	void CArc::OnDraw(IRenderTarget *pRT)
	{
		SAutoRefPtr<IPenS> pen,oldPen;
		pRT->CreatePen(PS_SOLID,IsSelected()?m_crHighlight:m_crNormal,1,&pen);
		pRT->SelectObject(pen,(IRenderObj**)&oldPen);

		pRT->DrawArc(m_rcBound, m_startAng, m_endAng, false);

		pRT->SelectObject(oldPen,NULL);
	}

	bool CArc::HitTest(CPoint pt) const
	{

		return false;
	}

	void CArc::OnContentChanged()
	{
		m_pt1 = m_ptCenter.toPoint();
		CRect rc(m_pt1,CSize(m_radious*2,m_radious*2));
		rc.OffsetRect(-m_radious,-m_radious);
		m_rcBound = rc;
	}

}