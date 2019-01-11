#include "TextComponent.hpp"

namespace bg
{
	namespace scene
	{

		TextComponent::TextComponent(bg::text::Font * f)
		{
			_font = f;
		}

		void TextComponent::init()
		{
			_stringRenderer = new bg::text::StringRenderer(_font.getPtr(), context());

			bg::text::Paragraph _p;
			_p.setAlignment(bg::text::Paragraph::kHAlignCenter);
			_p.setAlignment(bg::text::Paragraph::kVAlignCenter);

			_stringRenderer->setParagraph(_p);
			this->_initialized = true;
		}

		void TextComponent::draw(bg::base::Pipeline * pl)
		{
			_stringRenderer->draw(pl, _text);
		}

		void TextComponent::setFont(bg::text::Font * f)
		{
			_font = f;
			_stringRenderer->setFont(_font.getPtr());
		}

		void TextComponent::setText(const std::string & text)
		{
			_text = text;
		}

		void TextComponent::setParagraph(bg::text::Paragraph & p)
		{
			_stringRenderer->setParagraph(p);
		}

	}
}