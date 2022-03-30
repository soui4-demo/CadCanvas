#include "StdAfx.h"
#include "CadObj.h"
#include <helper/SplitString.h>

namespace SOUI
{
	CCadObj::CCadObj(void):m_bHover(false),m_bSel(false),m_bDeleted(false),m_bVisible(true),m_crNormal(RGBA(0,0,0,255))
		,m_crHighlight(RGBA(255,0,0,255))
	{
	}

	CCadObj::~CCadObj(void)
	{
	}



	void CCadObj::OnContentChanged()
	{//update bound
		m_rcBound.SetRectEmpty();
	}

	bool CCadObj::ObjInRect(CRect rc) const
	{
		if(m_xform.isIdentity())
		{
			CRect rcInter = rc & m_rcBound;
			bool bRet = rcInter.IsRectEmpty();
			if(bRet)
				return false;
			if(IsRectangle())
				return true;
			SAutoRefPtr<IRegionS> rgn;
			GETRENDERFACTORY->CreateRegion(&rgn);
			POINT pts[4];
			for(int i=0;i<4;i++)
			{
				pts[i] = m_quadBound.fPts[i].toPoint();
			}
			rgn->CombinePolygon(pts,4,WINDING,RGN_COPY);
			return rgn->RectInRegion(rc);
		}else
		{
			SQuad quad;
			if(IsRectangle())
				m_xform.mapRectToQuad(quad.fPts,SRect::IMake(m_rcBound));
			else
				m_xform.mapPoints(quad.fPts,m_quadBound.fPts,4);
			POINT pts[4];
			for(int i=0;i<4;i++)
			{
				pts[i] = quad.fPts[i].toPoint();
			}
			SAutoRefPtr<IRegionS> rgn;
			GETRENDERFACTORY->CreateRegion(&rgn);
			rgn->CombinePolygon(pts,4,WINDING,RGN_COPY);

			return rgn->RectInRegion(rc);
		}
	}

	void CCadObj::UpdateBoundRect()
	{
		OnContentChanged();
	}

	bool CCadObj::SetAttribute(SStringW strName,SStringW strValue)
	{
		bool bRet = false;
		if(strName == L"normal_color")
			bRet = SColorParser::ParseValue(strValue,m_crNormal);
		else if(strName == L"selected_color")
			bRet = SColorParser::ParseValue(strValue,m_crHighlight);
		else if(strName == L"offset")
		{
			SStringWList values;
			SplitString(strValue,L',',values);
			if(values.GetCount()==2)
			{
				float x = _wtof(values[0]);
				float y = _wtof(values[1]);
				m_xform.setTranslate(x,y);
				bRet = true;
			}
		}else if(strName == L"rotate")
		{
			UpdateBoundRect();
			CPoint ptCenter = m_rcBound.CenterPoint();
			float value = _wtof(strValue);
			m_xform.preTranslate(-ptCenter.x,-ptCenter.y);
			m_xform.rotate(value);
			m_xform.postTranslate(ptCenter.x,ptCenter.y);
			bRet = true;
		}
		return bRet;
	}

	bool CCadObj::fEqual(float a,float b)
	{
		return fabs(a-b)<0.0000001;
	}

	void CCadObj::SetMatrix(SMatrix mtx)
	{
		m_xform = mtx;
	}

}
