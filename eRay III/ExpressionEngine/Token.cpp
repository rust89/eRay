#include "Token.h"

Token::Token(void)
	: CustomSynItem()
{
	m_nPos = 0;
}

Token::Token(parser_string Text, int Type, int Spec, size_t Pos)
	: CustomSynItem(Text,Type,Spec)
{
	m_nPos = Pos;
}

Token::Token(const Token &Other)
	: CustomSynItem(Other)
{
	m_nPos = Other.m_nPos;
}

Token& Token::operator = (const Token &Other)
{
	if (&Other == this) return *this;
	m_sText = Other.m_sText;
	m_nType = Other.m_nType;
	m_nSpec = Other.m_nSpec;
	m_nPos = Other.m_nPos;
	return *this;
}

const std::size_t Token::getPos() const
{
	return m_nPos;
}

void Token::setPos(std::size_t Pos)
{
	m_nPos = Pos;
}

void TokenVectorSortByPos(tokenvector &item)
{
	if (item.empty()) return;
	// чёртов пузырёк, но идеально подходит для небольшого количества элементов
	for (size_t i(0), token_count(item.size()), i_end(token_count-1); i < i_end; i++)
	{
		for (size_t j(i + 1); j < token_count; j++)
		{
			if (item[i].m_nPos > item[j].m_nPos)
			{
				Token tmp(item[i]);
				item[i] = item[j];
				item[j] = tmp;
			}
		}
	}
}