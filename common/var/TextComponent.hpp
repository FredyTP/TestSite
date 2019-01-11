#pragma once

#include <bg\bg2e.hpp>
#include <string>

namespace bg
{
	namespace scene
	{

		class TextComponent : public bg::scene::Component
		{
		public:
			TextComponent(bg::text::Font *f);
			TextComponent * clone() { return new TextComponent(_font.getPtr()); }
			
			virtual void init();
			virtual void draw(bg::base::Pipeline * pl);
			void setFont(bg::text::Font *f);

			void setText(const std::string &text);
			void setParagraph(bg::text::Paragraph &p);

		protected:

			std::string _text;
			bg::ptr<bg::text::Font> _font;
			bg::ptr<bg::text::StringRenderer> _stringRenderer;
		};



	}
}



