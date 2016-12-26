#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)

#define EXPRESSION_VERSIONA "2.0.0.0"
#define EXPRESSION_VERSIONW WIDEN(EXPRESSION_VERSIONA)

#ifdef _UNICODE
	#define EXPRESSION_VERSION EXPRESSION_VERSIONW
#else
	#define EXPRESSION_VERSION EXPRESSION_VERSIONA
#endif


#include "Tokenizer.h"
#include "SynNode.h"
#include "SynTreeBuilder.h"
#include "Semantics.h"

class Expression
{
	private:
		SynNode *m_pRoot;
		parser_string m_szText;
	protected:
		void GetVarNameFromNode(SynNode *pItem, Variables &var);
		// преобразует все численные узлы представленные строками в цифровое представление
		// данное действие необходимо для увеличения производительности: 
		// нет необходимости на каждой итерации вычисления выполнять преобразование
		void TranslateStringDigitToNumber(SynNode *pItem);
	public:
		Expression(void);
		Expression(const Expression &Other);
		~Expression();
		void Clear();
		parser_string ToString();
		bool Initialization(parser_string& ExprStr, ExpressionError& ExprErr);
		bool GetValue(double &Result, Variables &var, ExpressionError &ExprErr);
		void GetAllVariableNames(Variables &var);

		Expression& operator = (const Expression& Other);
};

#endif