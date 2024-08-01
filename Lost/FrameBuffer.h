#pragma once
#include "../SokolReference.h"

namespace lost
{

	class FrameBuffer
	{
	public:
		// Initlizes the frame buffer, if no inputs are given frame buffer is set to render window size
		FrameBuffer(int width = -1, int height = -1);
		~FrameBuffer();

		void bind();
		void unbind();

		sg_image& getBuffer();

	private:
		sg_attachments m_BufferAttachments;
		sg_image m_ColorBuffer;
		sg_image m_DepthBuffer;
		sg_sampler m_Sampler;

		int m_Width = 0;
		int m_Height = 0;
	};

}
