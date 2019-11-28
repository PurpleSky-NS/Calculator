#pragma once

#include <stack>
#include "InfixExpression.h"

class SuffixExpression :public Expression
{
public:

	inline SuffixExpression() = default;
	SuffixExpression(const SuffixExpression&) = delete;
	SuffixExpression(SuffixExpression&&) = default;

	/*中缀表达式类构造*/
	inline SuffixExpression(InfixExpression&& expression);

	/*若失败，多半是因为括号不匹配*/
	/*成功进行转化的中值表达式会被清空，否则会发生重复Free*/
	inline bool ParseExpression(InfixExpression&& expression);

};

SuffixExpression::SuffixExpression(InfixExpression&& expression)
{
	ParseExpression(std::move(expression));
}

bool SuffixExpression::ParseExpression(InfixExpression&& expression)
{
	//静态优先级表，按二元运算符的优先级来
	//+ - * / % ^
	static const unsigned char binOpPriority[] = { 1,1,2,2,2,3 };

	std::stack<ItemBase*> operatorStack;//建立符号栈

	Clear();
	m_expression.reserve(expression.GetExpression().size());//为vector预留空间

	for (auto& i : expression.GetExpression())
	{
		switch (i->GetType())
		{
		case ItemBase::Value:
			m_expression.push_back(i);
			break;
		case ItemBase::Bracket:
			if (((BracketItem*)i)->GetBracketType() == BracketItem::Left)
				operatorStack.push(i);
			else
			{
				auto op = operatorStack.top();
				operatorStack.pop();
				/*当取出符号是左括号结束循环*/
				while (!(op->GetType() == ItemBase::Bracket && ((BracketItem*)op)->GetBracketType() == BracketItem::Left))
				{
					if (operatorStack.empty())
						return false;//括号不匹配
					m_expression.push_back(op);//将括号内运算符加入表达式
					op = operatorStack.top();//取出下一个符号
					operatorStack.pop();
				}
			}
			break;
		case ItemBase::Operator:
			if (((OperatorItem*)i)->GetOperatorType() == OperatorItem::BinaryOperator)
			{
				BinaryOperator* thisOp = (BinaryOperator*)i;
				ItemBase* preOp;//栈顶运算符
				while (!operatorStack.empty())
				{
					preOp = operatorStack.top();
					if (preOp->GetType() == ItemBase::Bracket)
					{
						operatorStack.push(thisOp);//栈顶是左括号就直接入栈
						break;
					}
					else //否则就是运算符
					{
						//一元运算符优先级最大
						if (((OperatorItem*)preOp)->GetOperatorType() == OperatorItem::UnaryOperator)
						{
							operatorStack.pop();//把栈顶一元运算符弹出
							m_expression.push_back(preOp);//加到表达式里
						}
						else //二元比较优先级
						{
							//当前运算符优先级比之前的高，压栈
							if (binOpPriority[(unsigned)((BinaryOperator*)thisOp)->GetBinaryOperatorType()] > binOpPriority[(unsigned)((BinaryOperator*)preOp)->GetBinaryOperatorType()])
							{
								operatorStack.push(thisOp);
								break;
							}
							else //小于等于的话，弹出之前的运算符
							{
								operatorStack.pop();//把栈顶一元运算符弹出
								m_expression.push_back(preOp);//加到表达式里
							}
						}
					}
				}
				//空栈了，说明该运算符优先级最低，把运算符压入
				if (operatorStack.empty())
					operatorStack.push(thisOp);
			}
			else
				operatorStack.push(i);
			break;
		}
	}
	while (!operatorStack.empty())
	{
		m_expression.push_back(operatorStack.top());
		operatorStack.pop();
	}
	m_expression.shrink_to_fit();
	expression.GetExpression().clear();
	return true;
}
