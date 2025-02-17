#include "stdafx.h"
#include "AniRender.h"
#include "RenderMgr.h"
#include "Asset.h"
#include <JTIME.h>
#include "Actor.h"
#include "Player.h"
#include <ColorRGB.h>
#include "GAMECORE.h"
#include <Debug.h>

AniRender::AniRender() : isDebug(true), isAfterImage(false), isBoss(false), aniPause(false)
{
	BossCloneDis = 50;
}


AniRender::~AniRender()
{
	std::map<std::wstring, Animation*>::iterator Start	= aniMap.begin();
	std::map<std::wstring, Animation*>::iterator End	= aniMap.end();

	for (; Start != End; ++Start)
	{
		if (nullptr != Start->second)
		{
			delete Start->second;
			Start->second = nullptr;
		}
	}
}


AniRender::Animation* AniRender::FindAnimation(const wchar_t* _AniName)
{
	std::map<std::wstring, Animation*>::iterator FindIter = aniMap.find(_AniName);

	if (FindIter == aniMap.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

bool AniRender::IsAni(const wchar_t* _AniName)
{
	return curAni == FindAnimation(_AniName);
}

void AniRender::PlayAnimation(const wchar_t* _AniName, bool _IndexCheck)
{
	Animation* findAni = FindAnimation(_AniName);

	if (nullptr == findAni)
	{
		assert(false);
	}

	if (nullptr != curAni &&
		findAni == curAni && 
		curAni->over == false)
	{
		return;
	}
	int NextIndex = findAni->curIndex;

	particleAni = nullptr;
	curAni = findAni;
	if (false == _IndexCheck)
	{
		curAni->ResetAnimation();
	}
	else 
	{
		curAni->ResetAnimation(NextIndex);
	}
}

void AniRender::PlayAnimation(const wchar_t* _AniName, const wchar_t* _Particle)
{
	Animation* findAni = FindAnimation(_AniName);
	Animation* findParticle = FindAnimation(_Particle);

	if (nullptr == findAni)
	{
		assert(false);
	}

	if (nullptr != curAni &&
		findAni == curAni &&
		curAni->over == false)
	{
		return;
	}

	curAni			= findAni;
	particleAni		= findParticle;

	curAni		->ResetAnimation();
	particleAni	->ResetAnimation();
	
}

void AniRender::CreateAnimation(const wchar_t* _AniName, const wchar_t* _SpriteName, int _StartIdx, int _EndIdx, float _AniTime, bool _Loop)
{
	CreateAnimation(_AniName, _SpriteName, _StartIdx, _EndIdx,  _Loop, _AniTime);
}

void AniRender::CreateAnimation(const wchar_t* _AniName, const wchar_t* _SpriteName, int _StartIdx, int _EndIdx, bool _Loop, float _AniTime)
{
	if (nullptr != FindAnimation(_AniName))
	{
		assert(false);
	}

	Sprite* findSprite = Asset::FIndSprite(_SpriteName);
	if (nullptr == findSprite)
	{
		assert(false);
	}

	Animation* newAni = new Animation();
	newAni->sprite = findSprite;

	newAni->startIndex	= _StartIdx;
	newAni->endIndex	= _EndIdx;
	newAni->aniTime		= _AniTime;
	newAni->loop		= _Loop;
	newAni->ResetAnimation();

	aniMap.insert(std::map<std::wstring, Animation*>::value_type(_AniName, newAni));

}


void AniRender::Render()
{
	if (nullptr == curAni)
	{
		assert(false);
	}	
	
	curAni->curTime -= Time::DeltaTime();	
	if (nullptr != particleAni)
	{
		particleAni->curTime -= Time::DeltaTime();
	}

	if (aniPause == false)
	{
		if (0 >= curAni->curTime)
		{
			++(curAni->curIndex);
			///掻娃拭 蕉艦五戚芝 穿発馬奄 是廃 昔畿什痕呪

			curAni->curTime = curAni->aniTime;

			if (curAni->curIndex == curAni->endIndex + 1)
			{
				if (true == curAni->loop)
				{
					curAni->curIndex = curAni->startIndex;
					//curAni->prevIndex = curAni->startIndex;
				}
				else
				{
					curAni->curIndex = curAni->endIndex;
					//curAni->prevIndex = curAni->endIndex;
					curAni->aniEnd = true; //蕉艦五戚芝戚 魁概澗走 端滴背爽澗 痕呪
				}
			}
		}
	}
	

	if (nullptr != particleAni)
	{
		if (0 >= particleAni->curTime)
		{
			++(particleAni->curIndex);
			particleAni->curTime = particleAni->aniTime;

			if (particleAni->curIndex == particleAni->endIndex + 1)
			{
				if (true == particleAni->loop)
				{
					particleAni->curIndex = particleAni->startIndex;
				}
				else
				{
					particleAni->curIndex = particleAni->endIndex;
				}
			}
		}
	}
	

	if (false == isCustomSize)
	{
		subRect.size = (*curAni->sprite)[curAni->curIndex].size;
	}


	// 域至背辞 隔嬢操醤 廃陥.
 	Pos			RenderPos	= CalPivotPos();
	ColorRGB	mazenta		= ColorRGB();
	//Rectangle(RenderMgr::BACKDC()
	//	, ActorPos().IX()
	//	, ActorPos().IY()
	//	, ActorPos().IX() + renderRect.size.IX()
	//	, ActorPos().IY() + renderRect.size.IY());

	// 亜舌 匙献闇 益撹 戚耕走 滴奄 益企稽 紫遂馬澗依,.
	// 鞠亀系戚檎 戚耕走 滴奄税 壕呪戚暗蟹


	   

	//接雌 益形爽澗 採歳
	if (true == isAfterImage)
	{

		//RGB( 0, 100, 255)
		BF.SourceConstantAlpha = 130;
		AlphaBlend(
			RenderMgr::BACKDC(),
			RenderPos.IX() -  (Player::Inst()->GetPos().IX() - Player::Inst()->GetPosArr(0).IX()),
			RenderPos.IY() - (Player::Inst()->GetPos().IY() - Player::Inst()->GetPosArr(0).IY()),
			subRect.size.IX(),
			subRect.size.IY(),
			curAni->sprite->GetImage()->GetDC(),
			(*curAni->sprite)[curAni->curIndex].pos.IX()  ,
			(*curAni->sprite)[curAni->curIndex].pos.IY(),
			(*curAni->sprite)[curAni->curIndex].size.IX(),
			(*curAni->sprite)[curAni->curIndex].size.IY(),
			BF
		);

		
		BF.SourceConstantAlpha = 190;
		AlphaBlend(
			RenderMgr::BACKDC(),
			RenderPos.IX() - (Player::Inst()->GetPos().IX() - Player::Inst()->GetPosArr(1).IX()),
			RenderPos.IY() - (Player::Inst()->GetPos().IY() - Player::Inst()->GetPosArr(1).IY()),
			subRect.size.IX(),
			subRect.size.IY(),
			curAni->sprite->GetImage()->GetDC(),
			(*curAni->sprite)[curAni->curIndex].pos.IX() ,
			(*curAni->sprite)[curAni->curIndex].pos.IY(),
			(*curAni->sprite)[curAni->curIndex].size.IX(),
			(*curAni->sprite)[curAni->curIndex].size.IY(),
			BF
		);

		BF.SourceConstantAlpha = 250;
		AlphaBlend(
			RenderMgr::BACKDC(),
			RenderPos.IX() - (Player::Inst()->GetPos().IX() - Player::Inst()->GetPosArr(2).IX()),
			RenderPos.IY() - (Player::Inst()->GetPos().IY() - Player::Inst()->GetPosArr(2).IY()),
			subRect.size.IX(),
			subRect.size.IY(),
			curAni->sprite->GetImage()->GetDC(),
			(*curAni->sprite)[curAni->curIndex].pos.IX() ,
			(*curAni->sprite)[curAni->curIndex].pos.IY(),
			(*curAni->sprite)[curAni->curIndex].size.IX(),
			(*curAni->sprite)[curAni->curIndex].size.IY(),
			BF
		);
	}
	else
	{		
		if (isBoss == true)
		{
			BF.SourceConstantAlpha = 160;
			AlphaBlend(
				RenderMgr::BACKDC(),
				RenderPos.IX(),
				RenderPos.IY(),
				subRect.size.IX(),
				subRect.size.IY(),
				curAni->sprite->GetImage()->GetDC(),
				(*curAni->sprite)[curAni->curIndex].pos.IX() ,
				(*curAni->sprite)[curAni->curIndex].pos.IY(),
				(*curAni->sprite)[curAni->curIndex].size.IX(),
				(*curAni->sprite)[curAni->curIndex].size.IY(),
				BF
			);

			
			AlphaBlend(
				RenderMgr::BACKDC(),
				RenderPos.IX(),
				RenderPos.IY(),
				subRect.size.IX(),
				subRect.size.IY(),
				curAni->sprite->GetImage()->GetDC(),
				(*curAni->sprite)[curAni->curIndex].pos.IX() - BossCloneDis,
				(*curAni->sprite)[curAni->curIndex].pos.IY(),
				(*curAni->sprite)[curAni->curIndex].size.IX(),
				(*curAni->sprite)[curAni->curIndex].size.IY(),
				BF
			);

		
			AlphaBlend(
				RenderMgr::BACKDC(),
				RenderPos.IX(),
				RenderPos.IY(),
				subRect.size.IX(),
				subRect.size.IY(),
				curAni->sprite->GetImage()->GetDC(),
				(*curAni->sprite)[curAni->curIndex].pos.IX() + BossCloneDis,
				(*curAni->sprite)[curAni->curIndex].pos.IY(),
				(*curAni->sprite)[curAni->curIndex].size.IX(),
				(*curAni->sprite)[curAni->curIndex].size.IY(),
				BF
			);
		}
		else
		{
			TransparentBlt(
				RenderMgr::BACKDC(),
				RenderPos.IX(),
				RenderPos.IY(),
				subRect.size.IX(),
				subRect.size.IY(),
				curAni->sprite->GetImage()->GetDC(),
				(*curAni->sprite)[curAni->curIndex].pos.IX(),
				(*curAni->sprite)[curAni->curIndex].pos.IY(),
				(*curAni->sprite)[curAni->curIndex].size.IX(),
				(*curAni->sprite)[curAni->curIndex].size.IY(),
				mazenta.m_Color
			);
		}	

	}



	
	
	

#pragma region DEBUGIMAGE




	///DEBUG RECT IMAGE
	if (Debug::Inst().GetActive() == true)
	{
		Rectangle(RenderMgr::BACKDC()	///蝶遣斗 匂走芝
			, ActorPos().IX() - 2
			, ActorPos().IY() - 2
			, ActorPos().IX() + 2
			, ActorPos().IY() + 2);

		Rectangle(RenderMgr::BACKDC()	///RIGHT 
			, (GetActor()->GetRPos().IX()) - 2
			, (GetActor()->GetRPos().IY()) - 2
			, (GetActor()->GetRPos().IX()) + 2
			, (GetActor()->GetRPos().IY()) + 2);		
		Rectangle(RenderMgr::BACKDC()	///LEFT けいしかいし
			, (ActorPos().IX() - 20) - 2
			, (ActorPos().IY() - 20) - 2
			, (ActorPos().IX() - 20) + 2
			, (ActorPos().IY() - 20) + 2);
		Rectangle(RenderMgr::BACKDC()	///CEILING 
			, (ActorPos().IX()) - 2
			, (ActorPos().IY() - 50) - 2
			, (ActorPos().IX()) + 2
			, (ActorPos().IY() - 50) + 2);
		Rectangle(RenderMgr::BACKDC()	///GROUND CHECK
			, ActorPos().IX()  - 2
			, (int)(ActorPos().y * (1.1f)) - 2
			, ActorPos().IX()  + 2
			, (int)(ActorPos().y * (1.1f)) + 2);
	}
	
#pragma endregion



	

	//Rectangle(RenderMgr::BACKDC()	//蝶遣斗 匂走芝 革乞
	//	, actor->GetPos().IX() - 5
	//	, actor->GetPos().IY() - 5
	//	, actor->GetPos().IX() + 5
	//	, actor->GetPos().IY() + 5);


}