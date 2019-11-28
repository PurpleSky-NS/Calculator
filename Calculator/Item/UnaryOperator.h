#pragma once

#include <string>
#include "OperatorItem.h"

class UnaryOperator : public OperatorItem
{
public:

	//还有很多以后在拓展
	enum UnaryOperatorType : char
	{
		Factorial = 0,		//!，本来就是后缀表达
		Lg,					//lg
		Ln,					//ln
		Sin,				//sin
		Cos,				//cos
		Tan,				//tan
		Arcsin,				//arcsin
		Arccos,				//arccos
		Arctan,				//arctan
		Count
	};

	static UnaryOperator* GetOperator(UnaryOperatorType type);

	static UnaryOperator* GetOperator(const std::string& typeStr);

	static std::string GetName(UnaryOperatorType type);

	inline UnaryOperatorType GetUnaryOperatorType()const;

	virtual inline OperatorType GetOperatorType() const override;

private:

	static UnaryOperator operators[Count];

	inline UnaryOperator(UnaryOperatorType type);
	inline UnaryOperator(const UnaryOperator&) = default;
	inline UnaryOperator(UnaryOperator&&) = default;
	inline ~UnaryOperator() = default;

	UnaryOperatorType m_type;

};

UnaryOperator::UnaryOperator(UnaryOperatorType type)
{
	m_type = type;
}

UnaryOperator::OperatorType UnaryOperator::GetOperatorType() const
{
	return OperatorType::UnaryOperator;
}

UnaryOperator::UnaryOperatorType UnaryOperator::GetUnaryOperatorType() const
{
	return m_type;
}
