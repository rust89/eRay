
#include "SynTreeBuilder.h"


SynTreeBuilder::SynHybrid::SynHybrid(void)
{
	node = 0;
}

SynTreeBuilder::SynHybrid::SynHybrid(const Token &_token)
{
	token = _token;
	node = 0;
}

SynTreeBuilder::SynTreeBuilder(void)
{
	// добавить код для инициализации класса
}

void SynTreeBuilder::HybridingTokenVector(const tokenvector &tv, SynHybridPool &shp)
{
	shp.resize(tv.size());
	for (std::size_t i = 0; i < shp.size(); i++)
	{
		shp[i].token = tv[i];
	}
}

void SynTreeBuilder::UnaryOperationFind(SynHybridPool &shp)
{
	for (std::size_t i = 0; i < shp.size(); i++)
	{
		if (!shp[i].node)
		{
			// ищем плюс или минус
			if (shp[i].token.getType() == lOperator && (shp[i].token.getSpec() == oComposing || shp[i].token.getSpec() == oSubtraction))
			{
				// если знак первый в строке
				if(i == 0)
				{
					shp[i].token.setSpec(shp[i].token.getSpec() == oComposing ? oUnComposing : oUnSubtraction);
				}
				if (i > 0)
				{
					// если предыдущий символ оператор кроме закрывающейся скобки
					if (shp[i-1].token.getType() == lOperator && shp[i-1].token.getSpec() != oBracketClose)
					{
						shp[i].token.setSpec(shp[i].token.getSpec() == oComposing ? oUnComposing : oUnSubtraction);
					}
				}
			}
		}
	}
}

bool SynTreeBuilder::GetActualOpIndex(SynHybridPool &shp, std::size_t &Index)
{
	bool OperationFinded = false; // флаг указывающий на встречу операции в потоке
	for (std::size_t i = 0; i < shp.size(); i++)
	{
		if (!shp[i].node && shp[i].token.getType() == lOperator) // это не узел и это операция
		{
			int OpPriority = GetOperationPriority(shp[i].token.getSpec());
			if (!OperationFinded && OpPriority)
			{
				Index = i;
				OperationFinded = true;
			}
			else if (OpPriority != 0 && GetOperationPriority(shp[Index].token.getSpec()) > OpPriority)
			{
				Index = i;
				OperationFinded = true;
			}
		}
	}
	return OperationFinded;
}

bool SynTreeBuilder::CreateOtherBinOperator(SynHybridPool &shp, std::size_t &Index, ExpressionError &ExprError)
{
	shp[Index].node = new SynNode(shp[Index].token.getText(), shp[Index].token.getType(), shp[Index].token.getSpec());
	
	std::size_t LeftArgIndex = Index - 1;
	std::size_t RightArgIndex = Index + 1;
	// проверка на возможность чтения предыдущего узла
	if ((signed)LeftArgIndex < 0)
	{
		ExprError.ParamString = shp[Index].token.getText();
		ExprError.SetError(seInvalidBinOpArg,2);
		return false;		
	}

	if (!shp[LeftArgIndex].node && shp[LeftArgIndex].token.getType() == lOperator)
	{
		ExprError.ParamString = shp[Index].token.getText();
		ExprError.SetError(seInvalidBinOpArg,2);
		return false;	
	}
	
	// проверка на возможность чтения следующего узла
	if (RightArgIndex >= shp.size())
	{
		ExprError.ParamString = shp[Index].token.getText();
		ExprError.SetError(seInvalidBinOpArg);
		return false;
	}

	if (!shp[RightArgIndex].node && shp[RightArgIndex].token.getType() == lOperator)
	{
		ExprError.ParamString = shp[Index].token.getText();
		ExprError.SetError(seInvalidBinOpArg);
		return false;	
	}

	if (shp[LeftArgIndex].node)
	{
		shp[Index].node->AddSubItem(shp[Index].node,shp[LeftArgIndex].node);
	} else shp[Index].node->AddSubItem(shp[Index].node, shp[LeftArgIndex].token.getText(), shp[LeftArgIndex].token.getType(), shp[LeftArgIndex].token.getSpec());

	if (shp[RightArgIndex].node)
	{
		shp[Index].node->AddSubItem(shp[Index].node,shp[RightArgIndex].node);
	} else shp[Index].node->AddSubItem(shp[Index].node, shp[RightArgIndex].token.getText(), shp[RightArgIndex].token.getType(), shp[RightArgIndex].token.getSpec());
	
	shp.erase(shp.begin() + RightArgIndex);
	shp.erase(shp.begin() + LeftArgIndex);
	return true;
}

bool SynTreeBuilder::CreateUnMinusPlusOperator(SynHybridPool &shp, std::size_t &Index, ExpressionError &ExprError)
{
	shp[Index].node = new SynNode(shp[Index].token.getText(), shp[Index].token.getType(), shp[Index].token.getSpec());
	std::size_t ArgIndex = Index + 1;
	if (ArgIndex >= shp.size())
	{
		ExprError.ParamString = shp[Index].token.getText();
		ExprError.SetError(seInvalidUnaryOpArg,1);
		return false;
	}

	if (!shp[ArgIndex].node && shp[ArgIndex].token.getType() == lOperator)
	{
		ExprError.ParamString = shp[Index].token.getText();
		ExprError.SetError(seInvalidUnaryOpArg,1);
		return false;
	}
	if (shp[ArgIndex].node)
	{
		shp[Index].node->AddSubItem(shp[Index].node,shp[ArgIndex].node);
	} else shp[Index].node->AddSubItem(shp[Index].node, shp[ArgIndex].token.getText(), shp[ArgIndex].token.getType(), shp[ArgIndex].token.getSpec());
	shp.erase(shp.begin() + ArgIndex);
	return true;
}

bool SynTreeBuilder::CreateBracketOperator(SynHybridPool &shp, std::size_t &Index, ExpressionError &ExprError)
{
	// ищем скобку на закрытие
	int OpenBraceCount = 1;
	std::size_t ii = Index + 1;
	while (true)
	{
		if (ii >= shp.size())
		{
			ExprError.SetError(seCloseBracketMissing);
			return false;
		}
		if (shp[ii].token.getType() == lOperator &&
			shp[ii].token.getSpec() == oBracketOpen)
		{
			OpenBraceCount++;
		}
		if (shp[ii].token.getType() == lOperator &&
			shp[ii].token.getSpec() == oBracketClose)
		{
			OpenBraceCount--;
		}
		if (OpenBraceCount == 0) break;
		ii++;
	}
	// выражение внутри скобок
	SynHybridPool BraceBody(shp.begin() + Index + 1, shp.begin() + ii);
	// получаем выражение
	if (SynNodeArrayToTree(BraceBody,ExprError))
	{
		if (BraceBody.size() == 1)
		{
			shp[Index].token = BraceBody[0].token;
			shp[Index].node = BraceBody[0].node;	
			shp.erase(shp.begin() + Index + 1, shp.begin() + ii + 1);
			return SynNodeArrayToTree(shp,ExprError);
		}
		else
		{
			ExprError.SetError(seInvalidExpr);
			return false;
		}
	}
	else
	{
		// ExprError содержит ошибку
		return false;
	}
	return true;
}

bool SynTreeBuilder::CreateCallOperator(SynHybridPool &shp, std::size_t &Index, ExpressionError &ExprError)
{	
	shp[Index].node = new SynNode(shp[Index].token.getText(),shp[Index].token.getType(),shp[Index].token.getSpec());
	SynNode *tmp = shp[Index].node; 
	std::size_t LastIndex = Index + 2; // предыдущий индекс
	std::size_t CurrIndex = Index + 2; // текущий индекс
	int BracketCount = 1; // количество скобок
	// ищем скобку на закрытие
	while (true)
	{
		if (CurrIndex >= shp.size())
		{
			ExprError.SetError(seCloseBracketMissing);
			return false;
		}
		if (shp[CurrIndex].token.getType() == lOperator &&
			shp[CurrIndex].token.getSpec() == oBracketOpen)
		{
			BracketCount++;
		}
		if (shp[CurrIndex].token.getType() == lOperator &&
			shp[CurrIndex].token.getSpec() == oBracketClose)
		{
			BracketCount--;
		}
		// если встречаем запятую
		if (shp[CurrIndex].token.getType() == lOperator &&
			shp[CurrIndex].token.getSpec() == oComma)
		{
            if (BracketCount > 1) {CurrIndex++; continue;}
			// считываем параметр в качестве нового выражения
			SynHybridPool ParamBody(shp.begin() + LastIndex, shp.begin() + CurrIndex);
			SynNode *SubNode = Work(ParamBody,ExprError);
			if (SubNode)
			{
				tmp->AddSubItem(tmp,SubNode);
				CurrIndex++;
				LastIndex = CurrIndex;
				continue;
			}
			else
			{
				// ExprError содержит ошибку
				return false;
			}
		}
		if (BracketCount == 0) // прошли все скобки
		{
			// дошли до конца вызова функции
			if ((CurrIndex - Index)  == 1) // функция без параметров
			{
				CurrIndex++;
				break;
			}
			else
			{
				// считываем оставшийся параметр
				SynHybridPool ParamBody(shp.begin() + LastIndex, shp.begin() + CurrIndex);
				SynNode *SubNode = Work(ParamBody,ExprError);
				if (SubNode)
				{
					tmp->AddSubItem(tmp,SubNode);
					CurrIndex++;
					LastIndex = CurrIndex;
					break;
				}
				else
				{
					// ExprError содержит ошибку
					return false;
				}
			}
		}
		CurrIndex++;
	}
	// удаляем обрезки
	shp.erase(shp.begin() + Index + 1, shp.begin() + CurrIndex);
	return SynNodeArrayToTree(shp,ExprError);
}

bool SynTreeBuilder::SynNodeArrayToTree(SynHybridPool &shp, ExpressionError &ExprError)
{
	if (!shp.size())
	{
		ExprError.SetError(seEmptyExpr);
		return false;
	}
	std::size_t Index = 0; // храним индекс операции с наибольшим приоритетом
	
	if (!GetActualOpIndex(shp,Index)) // если операция не найдена
	{
		if (shp.size() == 1)
		{
			if (!shp[0].node)
			{
				shp[0].node = new SynNode(shp[0].token.getText(),shp[0].token.getType(),shp[0].token.getSpec());
			}
			return true;
		}
		else 
		{
			ExprError.SetError(seInvalidExpr);
			return false;
		}
	}
	// разбор ситуации в зависимости от операции
	switch(shp[Index].token.getSpec())
	{
		case oComposing:
		case oSubtraction:
		case oDivision:
		case oMultiplication:
		case oPow:
		{
			if (CreateOtherBinOperator(shp,Index,ExprError))
			{
				return SynNodeArrayToTree(shp,ExprError);
			}
			return false;
		}
		case oUnComposing:
		case oUnSubtraction:
		{
			if (CreateUnMinusPlusOperator(shp,Index,ExprError))
			{
				return SynNodeArrayToTree(shp,ExprError);
			}
			return false;
		}
		case oBracketOpen:
		{
			if (Index > 0)
			{
				// это вызов функции ?
				if ((shp[Index - 1].node == 0) && (shp[Index - 1].token.getType() == lFunction))
				{
					Index--;
					return this->CreateCallOperator(shp,Index,ExprError);
				}
			}
			// это простые скобки
			if (CreateBracketOperator(shp,Index,ExprError))
			{
				return SynNodeArrayToTree(shp,ExprError);
			}
			return false;
		}
		default:
		{
			ExprError.SetError(seUnknowOperator);
			return false;
		}
	}
	return true;
}

SynNode* SynTreeBuilder::Work(const tokenvector &tv, ExpressionError &ExprError)
{
	SynHybridPool hybrid;
	HybridingTokenVector(tv,hybrid);
	if (!BracketControl(hybrid,ExprError))
	{
		return 0;
	}
	UnaryOperationFind(hybrid);
	if (SynNodeArrayToTree(hybrid, ExprError))
	{
		return hybrid[0].node;
	}
	return 0;
}

SynNode* SynTreeBuilder::Work(SynHybridPool &shp, ExpressionError &ExprError)
{
	if (SynNodeArrayToTree(shp, ExprError))
	{
		return shp[0].node;
	}
	return 0;
}

bool SynTreeBuilder::BracketControl(const SynHybridPool &shp, ExpressionError &ExprError)
{
	int BracketBalance = 0;
	for (std::size_t i = 0; i < shp.size(); i++)
	{
		if (shp[i].token.getType() == lOperator && shp[i].token.getSpec() == oBracketOpen)
		{
			BracketBalance++;
			continue;
		}
		if (shp[i].token.getType() == lOperator && shp[i].token.getSpec() == oBracketClose)
		{
			BracketBalance--;
			continue;
		}
	}
	if (BracketBalance == 0)
	{
		ExprError.SetError(seNoError);
		return true;
	}
	if (BracketBalance < 0)
	{
		ExprError.SetError(seOpenBracketMissing);
	}
	else
	{
		ExprError.SetError(seCloseBracketMissing);
	}
	return false;
}