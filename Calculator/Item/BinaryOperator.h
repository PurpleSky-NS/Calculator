#pragma once

#include "OperatorItem.h"

class BinaryOperator : public OperatorItem
{
public:

	enum BinaryOperatorType : char
	{
		Add = 0,		//+
		Subtract,		//-
		Multiply,		//*
		Divide,			///
		Mod,			//%
		Power,          //^
		Count
	};

	static BinaryOperator* GetOperator(BinaryOperatorType type);

	inline BinaryOperatorType GetBinaryOperatorType()const;

	virtual inline OperatorType GetOperatorType() const override;

private:

	static BinaryOperator operators[Count];

	inline BinaryOperator(BinaryOperatorType type);
	inline BinaryOperator(const BinaryOperator&) = default;
	inline BinaryOperator(BinaryOperator&&) = default;
	inline ~BinaryOperator() = default;

	BinaryOperatorType m_type;
};

BinaryOperator::BinaryOperator(BinaryOperator::BinaryOperatorType type)
{
	m_type = type;
}

BinaryOperator::OperatorType BinaryOperator::GetOperatorType() const
{
	return OperatorType::BinaryOperator;
}

BinaryOperator::BinaryOperatorType BinaryOperator::GetBinaryOperatorType() const
{
	return m_type;
}
