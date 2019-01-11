#ifndef _vr_model_hpp_
#define _vr_model_hpp_

#include <string>
#include <openvr.h>
#include <bg/base/context_object.hpp>

#include <bg/scene/drawable.hpp>

class VRModelFactory : public bg::base::ContextObject
{
public:

	VRModelFactory(bg::base::Context * ctx);

	bg::scene::Drawable * loadModel(const std::string & name);

	/*
	bool init(const vr::RenderModel_t & vrModel, const vr::RenderModel_TextureMap_t & vrDiffuseTexture);
	void destroy();
	void draw();
	const std::string & name() const { return m_sModelName; }

private:

	uint32_t m_glVertBuffer;
	uint32_t m_glIndexBuffer;
	uint32_t m_glVertArray;
	uint32_t m_glTexture;
	size_t m_unVertexCount;
	std::string m_sModelName;*/

protected:
	void addPolyList(bg::scene::Drawable *, vr::RenderModel_t *, vr::RenderModel_TextureMap_t *);
};


#endif
