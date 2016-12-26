
#include "SynNode.h"

SynNode::SynNode(parser_string Text, int Type, int Spec)
	: CustomSynItem(Text, Type, Spec)
{
	m_fValue = 0;

	m_pNext = 0; // указатель на следующий элемент
	m_pSubitem = 0; // список подэлементов
	m_pOwner = 0; // владелец узла
}

SynNode::SynNode(const SynNode &Other)
{
	m_sText = Other.m_sText;
	m_nType = Other.m_nType;
	m_nSpec = Other.m_nSpec;
	m_fValue = Other.m_fValue;
	
	m_pOwner = 0; // владелец узла

	m_pSubitem = getClone(Other.m_pSubitem);
	
	SynNode *tmp = m_pSubitem;
	while (tmp)
	{
		tmp->m_pOwner = this;
		tmp = tmp->m_pNext;
	}

	m_pNext = getClone(Other.m_pNext);
}

SynNode* SynNode::AddSubItem(SynNode* Owner, parser_string Text, int Type, int Spec)
{
	if (!Owner) return 0;
	if (!Owner->m_pSubitem)
	{
		Owner->m_pSubitem = new SynNode(Text,Type,Spec);
		Owner->m_pSubitem->m_pOwner = Owner;
		return Owner->m_pSubitem;
	}
	SynNode *tmp = Owner->m_pSubitem;
	while (tmp->m_pNext)
	{
		tmp = tmp->m_pNext;
	}
	tmp->m_pNext = new SynNode(Text, Type, Spec);
	tmp->m_pNext->m_pOwner = Owner;
	return tmp->m_pNext;
}

SynNode* SynNode::AddSubItem(SynNode* Owner, SynNode* Node)
{
	if (!Owner) return 0;
	if (!Owner->m_pSubitem)
	{
		Owner->m_pSubitem = Node;
		return Owner->m_pSubitem;
	}
	SynNode *tmp = Owner->m_pSubitem;
	while (tmp->m_pNext)
	{
		tmp = tmp->m_pNext;
	}
	tmp->m_pNext = Node;
	return tmp->m_pNext;
}

void SynNode::Destroy(SynNode *Item)
{
	if (!Item) return;
	if (Item->m_pSubitem)
	{
		Destroy(Item->m_pSubitem);
	}
	SynNode *tmp = Item->m_pNext;
	delete Item;
	Item = 0;
	Destroy(tmp);
}

const SynNode* SynNode::Next() const 
{
	return m_pNext; 
}

const SynNode* SynNode::SubItem() const 
{
	return m_pSubitem; 
}

const SynNode* SynNode::Owner() const 
{
	return m_pOwner; 
}

const double SynNode::getValue() const
{
	return m_fValue;
}

void SynNode::setValue(double Value)
{
	m_fValue = Value;
}

SynNode* SynNode::getClone(const SynNode* Item)
{
	if (!Item) return 0;
	SynNode *tmp;
	SynNode* new_node = new SynNode();

	new_node->m_sText = Item->m_sText;
	new_node->m_nType = Item->m_nType;
	new_node->m_nSpec = Item->m_nSpec;
	new_node->m_fValue = Item->m_fValue;

	new_node->m_pSubitem = getClone(Item->m_pSubitem);

	tmp = new_node->m_pSubitem;
	while (tmp)
	{
		tmp->m_pOwner = new_node;
		tmp = tmp->m_pNext;
	}

	new_node->m_pNext = getClone(Item->m_pNext);

	return new_node;
}

SynNode* SynNode::Clone()
{
	return getClone(this);
}