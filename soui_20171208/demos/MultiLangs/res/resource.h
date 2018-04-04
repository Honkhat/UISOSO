//stamp:091e54a5aaae8d24
/*<------------------------------------------------------------------------------------------------->*/
/*该文件由uiresbuilder生成，请不要手动修改*/
/*<------------------------------------------------------------------------------------------------->*/
#pragma once
#include <res.mgr/snamedvalue.h>
#define ROBJ_IN_CPP \
namespace SOUI \
{\
    const _R R;\
    const _UIRES UIRES;\
}
namespace SOUI
{
	class _UIRES{
		public:
		class _UIDEF{
			public:
			_UIDEF(){
				XML_INIT = _T("UIDEF:XML_INIT");
			}
			const TCHAR * XML_INIT;
		}UIDEF;
		class _LAYOUT{
			public:
			_LAYOUT(){
				XML_MAINWND = _T("LAYOUT:XML_MAINWND");
			}
			const TCHAR * XML_MAINWND;
		}LAYOUT;
		class _values{
			public:
			_values(){
				string = _T("values:string");
				color = _T("values:color");
				skin = _T("values:skin");
			}
			const TCHAR * string;
			const TCHAR * color;
			const TCHAR * skin;
		}values;
		class _lang{
			public:
			_lang(){
				cn = _T("lang:cn");
				en = _T("lang:en");
			}
			const TCHAR * cn;
			const TCHAR * en;
		}lang;
		class _ICON{
			public:
			_ICON(){
				ICON_LOGO = _T("ICON:ICON_LOGO");
			}
			const TCHAR * ICON_LOGO;
		}ICON;
		class _img{
			public:
			_img(){
				pic_100 = _T("img:pic_100");
				pic_125 = _T("img:pic_125");
				pic_150 = _T("img:pic_150");
				pic_200 = _T("img:pic_200");
				chat_9 = _T("img:chat.9");
			}
			const TCHAR * pic_100;
			const TCHAR * pic_125;
			const TCHAR * pic_150;
			const TCHAR * pic_200;
			const TCHAR * chat_9;
		}img;
		class _smenu{
			public:
			_smenu(){
				menu_lang = _T("smenu:menu_lang");
			}
			const TCHAR * menu_lang;
		}smenu;
	};
	const SNamedID::NAMEDVALUE namedXmlID[]={
		{L"_name_start",65535},
		{L"btn_close",65540},
		{L"btn_max",65538},
		{L"btn_menu",65536},
		{L"btn_min",65537},
		{L"btn_restore",65539},
		{L"btn_scale_100",65541},
		{L"btn_scale_125",65542},
		{L"btn_scale_150",65543},
		{L"btn_scale_200",65544},
		{L"col1",65547},
		{L"col2",65549},
		{L"col3",65551},
		{L"col4",65553},
		{L"lang_cn",100},
		{L"lang_en",101},
		{L"mclv_test",65546},
		{L"txt_age",65552},
		{L"txt_gender",65550},
		{L"txt_name",65548},
		{L"txt_score",65554},
		{L"txt_test",65545}		};
	class _R{
	public:
		class _name{
		public:
		_name(){
			_name_start = namedXmlID[0].strName;
			btn_close = namedXmlID[1].strName;
			btn_max = namedXmlID[2].strName;
			btn_menu = namedXmlID[3].strName;
			btn_min = namedXmlID[4].strName;
			btn_restore = namedXmlID[5].strName;
			btn_scale_100 = namedXmlID[6].strName;
			btn_scale_125 = namedXmlID[7].strName;
			btn_scale_150 = namedXmlID[8].strName;
			btn_scale_200 = namedXmlID[9].strName;
			col1 = namedXmlID[10].strName;
			col2 = namedXmlID[11].strName;
			col3 = namedXmlID[12].strName;
			col4 = namedXmlID[13].strName;
			lang_cn = namedXmlID[14].strName;
			lang_en = namedXmlID[15].strName;
			mclv_test = namedXmlID[16].strName;
			txt_age = namedXmlID[17].strName;
			txt_gender = namedXmlID[18].strName;
			txt_name = namedXmlID[19].strName;
			txt_score = namedXmlID[20].strName;
			txt_test = namedXmlID[21].strName;
		}
		 const wchar_t * _name_start;
		 const wchar_t * btn_close;
		 const wchar_t * btn_max;
		 const wchar_t * btn_menu;
		 const wchar_t * btn_min;
		 const wchar_t * btn_restore;
		 const wchar_t * btn_scale_100;
		 const wchar_t * btn_scale_125;
		 const wchar_t * btn_scale_150;
		 const wchar_t * btn_scale_200;
		 const wchar_t * col1;
		 const wchar_t * col2;
		 const wchar_t * col3;
		 const wchar_t * col4;
		 const wchar_t * lang_cn;
		 const wchar_t * lang_en;
		 const wchar_t * mclv_test;
		 const wchar_t * txt_age;
		 const wchar_t * txt_gender;
		 const wchar_t * txt_name;
		 const wchar_t * txt_score;
		 const wchar_t * txt_test;
		}name;

		class _id{
		public:
		const static int _name_start	=	65535;
		const static int btn_close	=	65540;
		const static int btn_max	=	65538;
		const static int btn_menu	=	65536;
		const static int btn_min	=	65537;
		const static int btn_restore	=	65539;
		const static int btn_scale_100	=	65541;
		const static int btn_scale_125	=	65542;
		const static int btn_scale_150	=	65543;
		const static int btn_scale_200	=	65544;
		const static int col1	=	65547;
		const static int col2	=	65549;
		const static int col3	=	65551;
		const static int col4	=	65553;
		const static int lang_cn	=	100;
		const static int lang_en	=	101;
		const static int mclv_test	=	65546;
		const static int txt_age	=	65552;
		const static int txt_gender	=	65550;
		const static int txt_name	=	65548;
		const static int txt_score	=	65554;
		const static int txt_test	=	65545;
		}id;

		class _string{
		public:
		const static int female	=	0;
		const static int male	=	1;
		const static int title	=	2;
		const static int unknown	=	3;
		const static int ver	=	4;
		}string;

		class _color{
		public:
		const static int blue	=	0;
		const static int gray	=	1;
		const static int green	=	2;
		const static int red	=	3;
		const static int white	=	4;
		}color;

	};

#ifdef R_IN_CPP
	 extern const _R R;
	 extern const _UIRES UIRES;
#else
	 extern const __declspec(selectany) _R & R = _R();
	 extern const __declspec(selectany) _UIRES & UIRES = _UIRES();
#endif//R_IN_CPP
}
