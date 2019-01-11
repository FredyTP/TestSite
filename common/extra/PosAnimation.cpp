#include "PosAnimation.h"

namespace bg
{
namespace animation
{


bool operator==(const positionKeyFrame & n1, const positionKeyFrame & n2)
{
	return n1.frameStart == n2.frameStart;
}


PosAnimation::PosAnimation()
{
	timer = 0.0f;
	frames = 0;
	actualFrame = -1;
	relative = false;
}


PosAnimation::~PosAnimation()
{
}

void PosAnimation::init()
{
	trx = node()->transform();
}

void PosAnimation::frame(float delta)
{
	if (playing)
	{
		timer += delta;
		if (timer >= PosFrames[actualFrame + 1].frameStart)
		{
			//NEXT KEY FRAME//
			//CALCULA LA VELOCIDAD DEL TRAMO//
			actualFrame++;
			if (actualFrame < frames - 1)
			{
				vel = (PosFrames[actualFrame + 1].pos - PosFrames[actualFrame].pos)
					/ (PosFrames[actualFrame + 1].frameStart - PosFrames[actualFrame].frameStart);
			}

		}
		if (actualFrame > -1)
		{
			if (actualFrame < frames - 1)
			{
				//REALIZA EL DESPLAZAMIENTO
				bg::math::Vector3 newPos = vel;
				newPos.scale(timer - PosFrames[actualFrame].frameStart);
				newPos.add(PosFrames[actualFrame].pos);
				trx->matrix().setPosition(newPos);
			}
			else
			{
				//SI TERMINA LA ANIMACION PONEMOS LA ULTIMA POSICION
				trx->matrix().setPosition(PosFrames[frames - 1].pos);
				if(_times!=-1)
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

void PosAnimation::addKeyFrame(float ms, bg::math::Vector3  pos)
{
	frames++;
	positionKeyFrame frame(ms, pos);
	if (relative)
	{


		//IF ALREADY KEYFRAME THERE; ERASE IT
		auto result = std::find(relPath.begin(), relPath.end(), frame);
		if (result != std::end(relPath)) {
			relPath.erase(result);
			frames--;
		}

		//ADDS AND SORT THE KEYFRAMES
		relPath.push_back(frame);
		std::sort(relPath.begin(), relPath.end(), [](positionKeyFrame a, positionKeyFrame b) {
			return a.frameStart < b.frameStart;
		});

		this->createPath();
		
		


	}
	else
	{
		//IF ALREADY KEYFRAME THERE; ERASE IT
		auto result=std::find(PosFrames.begin(), PosFrames.end(), frame);
		if (result != std::end(PosFrames)) {
			PosFrames.erase(result);
			frames--;
		}

		//ADDS AND SORT THE KEYFRAMES
		PosFrames.push_back(frame);
		std::sort(PosFrames.begin(), PosFrames.end(), [](positionKeyFrame a, positionKeyFrame b) {
			return a.frameStart < b.frameStart;
		});
	}
}

void PosAnimation::addKeyFrame(float ms, float x, float y, float z)
{
	this->addKeyFrame(ms, bg::math::Vector3(x, y, z));
}

void PosAnimation::print()
{
	for (int i = 0; i < frames; i++)
	{
		//std::cout << "Time: " << PosFrames[i].frameStart << " Position: " << PosFrames[i].pos.toString() << std::endl;
	}
}


void PosAnimation::play(int times)
{
	_times = times;
	playing = true;
}

void PosAnimation::stop()
{
	playing = false;
}

void PosAnimation::reset()
{
	actualFrame = -1;
	timer = 0;
}

void PosAnimation::relativeKeyFrames(bool rel)
{
	//THE FIRST KEYFRAME IS SET AS THE START POINT OF THE SECUENCE
	relative = true;
}

PosAnimation * PosAnimation::scale(float s)
{
	//SCALES THE WHOLE ANIMATION BY A FACTOR
	for (int i = 0; i < frames; i++)
	{
		PosFrames[i].pos.scale(s);
	}
	return this;
}

void PosAnimation::createPath()
{
	//EL PRIMER VALOR EN TIEMPO, SE TOMA COMO PUNTO DE INICIO//
	PosFrames = relPath;
	for (int i = 1; i < frames; i++)
	{
		PosFrames[i].pos = PosFrames[i - 1].pos + relPath[i].pos;
	}
}








}
}