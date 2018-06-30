/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <map>
#include <pluginface.hpp>
#include <filesystem>

//namespace boost {
//	namespace asio {
//
//#if !defined(BOOST_ASIO_NO_DEPRECATED)
//		/// Typedef for backwards compatibility.
//		class io_context;
//		typedef io_context io_service;
//#endif // !defined(BOOST_ASIO_NO_DEPRECATED)
//
//	} // namespace asio
//} // namespace boost

namespace fs = std::filesystem;

namespace xmaxapp
{
	/**
	*  derive class of ApplicationBase
	*  implement the concrete Initialization
	*/
	class Application : public ApplicationBase
	{
	public:
		Application();
		virtual ~Application();
		/**
		* return AppService
		*/
		AppService* GetService() const override;
		/**
		* Init a concrete plugin
		* @param[in]	name of a plugin
		*/
		void PluginToInit(const string& plugin_name);
		/**
		* entry point of whole app
		* @param[in]	argc	number of args
		* @param[in]	argv	the content of params
		*/
		void Initialize(int argc, char** argv);
		/**
		*	start the app
		*/
		void Startup();
		/**
		*	shutdown the app
		*/
		void Shutdown();
		/**
		*	Main loop of the logic
		*/
		void Loop();
		/**
		*	quit the app
		*/
		void Quit();


		//==============Utility methods ============
		void SetDefaultConfigFilePath(const fs::path& path) { cfg_file_path_ = path; }
	

	private:
		void LoadCfgOptions();

	private:

		std::map<string, std::unique_ptr<PluginFace>>	pluginmap_;
		std::vector<PluginFace*>						initialized_plugins_;
		std::vector<PluginFace*>						startup_plugins_;

		OptionsDesc     app_options_;
		OptionsDesc     cfg_options_;

		fs::path cfg_file_path_;		

		std::unique_ptr<AppService>  service_face_;
	};


}
