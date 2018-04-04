#ifndef _ILISTITEM_CALLBACK_H
#define _ILISTITEM_CALLBACK_H

#pragma once 
 
namespace live
{
    struct IListItemCallback
    {
        virtual void OnItemDBClick(int data,int position,void *pData) =0;
    };  
} 

   
#endif //_ILISTITEM_CALLBACK_H