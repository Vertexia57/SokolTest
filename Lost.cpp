#include "Lost.h"

void lost::createManagers()
{
	createImageManager();
	createShaderManager();
}

void lost::destroyManagers()
{
	destroyImageManager();
	destroyShaderManager();
}
