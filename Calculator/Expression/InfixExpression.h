#pragma once

#include <string>
#include <cctype>
#include <cmath>
#include "Expression.h"
#include "Item/ValueItem.h"
#include "Item/BracketItem.h"
#include "Item/BinaryOperator.h"
#include "Item/UnaryOperator.h"

/*Warning：注意，在使用cctype系列库时要讲char强转为unsigned char，
不然在非ascii环境下会引发isctype断言*/
class InfixExpression : public Expression
{
public:

	inline InfixExpression() = default;
	/*后缀表达式字符串构造*/;
	inline InfixExpression(const std::string& exp);
	InfixExpression(const InfixExpression&) = delete;
	InfixExpression(InfixExpression&&) = default;

	inline bool ParseExpression(const std::string& expression);

private:

	ItemBase* m_prevItem = nullptr;

	/*去除空白字符*/
	inline std::string RemoveSpace(const std::string& exp)const;

	inline bool IsNumber(char ch);

	/* 是否是分隔符，+ - * / % ^ ! , ( ) */
	inline bool IsDivOperator(char ch);

	/*获得一个元素*/
	inline ItemBase* GetItem(const std::string& exp, size_t& bgPos);

	/*若碰到非数字或者重复小数点(不是纯数字) 或者 正常分隔符会停下,返回是否是数字*/
	inline bool GetNumber(const std::string& exp, size_t& bgPos, std::string& itemStr);

	/*获取标识符，返回是不是函数*/
	bool GetIdentification(const std::string& exp, size_t& pos, std::string& itemStr);
};

InfixExpression::InfixExpression(const std::string& exp)
{
	ParseExpression(exp);
}

bool InfixExpression::ParseExpression(const std::string& srcExp)
{
	m_prevItem = nullptr;
	Clear();
	std::string exp = RemoveSpace(srcExp);
	for (size_t i = 0; i < exp.size();)
	{
		m_prevItem = GetItem(exp, i);
		if (m_prevItem == nullptr)
		{
			Clear();
			return false;
		}
		m_expression.push_back(m_prevItem);
	}
	return true;
}

std::string InfixExpression::RemoveSpace(const std::string& exp) const
{
	std::string newExp;
	newExp.reserve(exp.size());
	for (auto& i : exp)
		if (!isspace((unsigned char)i))
			newExp += i;
	return newExp;
}

bool InfixExpression::IsNumber(char ch)
{
	return ch == '.' || isdigit((unsigned char)ch);
}

bool InfixExpression::IsDivOperator(char ch)
{
	return !(ch != '+' && ch != '-' && ch != '*' && ch != '/' && ch != '%' &&
		ch != '^' && ch != '!' && ch != ',' && ch != '(' && ch != ')' && ch != '[' && ch != ']');
}

ItemBase* InfixExpression::GetItem(const std::string& exp, size_t& pos)
{
	static bool hasSignedOp = false;//读入数字/变量是否带符号位
	static bool isNegOp = true;//是否是‘-’
	/*当+-在表达式的开头，'('的后面或者二元运算符的后面时，就是符号位*/
	switch (exp[pos])
	{
	case '(':
		++pos;
		return BracketItem::GetBracket(BracketItem::Left);
	case ')':
		++pos;
		return BracketItem::GetBracket(BracketItem::Right);
	case '+':
		if (pos == 0 || m_prevItem != nullptr &&
			(m_prevItem->GetType() == ItemBase::Bracket && ((BracketItem*)m_prevItem)->GetBracketType() == BracketItem::Left
				|| m_prevItem->GetType() == ItemBase::Operator && ((OperatorItem*)m_prevItem)->GetOperatorType() == OperatorItem::BinaryOperator))//说明是符号位
		{
			hasSignedOp = true;
			isNegOp = false;
			return GetItem(exp, ++pos);
		}
		++pos;
		return BinaryOperator::GetOperator(BinaryOperator::Add);
	case '-':
		if (pos == 0 || m_prevItem != nullptr &&
			(m_prevItem->GetType() == ItemBase::Bracket && ((BracketItem*)m_prevItem)->GetBracketType() == BracketItem::Left
				|| m_prevItem->GetType() == ItemBase::Operator && ((OperatorItem*)m_prevItem)->GetOperatorType() == OperatorItem::BinaryOperator))//说明是符号位
		{
			hasSignedOp = true;
			isNegOp = true;
			return GetItem(exp, ++pos);
		}
		++pos;
		return BinaryOperator::GetOperator(BinaryOperator::Subtract);
	case '*':
		++pos;
		return BinaryOperator::GetOperator(BinaryOperator::Multiply);
	case '/':
		++pos;
		return BinaryOperator::GetOperator(BinaryOperator::Divide);
	case '%':
		++pos;
		return BinaryOperator::GetOperator(BinaryOperator::Mod);
	case '^':
		++pos;
		return BinaryOperator::GetOperator(BinaryOperator::Power);
	case '!':
		++pos;
		return UnaryOperator::GetOperator(UnaryOperator::Factorial);
	default:
		std::string itemStr;
		if (isdigit((unsigned char)exp[pos]) && GetNumber(exp, pos, itemStr)) //数字开头
		{
			if (hasSignedOp)//带符号位
			{
				hasSignedOp = false;
				return new ValueItem((isNegOp ? -stod(itemStr) : stod(itemStr)));
			}
			return new ValueItem(stod(itemStr));
		}//取不出来就继续按标识符解析
		if (GetIdentification(exp, pos, itemStr))//如果是函数
			return UnaryOperator::GetOperator(itemStr);
		else //否则就是常量
		{
			/*可能是常量pi和e*/
			if (itemStr == "pi")
				return new ValueItem(ValueItem::VALUE_PI);
			else if (itemStr == "e")
				return new ValueItem(ValueItem::VALUE_E);
		}
	}
	return nullptr;
}

bool InfixExpression::GetNumber(const std::string& exp, size_t& bgPos, std::string& itemStr)
{
	bool isMeetDot = false;
	bool isBad = false;
	size_t pos = bgPos;
	for (; pos < exp.size() && IsNumber(exp[pos]); ++pos)
		if (exp[pos] == '.')
		{
			if (isMeetDot)
			{
				isBad = true;
				break;
			}
			else
				isMeetDot = true;
		}

	itemStr = exp.substr(bgPos, pos - bgPos);
	bgPos = pos;
	if (pos == exp.size())//如果读到底了
		return true;
	return !isBad && IsDivOperator(exp[pos]);//如果没有重复小数点并且遇到分隔符说明是数字
}

bool InfixExpression::GetIdentification(const std::string& exp, size_t& pos, std::string& itemStr)
{
	while (pos < exp.size() && !IsDivOperator(exp[pos]))//直到遇到分隔符停止
		itemStr += exp[pos++];
	if (pos == exp.size())//如果读到底了，说明是变量
		return false;
	if (exp[pos] == '(') //遇到右括号说明是函数
		return true;
	return false;//否则是变量
}
