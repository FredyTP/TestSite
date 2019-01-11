#pragma once
#include <bg\bg2e.hpp>
#include<vector>
namespace bg
{
namespace animation
{



struct positionKeyFrame
{
	float frameStart;
	bg::math::Vector3 pos;
	positionKeyFrame()
	{
		frameStart = 0.0f;
	}
	positionKeyFrame(float time, bg::math::Vector3 position)
	{
		frameStart = time;
		pos = position;
	}

};
bool operator == (const positionKeyFrame &n1, const positionKeyFrame &n2);
class PosAnimation : public bg::scene::Component
{
public:
	PosAnimation();
	~PosAnimation();

	bg::scene::Component* clone() { return new PosAnimation; }
	void init();
	void frame(float delta);

	void addKeyFrame(float ms, bg::math::Vector3 pos);
	void addKeyFrame(float ms, float x, float y, float z);
	void print();


	void play(int times = 1);
	void stop();
	void reset();

	void relativeKeyFrames(bool rel);
	PosAnimation* scale(float s);
	//inline PosAnimation* translate(bg::math::Vector3 translation);

private:

	void createPath();
	float timer;
	std::vector<positionKeyFrame> PosFrames;
	std::vector<positionKeyFrame> relPath;
	bg::math::Vector3 vel;
	int frames;
	int actualFrame;
	bool playing;
	bg::scene::Transform* trx;
	bool relative;
	int _times;
};






}
}