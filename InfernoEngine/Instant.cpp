#include"Instant.h"
#include"Substance.h"
#include"GraphicsDirectX11.h"
#include"WindowsCommon.h"

using namespace Inferno;
using namespace DirectX;

using Base = Substance;

Instant::Instant() :
	Base(Idea())
{
	//m_tex = new Texture();
	//SetAttribute(GEAttribute::Visible, true);
}

Instant::~Instant() noexcept
{
	/*SAFE_DELETE(m_tex);*/
}