#include "Item/BracketItem.h"
BracketItem BracketItem::brackets[Count]
{
	BracketItem((BracketItem::BracketType)0),
	BracketItem((BracketItem::BracketType)1)
};
BracketItem* BracketItem::GetBracket(BracketType type)
{
	return &brackets[(size_t)type];
}
