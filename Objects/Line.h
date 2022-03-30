#pragma once
#include "CadObj.h"
#include "YJPoint.h"

namespace SOUI
{

	class CLine : public CCadObj
	{
	public:
		CLine(void);
		CLine(YJPoint pt1,YJPoint pt2);
		~CLine(void);
		static LPCWSTR GetClassName() {return L"line";}
	public:
		virtual bool SetAttribute(SStringW strName,SStringW strValue);

	protected:
		virtual bool IsRectangle() const;

		virtual void OnDraw(IRenderTarget *pRT);

		virtual bool HitTest(CPoint pt) const;

		virtual void OnContentChanged();


	private:
		SPoint m_pt1;
		SPoint m_pt2;

	private:
		YJPoint m_startPt;
		YJPoint m_endPt;
	};

}