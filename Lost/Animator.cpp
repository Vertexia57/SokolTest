#include "Animator.h"
#include "DeltaTime.h"

namespace lost
{

	// Animator

	Animator::Animator()
	{
	}

	Animator::~Animator()
	{
		for (Animation* anim : m_Animations)
			delete anim;
		for (AnimationChain* anim : m_AnimationChains)
			delete anim;
	}

	void Animator::tick()
	{

		for (Animation* anim : m_Animations)
			anim->tick();
		for (AnimationChain* anim : m_AnimationChains)
			anim->tick();

	}

	Animation* Animator::addAnimation(EaseMode easeMode, float durationSeconds)
	{
		Animation* anim = new Animation(easeMode, durationSeconds);
		m_Animations.push_back(anim);
		return anim;
	}

	AnimationChain* Animator::addAnimationChain(std::vector<Animation> animations)
	{
		AnimationChain* anim = new AnimationChain(animations);
		m_AnimationChains.push_back(anim);
		return anim;
	}

	float Animator::getVal(int animIndex)
	{
		return m_Animations[animIndex]->getVal();
	}

	void Animator::setEnd(int animIndex, float endVal)
	{
		m_Animations[animIndex]->setEnd(endVal);
	}

	void Animator::setValue(int animIndex, float val)
	{
		m_Animations[animIndex]->setVal(val);
	}

	// Animation

	Animation::Animation(EaseMode easeMode_, float duration_)
	{
		m_EaseMode = easeMode_;
		m_Duration = duration_;
	}

	Animation::~Animation()
	{
	}

	void Animation::tick()
	{
		if (m_Time < m_Duration)
		{
			m_Time += lost::deltaTime / 1000.0f;
		}
		else
		{
			m_Time = m_Duration;
			m_Complete = true;
		}
	}

	void Animation::setStart(float val)
	{
		m_StartVal = val;
	}

	void Animation::setEnd(float val)
	{
		m_EndVal = val;
	}

	void Animation::setGoal(float val)
	{
		m_StartVal = getVal();
		m_EndVal = val;
		m_Time = 0.0f;
		m_Complete = false;
	}

	void Animation::setVal(float val)
	{
		m_StartVal = val;
		m_EndVal = val;
		m_Time = 0.0f;
		m_Complete = false;
	}

	void Animation::restart()
	{
		m_Time = 0.0f;
		m_Complete = false;
	}

	float Animation::getVal() const
	{
		if (m_Time < m_Duration)
		{
			switch (m_EaseMode)
			{
			case EaseMode::LINEAR:
				return m_StartVal + (m_EndVal - m_StartVal) * (m_Time / m_Duration);
			case EaseMode::IN:
				return m_StartVal + (m_EndVal - m_StartVal) * (powf(2, 10 * m_Time / m_Duration - 10));
			case EaseMode::OUT:
				return m_StartVal + (m_EndVal - m_StartVal) * (1 - powf(2, -10 * m_Time / m_Duration));
			default:
				return m_EndVal;
			}
		}
		else
		{
			return m_EndVal;
		}
	};

	AnimationChain::AnimationChain(std::vector<Animation>& animations_)
	{
		animations = animations_;
	}

	AnimationChain::~AnimationChain()
	{
	}

	void AnimationChain::tick()
	{
		animations[m_CurrentSector].tick();
		if (animations[m_CurrentSector].getComplete())
		{
			if (m_CurrentSector < animations.size() - 1)
				m_CurrentSector++;
		}
	}

	float AnimationChain::getVal() const
	{
		return animations[m_CurrentSector].getVal();
	}

	Animation& AnimationChain::getAnim(int index)
	{
		return animations[index];
	}

	Animation& AnimationChain::operator[](int index)
	{
		return animations[index];
	}

	void AnimationChain::restart()
	{
		for (Animation& anim : animations)
			anim.restart();

		m_CurrentSector = 0;
	}
}