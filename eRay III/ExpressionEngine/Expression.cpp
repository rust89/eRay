
#include "Expression.h"

// √лавный подключаемый модуль проекта

Expression::Expression(void)
{
	m_pRoot = 0;
}

Expression::Expression(const Expression &Other)
{
	m_szText = Other.m_szText;
	m_pRoot = Other.m_pRoot->Clone();
}

Expression::~Expression()
{
	Clear();
}

Expression& Expression::operator = (const Expression& Other)
{
	if (&Other == this)
		return *this;
	Clear();

	m_szText = Other.m_szText;
	m_pRoot = Other.m_pRoot->Clone();

	return *this;
}

void Expression::Clear()
{
	if (m_pRoot)
	{
		SynNode::Destroy(m_pRoot);
		m_pRoot = 0;
	}

	m_szText.clear();
}

bool Expression::Initialization(parser_string& ExprStr, ExpressionError& ExprErr)
{
	// инициализаци€ дополнительных переменных
	Parser parser;
	SynTreeBuilder stb;
	tokenvector tv;

	if (!parser.Work(ExprStr,tv)) // разбиваем входную строку на токены
	{
		ExprErr.SetError(seLexError);
		return false;
	}

	if (m_pRoot) 
		SynNode::Destroy(m_pRoot); // удал€ем предыдущее выражение, если есть...
	
	m_pRoot = stb.Work(tv,ExprErr); // строим синтаксическое дерево
	
	if (!m_pRoot) 
		return false; // выходим в случае неудачи
	
	TranslateStringDigitToNumber(m_pRoot);

	m_szText = ExprStr;

	return true;
}

bool Expression::GetValue(double &Result, Variables &var, ExpressionError &ExprErr)
{
	if (!m_pRoot)
	{
		ExprErr.SetError(seEmptyExpr);
		return false;
	}

	if (CalcTree(m_pRoot, var, ExprErr))
	{
		Result = m_pRoot->getValue();
		return true;
	}
	return false;
}

parser_string Expression::ToString()
{
	return m_szText;
}

void Expression::GetAllVariableNames(Variables &var)
{
	var.clear();
	GetVarNameFromNode(m_pRoot, var);
}

void Expression::GetVarNameFromNode(SynNode *pItem, Variables &var)
{
	if (!pItem)
		return;
	if (pItem->getType() == lVariable)
	{
		// если узел - переменна€
		parser_string VarName = pItem->getText();
		for (Variables::iterator i = var.begin(); i != var.end(); i++)
		{
			// если переменна€ встретилась, то пропускаем еЄ
			if (i->GetVarName() == VarName)
			{
				goto NextNodes;
			}
		}
		// если всЄ прошли, а переменна€ не встретилась,
		// то добавл€ем еЄ в список
		var.push_back(VarItem(VarName));
	}

NextNodes:

	// провер€ем все подузлы
	GetVarNameFromNode(const_cast<SynNode*>(pItem->SubItem()),var);

	// провер€ем следующий узел
	GetVarNameFromNode(const_cast<SynNode*>(pItem->Next()),var);
}

void Expression::TranslateStringDigitToNumber(SynNode *pItem)
{
	if (!pItem)
		return;
	if (pItem->getType() == lDigit) // если это цифра
	{
		// если узел - переменна€
		pItem->setValue(GetDigitValue(pItem->getText()));
	}

	// провер€ем все подузлы
	TranslateStringDigitToNumber(const_cast<SynNode*>(pItem->SubItem()));

	// провер€ем следующий узел
	TranslateStringDigitToNumber(const_cast<SynNode*>(pItem->Next()));
}