#ifndef _TOKEN_H_
#define _TOKEN_H_

#include "CustomSynItem.h"

#include <vector>

class Token;
typedef std::vector <Token> tokenvector;

/**
* \brief A class designed to work with tokens for lexical analysis.
*
* \author Rustam Z.
* \file Token.h
*/
class Token : public CustomSynItem
{
	protected:
		std::size_t m_nPos;
	public:
		Token(void);
		Token(parser_string Text, int Type, int Spec, size_t Pos);
		Token(const Token &Other);
		Token& operator = (const Token &Other);

		// сортировка вектора токенов в порядке возрастания позиции
		friend void TokenVectorSortByPos(tokenvector &item);

		/**
		* \brief This method return token position at the text.
		*
		* \return Token position.
		*/
		const std::size_t getPos() const;

		/**
		* \brief This method set new token position at the text.
		*
		* \param Pos New token position
		*/
		void setPos(std::size_t Pos);
};

#endif