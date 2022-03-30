#include "StdAfx.h"
#include "Composite.h"
namespace SOUI{

	CComposite::CComposite(void)
	{
	}

	CComposite::~CComposite(void)
	{
	}

	bool CComposite::AddChild(CCadObj* pObj,bool bUpdate)
	{
		CHILDLIST::iterator it = findChild(pObj);
		if (it != m_childs.end())
			return false;

		m_childs.push_back(pObj);
		OnContentChanged();
		return true;
	}

	bool CComposite::RemoveChild(CCadObj *pObj,bool bUpdate)
	{
		bool bFind = false;
		CHILDLIST::iterator it = findChild(pObj);
		if (it == m_childs.end())
			return false;
		m_childs.erase(it);
		OnContentChanged();
		return true;
	}

	CComposite::CHILDLIST::iterator CComposite::findChild(CCadObj * pObj)
	{
		CHILDLIST::iterator it = m_childs.begin();
		while (it != m_childs.end())
		{
			if (*it == pObj)
				return it;
			it++;
		}
		return m_childs.end();
	}


	void CComposite::OnContentChanged()
	{
		m_rcBound.SetRectEmpty();
		CHILDLIST::iterator it = m_childs.begin();
		while(it!= m_childs.end())
		{
			m_rcBound.UnionRect(m_rcBound,(*it)->GetBoundRect());
			it++;
		}
	}

	void CComposite::OnDraw(IRenderTarget *pRT)
	{
		CHILDLIST::iterator it = m_childs.begin();
		while(it!= m_childs.end())
		{
			(*it)->OnDraw(pRT);
			it++;
		}
	}

	bool CComposite::HitTest(CPoint pt) const
	{
		CHILDLIST::const_iterator it = m_childs.begin();
		while(it!= m_childs.end())
		{
			if((*it)->HitTest(pt))
				return true;
			it++;
		}
		return false;
	}

}
