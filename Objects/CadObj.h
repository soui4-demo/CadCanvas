#pragma once
#include <interface/obj-ref-i.h>
#include <helper/obj-ref-impl.hpp>
namespace SOUI
{
	//对象Id类
	class ObjectId
	{
	public:
		ObjectId(){m_Id=-1;}
		ObjectId(int a):m_Id(a){}
		~ObjectId(){}

		int GetId(){return m_Id;}
		void SetId(int a){m_Id=a;}
	private:
		int m_Id;
	};

	class Layer
	{
	public:
		Layer(){}
		~Layer(){}
		Layer(ObjectId id,SStringW layerName,SStringW lineType,double lineWidth)
			:m_Id(id),m_LayerName(layerName),m_LineType(lineType),m_LineWidth(lineWidth)
		{
		}


		/**
		* 描述      读写图层名
		*/
		void SetLayerName(SStringW layerName)
		{
			m_LayerName = layerName;
		}
		SStringW GetLayerName()const
		{
			return m_LayerName;
		}

		/**
		* 描述      读写线型名
		*/
		void SetLineType(SStringW lineType)
		{
			m_LineType = lineType;
		}
		SStringW GetLineType()const
		{
			return m_LineType;
		}

		/**
		* 描述      读写线宽
		*/
		void SetLineWidth(double width)
		{
			m_LineWidth = width;
		}
		double GetLineWidth()const
		{
			return m_LineWidth;
		}

	private:
		ObjectId m_Id;
		SStringW m_LayerName;	//图层名
		SStringW m_LineType;	//线型，实线、虚线……
		double m_LineWidth;	//线宽
	};


	class CCadObj  : public TObjRefImpl<IObjRef>
	{
	public:
		CCadObj(void);
		~CCadObj(void);

	public:
		static bool fEqual(float a,float b);
	public:
		void MarkDeleted(){m_bDeleted = true;}
		void SetSel(bool bSel){m_bSel = bSel;}
		void SetHover(bool bHover){m_bHover = bHover;}
		void SetVisible(bool bVisible){m_bVisible = bVisible;}
		void SetDeleted(bool bDeleted){m_bDeleted = bDeleted;}
		void SetTmp(bool bTmp){m_bTmp=bTmp;}

		bool IsVisible() const {return m_bVisible;}
		bool IsSelected() const {return m_bSel;}
		bool IsHover() const {return m_bHover;}
		bool IsDeleted() const {return m_bDeleted;}
		bool IsTmp()const{return m_bTmp;}

		CRect GetBoundRect() const{return m_rcBound;}
		void UpdateBoundRect() ;
		SMatrix GetMatrix() const {return m_xform;}
		void SetMatrix(SMatrix mtx);
	public:
		virtual void OnDraw(IRenderTarget *pRT) = 0;
		virtual bool HitTest(CPoint pt) const = 0;

		virtual bool IsRectangle() const = 0;
		virtual bool ObjInRect(CRect rc) const;
		virtual bool SetAttribute(SStringW strName,SStringW strValue);
	protected:
		virtual void OnContentChanged();
	protected:
		CRect	m_rcBound;
		SQuad	m_quadBound;

		bool	m_bHover;
		bool    m_bSel;
		bool	m_bDeleted;
		bool    m_bVisible;
		bool	m_bTmp;			//临时对象

		SMatrix			m_xform;		//变形矩阵
		COLORREF		m_crNormal;		//默认颜色
		COLORREF		m_crHighlight;	//亮显颜色
	};

}
