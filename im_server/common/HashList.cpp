#include "stdafx.h"
#include "HashList.h"

					
HashListItem::HashListItem()
{
	m_hashPre = NULL;
	m_hashNext= NULL;

	m_ListPre = NULL;
	m_ListNext= NULL;
}


HashListItem::~HashListItem()
{
	
}


//////////////////////////////////////////////////////////////////////////

TranPacketItem::TranPacketItem()
{
	m_id	= 0;
	m_seq	= 0;
	m_vlink	= 0;
}

uint32 TranPacketItem::GetHashNum() 
{
	return m_seq + m_id + m_vlink;
}

bool TranPacketItem::IsMatch(HashListItem& CompareItem)
{
	TranPacketItem* pCompareItem = (TranPacketItem*)&CompareItem;
	if( m_id  == pCompareItem->m_id &&
		m_seq  == pCompareItem->m_seq &&
		m_vlink== pCompareItem->m_vlink  )
	{
		return true;
	}

	return false;
}

void TranPacketItem::InitIndex(HashListItem& IndexItem)
{
	TranPacketItem* pIndexItem = (TranPacketItem*)&IndexItem;
	m_id = pIndexItem->m_id;
	m_seq= pIndexItem->m_seq;
	m_vlink = pIndexItem->m_vlink;
}


CHashList<TranPacketItem> g_hashlist;

