#include "CustomSynItem.h"

CustomSynItem::CustomSynItem()
{
	m_sText = PARSER_TEXT("");
	m_nType = 0;
	m_nSpec = 0;
}

CustomSynItem::CustomSynItem(parser_string Text, int Type, int Spec)
{
	m_sText = Text;
	m_nType = Type;
	m_nSpec = Spec;
}

CustomSynItem::CustomSynItem(const CustomSynItem &Other)
{
	m_sText = Other.m_sText;
	m_nType = Other.m_nType;
	m_nSpec = Other.m_nSpec;
}

CustomSynItem::~CustomSynItem()
{
	Clear();
}

void CustomSynItem::setText(parser_string Text)
{
	m_sText = Text;
}

void CustomSynItem::setType(int Type)
{
	m_nType = Type;
}

void CustomSynItem::setSpec(int Spec)
{
	m_nSpec = Spec;
}

const parser_string CustomSynItem::getText() const
{
	return m_sText;
}

const int CustomSynItem::getType() const
{
	return m_nType;
}

const int CustomSynItem::getSpec() const
{
	return m_nSpec;
}

void CustomSynItem::Clear()
{
	m_sText.clear();
	m_nType = 0;
	m_nSpec = 0;
}