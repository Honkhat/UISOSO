#pragma once 
#include <helper/SAdapterBase.h>
#include <helper/STime.h> 
#include "ui/adapter/IListItemCallBack.h"
#include "ui/adapter/SimpleListDef.h" 
#include "ui/adapter/NodeMgr.h"

namespace SOUI
{  
struct NewChannelInfo
{
	std::wstring strNick;
	unsigned int  channel_id;
    unsigned int  uid;//频道管理员
}; 
  
class CNewChannelListItemAdapter : public SOUI::SMcAdapterBase
{
protected:
    enum{
        VT_GROUP=0,
        VT_DATA,
    };
	int m_nItemHeight[2];
	void Clear();
public:

	CNewChannelListItemAdapter(int data, live::IListItemCallback *pCB);
    ~CNewChannelListItemAdapter();
	void SetCallback(int data, live::IListItemCallback *pCB);
	int EnsureKeyVisible(BOOL bExpired, unsigned int  id);

	Node<NewChannelInfo>* GetRoot();

	Node<NewChannelInfo>* AddNode(const std::wstring &title, unsigned int  channel_id, bool has_child, Node<NewChannelInfo>* parent = NULL);
	Node<NewChannelInfo>* AddGroupNode(const std::wstring &title, bool has_child, Node<NewChannelInfo>* parent = NULL);

	void ExpendNode(int position, bool bChildVisible);
	Node<NewChannelInfo> *GetNode(int id);

	bool RemoveNode(Node<NewChannelInfo>* node);

    void RemoveAll();

	void SetChildVisible(Node<NewChannelInfo>* node, bool visible);

	bool CanExpand(Node<NewChannelInfo>* node) const;
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
	Node<NewChannelInfo>*   root_node_;
    LONG    delay_deltaY_;
    DWORD   delay_number_;
    DWORD   delay_left_; 
    int level_text_start_pos_;

	NodeMgr m_Nodes;
};


}