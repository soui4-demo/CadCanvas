//***********************************************
// Copyright 2019-2029
// All rights reserved
// 
// 文件名：  YJPoint
// 作者：    YiJia
// 时间：    2020/11/06
// 描述：    点
// 修改记录：
// 修改时间：
//***********************************************
#pragma once

namespace SOUI
{
	class YJPoint
	{
	public:
		YJPoint(){}
		~YJPoint(){}
		YJPoint(double a,double b,double c)
			:x(a),y(-b),z(c)
		{
		}

		YJPoint AddXYZ(double _x,double _y,double _z=0)
		{
			return YJPoint(x+_x,y+_y,z+_z);
		}

		SPoint toSPoint() const
		{
			return SPoint::Make(x,y);
		}
		
		CPoint toPoint() const
		{
			return CPoint(int(x+0.5f),int(y+0.5));
		}
		double x;
		double y;
		double z;
	};

}