//#include"Graphics.h"
//#include"GraphicsDirectX9X.h"
//#include"GraphicsDirectX11.h"
//
//using namespace Inferno;
//
////std::shared_ptr<IGraphics> Graphics::Create(GraphicApi apiType, HWND hWnd)
////{
////	std::shared_ptr<IGraphics> g;
////
////	switch (apiType)
////	{
////	case GraphicApi::DirectX9X:
////		g = std::make_shared<GraphicsDirectX9X>(hWnd);
////		break;
////
////	case GraphicApi::DirectX10:
////		g = std::make_shared<GraphicsDirectX11>(hWnd);
////		break;
////
////	default:
////		assert(false);
////		break;
////	}
////
////	return g;
////}