
#include "Expression.h"

// ������� ������������ ������ �������

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
	// ������������� �������������� ����������
	Parser parser;
	SynTreeBuilder stb;
	tokenvector tv;

	if (!parser.Work(ExprStr,tv)) // ��������� ������� ������ �� ������
	{
		ExprErr.SetError(seLexError);
		return false;
	}

	if (m_pRoot) 
		SynNode::Destroy(m_pRoot); // ������� ���������� ���������, ���� ����...
	
	m_pRoot = stb.Work(tv,ExprErr); // ������ �������������� ������
	
	if (!m_pRoot) 
		return false; // ������� � ������ �������
	
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
		// ���� ���� - ����������
		parser_string VarName = pItem->getText();
		for (Variables::iterator i = var.begin(); i != var.end(); i++)
		{
			// ���� ���������� �����������, �� ���������� �
			if (i->GetVarName() == VarName)
			{
				goto NextNodes;
			}
		}
		// ���� �� ������, � ���������� �� �����������,
		// �� ��������� � � ������
		var.push_back(VarItem(VarName));
	}

NextNodes:

	// ��������� ��� �������
	GetVarNameFromNode(const_cast<SynNode*>(pItem->SubItem()),var);

	// ��������� ��������� ����
	GetVarNameFromNode(const_cast<SynNode*>(pItem->Next()),var);
}

void Expression::TranslateStringDigitToNumber(SynNode *pItem)
{
	if (!pItem)
		return;
	if (pItem->getType() == lDigit) // ���� ��� �����
	{
		// ���� ���� - ����������
		pItem->setValue(GetDigitValue(pItem->getText()));
	}

	// ��������� ��� �������
	TranslateStringDigitToNumber(const_cast<SynNode*>(pItem->SubItem()));

	// ��������� ��������� ����
	TranslateStringDigitToNumber(const_cast<SynNode*>(pItem->Next()));
}