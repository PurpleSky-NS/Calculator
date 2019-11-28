#pragma once

#include <cmath>
#include <functional>
#include <stdexcept>
#include "Expression/SuffixExpression.h"

class Calculator
{
public:

	class DivideByZeroException :public std::runtime_error
	{
	public:
		DivideByZeroException() = default;
		DivideByZeroException(const std::string& msg) :
			runtime_error(msg) {}
	};

	class MathError :public std::runtime_error
	{
	public:
		MathError() = default;
		MathError(const std::string& msg) :
			runtime_error(msg) {}
	};

	class ExpressionError :public std::runtime_error
	{
	public:
		ExpressionError() = default;
		ExpressionError(const std::string& msg) :
			runtime_error(msg) {}
	};

	/*运算的表达式必须都是后缀表达式，否则计算不成功*/
	static inline double Calculate(const std::string& expression);

	/*运算的表达式必须都是后缀表达式，否则计算不成功*/
	static inline double Calculate(const SuffixExpression& expression);

private:

	Calculator() = delete;
	Calculator(const Calculator&) = delete;
	Calculator(Calculator&&) = delete;
	~Calculator() = delete;

	static inline double CalculateBinaryOperator(BinaryOperator::BinaryOperatorType type, double left, double right);

	static inline double CalculateUnaryOperator(UnaryOperator::UnaryOperatorType type, double value);

	static inline bool IsZero(double value);

	static inline bool IsDigit(double v);

};

inline double Calculator::Calculate(const std::string& expression)
{
	return Calculate(SuffixExpression(expression));
}

double Calculator::Calculate(const SuffixExpression& suffExp)
{
	const ExpressionType& expression = suffExp.GetExpression();
	if (expression.empty())//空的算啥
		throw ExpressionError("expression is empty");
	else if (expression.size() == 1)//只有一个还用算？
	{
		switch (expression[0]->GetType())
		{
		case ItemBase::Value:
			return ((ValueItem*)expression[0])->Value();
		default:
			throw ExpressionError("expression only have one item,but it's not a value");
		}
	}
	std::stack<double> calcStack;//计算栈
	for (auto& i : expression)
	{
		switch (i->GetType())
		{
		case ItemBase::Value:
			calcStack.push(((ValueItem*)i)->Value());
			break;
		case ItemBase::Operator:
			if (((OperatorItem*)i)->GetOperatorType() == OperatorItem::BinaryOperator)
			{
				/*二元运算符*/
				if (calcStack.size() < 2)//操作数不够
					throw ExpressionError("can't find enough value items to calculate");
				double rightValue = calcStack.top();
				calcStack.pop();
				/*左操作数还在栈里，不取出来了*/
				calcStack.top() = CalculateBinaryOperator(((BinaryOperator*)i)->GetBinaryOperatorType(), calcStack.top(), rightValue);
			}
			else
			{
				/*一元运算符*/
				if (calcStack.empty())//操作数不够
					throw ExpressionError("can't find enough value items to calculate");
				calcStack.top() = CalculateUnaryOperator(((UnaryOperator*)i)->GetUnaryOperatorType(), calcStack.top());
			}
			break;
		}
	}

	if (calcStack.size() == 1)
		return calcStack.top();

	throw ExpressionError("expression calculate error");
}

bool Calculator::IsDigit(double v)
{
	return IsZero(v - floor(v));
}

double Calculator::CalculateBinaryOperator(BinaryOperator::BinaryOperatorType type, double left, double right)
{
	switch (type)
	{
	case BinaryOperator::Add:
		return left + right;
	case BinaryOperator::Subtract:
		return left - right;
	case BinaryOperator::Multiply:
		return left * right;
	case BinaryOperator::Divide:
		if (IsZero(right))
			throw DivideByZeroException("value divide by zero");
		return left / right;
	case BinaryOperator::Mod:
		return fmod(left, right);
	case BinaryOperator::Power:
		return pow(left, right);
	}
	throw ExpressionError("unknow binary operatr");
}

double Calculator::CalculateUnaryOperator(UnaryOperator::UnaryOperatorType type, double value)
{
	switch (type)
	{
	case UnaryOperator::Factorial:
		if (value < -1)
			throw MathError("factorial value can't less than 0");
		return tgamma(value + 1);
	case UnaryOperator::Lg:
		return log10(value);
	case UnaryOperator::Ln:
		return log(value);
	case UnaryOperator::Sin:
		return sin(value);
	case UnaryOperator::Cos:
		return cos(value);
	case UnaryOperator::Tan:
		return tan(value);
	case UnaryOperator::Arcsin:
		return asin(value);
	case UnaryOperator::Arccos:
		return acos(value);
	case UnaryOperator::Arctan:
		return atan(value);
	}
	throw ExpressionError("unknow unary operatr");
}

bool Calculator::IsZero(double value)
{
	return value == 0.0;
}
