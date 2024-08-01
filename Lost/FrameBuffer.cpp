#include "FrameBuffer.h"
#include "../SokolReference.h"

namespace lost
{

	FrameBuffer::FrameBuffer(int width, int height)
	{
		sg_sampler_desc linear_sampler_desc = {};
		linear_sampler_desc.min_filter = SG_FILTER_LINEAR;
		linear_sampler_desc.mag_filter = SG_FILTER_LINEAR;
		linear_sampler_desc.wrap_u = SG_WRAP_CLAMP_TO_EDGE;
		linear_sampler_desc.wrap_v = SG_WRAP_CLAMP_TO_EDGE;
		m_Sampler = sg_make_sampler(&linear_sampler_desc);

		m_Width = width == -1 ? sapp_width() : width;
		m_Height = height == -1 ? sapp_height() : height;


		sg_image_desc color_buffer_desc = {};
		color_buffer_desc.render_target = true;
		color_buffer_desc.width = m_Width;
		color_buffer_desc.height = m_Height;
		m_ColorBuffer = sg_make_image(&color_buffer_desc);

		sg_image_desc depth_buffer_desc = {};
		depth_buffer_desc.render_target = true;
		depth_buffer_desc.width = m_Width;
		depth_buffer_desc.height = m_Height;
		depth_buffer_desc.pixel_format = SG_PIXELFORMAT_DEPTH_STENCIL;
		m_DepthBuffer = sg_make_image(&depth_buffer_desc);


		sg_attachments_desc attachments_desc = {};
		attachments_desc.colors->image = m_ColorBuffer;
		attachments_desc.depth_stencil.image = m_DepthBuffer;
		m_BufferAttachments = sg_make_attachments(attachments_desc);
	}

	FrameBuffer::~FrameBuffer()
	{
		sg_destroy_sampler(m_Sampler);
		sg_destroy_image(m_ColorBuffer);
		sg_destroy_image(m_DepthBuffer);
		sg_destroy_attachments(m_BufferAttachments);
	}

	void FrameBuffer::bind()
	{
		sg_pass_action pass_action = { };
		pass_action.colors[0].load_action = SG_LOADACTION_CLEAR;
		pass_action.colors[0].clear_value.r = 0.0f;
		pass_action.colors[0].clear_value.g = 0.0f;
		pass_action.colors[0].clear_value.b = 0.0f;
		pass_action.colors[0].clear_value.a = 0.0f;

		sg_pass pass = { };
		pass.action = pass_action;
		pass.attachments = m_BufferAttachments;
		sg_begin_pass(&pass);

		sgp_begin(m_Width, m_Height);
		// Set frame buffer drawing region to (0,0,width,height).
		sgp_viewport(0, 0, m_Width, m_Height);
		sgp_project(0, m_Width, m_Height, 0);
	}

	void FrameBuffer::unbind()
	{
		sgp_flush();
		sgp_end();
		sg_end_pass();
	}

	sg_image& FrameBuffer::getBuffer()
	{
		return m_ColorBuffer;
	}

}