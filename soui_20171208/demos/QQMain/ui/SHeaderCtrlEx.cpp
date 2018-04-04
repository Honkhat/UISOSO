#include "stdafx.h"
#include "SHeaderCtrlEx.h"


namespace SOUI
{


    SHeaderCtrlEx::SHeaderCtrlEx(void)
		:m_bMatchParent(FALSE)
    {       
    }  

// 	BOOL SHeaderCtrlEx::CreateChildren( pugi::xml_node xmlNode )
//     {
//         pugi::xml_node xmlItems=xmlNode.child(L"items");
//         if(!xmlItems) return FALSE;
//         pugi::xml_node xmlItem=xmlItems.child(L"item");
//         int iOrder=0;
//         while(xmlItem)
//         {
//             SHDITEM item={0};
//             item.mask=0xFFFFFFFF;
//             item.iOrder=iOrder++;
//             SStringW strText = xmlItem.text().get();
//             strText.TrimBlank();
//             strText = tr(GETSTRING(strText));
//             SStringT strText2 = S_CW2T(strText);
//             item.pszText=_tcsdup(strText2);
//             item.cchTextMax=strText2.GetLength();
// 			if (isViewWidthMatchParent())
// 			{
// 				int cx = xmlItem.attribute(L"width").as_int(50);
// 				m_widItems.InsertAt(m_arrItems.GetCount(), cx);
// 			}
// 			else
// 			{
// 				item.cx = xmlItem.attribute(L"width").as_int(50);
// 			}
//             item.lParam=xmlItem.attribute(L"userData").as_uint(0);
//             item.stFlag=(SHDSORTFLAG)xmlItem.attribute(L"sortFlag").as_uint(ST_NULL);
//             m_arrItems.InsertAt(m_arrItems.GetCount(),item);
//             xmlItem=xmlItem.next_sibling(L"item");
//         }
// 		if (isViewWidthMatchParent())
// 		{
// 			m_bFixWidth = TRUE;
// 		}		
//         return TRUE;
//     }
	BOOL SHeaderCtrlEx::CreateChildren(pugi::xml_node xmlNode)
	{
		pugi::xml_node xmlItems = xmlNode.child(L"items");
		if (!xmlItems) return FALSE;
		pugi::xml_node xmlItem = xmlItems.child(L"item");
		LockUpdate();

		while (xmlItem)
		{
			SStringW strText = xmlItem.text().get();
			strText.TrimBlank();
			strText = tr(GETSTRING(strText));
			SStringT strText2 = S_CW2T(strText);
			int cx = xmlItem.attribute(L"width").as_int(50);
			if (isViewWidthMatchParent())
			{
				m_widItems.InsertAt(m_arrItems.GetCount(), cx);
			}
			LPARAM lParam = xmlItem.attribute(L"userData").as_uint(0);
			SHDSORTFLAG stFlag = (SHDSORTFLAG)xmlItem.attribute(L"sortFlag").as_uint(ST_NULL);

			InsertItem(-1, strText2, cx, stFlag, lParam);

			xmlItem = xmlItem.next_sibling(L"item");
		}

		UnlockUpdate();


		if (isViewWidthMatchParent())
		{
			m_bFixWidth = TRUE;
		}
		return TRUE;
	}
	BOOL SHeaderCtrlEx::OnSetCursor(const CPoint & pt)
	{
		return SWindow::OnSetCursor(pt);
	}
	 
}//end of namespace SOUI
