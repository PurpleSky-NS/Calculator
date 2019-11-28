#pragma once

#include <string>
#include "ItemBase.h"

class ValueItem :public ItemBase
{
public:

	static const double VALUE_PI;
	static const double VALUE_E;

	ValueItem() = default;
	inline ValueItem(double value);
	ValueItem(const ValueItem&) = default;
	ValueItem(ValueItem&&) = default;
	~ValueItem() = default;

	virtual inline ItemType GetType() const override;

	inline double& Value();
	inline const double& Value()const;

	inline virtual void Free() override;

private:
	double m_value = 0.0;

};
ValueItem::ValueItem(double value)
{
	m_value = value;
}

ValueItem::ItemType ValueItem::GetType() const
{
	return ItemType::Value;
}

double& ValueItem::Value()
{
	return m_value;
}

const double& ValueItem::Value()const
{
	return m_value;
}

void ValueItem::Free()
{
	delete this;
}
