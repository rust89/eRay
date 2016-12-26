#ifndef _SYNTREEBUILDER_H_
#define _SYNTREEBUILDER_H_

#include "parsertypes.h"
#include "SynNode.h"
#include "Token.h"

// строит дерево
class SynTreeBuilder
{
	protected:
		// класс полутокен - полудерево
		class SynHybrid
		{
			public:
				Token token;
				SynNode *node;
				SynHybrid(void);
				SynHybrid(const Token &_token);
		};
		// вектор гибридного дерева
		typedef std::vector <SynHybrid> SynHybridPool;

		bool GetActualOpIndex(SynHybridPool &shp, std::size_t &Index);
		bool SynNodeArrayToTree(SynHybridPool &shp, ExpressionError &ExprError);
		void HybridingTokenVector(const tokenvector &tv, SynHybridPool &shp);
		bool CreateOtherBinOperator(SynHybridPool &shp, std::size_t &Index, ExpressionError &ExprError);
		bool CreateUnMinusPlusOperator(SynHybridPool &shp, std::size_t &Index, ExpressionError &ExprError);
		bool CreateBracketOperator(SynHybridPool &shp, std::size_t &Index, ExpressionError &ExprError);
		bool CreateCallOperator(SynHybridPool &shp, std::size_t &Index, ExpressionError &ExprError);
		void UnaryOperationFind(SynHybridPool &shp);
		SynNode* Work(SynHybridPool &shp, ExpressionError &ExprError);
		bool BracketControl(const SynHybridPool &shp, ExpressionError &ExprError);
	public:
		SynTreeBuilder(void);
		SynNode* Work(const tokenvector &tv, ExpressionError &ExprError);
};

#endif