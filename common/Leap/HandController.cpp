#ifdef _LEAP_H

#include "HandController.hpp"
#include <app/App.h>

namespace ts
{
	namespace leap
	{

		HandController::HandController(bg::scene::Node * cameraNode)
		{
			_cameraNode = cameraNode;
			ts::App::Get().leapController()->addListener(_leap);
		}

		void HandController::init()
		{
			using namespace bg::math;
			using namespace bg::scene;

			PrimitiveFactory factory(context());
			_leapNode = new Node(context());
			_leapNode->addComponent(new Transform);
			_leapNode->addComponent(factory.cube(0.1f, 0.02f, 0.03f));
			_leapNode->transform()->matrix().setPosition(0.0f, -0.2f, -0.6f);
			//_leapNode->transform()->matrix().rotate(bg::math::kPiOver2, 1.0f, 0.0f, 0.0f);

			_cameraNode->addChild(_leapNode);

			_palmCenter = new Node(context());
			_palmCenter->addComponent(factory.sphere(0.03f, 10.0f, 10.0f));
			_palmCenter->addComponent(new Transform);
			_leapNode->addChild(_palmCenter);

			_fingerNodes.resize(5);
			for (int i = 0; i < 5; i++)
			{
				_fingerNodes[i].resize(4);
				for (int j = 0; j < 4; ++j)
				{
					_fingerNodes[i][j] = new Node(context());
					_fingerNodes[i][j]->addComponent(factory.sphere(0.008f, 10, 10));
					_fingerNodes[i][j]->addComponent(new Transform);
					_leapNode->addChild(_fingerNodes[i][j]);
				}
			}

			_bones.resize(5);
			for (int i = 0; i < 5; i++)
			{
				_bones[i].resize(4);
				for (int j = 0; j < 4; ++j)
				{
					_bones[i][j] = new Node(context());
					_bones[i][j]->addComponent(new Transform);
					_leapNode->addChild(_bones[i][j]);
				}
			}

		}

		void HandController::frame(float delta)
		{
			const Leap::Frame frame = ts::App::Get().leapController()->frame();
			Leap::HandList hands = frame.hands();
		
			

			for (Leap::HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
				// Get the first hand
				
				const Leap::Hand hand = *hl;
				if (hand.isRight())
				{
					_palmCenter->transform()->matrix().setPosition(hand.palmPosition().x / 1000, hand.palmPosition().y / 1000, hand.palmPosition().z / 1000);
					
					for (int i = 0; i < hand.fingers().count(); ++i)
					{
						Leap::Finger finger = hand.fingers()[i];
						for (int j = 0; j < 4; ++j) 
						{
							//NODES
							Leap::Bone::Type boneType = static_cast<Leap::Bone::Type>(j);
							Leap::Bone bone = finger.bone(boneType);
							_fingerNodes[i][j]->transform()->matrix().setPosition(bone.nextJoint().x / 1000, bone.nextJoint().y / 1000, bone.nextJoint().z / 1000);
							
							//BONES
							/*bg::math::Vector3 normal(-bone.basis().yBasis.x, -bone.basis().yBasis.y, -bone.basis().yBasis.z);

							bg::math::Vector3 next(bone.nextJoint().x / 1000, bone.nextJoint().y / 1000, bone.nextJoint().z / 1000);
							bg::math::Vector3 prev(bone.prevJoint().x / 1000, bone.prevJoint().y / 1000, bone.prevJoint().z / 1000);

							bg::math::Vector3 dir(bone.direction().x, bone.direction().y, bone.direction().z);
							dir.scale(-1);
							bg::math::Vector3 up(bone.basis().yBasis.x, bone.basis().yBasis.y, bone.basis().yBasis.z);
							up.scale(-1);
							bg::math::Vector3 center(bone.center().x / 1000, bone.center().y / 1000, bone.center().z / 1000);

							bg::math::Matrix4 matrix;
							matrix.lookAt(prev, next, normal);
							_bones[i][j]->transform()->setMatrix(matrix);
							//_bones[i][j]->transform()->matrix().setPosition(center);
							
	
							std::cout << up.toString() << std::endl;
							if (bonesInit<20)
							{
								bg::scene::PrimitiveFactory factory(context());
								_bones[i][j]->addComponent(factory.cube(0.005, 0.005, bone.length() / 2000));
								bonesInit++;
							}*/


						}

					}

					/*bg::math::Vector3 normal(-hand.palmNormal().x, -hand.palmNormal().y, -hand.palmNormal().z);
					bg::math::Vector3 dir(-hand.direction().x, -hand.direction().y, -hand.direction().z);

					bg::math::Matrix4 matrix;
					matrix.lookAt(bg::math::Vector3(0.0f, 0.0f, 0.0f), dir, normal);

					item->transform()->setMatrix(matrix);
					//item->transform()->matrix().setPosition(hand.palmPosition().x/100, hand.palmPosition().y/100, hand.palmPosition().z/100);

					bg::math::Vector3 fPos1(hand.fingers()[1].tipPosition().x / 100, hand.fingers()[1].tipPosition().y / 100, hand.fingers()[1].tipPosition().z / 100);
					fTip->transform()->matrix().setPosition(fPos1);

					bg::math::Vector3 fPos2(hand.fingers()[2].tipPosition().x / 100, hand.fingers()[2].tipPosition().y / 100, hand.fingers()[2].tipPosition().z / 100);
					fTip2->transform()->matrix().setPosition(fPos2);*/

				}
			}


		}

		void HandController::leftClick()
		{
			{
				INPUT    Input = { 0 };
				// left down 
				Input.type = INPUT_MOUSE;
				Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
				::SendInput(1, &Input, sizeof(INPUT));

				// left up
				::ZeroMemory(&Input, sizeof(INPUT));
				Input.type = INPUT_MOUSE;
				Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
				::SendInput(1, &Input, sizeof(INPUT));
			}
		}

		HandController::~HandController()
		{
			ts::App::Get().leapController()->removeListener(_leap);
		}

	}
}

#endif // _LEAP_H