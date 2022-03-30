#include "StdAfx.h"
#include "Text.h"

namespace SOUI{

	CText::CText(void):m_angle(0.0)
	{
	}

	CText::~CText(void)
	{
	}

	void CText::OnDraw(IRenderTarget *pRT)
	{
		SAutoRefPtr<IFontS> font,oldFont;
		font = SFontPool::getSingleton().GetFont(m_strFont,100);
		pRT->SelectObject(font,(IRenderObj**)&oldFont);
		COLORREF crOld = pRT->SetTextColor(IsSelected()?m_crHighlight:m_crNormal);
		pRT->TextOut(m_pt.fX,m_pt.fY,m_text,m_text.GetLength());
		pRT->SetTextColor(crOld);
		pRT->SelectObject(oldFont,NULL);
	}

	bool CText::HitTest(CPoint pt) const
	{
		return m_rcBound.PtInRect(pt);
	}

	void CText::OnContentChanged()
	{
		SAutoRefPtr<IRenderTarget> pRT;
		GETRENDERFACTORY->CreateRenderTarget(&pRT,0,0);
		SAutoRefPtr<IFontS> font = SFontPool::getSingleton().GetFont(m_strFont,100);
		pRT->SelectObject(font,NULL);
		CSize szText;
		pRT->MeasureText(m_text,m_text.GetLength(),&szText);
		
		m_rcBound = CRect(m_pt.toPoint(),szText);
	}

	bool CText::SetAttribute(SStringW strName,SStringW strValue)
	{
		bool bRet = CCadObj::SetAttribute(strName,strValue);
		if(bRet) return true;
		bRet = true;
		if(strName == L"text") m_text = strValue;
		else if(strName == L"x") m_pt.fX = _wtof(strValue);
		else if(strName == L"y") m_pt.fY = _wtof(strValue);
		else if(strName == L"angle") m_angle = _wtof(strValue);
		else if(strName == L"font") m_strFont = strValue;
		else bRet = false;
		return bRet;
	}

}
