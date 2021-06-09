#include "CharDataTellerMachine.h"
#include "CharacterStorage.h"
#include "CharacterEntity.h"
#include "CharacterDTO.h"

void CharDataTellerMachine::UpdateCharacterData(CharacterEntity* characterEntity)
{
	auto mCharacterStore = CharacterStorage::GetSingleton()->GetMCharacterStore();
	auto it = mCharacterStore->begin();
	while (it != mCharacterStore->end())
	{
		auto character = it->second;
		if (character->GetName() == characterEntity->GetName())
		{
			SAFE_DELETE(character);//이전 데이터 삭제
			it = mCharacterStore->erase(it);
			break;
		}
		else
			it++;
	}
	mCharacterStore->insert(make_pair(characterEntity->GetName(), characterEntity));//새 데이터 주입
}

//############################### public functions #########################################
void CharDataTellerMachine::SaveCharacter(CharacterDTO* character)
{
	CharacterEntity* characterEntity = character->TurnItIntoCharacterEntity();
	UpdateCharacterData(characterEntity);//본인 정보 업데이트

	CharacterStorage::GetSingleton()->SaveCharacter();//CharacterStorage 데이터 저장
}

map<string, CharacterDTO*>* CharDataTellerMachine::LoadCharacter(string name)
{
	CharacterStorage::GetSingleton()->LoadCharacter(name);//같은 맵에 있는 캐릭터를 storage에 로드한다.

	map<string, CharacterDTO*>* mCharacterData = new map<string, CharacterDTO*>;
	auto mCharacterStore = CharacterStorage::GetSingleton()->GetMCharacterStore();//로드한 데이터를 직접 접근, 변경할 수 없는 새 보관함에 담아 돌려준다.
	for (auto characterPair : *mCharacterStore)
	{
		CharacterDTO* character = new CharacterDTO(characterPair.second);
		mCharacterData->insert(make_pair(character->GetName(), character));
	}

	return mCharacterData;
}

void CharDataTellerMachine::Release()
{
	CharacterStorage::GetSingleton()->Release();
}
