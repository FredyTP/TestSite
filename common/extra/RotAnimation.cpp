#include "RotAnimation.h"

namespace bg
{
namespace animation
{


bool operator==(const rotationKeyFrame & n1, const rotationKeyFrame & n2)
{
	return n1.frameStart == n2.frameStart;
}

RotAnimation::RotAnimation()
{
	timer = 0.0f;
	frames = 0;
	actualFrame = -1;
	relative = false;
}


RotAnimation::~RotAnimation()
{
}

void RotAnimation::init()
{
	trx = node()->transform();
}

void RotAnimation::frame(float delta)
{
	if (playing)
	{
		timer += delta;
		if (timer >= RotFrames[actualFrame + 1].frameStart)
		{
			//NEXT KEY FRAME//
			//CALCULA LA VELOCIDAD DEL TRAMO//
			actualFrame++;
			if (actualFrame < frames - 1)
			{
				angVel = (RotFrames[actualFrame + 1].rot - RotFrames[actualFrame].rot)
					/ (RotFrames[actualFrame + 1].frameStart - RotFrames[actualFrame].frameStart);
			}

		}
		if (actualFrame > -1)
		{
			if (actualFrame < frames - 1)
			{
				//REALIZA EL DESPLAZAMIENTO
				bg::math::Vector3 newAngles = angVel;
				newAngles.scale(timer - RotFrames[actualFrame].frameStart);
				newAngles.add(RotFrames[actualFrame].rot);
				
				trx->matrix().resetRotation();
				trx->matrix()
					.rotate(newAngles.z(), 0.0f, 0.0f, 1.0f)
					.rotate(newAngles.y(), 0.0f, 1.0f, 0.0f)
					.rotate(newAngles.x(), 1.0f, 0.0f, 0.0f);
				
			}
			else
			{
				//SI TERMINA LA ANIMACION PONEMOS LA ULTIMA POSICION
				bg::math::Vector3 newAngles = RotFrames[frames-1].rot;
				trx->matrix().resetRotation();
				trx->matrix()
					.rotate(newAngles.z(), 0.0f, 0.0f, 1.0f)
					.rotate(newAngles.y(), 0.0f, 1.0f, 0.0f)
					.rotate(newAngles.x(), 1.0f, 0.0f, 0.0f);

				if (_times != -1)
					_times--;
				if (_times == -1)
				{
					this->reset();
					this->play(_times);
				}
				else if (_times > 0)
				{
					this->reset();
					this->play(_times);
				}
				else
				{
					this->stop();
					this->reset();
				}
			}
		}
	

	}
}

void RotAnimation::addKeyFrame(float ms, bg::math::Vector3  pos)
{
	frames++;
	rotationKeyFrame frame(ms, pos);

	//IF ALREADY KEYFRAME THERE; ERASE IT
	auto result = std::find(RotFrames.begin(), RotFrames.end(), frame);
	if (result != std::end(RotFrames)) {
		RotFrames.erase(result);
		frames--;
	}

	//ADDS AND SORT THE KEYFRAMES
	RotFrames.push_back(frame);
	std::sort(RotFrames.begin(), RotFrames.end(), [](rotationKeyFrame a, rotationKeyFrame b) {
		return a.frameStart < b.frameStart;
	});
}

void RotAnimation::addKeyFrame(float ms, float x, float y, float z)
{
	this->addKeyFrame(ms, bg::math::Vector3(x, y, z));
}

void RotAnimation::print()
{
	for (int i = 0; i < frames; i++)
	{
		//std::cout << "Time: " << RotFrames[i].frameStart << " Position: " << RotFrames[i].rot.toString() << std::endl;
	}
}

void RotAnimation::play(int times)
{
	_times = times;
	playing = true;
}

void RotAnimation::stop()
{
	playing = false;
}

void RotAnimation::reset()
{
	actualFrame = -1;
	timer = 0;
}






}
}