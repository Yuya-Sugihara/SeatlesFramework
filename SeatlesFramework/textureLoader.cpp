#include "textureLoader.h"
#include "assert.h"

using namespace seatlesFramework;
using namespace render;

//ScratchImage& TextureLoader::load(const wchar_t* fileName)
//{
//	TexMetadata metaData{};
//	ScratchImage scratchImage = {};
//
//	auto result = LoadFromWICFile(fileName, WIC_FLAGS_NONE, &metaData, scratchImage);
//	throwAssertIfFailed(result, "テクスチャのロードに失敗しました。");
//
//	return scratchImage;
//}