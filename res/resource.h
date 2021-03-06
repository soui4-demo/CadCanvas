//stamp:0933261c308daad7
/*<------------------------------------------------------------------------------------------------->*/
/*该文件由uiresbuilder生成，请不要手动修改*/
/*<------------------------------------------------------------------------------------------------->*/
#ifndef _UIRES_H_
#define _UIRES_H_
	struct _UIRES{
		struct _UIDEF{
			const TCHAR * XML_INIT;
			}UIDEF;
		struct _LAYOUT{
			const TCHAR * XML_MAINWND;
			const TCHAR * dlg_Probe_Select;
			}LAYOUT;
		struct _values{
			const TCHAR * string;
			const TCHAR * color;
			const TCHAR * skin;
			}values;
		struct _MYMENU{
			const TCHAR * MENU_FILE;
			const TCHAR * MENU_EDIT;
			const TCHAR * MENU_PROJ;
			const TCHAR * MENU_FILEEX;
			const TCHAR * MENU_EDITEX;
			const TCHAR * MENU_PROJEX;
			}MYMENU;
		struct _IMG{
			const TCHAR * icons;
			const TCHAR * tab_left_png;
			}IMG;
		struct _ICON{
			const TCHAR * ICON_LOGO;
			}ICON;
	};
#endif//_UIRES_H_
#ifdef INIT_R_DATA
struct _UIRES UIRES={
		{
			_T("UIDEF:XML_INIT"),
		},
		{
			_T("LAYOUT:XML_MAINWND"),
			_T("LAYOUT:dlg_Probe_Select"),
		},
		{
			_T("values:string"),
			_T("values:color"),
			_T("values:skin"),
		},
		{
			_T("MYMENU:MENU_FILE"),
			_T("MYMENU:MENU_EDIT"),
			_T("MYMENU:MENU_PROJ"),
			_T("MYMENU:MENU_FILEEX"),
			_T("MYMENU:MENU_EDITEX"),
			_T("MYMENU:MENU_PROJEX"),
		},
		{
			_T("IMG:icons"),
			_T("IMG:tab_left_png"),
		},
		{
			_T("ICON:ICON_LOGO"),
		},
	};
#else
extern struct _UIRES UIRES;
#endif//INIT_R_DATA

#ifndef _R_H_
#define _R_H_
struct _R{
	struct _name{
		 const wchar_t * btn_close;
		 const wchar_t * btn_load_file;
		 const wchar_t * btn_loaddxf;
		 const wchar_t * btn_max;
		 const wchar_t * btn_min;
		 const wchar_t * btn_restore;
		 const wchar_t * btn_save_file;
		 const wchar_t * btn_zoomfit;
		 const wchar_t * btn_zoomin;
		 const wchar_t * btn_zoomout;
		 const wchar_t * cc_test;
		 const wchar_t * txt_title;
	}name;
	struct _id{
		int btn_close;
		int btn_load_file;
		int btn_loaddxf;
		int btn_max;
		int btn_min;
		int btn_restore;
		int btn_save_file;
		int btn_zoomfit;
		int btn_zoomin;
		int btn_zoomout;
		int cc_test;
		int txt_title;
	}id;
	struct _color{
		int blue;
		int gray;
		int green;
		int red;
		int white;
	}color;
	struct _string{
		int title;
		int ver;
	}string;

};
#endif//_R_H_
#ifdef INIT_R_DATA
struct _R R={
	{
		L"btn_close",
		L"btn_load_file",
		L"btn_loaddxf",
		L"btn_max",
		L"btn_min",
		L"btn_restore",
		L"btn_save_file",
		L"btn_zoomfit",
		L"btn_zoomin",
		L"btn_zoomout",
		L"cc_test",
		L"txt_title"
	}
	,
	{
		65536,
		65541,
		65540,
		65537,
		65539,
		65538,
		65542,
		65545,
		65543,
		65544,
		65546,
		65547
	}
	,
	{
		0,
		1,
		2,
		3,
		4
	}
	,
	{
		0,
		1
	}
	
};
#else
extern struct _R R;
#endif//INIT_R_DATA
