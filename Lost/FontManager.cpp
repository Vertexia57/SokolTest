#include "FontManager.h"
#include "ImageManager.h"

#include "ft2build.h"
#include FT_FREETYPE_H

int min(int a, int b)
{
	return (a > b) ? b : a;
}

int max(int a, int b)
{
	return (a > b) ? a : b;
}

namespace lost
{

	FontManager::FontManager()
	{
		textShader = loadShader("Shaders/vertex.vert", "Shaders/textFragment.frag", "text");
	}

	FontManager::~FontManager()
	{
		cleanup();
	}

	Font* FontManager::getFont(FontID fontID)
	{
		return m_Fonts[fontID];
	}

	Font* FontManager::getFont(std::string fontName)
	{
		return m_Fonts[m_FontMap[fontName]];
	}

	void FontManager::setFont(FontID fontID)
	{
		currentFont = fontID;
	}

	void FontManager::setFont(std::string fontName)
	{
		currentFont = m_FontMap[fontName];
	}

	void FontManager::cleanup()
	{
		for (Font* font : m_Fonts)
			sg_destroy_image(font->fontTexture);
		m_Fonts.clear();
	}

	FontID FontManager::loadFont(std::string filePath, float fontSize, std::string fontName)
	{
		Font* newFont = new Font();

		FT_Library fontLibrary;
		FT_Init_FreeType(&fontLibrary);

		FT_Face fontFace;
		FT_New_Face(fontLibrary, filePath.c_str(), 0, &fontFace);
		FT_Set_Pixel_Sizes(fontFace, 0, fontSize);

		int padding = 2;
		int row = 0;
		int col = padding;

		const int textureWidth = 512;
		char* textureBuffer = new char[textureWidth * textureWidth];

		// Set the buffer to black
		for (unsigned int y = 0; y < textureWidth; ++y)
		{
			for (unsigned int x = 0; x < textureWidth; ++x)
				textureBuffer[y * textureWidth + x] = 0;
		}

		for (FT_ULong glyphIdx = 32; glyphIdx < 127; ++glyphIdx)
		{
			FT_UInt glyphIndex = FT_Get_Char_Index(fontFace, glyphIdx);
			FT_Load_Glyph(fontFace, glyphIndex, FT_LOAD_DEFAULT);
			FT_Error error = FT_Render_Glyph(fontFace->glyph, FT_RENDER_MODE_NORMAL);

			if (col + fontFace->glyph->bitmap.width + padding >= 512)
			{
				col = padding;
				row += fontSize;
			}

			// Font Height
			newFont->fontHeight = max((fontFace->size->metrics.ascender - fontFace->size->metrics.descender) >> 6, newFont->fontHeight);

			for (unsigned int y = 0; y < fontFace->glyph->bitmap.rows; ++y)
			{
				for (unsigned int x = 0; x < fontFace->glyph->bitmap.width; ++x)
				{
					textureBuffer[(row + y) * textureWidth + col + x] =
						fontFace->glyph->bitmap.buffer[y * fontFace->glyph->bitmap.width + x];
				}
			}

			Glyph* glyph = &newFont->glyphs[glyphIdx];
			glyph->textureCoords = { col, row };
			glyph->size =
			{
				(int)fontFace->glyph->bitmap.width,
				(int)fontFace->glyph->bitmap.rows
			};
			glyph->advance =
			{
				(float)(fontFace->glyph->advance.x >> 6),
				(float)(fontFace->glyph->advance.y >> 6)
			};
			glyph->offset =
			{
				(float)fontFace->glyph->bitmap_left,
				(float)fontFace->glyph->bitmap_top,
			};

			col += fontFace->glyph->bitmap.width + padding;
		}

		FT_Done_Face(fontFace);
		FT_Done_FreeType(fontLibrary);

		// Upload OpenGL Texture
		{
			sg_image_desc image_desc = { };
			image_desc.width = textureWidth;
			image_desc.height = textureWidth;
			image_desc.pixel_format = SG_PIXELFORMAT_R8;
			image_desc.data.subimage[0][0].ptr = textureBuffer;
			image_desc.data.subimage[0][0].size = (size_t)(textureWidth * textureWidth);
			newFont->fontTexture = sg_make_image(&image_desc);
		}

		delete[] textureBuffer;

		newFont->fontID = m_Fonts.size();
		m_FontMap[fontName] = newFont->fontID;
		m_Fonts.push_back(newFont);

		return newFont->fontID;
	}

	FontManager* fontManager;

	void createFontManager()
	{
		fontManager = new FontManager();
	}

	void destroyFontManager()
	{
		delete fontManager;
	}

	FontID loadFont(std::string filePath, float fontSize, std::string fontName)
	{
		return fontManager->loadFont(filePath, fontSize, fontName);
	}

	Font* getFont(FontID fontID)
	{
		return fontManager->getFont(fontID);
	}

	Font* getFont(std::string fontName)
	{
		return fontManager->getFont(fontName);
	}

	void setFont(FontID fontID)
	{
		return fontManager->setFont(fontID);
	}

	void setFont(std::string fontName)
	{
		return fontManager->setFont(fontName);
	}

	void renderTextPro(std::string text, Vector2D position, float size, int hAlign, int vAlign, FontID fontID)
	{

		if (text.size() > 0)
		{

			Font* font;
			if (fontID == (FontID)(-1))
				font = getFont(fontManager->currentFont);
			else
				font = getFont(fontID);

			Vector2D origin = position;

			switch (hAlign)
			{
			case LOST_TEXT_ALIGN_LEFT:
				break;
			case LOST_TEXT_ALIGN_MIDDLE:
				origin.x -= (float)textWidth(text, size, fontID) / 2.0f;
				break;
			case LOST_TEXT_ALIGN_RIGHT:
				origin.x -= (float)textWidth(text, size, fontID);
				break;
			}

			switch (vAlign)
			{
			case LOST_TEXT_ALIGN_TOP:
				origin.y -= (float)font->fontHeight * size;
				break;
			case LOST_TEXT_ALIGN_MIDDLE:
				origin.y -= (float)textHeight(text, size, fontID) / 2.0f - font->fontHeight * size / 2.0f;
				break;
			case LOST_TEXT_ALIGN_BOTTOM:
				origin.y -= (float)textHeight(text, size, fontID) - font->fontHeight * size;
				break;
			}

			Vector2D pos = origin;

			ShaderID oldShader = shaderManager->currentShader;
			if (oldShader != fontManager->textShader)
				bindShader(fontManager->textShader);

			sgp_set_image(0, font->fontTexture);
			sgp_set_sampler(0, getSampler(LOST_SAMPLER_LINEAR));

			for (char c : text)
			{
				Glyph& glyph = font->glyphs[c];

				sgp_draw_textured_rect(0, 
					{ pos.x - glyph.offset.x * size, pos.y - glyph.offset.y * size, (float)glyph.size.x * size, (float)glyph.size.y * size }, // Position
					{ (float)glyph.textureCoords.x, (float)glyph.textureCoords.y, (float)glyph.size.x, (float)glyph.size.y } // Texture Coords
				);

				pos.x += glyph.advance.x * size;
				if (c == '\n')
				{
					pos.x = origin.x;
					pos.y += font->fontHeight * size;
				}
			}

			if (oldShader != fontManager->textShader)
				bindShader(oldShader);
			sgp_set_sampler(0, getSampler(LOST_SAMPLER_NEAREST));

		}
	}

	void renderText(std::string text, Vector2D position, float size, FontID fontID)
	{
		renderTextPro(text, position, size, LOST_TEXT_ALIGN_LEFT, LOST_TEXT_ALIGN_TOP, -1);
	}

	float textWidth(std::string text, float size, FontID fontID)
	{
		if (text.size() > 0)
		{
			Font* font;
			if (fontID == (FontID)(-1))
				font = getFont(fontManager->currentFont);
			else
				font = getFont(fontID);

			Vector2D pos = { 0.0f, 0.0f };
			float widest = 0.0f;

			for (char c : text)
			{
				Glyph& glyph = font->glyphs[c];
				pos.x += glyph.advance.x * size;
				if (c == '\n')
				{
					widest = fmaxf(fabs(pos.x), widest);
					pos.x = 0.0f;
				}
			}

			widest = fmaxf(fabs(pos.x), widest);

			return widest;
		}

		return 0.0f;
	}

	float textHeight(std::string text, float size, FontID fontID)
	{
		if (text.size() > 0)
		{
			Font* font;
			if (fontID == (FontID)(-1))
				font = getFont(fontManager->currentFont);
			else
				font = getFont(fontID);

			Vector2D pos = { 0.0f, 0.0f };

			float minHeight = FLT_MAX;
			float maxHeight = FLT_MIN;

			for (char c : text)
			{
				Glyph& glyph = font->glyphs[c];

				minHeight = fminf(minHeight, pos.y - glyph.offset.y * size);
				maxHeight = fmaxf(maxHeight, pos.y - glyph.offset.y * size + (float)glyph.size.y * size);

				if (c == '\n')
					pos.y += font->fontHeight * size;
			}

			return maxHeight - minHeight;
		}

		return 0.0f;
	}

	Bound2D textBounds(std::string text, Vector2D position, float size, FontID fontID)
	{
		if (text.size() > 0)
		{

			Font* font;
			if (fontID == (FontID)(-1))
				font = getFont(fontManager->currentFont);
			else
				font = getFont(fontID);

			Vector2D origin = position + Vector2D{ 0.0f, (float)font->fontHeight * size }; //(float)font->fontHeight * size
			Vector2D pos = origin;

			Vector2D minCorner = { FLT_MAX, FLT_MAX };
			Vector2D maxCorner = { FLT_MIN, FLT_MIN };

			for (char c : text)
			{
				Glyph& glyph = font->glyphs[c];

				minCorner.x = fminf(minCorner.x, pos.x - glyph.offset.x * size);
				minCorner.y = fminf(minCorner.y, pos.y - glyph.offset.y * size);
				maxCorner.x = fmaxf(maxCorner.x, pos.x - glyph.offset.x * size + (float)glyph.size.x * size);
				maxCorner.y = fmaxf(maxCorner.y, pos.y - glyph.offset.y * size + (float)glyph.size.y * size);

				pos.x += glyph.advance.x * size;
				if (c == '\n')
				{
					pos.x = 0.0f;
					pos.y += font->fontHeight * size;
				}

			}
			
			return { minCorner.x, minCorner.y, maxCorner.x - minCorner.x, maxCorner.y - minCorner.y };
		}

		return { position.x, position.y, 0.0f, 0.0f };
	}

}