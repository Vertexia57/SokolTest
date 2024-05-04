#include "Lost.h"

void lost::createManagers()
{
	createImageManager();
	createShaderManager();
	createFontManager();
	createUIManager();
}

void lost::destroyManagers()
{
	destroyImageManager();
	destroyShaderManager();
	destroyFontManager();
	destroyUIManager();
}