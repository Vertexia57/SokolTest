#include "Lost.h"

void lost::createManagers()
{
	createImageManager();
	createShaderManager();
	createFontManager();
}

void lost::destroyManagers()
{
	destroyImageManager();
	destroyShaderManager();
	destroyFontManager();
}
