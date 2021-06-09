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
			SAFE_DELETE(character);//���� ������ ����
			it = mCharacterStore->erase(it);
			break;
		}
		else
			it++;
	}
	mCharacterStore->insert(make_pair(characterEntity->GetName(), characterEntity));//�� ������ ����
}

//############################### public functions #########################################
void CharDataTellerMachine::SaveCharacter(CharacterDTO* character)
{
	CharacterEntity* characterEntity = character->TurnItIntoCharacterEntity();
	UpdateCharacterData(characterEntity);//���� ���� ������Ʈ

	CharacterStorage::GetSingleton()->SaveCharacter();//CharacterStorage ������ ����
}

map<string, CharacterDTO*>* CharDataTellerMachine::LoadCharacter(string name)
{
	CharacterStorage::GetSingleton()->LoadCharacter(name);//���� �ʿ� �ִ� ĳ���͸� storage�� �ε��Ѵ�.

	map<string, CharacterDTO*>* mCharacterData = new map<string, CharacterDTO*>;
	auto mCharacterStore = CharacterStorage::GetSingleton()->GetMCharacterStore();//�ε��� �����͸� ���� ����, ������ �� ���� �� �����Կ� ��� �����ش�.
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
