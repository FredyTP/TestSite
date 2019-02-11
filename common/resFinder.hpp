#pragma once

#include <string>
#include <bg/bg2e.hpp>

namespace ts
{
	const struct Resources
	{		
		struct Texture
		{
			bg::system::Path path = bg::system::Path::AppDir();
			bg::system::Path base = path;

			bg::system::Path grass;
			bg::system::Path mainIcon;
			bg::system::Path clock;
			bg::system::Path colorCircle;
			bg::system::Path wood;
			Texture()
			{
				base.addComponent("resources/texture/");
				grass = base;
				mainIcon = base;
				clock = base;
				colorCircle = base;
				wood = base;
				grass.addComponent("grass1.jpg");
				mainIcon.addComponent("mainIcon.png");
				clock.addComponent("clockTexture.png");
				colorCircle.addComponent("colorCircle.bmp");
				wood.addComponent("wood.jpg");
			}
		}texture;
		struct Font
		{
			bg::system::Path path = bg::system::Path::AppDir();
			bg::system::Path base = path;
			bg::system::Path arial;
			bg::system::Path nunito_black;
			bg::system::Path digital;
			Font()
			{
				base.addComponent("resources/font/");
				arial = base;
				nunito_black = base;
				digital = base;
				arial.addComponent("arial.ttf");
				nunito_black.addComponent("Nunito-Black.ttf");
				digital.addComponent("DS-DIGI.ttf");
				
			}
		}font;
		struct Icon
		{
			bg::system::Path path = bg::system::Path::AppDir();
			bg::system::Path base = path;

			bg::system::Path main;
			bg::system::Path finger;
			bg::system::Path vr;
			Icon()
			{
				base.addComponent("resources/icon/");

				main = base;
				finger = base;
				vr = base;

				main.addComponent("mainIcon.ico");
				finger.addComponent("finger.ico");
				vr.addComponent("vrIcon.ico");
			}
		}icon;
		struct Model
		{
			bg::system::Path path = bg::system::Path::AppDir();
			bg::system::Path base = path;

			bg::system::Path CtrlTile;
			Model()
			{
				base.addComponent("resources/model/");

				CtrlTile = base;
				CtrlTile.addComponent("CtrlTile.bg2");
			}
		}model;
	}resources;
}