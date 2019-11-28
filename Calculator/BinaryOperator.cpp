#include "Item/BinaryOperator.h"
BinaryOperator BinaryOperator::operators[Count]
{
	BinaryOperator((BinaryOperator::BinaryOperatorType)0),
	BinaryOperator((BinaryOperator::BinaryOperatorType)1),
	BinaryOperator((BinaryOperator::BinaryOperatorType)2),
	BinaryOperator((BinaryOperator::BinaryOperatorType)3),
	BinaryOperator((BinaryOperator::BinaryOperatorType)4),
	BinaryOperator((BinaryOperator::BinaryOperatorType)5)
};
BinaryOperator* BinaryOperator::GetOperator(BinaryOperatorType type)
{
	return &operators[(size_t)type];
}
