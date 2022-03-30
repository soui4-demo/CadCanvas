#pragma once
#include <vector>
#include "CadObj.h"
namespace SOUI
{

	class CComposite : public CCadObj
	{
	public:
		static LPCWSTR GetClassName() {return L"composite";}
		CComposite(void);
		~CComposite(void);

	public:
		bool AddChild(CCadObj* pObj,bool bUpdateBound=true);
		bool RemoveChild(CCadObj *pObj,bool bUpdateBound=true);
	public:
		virtual void OnDraw(IRenderTarget *pRT);

		virtual bool HitTest(CPoint pt) const;
		
		virtual bool IsRectangle() const {return true;}
	protected:
		virtual void OnContentChanged();
	private:
		typedef std::vector< SAutoRefPtr<CCadObj> > CHILDLIST;
		CHILDLIST::iterator findChild(CCadObj* pObj);


		CHILDLIST m_childs;

	};
}
