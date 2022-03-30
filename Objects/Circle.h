//***********************************************
// Copyright 2019-2029
// All rights reserved
// 
// 文件名：  Circle
// 作者：    YiJia
// 时间：    2020/11/16
// 描述：    
// 修改记录：
// 修改时间：
//***********************************************
#pragma once
#include "CadObj.h"

namespace SOUI
{

	class CCircle : public CCadObj
	{
	public:
		CCircle(void);
		CCircle(SPoint pt1,double radious);
		~CCircle(void);
		static LPCWSTR GetClassName() {return L"circle";}
	public:
		virtual bool SetAttribute(SStringW strName,SStringW strValue);

	protected:
		virtual void OnDraw(IRenderTarget *pRT);

		virtual bool HitTest(CPoint pt) const;

		virtual void OnContentChanged();
		virtual bool IsRectangle() const {return true;}
	private:
		SPoint m_ptCenter;
		double m_radious;
	};

}