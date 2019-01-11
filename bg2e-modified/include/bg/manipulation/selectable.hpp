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

#ifndef _bg2e_manipulation_selectable_hpp_
#define _bg2e_manipulation_selectable_hpp_


#include <bg/scene/node.hpp>
#include <bg/scene/drawable.hpp>
#include <bg/system/system.hpp>

namespace bg {
namespace manipulation {

union ColorId {
	uint32_t uint32;
	uint8_t uint8[4];
};

class Selectable;
class BG2E_EXPORT ColorPickId {
	friend class Selectable;
public:

	inline uint8_t b0() const { return _colorId.uint8[0]; }
	inline uint8_t b1() const { return _colorId.uint8[1]; }
	inline uint8_t b2() const { return _colorId.uint8[2]; }
	inline uint8_t b3() const { return _colorId.uint8[3]; }
	inline uint8_t & b0() { return _colorId.uint8[0]; }
	inline uint8_t & b1() { return _colorId.uint8[1]; }
	inline uint8_t & b2() { return _colorId.uint8[2]; }
	inline uint8_t & b3() { return _colorId.uint8[3]; }
	inline uint32_t uintValue() const { return _colorId.uint32; }
	inline uint32_t & uintValue() { return _colorId.uint32; }

	ColorPickId() {
		_colorId.uint32 = 0;
	}
	
	ColorPickId(uint32_t value) {
		_colorId.uint32 = value;
	}
	
	ColorPickId(uint8_t * value) {
		b0() = value[0];
		b1() = value[1];
		b2() = value[2];
		b3() = value[3];
	}
	
	ColorPickId(const bg::math::Color & color) {
		if (bg::system::isBigEndian()) {
			b0() = static_cast<uint8_t>(color.r() * 255.0f);
			b1() = static_cast<uint8_t>(color.g() * 255.0f);
			b2() = static_cast<uint8_t>(color.b() * 255.0f);
			b3() = static_cast<uint8_t>(color.a() * 255.0f);
		}
		else {
			b0() = static_cast<uint8_t>(color.a() * 255.0f);
			b1() = static_cast<uint8_t>(color.b() * 255.0f);
			b2() = static_cast<uint8_t>(color.g() * 255.0f);
			b3() = static_cast<uint8_t>(color.r() * 255.0f);
		}
	}
	
	ColorPickId(const ColorPickId & other) {
		_colorId.uint32 = other._colorId.uint32;
	}
	
	inline void operator=(uint32_t value) {
		_colorId.uint32 = value;
	}
	
	inline void operator=(uint8_t * value) {
		b0() = value[0];
		b1() = value[1];
		b2() = value[2];
		b3() = value[3];
	}
	
	inline void operator=(const bg::math::Color & color) {
		if (bg::system::isBigEndian()) {
			b0() = static_cast<uint8_t>(color.r() * 255.0f);
			b1() = static_cast<uint8_t>(color.g() * 255.0f);
			b2() = static_cast<uint8_t>(color.b() * 255.0f);
			b3() = static_cast<uint8_t>(color.a() * 255.0f);
		}
		else {
			b0() = static_cast<uint8_t>(color.a() * 255.0f);
			b1() = static_cast<uint8_t>(color.b() * 255.0f);
			b2() = static_cast<uint8_t>(color.g() * 255.0f);
			b3() = static_cast<uint8_t>(color.r() * 255.0f);
		}
	}
	
	inline void operator=(const ColorPickId & other) {
		_colorId.uint32 = other._colorId.uint32;
	}
	
	inline bool operator==(const ColorPickId & other) {
		return _colorId.uint32 == other._colorId.uint32;
	}
	
	inline bg::math::Color colorValue() {
		if (bg::system::isBigEndian()) {
			return bg::math::Color(static_cast<float>(b0()) / 255.0f,
								   static_cast<float>(b1()) / 255.0f,
								   static_cast<float>(b2()) / 255.0f,
								   static_cast<float>(b3()) / 255.0f);
		}
		else {
			return bg::math::Color(static_cast<float>(b3()) / 255.0f,
								   static_cast<float>(b2()) / 255.0f,
								   static_cast<float>(b1()) / 255.0f,
								   static_cast<float>(b0()) / 255.0f);
		}
		
	}
	
protected:
	static uint32_t s_genValue;
	
	void generateIdentifier();
	
	ColorId _colorId;
};

class SelectableItem : public bg::base::ReferencedPointer {
public:
	enum Type {
		kPlist = 1,
		kGizmo
	};
	
	SelectableItem() :_type(kPlist) {}
	SelectableItem(const ColorPickId & id, bg::base::PolyList * plist, bg::base::Material * mat,
				   bg::scene::Drawable * drw, bg::scene::Node * node,
				   Type t, const bg::math::Matrix4 & trx)
		:_id(id), _plist(plist), _material(mat), _drawable(drw), _node(node), _type(t), _trx(trx)
	{}
	
	inline const ColorPickId & id() const { return _id; }
	inline ColorPickId & id() { return _id; }
	inline const bg::base::PolyList * polyList() const { return _plist.getPtr(); }
	inline bg::base::PolyList * polyList() { return _plist.getPtr(); }
	inline const bg::base::Material * material() const { return _material.getPtr(); }
	inline bg::base::Material * material() { return _material.getPtr(); }
	inline const bg::scene::Drawable * drawable() const { return _drawable.getPtr(); }
	inline bg::scene::Drawable * drawable() { return _drawable.getPtr(); }
	inline const bg::scene::Node * node() const { return _node.getPtr(); }
	inline bg::scene::Node * node() { return _node.getPtr(); }
	inline Type type() const { return _type; }
	
	
	inline void setId(const ColorPickId & id) { _id = id; }
	inline void setPolyList(bg::base::PolyList * plist) { _plist = plist; }
	inline void setMaterial(bg::base::Material * mat) { _material = mat; }
	inline void setDrawable(bg::scene::Drawable * drw) { _drawable = drw; }
	inline void setNode(bg::scene::Node * node) { _node = node; }
	inline void setType(Type t) { _type = t; }
	
	inline void clear() {
		_id.uintValue() = 0;
		_plist = nullptr;
		_material = nullptr;
		_drawable = nullptr;
		_node = nullptr;
	}
	
	inline bool valid() const { return _id.uintValue()!=0; }
	
	inline const bg::math::Matrix4 & trx() const { return _trx; }
	inline void setTrx(const bg::math::Matrix4 & trx) { _trx = trx; }
	
	inline void operator=(SelectableItem & item) {
		_id = item._id;
		_plist = item._plist.getPtr();
		_material = item._material.getPtr();
		_drawable = item._drawable.getPtr();
		_node = item._node.getPtr();
		_type = item._type;
	}
	
	inline bool operator==(const SelectableItem & item) {
		return	_id == item._id &&
				_plist.getPtr() == item._plist.getPtr() &&
				_material.getPtr() == item._material.getPtr() &&
				_drawable.getPtr() == item._drawable.getPtr() &&
				_node.getPtr() == item._node.getPtr() &&
				_type == item._type;
	}
	
protected:
	virtual ~SelectableItem() {}
	
	ColorPickId _id;
	bg::ptr<bg::base::PolyList> _plist;
	bg::ptr<bg::base::Material> _material;
	bg::ptr<bg::scene::Drawable> _drawable;
	bg::ptr<bg::scene::Node> _node;
	Type _type;
	
	// Used only for representation
	bg::math::Matrix4 _trx;
};

class BG2E_EXPORT Selectable : public bg::scene::Component {
public:

	static ColorPickId GetIdentifier();
	
	Selectable();
	
	virtual bg::scene::Component * clone();
	
	virtual void init();
	virtual void draw(bg::base::Pipeline * pl);
	
	inline void buildIdentifier() { init(); }
	
	SelectableItem * findId(const ColorPickId & id);
	
protected:
	virtual ~Selectable();
	
	std::vector<ptr<SelectableItem>> _selectablePlist;
	
	static bool s_selectMode;
};

}
}

#endif
