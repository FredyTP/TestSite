#pragma once

#include <string>

namespace ts
{
	const struct 
	{		
		const struct 
		{
			const std::string base = "resources//texture//";
			const std::string grass = base + "grass1.jpg";
			const std::string mainIcon = base + "mainIcon.png";
			const std::string clock = base + "clockTexture.png";
			const std::string colorCircle = base + "colorCircle.bmp";
			const std::string wood = base + "wood.jpg";
		}texture;
		const struct
		{
			const std::string base = "resources//font//";
			const std::string arial = base + "arial.ttf";
			const std::string nunito_black = base + "Nunito-Black.ttf";
			const std::string digital = base + "DS-DIGI.ttf";
		}font;
		const struct
		{
			const std::string base = "resources//icon//";
			const std::string main = base + "mainIcon.ico";
			const std::string finger = base + "finger.ico";
			const std::string vr = base + "vrIcon.ico";
		}icon;
	}resources;
}