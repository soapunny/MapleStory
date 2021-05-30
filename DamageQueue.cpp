#pragma once
#include "DamageQueue.h"
#include "Managers.h"

DamageQueue::DamageQueue(COLORREF damageColor, COLORREF criticalDamageColor, HFONT damageFont, HFONT criticalDamageFont)
{
	maxSize = 10;
	vDamageQueue.resize(maxSize);
	front = -1;
	rear = -1;
	this->damageColor = damageColor;
	this->criticalDamageColor = criticalDamageColor;
	this->damageFont = damageFont;
	this->criticalDamageFont = criticalDamageFont;

	displayTime = 0.6f;
	updateTimer = 0.0f;
	isMultiDamage = false;
}

void DamageQueue::Update()
{
	if (empty()){
		isMultiDamage = false;
		return;
	}
	else if (size() >= 2)
	{
		isMultiDamage = true;
	}

	float elapsedTime = TimerManager::GetSingleton()->GetElapsedTime();
	for (int i = front; i <= rear; i++)
	{
		vDamageQueue[i]->displayTime -= elapsedTime;//지속시간 차감
	}

	updateTimer += elapsedTime;
	if(updateTimer >= 0.10f)
	{ 
		if (vDamageQueue[front]->displayTime <= 0.0f)//Incase the displayTime over
		{
			pop_front();
		}
		updateTimer = 0.0f;
	}
}

void DamageQueue::Render(HDC hdc, FPOINT startPos)
{
	if (empty())
		return;

	HFONT oldFont;
	for(int i = front; i <= rear; i++)
	{
		if (vDamageQueue[i]->displayTime > displayTime)
			return;

		string damageWithMark = "";
		float posX = 0.0f, posY = 0.0f;

		if (vDamageQueue[i]->isCritical)//Incase it's the critical damage
		{
			SetTextColor(hdc, criticalDamageColor); //폰트 색상 흰색 지정
			oldFont = (HFONT)SelectObject(hdc, criticalDamageFont);
			damageWithMark.append("*");
			damageWithMark.append(vDamageQueue[i]->damage);
			posX = startPos.x - 30 * vDamageQueue[i]->damage.length() / 2 - 30;
			posY = startPos.y - 25 * i;
		}
		else
		{ 
			SetTextColor(hdc, damageColor); //폰트 색상 흰색 지정
			oldFont = (HFONT)SelectObject(hdc, damageFont);
			damageWithMark.append(vDamageQueue[i]->damage);
			posX = startPos.x - 30 * vDamageQueue[i]->damage.length() / 2;
			posY = startPos.y - 25 * i;
		}

		if (front == rear && !isMultiDamage)//Incase the queue only has a single element
		{
			posY = startPos.y - 25 * (1.0f + displayTime - vDamageQueue[i]->displayTime * 3.0f);
		}
		TextOut(
			hdc,
			posX,
			posY,
			damageWithMark.c_str(),
			damageWithMark.length()
		);
	}
}

void DamageQueue::Release()
{
	if(!empty())
	{ 
		for (int i = front; i <= rear; i++)
		{
			SAFE_DELETE(vDamageQueue[i]);
			vDamageQueue[i] = nullptr;
		}
	}
	front = rear = -1;
	vDamageQueue.clear();

	DeleteObject(damageFont);
	DeleteObject(criticalDamageFont);
}


void DamageQueue::push_back(string damage, bool isCritical)
{
	if (rear >= maxSize - 1)
		return;
	if (front == -1)
		front = 0;
	vDamageQueue[rear] = (new DamageInfo{ displayTime + (++rear)*0.1f, damage, isCritical });
}

void DamageQueue::pop_front()
{
	if (front < 0)//Incase the Queue is empty
		return;
	delete vDamageQueue[front];//Delete the front element
	vDamageQueue[front++] = nullptr;

	if (front > rear)//Incase the Queue is empty
	{
		front = -1;
		rear = -1;
	}
}

bool DamageQueue::empty()
{
	if (front == -1 && rear == -1 || front > rear)
		return true;
	return false;
}
