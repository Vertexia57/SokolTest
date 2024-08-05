#include "StarBackground.h"
#include "Random.h"
#include "Lost.h"

StarBackground::StarBackground()
	: Background()
{
	m_Stars = new Star[m_StarCount];
	m_StarRects = new sgp_textured_rect[m_StarCount];
	for (int i = 0; i < m_StarCount; i++)
	{
		m_StarRects[i].src = {
			0.0f, 0.0f,
			5.0f, 5.0f
		};
		m_Stars[i] = {
			lost::Vector2D{ randomf(0.0f, sapp_widthf()), randomf(0.0f, sapp_heightf()) }, // position
			randomf(1.0f, 5.0f), // depth
			randomf(0.5f, 2.0f) // rotation
		};
	}

	m_FrameBuffer = new lost::FrameBuffer();
	m_PixelateShaderID = lost::loadShader("Shaders/vertex.vert", "Shaders/pixelatefrag.frag", "PixelateShader");
	lost::getShader(m_PixelateShaderID)->setUniform(0, sapp_widthf());
	lost::getShader(m_PixelateShaderID)->setUniform(1, sapp_heightf());
}

StarBackground::~StarBackground()
{
	delete[] m_Stars;
	delete[] m_StarRects;

	delete m_FrameBuffer;
}

void StarBackground::update()
{
	for (int i = 0; i < m_StarCount; i++)
	{
		m_Stars[i].update();
	}
}

void StarBackground::render()
{
	m_FrameBuffer->bind();

	sgp_push_transform();
	sgp_rotate_at(lost::globalCamera.getRotation(), sapp_widthf() / 2.0f, sapp_heightf() / 2.0f);

	lost::useImage("starImage");
	for (int i = 0; i < m_StarCount; i++)
	{
		m_StarRects[i].dst = {
			m_Stars[i].renderPosition.x, m_Stars[i].renderPosition.y,
			10.0f, 10.0f
		};

		float fadeVal = 1.0f - (m_Stars[i].depth - 1.0f) / 10.0f;
		sgp_set_color(randomf(0.8f, 1.0f), randomf(0.8f, 1.0f), randomf(0.8f, 1.0f), fadeVal);

		sgp_push_transform();
		sgp_rotate_at(m_Stars[i].rotation, m_Stars[i].renderPosition.x + 5.0f, m_Stars[i].renderPosition.y + 5.0f);
		sgp_draw_textured_rect(0, m_StarRects[i].dst, m_StarRects->src);
		sgp_pop_transform();
	}

	sgp_pop_transform();

	m_FrameBuffer->unbind();

	lost::bindShader(m_PixelateShaderID);
	sgp_set_image(0, m_FrameBuffer->getBuffer());
	sgp_draw_textured_rect(0, { 0.0f, 0.0f, sapp_widthf(), sapp_heightf() }, { 0.0f, 0.0f, sapp_widthf(), sapp_heightf() });
	lost::resetShader();
}

void StarBackground::Star::update()
{
	position.x += lost::deltaTime / 100.0f / depth;
	rotation += lost::deltaTime / 1000.0f * rotationSpeed;

	/* Doesn't make sense with logic, and ruins scrolling vibes
	renderPosition = position - (lost::globalCamera.getCenter()) / depth;

	float xPosInt = 0;
	float yPosInt = 0;

	float xPosFrac = modf(renderPosition.x / (sapp_widthf() + 20.0f), &xPosInt);
	float yPosFrac = modf(renderPosition.y / (sapp_heightf() + 20.0f), &yPosInt);

	renderPosition.x = fmodf(renderPosition.x + (-xPosInt + 1) * (sapp_widthf() + 20.0f), (sapp_widthf() + 20.0f)) - 10.0f;
	renderPosition.y = fmodf(renderPosition.y + (-yPosInt + 1) * (sapp_heightf() + 20.0f), (sapp_heightf() + 20.0f)) - 10.0f;
	*/

	if (position.x > sapp_widthf())
		position.x = -10.0f;
	if (position.x < -10.0f)
		position.x = sapp_widthf();
	if (position.y > sapp_heightf())
		position.y = -10.0f;
	if (position.y < -10.0f)
		position.y = sapp_heightf();

	renderPosition = position;
}
