#include "Shader.h"

Shader::Shader()
{
}

Shader::~Shader()
{
	delete m_ShaderDescriptor;

	sg_destroy_shader(shader);
	sg_destroy_pipeline(pipeline);

	if (m_Uniforms)
		delete[] m_Uniforms;
}

void Shader::loadShader(const char* vertexShaderLoc, const char* fragmentShaderLoc, const char* _name)
{
	std::string vertexShaderCode = read_UTF8_file(vertexShaderLoc);
	std::string fragmentShaderCode = read_UTF8_file(fragmentShaderLoc);
	name = _name;

	m_CreateDescriptor(vertexShaderCode.c_str(), fragmentShaderCode.c_str());
	m_CreateShader();
}

void Shader::bindShader()
{
	sgp_set_pipeline(pipeline);
	sgp_set_uniform(m_Uniforms, m_UniformCount * sizeof(float));
}

void Shader::setUniform(int slot, float val)
{
	m_Uniforms[slot] = val;
}

sg_shader_desc* Shader::m_CreateDescriptor(const char* vertexShaderCode, const char* fragmentShaderCode)
{
	sg_shader_desc* desc = new sg_shader_desc();

	std::string fragmentCode = fragmentShaderCode;
	size_t uniformsLoc = fragmentCode.find("uniforms", 0);
	int count = 0;
	if (uniformsLoc != std::string::npos)
		count = fragmentCode.at(uniformsLoc + 9) - '0';

	// Create uniforms
	m_UniformCount = count * 4;
	m_Uniforms = new float[m_UniformCount];
	for (int i = 0; i < m_UniformCount; i++)
		m_Uniforms[i] = 0.0f;

	// Vertex Attribute init
	desc->attrs[0].name = "coord";
	desc->attrs[1].name = "color";
	// Code init
	desc->vs.source = vertexShaderCode;
	desc->vs.entry = "main";
	desc->fs.source = fragmentShaderCode;
	desc->fs.entry = "main";
	// Uniform Buffer
	if (count > 0)
	{
		desc->fs.uniform_blocks[0].size = count * 16; // Size of uniform block in bytes it seems, there are 8 floats, floats take 4 bytes
		desc->fs.uniform_blocks[0].layout = SG_UNIFORMLAYOUT_STD140;
		desc->fs.uniform_blocks[0].uniforms[0].name = "uniforms";
		desc->fs.uniform_blocks[0].uniforms[0].type = SG_UNIFORMTYPE_FLOAT4;
		desc->fs.uniform_blocks[0].uniforms[0].array_count = count;
	}
	// Sampler and texture2d init
	desc->fs.images[0].used = true;
	desc->fs.images[0].multisampled = false;
	desc->fs.images[0].image_type = SG_IMAGETYPE_2D;
	desc->fs.images[0].sample_type = SG_IMAGESAMPLETYPE_FLOAT;
	desc->fs.samplers[0].used = true;
	desc->fs.samplers[0].sampler_type = SG_SAMPLERTYPE_FILTERING;
	desc->fs.image_sampler_pairs[0].used = true;
	desc->fs.image_sampler_pairs[0].image_slot = 0;
	desc->fs.image_sampler_pairs[0].sampler_slot = 0;
	desc->fs.image_sampler_pairs[0].glsl_name = "samplerChannel0";
	// Shader name
	desc->label = name.c_str();

	m_ShaderDescriptor = desc;
	return desc;
}

void Shader::m_CreateShader()
{
	shader = sg_make_shader(m_ShaderDescriptor);
	if (sg_query_shader_state(shader) != SG_RESOURCESTATE_VALID)
		fprintf(stderr, "failed to make custom pipeline shader\n");

	sgp_pipeline_desc pipedesc = {};
	pipedesc.shader = shader;
	pipedesc.has_vs_color = true;
	pipedesc.blend_mode = SGP_BLENDMODE_BLEND;

	pipeline = sgp_make_pipeline(&pipedesc);
	if (sg_query_pipeline_state(pipeline) != SG_RESOURCESTATE_VALID)
		fprintf(stderr, "failed to make custom pipeline shader\n");
}
