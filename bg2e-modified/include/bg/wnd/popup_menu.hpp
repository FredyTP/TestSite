/*
 *	bg2 engine license
 *	Copyright (c) 2016 Fernando Serrano <ferserc1@gmail.com>
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 *	of the Software, and to permit persons to whom the Software is furnished to do
 *	so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 *	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 *	PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *	OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef _bg2e_wnd_popup_menu_hpp_
#define _bg2e_wnd_popup_menu_hpp_

#include <bg/base/referenced_pointer.hpp>

#include <bg/base/keyboard.hpp>

#include <bg/math/vector.hpp>

#include <vector>
#include <string>
#include <functional>
#include <unordered_map>

namespace bg {
namespace wnd {

typedef int32_t MenuItemIdentifier;

enum MenuIdentifiers {
	kCodeQuit = 0,
	
	kCodeOpen = 101,
	kCodeClose = 102,
	kCodeSave = 103,
	kCodeSaveAs = 104,
	kCodePrint = 105,
	kCodeExport = 106,
	kCodeExportAs = 107,
	
	kCodeCopy = 201,
	kCodeCut = 202,
	kCodePaste = 203,
	kCodeUndo = 204,
	kCodeRedo = 205,

	kCodeCustom = 9000
};

struct PopUpMenuItem {
	
	MenuItemIdentifier identifier;
	std::string title;
	bg::base::KeyboardShortcut shortcut;


	PopUpMenuItem() :identifier(0), title("") {}
	PopUpMenuItem(MenuItemIdentifier id, const std::string & t) :identifier(id), title(t) {}
	PopUpMenuItem(MenuItemIdentifier id, const std::string & t, const bg::base::KeyboardShortcut & sc) :identifier(id), title(t), shortcut(sc) {}

	inline void operator=(const PopUpMenuItem & other) {
		identifier = other.identifier;
		title = other.title;
	}

	inline bool operator==(const PopUpMenuItem & other) {
		return other.identifier==identifier && other.title==title;
	}

	inline bool valid() const { return identifier!=-1; }
	inline bool invalid() const { return identifier==-1; }
};



class BG2E_EXPORT PopUpMenu : public bg::base::ReferencedPointer {
public:

	enum MenuFlags
	{
		kBitMap = MF_BITMAP,
		kChecked = MF_CHECKED,
		kUnchecked = MF_UNCHECKED,
		kDisabled = MF_DISABLED,
		kEnabled = MF_ENABLED,
		kGrayed = MF_GRAYED,
		kMenuBarBreak = MF_MENUBARBREAK,
		kMenuBreak = MF_MENUBREAK,
		kPopUp = MF_POPUP,
		kSeparator = MF_SEPARATOR,
		kString = MF_STRING, 
		kOwnerDraw = MF_OWNERDRAW,
	};
	typedef std::vector<PopUpMenuItem> PopUpMenuItemVector;
	typedef std::function<void(MenuItemIdentifier)> ItemSelectedClosure;
	
	static PopUpMenu * New();
	static PopUpMenu * New(const int m_flags);
	static PopUpMenu * New(const std::string & title);
	static PopUpMenu * New(const std::string & title , const int m_flags);
	static PopUpMenu * New(const std::string & title, const int m_flags , const MenuItemIdentifier id);

	inline void setTitle(const std::string & title) { _title = title; }
	inline const std::string & title() const { return _title; }

	inline void setPosition(const bg::math::Position2Di & pos) { _position = pos; }
	inline const bg::math::Position2Di getPosition() const { return _position; }
	inline bg::math::Position2Di getPosition() { return _position; }

	inline void setFlags(const int m_flags) { _flags = m_flags; }
	inline int flags() const { return _flags; }

	virtual void setCheck(bool check) const = 0;
	virtual bool isChecked() const =0 ;
	virtual int changeCheckState() const = 0;

	virtual void setEnabled(bool enable)const = 0;
	virtual bool isEnabled()const = 0;
	virtual int changeEnabledState()const = 0;

	inline void setIdentifier(const MenuItemIdentifier m_id) { _identifier = m_id; }
	inline MenuItemIdentifier identifier() const { return _identifier; }

	inline void addItem(const std::string & title, const MenuItemIdentifier id)
	{
		this->addSubMenu(New(title, kString, id));
	}
	inline void addItem(const std::string & title, const int m_flags, const MenuItemIdentifier id)
	{
		this->addSubMenu(New(title, m_flags, id));
	}
	inline void addSubMenu(const std::string title, const int m_flags = (kPopUp | kString) )
	{
		this->addSubMenu(New(title, m_flags));
	}
	inline PopUpMenu* subMenu(const std::string &name) const
	{
		for (auto menu : _subMenus)
		{
			if (name.compare(menu->title()) == 0)
			{
				return menu.getPtr();
			}
		}
		return nullptr;
	}
	/*inline int addMenuItem(const PopUpMenuItem & item) {
		_menuItems.push_back(item);
		return static_cast<int>(_menuItems.size() - 1);
	}
	inline void setMenuItem(MenuItemIdentifier identifier, const PopUpMenuItem & newItem) {
		for (auto & item : _menuItems) {
			if (item.identifier==identifier) {
				item = newItem;
			}
		}
	}
	inline const PopUpMenuItem & getMenuItem(MenuItemIdentifier identifier) const {
		for (auto & item : _menuItems) {
			if (item.identifier==identifier) {
				return item;
			}
		}
		return _invalidItem;
	}
	inline size_t getNumberOfItems() const { return _menuItems.size(); }
	inline void eachMenuItem(std::function<void(const PopUpMenuItem & item)> closure) const {
		for (auto & item : _menuItems) {
			closure(item);
		}
	}*/
	inline void eachSubMenu(std::function<void(const PopUpMenu * menu, int index)> closure) const {
		int index = 0;
		for (auto & menu : _subMenus)
		{
			closure(menu.getPtr(),index);
			index++;
		}
	}
	inline void someSubMenu(std::function<bool(const PopUpMenu * menu)> closure) const {
		for (auto & menu : _subMenus) {
			if (closure(menu.getPtr())) break;
		}
	}
	inline void someSubMenu(std::function<bool(const PopUpMenu * menu,int index)> closure) const {
		int index = 0;
		for (auto & menu : _subMenus) {
			if (closure(menu.getPtr(), index)) break;
			++index;
		}
	}
	/*inline void eachMenuItem(std::function<void(const PopUpMenuItem & item, int index)> closure) const {
		int index = 0;
		for (auto & item : _menuItems) {
			closure(item, index);
			++index;
		}
	}
	inline void someMenuItem(std::function<bool(const PopUpMenuItem & item)> closure) const {
		for (auto & item : _menuItems) {
			if (closure(item)) break;
		}
	}
	inline void someMenuItem(std::function<bool(const PopUpMenuItem & item, int index)> closure) const {
		int index = 0;
		for (auto & item : _menuItems) {
			if (closure(item, index)) break;
			++index;
		}
	}*/
	
	virtual void show(ItemSelectedClosure) = 0;
	virtual void addSubMenu(PopUpMenu *menu)
	{ 
		menu->setIdx(_subMenus.size());
		menu->setParent(this);
		_subMenus.push_back(menu);
	}

	inline std::vector<bg::ptr<PopUpMenu> > & subMenus() { return _subMenus; }

	//MENU HANDLER
	virtual void setHMenu(bg::plain_ptr hmenu) const { _hMenu = hmenu; }
	inline bg::plain_ptr & hMenu() const { return _hMenu; }

	PopUpMenu* parent() const{ return _parent; }
	size_t idx() const { return _idx; }
	virtual void initSubMenus() const = 0;

protected:
	PopUpMenu();

	

	virtual ~PopUpMenu();

	//PUT IN STRUCT
		std::string _title;
		MenuItemIdentifier _identifier = -1;
		bg::base::KeyboardShortcut shortcut;
		int _flags;
	//

	void setParent(PopUpMenu *p) { _parent = p; }
	void setIdx(size_t i) { _idx = i; }
	PopUpMenu* _parent = nullptr;
	std::vector<bg::ptr<PopUpMenu> > _subMenus;
	

	mutable bg::plain_ptr _hMenu;
	PopUpMenuItem _invalidItem = { -1, "" };
	bg::math::Position2Di _position;
	size_t _idx;
	
};

typedef std::vector<bg::ptr<PopUpMenu>> MenuDescriptor;

}
}

#endif
