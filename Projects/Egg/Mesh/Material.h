#pragma once
#include <map>
#include <vector>
#include "ShaderStageFlag.h"
#include "Shader.h"

namespace Egg { namespace Mesh
{
	GG_CLASS(Material)
		Microsoft::WRL::ComPtr<ID3DBlob> inputSignature;

		std::map<ShaderStageFlag, Shader::P> shaders;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencilState;
		Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;

		std::map<ShaderStageFlag, std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer	>	>	> cbs;
		std::map<ShaderStageFlag, std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState	>	>	> sss;
		std::map<ShaderStageFlag, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView	>	>	> srs;

	protected:
		Material();
	public:
		~Material();
		
		void setShader(ShaderStageFlag stage, Shader::P shader);

		void setCb(std::string name, Microsoft::WRL::ComPtr<ID3D11Buffer> cb, ShaderStageFlag stage = 
			MetaEnumerations::Flags::of(ShaderStageFlag::Compute, ShaderStageFlag::Domain, ShaderStageFlag::Geometry, ShaderStageFlag::Hull, ShaderStageFlag::Pixel, ShaderStageFlag::Vertex));
		void setCb(ShaderStageFlag stage, uint index, Microsoft::WRL::ComPtr<ID3D11Buffer> cb)
		{
			BOOST_FOREACH(ShaderStageFlag stage, MetaEnumerations::Flags::decompose(stage))
			{
				auto& shaderCbs = cbs[stage];
				if (shaderCbs.size() <= index) {
					shaderCbs.resize(index + 1);
				}	
				shaderCbs.at(index) = cb;
			}
		}

		void setSamplerState(std::string name, Microsoft::WRL::ComPtr<ID3D11SamplerState> ss, ShaderStageFlag stage =
			MetaEnumerations::Flags::of(ShaderStageFlag::Compute, ShaderStageFlag::Domain, ShaderStageFlag::Geometry, ShaderStageFlag::Hull, ShaderStageFlag::Pixel, ShaderStageFlag::Vertex));

		void setSamplerState(ShaderStageFlag stage, uint index, Microsoft::WRL::ComPtr<ID3D11SamplerState> ss)
		{
			BOOST_FOREACH(ShaderStageFlag stage, MetaEnumerations::Flags::decompose(stage))
			{
				auto& shaderSss = sss[stage];
				if (shaderSss.size() <= index) {
					shaderSss.resize(index + 1);
				}
				shaderSss.at(index) = ss;
			}
		}

		void setShaderResource(std::string name, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sr, ShaderStageFlag stage =
			MetaEnumerations::Flags::of(ShaderStageFlag::Compute, ShaderStageFlag::Domain, ShaderStageFlag::Geometry, ShaderStageFlag::Hull, ShaderStageFlag::Pixel, ShaderStageFlag::Vertex));

		void setShaderResource(ShaderStageFlag stage, uint index, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> sr)
		{
			BOOST_FOREACH(ShaderStageFlag stage, MetaEnumerations::Flags::decompose(stage))
			{
				auto& shaderSrs = srs[stage];
				if (shaderSrs.size() <= index) {
					shaderSrs.resize(index + 1);
				}
				shaderSrs.at(index) = sr;
			}
		}

		void apply(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);

	GG_ENDCLASS

}} // namespace Egg::Mesh