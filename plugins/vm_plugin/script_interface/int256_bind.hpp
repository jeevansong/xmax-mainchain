//@file
//@copyright defined in xmax/LICENSE
#pragma once
#include <functional>
#include <map>
#include <libplatform/libplatform.h>
#include <v8.h>
#include "V8BindObject.hpp"
#include "pro/types/generictypes.hpp"

namespace xmax {
	namespace scriptv8 {
		class V8i256 : public V8BindObject<V8i256>
		{
		public:
			V8i256():value_(){}
			~V8i256() {}
			static V8i256* NewV8CppObj(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void RegisterWithV8(v8::Isolate* isolate, v8::Handle<v8::ObjectTemplate> global);
			static void ConstructV8Object(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void WeakExternalReferenceCallback(const v8::WeakCallbackInfo<V8i256>& data);

			static constexpr const char* TypeName() {
				return "V8i256";
			}
		private:
			pro::int256 value_;
		};


	}
}