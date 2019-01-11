#pragma once
#include <bg\bg2e.hpp>
#include <vector>

namespace bg
{
namespace animation
{


struct rotationKeyFrame
{
	float frameStart;
	bg::math::Vector3 rot;
	rotationKeyFrame()
	{
		frameStart = 0.0f;
	}
	rotationKeyFrame(float time, bg::math::Vector3 _rotation)
	{
		frameStart = time;
		rot = _rotation;
	}

};
bool operator == (const rotationKeyFrame &n1, const rotationKeyFrame &n2);

class RotAnimation : public bg::scene::Component
{
public:
	RotAnimation();
	~RotAnimation();

	bg::scene::Component* clone() { return new RotAnimation; }
	void init();
	void frame(float delta);

	void addKeyFrame(float ms, bg::math::Vector3 pos);
	void addKeyFrame(float ms, float x, float y, float z);
	void print();


	void play(int _times = 1);
	void stop();
	void reset();

private:

	float timer;
	std::vector<rotationKeyFrame> RotFrames;
	bg::math::Vector3 angVel;
	int frames;
	int actualFrame;
	bool playing;
	bg::scene::Transform* trx;
	bool relative;
	int _times;
};







}
}