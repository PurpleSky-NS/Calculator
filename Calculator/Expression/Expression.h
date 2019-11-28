#pragma once

#include <vector>
#include "Item/ItemBase.h"

typedef std::vector<ItemBase*> ExpressionType;
class Expression
{
public:

	inline Expression() = default;
	inline Expression(const Expression&) = delete;
	inline Expression(Expression&&) = default;
	virtual inline ~Expression();

	/*获取表达式元素序列*/
	inline ExpressionType& GetExpression();

	/*获取表达式元素序列*/
	inline const ExpressionType& GetExpression()const;

	/*添加元素*/
	inline void AddItem(ItemBase* item);

	/*清空表达式，调用元素的Free方法*/
	inline void Clear();

	inline operator ExpressionType& ();
	inline operator const ExpressionType& ()const;

protected:
	ExpressionType m_expression;
};

Expression::~Expression()
{
	Clear();
}

ExpressionType& Expression::GetExpression()
{
	return m_expression;
}

const ExpressionType& Expression::GetExpression()const
{
	return m_expression;
}

void Expression::AddItem(ItemBase* item)
{
	m_expression.push_back(item);
}

void Expression::Clear()
{
	for (auto& i : m_expression)
		i->Free();
	m_expression.clear();
}

Expression::operator ExpressionType& ()
{
	return m_expression;
}

Expression::operator const ExpressionType& () const
{
	return m_expression;
}
