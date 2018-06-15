/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <pluginface.hpp>


namespace xmax
{
	using xmaxapp::OptionsDesc;
	using xmaxapp::VarsMap;

	class BlockBuilderImpl;
	class BlockBuilderPlugin : public xmaxapp::PluginFace
	{
		GENERATED_PLUGIN(BlockBuilderPlugin, xmaxapp::PluginFace, &InitOptions)
	public:
		BlockBuilderPlugin();

		virtual void Initialize(const VarsMap& options) override;
		virtual void Startup() override;

	protected:

		virtual void OnCreated() override;

		static void InitOptions(OptionsDesc& cli, OptionsDesc& cfg);


		std::unique_ptr<BlockBuilderImpl> impl_;
	};
}
