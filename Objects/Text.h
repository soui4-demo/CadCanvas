#pragma once
#include "CadObj.h"

namespace SOUI{

	class CText : public CCadObj
	{
	public:
		static LPCWSTR GetClassName() {return L"text";}
		CText(void);
		~CText(void);
		CText(SStringW text,double angle,YJPoint pt,SStringW font)
			:m_text(text),m_angle(angle),m_pt(pt.toSPoint()),m_strFont(font),m_InsertPt(pt)
		{

		}

	public:
		virtual bool IsRectangle() const {return true;}
		virtual void OnDraw(IRenderTarget *pRT);

		virtual bool HitTest(CPoint pt) const;

		virtual void OnContentChanged();

		virtual bool SetAttribute(SStringW strName,SStringW strValue);

	private:
		SStringW m_text;
		double   m_angle;
		SPoint	 m_pt;
		SStringW m_strFont;

	private:
		YJPoint m_InsertPt;
	};
}
