#pragma once
#include <vector>

namespace lost
{

	enum EaseMode
	{
		NONE,
		LINEAR,
		IN,
		OUT,
	};

	class Animation
	{
	public:
		Animation(EaseMode easeMode_, float duration_);
		~Animation();

		void tick();
		void setStart(float val);
		void setEnd(float val);
		void setGoal(float val);
		void setVal(float val);
		void restart();
		float getVal() const;

		inline bool getComplete() const { return m_Complete; };

	private:
		float m_StartVal = 0.0f;
		float m_EndVal = 0.0f;
		float m_Time = 0.0f;
		float m_Duration = 1.0f;
		bool m_Complete = false;
		EaseMode m_EaseMode = EaseMode::NONE;
	};

	class AnimationChain
	{
	public:
		AnimationChain(std::vector<Animation>& animations_);
		~AnimationChain();

		void tick();
		float getVal() const;

		Animation& getAnim(int index);
		Animation& operator[](int index);
		
		void restart();

		std::vector<Animation> animations;
	private:
		float m_CurrentSector = 0;
	};

	class Animator
	{
	public:
		Animator();
		~Animator();

		// Ticks the animator, updating all held animations time values
		void tick();

		// Adds an animation to the animator returning the index for that animation, durationSeconds is the amount of seconds the animation will take to reach it's end value
		Animation* addAnimation(EaseMode easeMode, float durationSeconds);
		// Adds an animation chain to the animator returning the index for that animation
		AnimationChain* addAnimationChain(std::vector<Animation> animations);

		// Returns the value that the animation at that index is at in it's current animation
		float getVal(int animIndex);
		// Sets the end of the animation at the index given
		void setEnd(int animIndex, float endVal);
		// Sets the current value of an animation, instantly with no interpolation
		void setValue(int animIndex, float val);

	private:
		std::vector<Animation*> m_Animations;
		std::vector<AnimationChain*> m_AnimationChains;
	};

}