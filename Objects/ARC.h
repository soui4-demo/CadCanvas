//***********************************************
// Copyright 2019-2029
// All rights reserved
// 
// 文件名：  ARC
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

	class CArc : public CCadObj
	{
	public:
		CArc(void);

		/**
		* 描述      圆弧，注意构造函数中对于点及角度的转化
		* 
		* 参数      YJPoint pt1
		* 参数      double radious
		* 参数      float startAngle		//起始角度
		* 参数      float sweepAngle		//终止角度
		* 参数      bool useCenter
		* 返回值    
		*/
		CArc(SPoint pt1,double radious,float startAngle,float sweepAngle,bool useCenter);
		~CArc(void);
		static LPCWSTR GetClassName() {return L"arc";}
	public:
		virtual bool SetAttribute(SStringW strName,SStringW strValue);

	protected:
		virtual void OnDraw(IRenderTarget *pRT);

		virtual bool HitTest(CPoint pt) const;

		virtual void OnContentChanged();

		//todo:hjx
		virtual bool IsRectangle() const {return true;}
	private:
		CPoint m_pt1;
	private:
		SPoint m_ptCenter;
		double m_radious;
		double m_startAng;
		double m_endAng;
		bool m_useCenter;
	};

}