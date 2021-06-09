#include "ConsumableItem.h"
#include "ConsumableItemEntity.h"

ConsumableItem::ConsumableItem(ConsumableItemEntity* consumableItemEntity)
{
	itemType = consumableItemEntity->GetItemType();
	itemName = consumableItemEntity->GetItemName();
	explanation = consumableItemEntity->GetExplanation();
	price = consumableItemEntity->GetPrice();
	maxBundleCnt = consumableItemEntity->GetMaxBundleCnt();

	hpRecovery = consumableItemEntity->GetHpRecovery();
	mpRecovery = consumableItemEntity->GetMpRecovery();
	hpRecoveryRate = consumableItemEntity->GetHpRecoveryRate();
	mpRecoveryRate = consumableItemEntity->GetMpRecoveryRate();

	physicalDamage = consumableItemEntity->GetPhysicalDamage();
	magicalDamage = consumableItemEntity->GetMagicalDamage();
	accuracy = consumableItemEntity->GetAccuracy();
	defense = consumableItemEntity->GetDefense();
	moveSpeed = consumableItemEntity->GetMoveSpeed();

	str = consumableItemEntity->GetStr();
	dex = consumableItemEntity->GetDex();
	intel = consumableItemEntity->GetInt();
	luck = consumableItemEntity->GetLuk();
	duration = consumableItemEntity->GetDuration();

	//TODO 나머지 필드들도 초기화
	//image, consumableItemFrame
	durationTimer = 0.0f;
}
