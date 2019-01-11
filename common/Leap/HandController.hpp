#pragma once


//TestSite
#include <extra/Primitives.h>

//Bg2e
#include <bg\bg2e.hpp>

//Leap
#include <Leap/Listener.hpp>

//std
#include <vector>
#include <Windows.h>


namespace ts
{
	namespace leap
	{

		class HandController : public bg::scene::Component
		{
		public:
			HandController(bg::scene::Node* cameraNode);
			bg::scene::Component* clone() { return new HandController(_cameraNode); }

			void init();
			void frame(float delta);
			void leftClick();


		protected:
			~HandController();

			Leap::Listener _leap;
			std::vector<std::vector<bg::scene::Node*> > _fingerNodes;
			std::vector<std::vector<bg::scene::Node*> > _bones;
			std::vector<bg::scene::Node*> _fingerMiddle;
			bg::scene::Node* _palmCenter;
			bg::scene::Node* _cameraNode;
			bg::scene::Node* _leapNode;

			int bonesInit = 0;
			

		};



	}
}