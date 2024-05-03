#pragma once
#include "freetype/freetype.h"
#include "../SokolReference.h"
#include "Transform2D.h"
#include <string>
#include <map>
#include <vector>
#include "ShaderManager.h"

typedef unsigned int FontID;

enum {
	LOST_TEXT_ALIGN_LEFT = 0,
	LOST_TEXT_ALIGN_TOP = 0,
	LOST_TEXT_ALIGN_MIDDLE = 1,
	LOST_TEXT_ALIGN_RIGHT = 2,
	LOST_TEXT_ALIGN_BOTTOM = 2
};

namespace lost
{

	struct Glyph
	{
		Vector2D offset;
		Vector2D advance;
		IntVector2D textureCoords;
		IntVector2D size;
	};

	struct Font
	{
		int fontHeight;
		Glyph glyphs[127];
		sg_image fontTexture;
		FontID fontID;
	};

	class FontManager
	{
	public:
		FontManager();
		~FontManager();

		// Loads the ttf file at "filePath" with the name and size given
		FontID loadFont(std::string filePath, float fontSize, std::string fontName);

		// Returns a pointer to the font struct with fontID
		Font* getFont(FontID fontID);
		// Returns a pointer to the font struct with fontName
		Font* getFont(std::string fontName);

		// Sets the current font to the font with fontID
		void setFont(FontID fontID);
		// Sets the current font to the font with fontName
		void setFont(std::string fontName);

		void cleanup();

		FontID currentFont;

		ShaderID textShader;
	private:
		std::vector<Font*> m_Fonts;
		std::map<std::string, FontID> m_FontMap;
	};

	extern FontManager* fontManager;

	// Creates the font manager
	extern void createFontManager();
	// Destroys the font data loaded into the font manager
	extern void destroyFontManager();

	// Loads the ttf file at "filePath" with the name and size given
	extern FontID loadFont(std::string filePath, float fontSize, std::string fontName);

	// Returns a pointer to the font struct with fontID
	extern Font* getFont(FontID fontID);
	// Returns a pointer to the font struct with fontName
	extern Font* getFont(std::string fontName);

	// Sets the current font in the font manager to the font with fontID
	extern void setFont(FontID fontID);
	// Sets the current font in the font manager to the font with fontName
	extern void setFont(std::string fontName);

	extern void renderTextPro(std::string text, Vector2D position, float size, int hAlign = 0, int vAlign = 0, FontID fontID = -1);
	extern void renderText(std::string text, Vector2D position, float size, FontID fontID = -1);

	// Returns the width that the given text would have on the screen with the font given
	extern float textWidth(std::string text, float size, FontID fontID);
	// Returns the height that the given text would have on the screen with the font given
	extern float textHeight(std::string text, float size, FontID fontID);
	// Returns the bounds that the given text would have on the screen with the font given
	extern Bound2D textBounds(std::string text, Vector2D position, float size, FontID fontID);

}