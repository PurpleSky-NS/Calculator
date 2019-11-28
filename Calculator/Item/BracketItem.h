#pragma once

#include <string>
#include "ItemBase.h"

class BracketItem : public ItemBase
{
public:

	enum BracketType : char
	{
		Left = 0,		//(
		Right,			//)
		Count
	};

	static BracketItem* GetBracket(BracketType type);

	inline BracketType GetBracketType()const;

	virtual inline ItemType GetType() const override;

	virtual void Free()override {}

private:

	static BracketItem brackets[Count];

	inline BracketItem() = default;
	inline BracketItem(BracketType type);
	inline BracketItem(const BracketItem&) = default;
	inline BracketItem(BracketItem&&) = default;
	inline ~BracketItem() = default;

	BracketType m_type;
};
BracketItem::BracketItem(BracketType type)
{
	m_type = type;
}

BracketItem::BracketType BracketItem::GetBracketType() const
{
	return m_type;
}

BracketItem::ItemType BracketItem::GetType() const
{
	return ItemBase::Bracket;
}
