#pragma once
#include "MonsterManager.h"
#include "Managers.h"
#include "Mushmom.h"

HRESULT MonsterManager::Init()
{
	vMonsters.resize(1);
	
	Monster* monster = new Mushmom;
	monster->Init();

	vMonsters[0] = monster;

	CollisionManager::GetSingleton()->RegisterVEnemyUnits(&vMonsters);

	return S_OK;
}

void MonsterManager::Release()
{
	for (Unit* monster : vMonsters)
	{
		SAFE_RELEASE(monster);
	}
	vMonsters.clear();
	CollisionManager::GetSingleton()->RegisterVEnemyUnits(nullptr);
}

void MonsterManager::Update()
{
	for (Unit* monster : vMonsters)
	{
		if(monster)
			monster->Update();
	}
}

void MonsterManager::Render(HDC hdc)
{/*
	for (Unit* monster : vMonsters)
	{
		if (monster)
			monster->Render(hdc);
	}*/
}
