#pragma once 
#include <helper/SAdapterBase.h>
#include <helper/STime.h>
#include "ui/adapter/IListItemCallBack.h"
#include "ui/adapter/SimpleListDef.h" 
#include "ui/adapter/NodeMgr.h"
namespace SOUI
{  
struct FriendItemData
{
	std::wstring title;
	unsigned int uid;
}; 

class CFriendListAdapter : public SOUI::SMcAdapterBase
{
protected:
    enum{
        VT_GROUP=0,
        VT_DATA,
    };
	int m_nItemHeight[2];
	void Clear();
public:

	CFriendListAdapter(int data, live::IListItemCallback *pCB);
    ~CFriendListAdapter();
	void SetCallback(int data, live::IListItemCallback *pCB);
	int EnsureKeyVisible(BOOL bExpired, unsigned int id);

	Node<FriendItemData>* GetRoot();

	Node<FriendItemData>* AddNode(const std::wstring &title, unsigned int uid, bool has_child, Node<FriendItemData>* parent = NULL);
	Node<FriendItemData>* AddGroupNode(const std::wstring &title, bool has_child, Node<FriendItemData>* parent = NULL);

	void ExpendNode(int position, bool bChildVisible);
	Node<FriendItemData> *GetNode(int id);

	bool RemoveNode(Node<FriendItemData>* node);

    void RemoveAll();

	void SetChildVisible(Node<FriendItemData>* node, bool visible);

	bool CanExpand(Node<FriendItemData>* node) const;
protected:    
	virtual void InitByTemplate(pugi::xml_node xmlTemplate);
    virtual int getCount();
    virtual int getItemViewType(int position);
	virtual SIZE getViewDesiredSize(int position, SOUI::SWindow *pItem, LPCRECT prcContainer);
    virtual int getViewTypeCount();
    virtual SOUI::SStringW GetColumnName(int iCol) const;
    virtual void getView(int position, SOUI::SWindow * pItem, pugi::xml_node xmlTemplate);     
    bool OnGroupClick(SOUI::EventArgs *e);  
	bool OnItemDblClick(SOUI::EventArgs *e); 
private:
	int m_cb_data;
	live::IListItemCallback *m_pCB;
	 
	std::wstring level_expand_image_;
	std::wstring level_collapse_image_;
	Node<FriendItemData>*   root_node_;
    LONG    delay_deltaY_;
    DWORD   delay_number_;
    DWORD   delay_left_; 
    int level_text_start_pos_;

	NodeMgr m_Nodes;
};


}