/**
* If not stated otherwise in this file or this component's LICENSE
* file the following copyright and licenses apply:
*
* Copyright 2019 RDK Management
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
**/

#pragma once

//#include "Module.h"

namespace WPEFramework {

    namespace Plugin {

		// This is a server for a JSONRPC communication channel.
		// For a plugin to be capable to handle JSONRPC, inherit from PluginHost::JSONRPC.
		// By inheriting from this class, the plugin realizes the interface PluginHost::IDispatcher.
		// This realization of this interface implements, by default, the following methods on this plugin
		// - exists
		// - register
		// - unregister
		// Any other methood to be handled by this plugin  can be added can be added by using the
		// templated methods Register on the PluginHost::JSONRPC class.
		// As the registration/unregistration of notifications is realized by the class PluginHost::JSONRPC,
		// this class exposes a public method called, Notify(), using this methods, all subscribed clients
		// will receive a JSONRPC message as a notification, in case this method is called.
        class AbstractPlugin : public PluginHost::IPlugin, public PluginHost::JSONRPC {
        private:

            // We do not allow this plugin to be copied !!
            AbstractPlugin(const AbstractPlugin&) = delete;
            AbstractPlugin& operator=(const AbstractPlugin&) = delete;

            //Begin methods
            virtual uint32_t getQuirks(const JsonObject& parameters, JsonObject& response)
            {
                JsonArray array;
                response["quirks"] = array;
                returnResponse(true);
            }
            //End methods

        protected:
            template <typename METHOD, typename REALOBJECT>
            void registerMethod(const string& methodName, const METHOD& method, REALOBJECT* objectPtr)
            {
                Register(methodName, method, objectPtr);
                m_registeredMethods.push_back(methodName);
            }

        public:
            AbstractPlugin() : PluginHost::JSONRPC()
            {
                LOGINFO();

                registerMethod("getQuirks", &AbstractPlugin::getQuirks, this);
            }
            
            AbstractPlugin(const std::vector<uint8_t> versions) : PluginHost::JSONRPC(versions)
            {
                LOGINFO();

                registerMethod("getQuirks", &AbstractPlugin::getQuirks, this);
            }

            virtual ~AbstractPlugin()
            {
                LOGINFO();

                for (auto& i : m_registeredMethods) { Unregister(i); }
            }

            //Build QueryInterface implementation, specifying all possible interfaces to be returned.
            BEGIN_INTERFACE_MAP(MODULE_NAME)
            INTERFACE_ENTRY(PluginHost::IPlugin)
            INTERFACE_ENTRY(PluginHost::IDispatcher)
            END_INTERFACE_MAP

            //IPlugin methods
            virtual const string Initialize(PluginHost::IShell* service)
            {
                LOGINFO();
                // On success return empty, to indicate there is no error text.
                return(string());
            }

            virtual void Deinitialize(PluginHost::IShell* service)
            {
                LOGINFO();
            }

            virtual string Information() const
            {
                // No additional info to report.
                return(string());
            }
        private:
            std::vector<std::string> m_registeredMethods;
        };
	} // namespace Plugin
} // namespace WPEFramework
