#include "Item/UnaryOperator.h"
UnaryOperator UnaryOperator::operators[Count]
{
	UnaryOperator((UnaryOperator::UnaryOperatorType)0),
	UnaryOperator((UnaryOperator::UnaryOperatorType)1),
	UnaryOperator((UnaryOperator::UnaryOperatorType)2),
	UnaryOperator((UnaryOperator::UnaryOperatorType)3),
	UnaryOperator((UnaryOperator::UnaryOperatorType)4),
	UnaryOperator((UnaryOperator::UnaryOperatorType)5),
	UnaryOperator((UnaryOperator::UnaryOperatorType)6),
	UnaryOperator((UnaryOperator::UnaryOperatorType)7),
	UnaryOperator((UnaryOperator::UnaryOperatorType)8)
};
UnaryOperator* UnaryOperator::GetOperator(UnaryOperatorType type)
{
	return &operators[(size_t)type];
}

UnaryOperator* UnaryOperator::GetOperator(const std::string& typeStr)
{
	if (typeStr.empty())
		return nullptr;

	if (typeStr == "!")
		return GetOperator(Factorial);
	else if (typeStr == "lg")
		return GetOperator(Lg);
	else if (typeStr == "ln")
		return GetOperator(Ln);
	else if (typeStr == "sin")
		return GetOperator(Sin);
	else if (typeStr == "cos")
		return GetOperator(Cos);
	else if (typeStr == "tan")
		return GetOperator(Tan);
	else if (typeStr == "arcsin")
		return GetOperator(Arcsin);
	else if (typeStr == "arccos")
		return GetOperator(Arccos);
	else if (typeStr == "arctan")
		return GetOperator(Arctan);
	return nullptr;
}

std::string UnaryOperator::GetName(UnaryOperatorType type)
{
	switch (type)
	{
	case UnaryOperator::Factorial:
		return "!";
	case UnaryOperator::Lg:
		return "lg";
	case UnaryOperator::Ln:
		return "ln";
	case UnaryOperator::Sin:
		return "sin";
	case UnaryOperator::Cos:
		return "cos";
	case UnaryOperator::Tan:
		return "tan";
	case UnaryOperator::Arcsin:
		return "arcsin";
	case UnaryOperator::Arccos:
		return "arccos";
	case UnaryOperator::Arctan:
		return "arctan";
	}
	return "";
}
