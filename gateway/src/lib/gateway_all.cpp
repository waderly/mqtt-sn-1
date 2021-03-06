//
// Copyright 2016 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "mqttsn/gateway/gateway_all.h"

#include <sstream>
#include <fstream>
#include <algorithm>
#include <limits>

#include "mqttsn/gateway/gateway_allpp.h"

namespace
{

typedef mqttsn::gateway::Config Config;
typedef mqttsn::gateway::Gateway Gateway;
typedef mqttsn::gateway::Session Session;

}  // namespace

/*===================== Gateway Object ======================*/

MqttsnGatewayHandle mqttsn_gw_alloc(void)
{
    MqttsnGatewayHandle gw;
    gw.obj = new Gateway();
    return gw;
}

void mqttsn_gw_free(MqttsnGatewayHandle gw)
{
    std::unique_ptr<Gateway>(reinterpret_cast<Gateway*>(gw.obj));
}

void mqttsn_gw_set_advertise_period(
    MqttsnGatewayHandle gw,
    unsigned short value)
{
    if (gw.obj == nullptr) {
        return;
    }

    reinterpret_cast<Gateway*>(gw.obj)->setAdvertisePeriod(value);
}

void mqttsn_gw_set_id(MqttsnGatewayHandle gw, unsigned char id)
{
    if (gw.obj == nullptr) {
        return;
    }

    reinterpret_cast<Gateway*>(gw.obj)->setGatewayId(id);
}

void mqttsn_gw_set_tick_req_cb(MqttsnGatewayHandle gw, MqttsnGwTickReqCb cb, void* data)
{
    if ((gw.obj == nullptr) || (cb == nullptr)) {
        return;
    }

    reinterpret_cast<Gateway*>(gw.obj)->setNextTickProgramReqCb(
        [cb, data](unsigned duration)
        {
            cb(data, duration);
        });
}

void mqttsn_gw_set_advertise_broadcast_req_cb(
    MqttsnGatewayHandle gw,
    MqttsnGwBroadcastReqCb cb,
    void* data)
{
    if ((gw.obj == nullptr) || (cb == nullptr)) {
        return;
    }

    reinterpret_cast<Gateway*>(gw.obj)->setSendDataReqCb(
        [cb, data](const unsigned char* buf, unsigned bufLen)
        {
            cb(data, buf, bufLen);
        });
}

bool mqttsn_gw_start(MqttsnGatewayHandle gw)
{
    if (gw.obj == nullptr) {
        return false;
    }

    return reinterpret_cast<Gateway*>(gw.obj)->start();
}

void mqttsn_gw_stop(MqttsnGatewayHandle gw)
{
    if (gw.obj == nullptr) {
        return;
    }

    reinterpret_cast<Gateway*>(gw.obj)->stop();
}

void mqttsn_gw_tick(MqttsnGatewayHandle gw)
{
    if (gw.obj == nullptr) {
        return;
    }

    reinterpret_cast<Gateway*>(gw.obj)->tick();
}

/*===================== Session Object ======================*/


MqttsnSessionHandle mqttsn_gw_session_alloc(void)
{
    MqttsnSessionHandle session;
    session.obj = new Session;
    return session;
}

void mqttsn_gw_session_free(MqttsnSessionHandle session)
{
    std::unique_ptr<Session>(reinterpret_cast<Session*>(session.obj));
}

void mqttsn_gw_session_set_tick_req_cb(
    MqttsnSessionHandle session,
    MqttsnSessionTickReqCb cb,
    void* data)
{
    if ((session.obj == nullptr) || (cb == nullptr)) {
        return;
    }

    reinterpret_cast<Session*>(session.obj)->setNextTickProgramReqCb(
        [cb, data](unsigned duration)
        {
            cb(data, duration);
        });
}

void mqttsn_gw_session_set_cancel_tick_cb(
    MqttsnSessionHandle session,
    MqttsnSessionCancelTickReqCb cb,
    void* data)
{
    if ((session.obj == nullptr) || (cb == nullptr)) {
        return;
    }

    reinterpret_cast<Session*>(session.obj)->setCancelTickWaitReqCb(
        [cb, data]() -> unsigned
        {
            return cb(data);
        });
}

void mqttsn_gw_session_set_send_data_to_client_cb(
    MqttsnSessionHandle session,
    MqttsnSessionSendDataReqCb cb,
    void* data)
{
    if ((session.obj == nullptr) || (cb == nullptr)) {
        return;
    }

    reinterpret_cast<Session*>(session.obj)->setSendDataClientReqCb(
        [cb, data](const std::uint8_t* buf, std::size_t bufLen)
        {
            cb(data, buf, static_cast<unsigned>(bufLen));
        });
}


void mqttsn_gw_session_set_send_data_to_broker_cb(
    MqttsnSessionHandle session,
    MqttsnSessionSendDataReqCb cb,
    void* data)
{
    if ((session.obj == nullptr) || (cb == nullptr)) {
        return;
    }

    reinterpret_cast<Session*>(session.obj)->setSendDataBrokerReqCb(
        [cb, data](const std::uint8_t* buf, std::size_t bufLen)
        {
            cb(data, buf, static_cast<unsigned>(bufLen));
        });
}

void mqttsn_gw_session_set_term_req_cb(
    MqttsnSessionHandle session,
    MqttsnSessionTermReqCb cb,
    void* data)
{
    if ((session.obj == nullptr) || (cb == nullptr)) {
        return;
    }

    reinterpret_cast<Session*>(session.obj)->setTerminationReqCb(
        [cb, data]()
        {
            cb(data);
        });
}

void mqttsn_gw_session_set_broker_reconnect_req_cb(
    MqttsnSessionHandle session,
    MqttsnSessionBrokerReconnectReqCb cb,
    void* data)
{
    if ((session.obj == nullptr) || (cb == nullptr)) {
        return;
    }

    reinterpret_cast<Session*>(session.obj)->setBrokerReconnectReqCb(
        [cb, data]()
        {
            cb(data);
        });
}

void mqttsn_gw_session_set_client_connect_report_cb(
    MqttsnSessionHandle session,
    MqttsnSessionClientConnectReportCb cb,
    void* data)
{
    if (session.obj == nullptr) {
        return;
    }

    if (cb == nullptr) {
        reinterpret_cast<Session*>(session.obj)->setClientConnectedReportCb(nullptr);
        return;
    }

    reinterpret_cast<Session*>(session.obj)->setClientConnectedReportCb(
        [cb, data](const std::string& clientId)
        {
            cb(data, clientId.c_str());
        });
}

void mqttsn_gw_session_set_auth_info_req_cb(
    MqttsnSessionHandle session,
    MqttsnSessionAuthInfoReqCb cb,
    void* data)
{
    if (session.obj == nullptr) {
        return;
    }

    if (cb == nullptr) {
        reinterpret_cast<Session*>(session.obj)->setAuthInfoReqCb(nullptr);
        return;
    }

    reinterpret_cast<Session*>(session.obj)->setAuthInfoReqCb(
        [cb, data](const std::string& clientId) -> Session::AuthInfo
        {
            const char* username = nullptr;
            const std::uint8_t* password = nullptr;
            unsigned passLen = 0U;
            cb(data, clientId.c_str(), &username, &password, &passLen);

            Session::AuthInfo info;
            if (username != nullptr) {
                info.first = username;
            }

            if ((password != nullptr) && (0U < passLen)) {
                info.second.assign(password, password + passLen);
            }

            return info;
        });
}

void mqttsn_gw_session_set_id(MqttsnSessionHandle session, unsigned char id)
{
    if (session.obj == nullptr) {
        return;
    }

    reinterpret_cast<Session*>(session.obj)->setGatewayId(id);
}

void mqttsn_gw_session_set_retry_period(MqttsnSessionHandle session, unsigned value)
{
    if (session.obj == nullptr) {
        return;
    }

    reinterpret_cast<Session*>(session.obj)->setRetryPeriod(value);
}

void mqttsn_gw_session_set_retry_count(MqttsnSessionHandle session, unsigned value)
{
    if (session.obj == nullptr) {
        return;
    }

    reinterpret_cast<Session*>(session.obj)->setRetryCount(value);
}

void mqttsn_gw_session_set_sleeping_client_msg_limit(
    MqttsnSessionHandle session,
    unsigned value)
{
    if (session.obj == nullptr) {
        return;
    }

    reinterpret_cast<Session*>(session.obj)->setSleepingClientMsgLimit(value);
}

void mqttsn_gw_session_set_default_client_id(MqttsnSessionHandle session, const char* clientId)
{
    if (session.obj == nullptr) {
        return;
    }

    reinterpret_cast<Session*>(session.obj)->setDefaultClientId(clientId);
}

void mqttsn_gw_session_set_pub_only_keep_alive(
    MqttsnSessionHandle session,
    unsigned value)
{
    if (session.obj == nullptr) {
        return;
    }

    reinterpret_cast<Session*>(session.obj)->setPubOnlyKeepAlive(value);
}

bool mqttsn_gw_session_start(MqttsnSessionHandle session)
{
    if (session.obj == nullptr) {
        return false;
    }

    return reinterpret_cast<Session*>(session.obj)->start();
}

void mqttsn_gw_session_stop(MqttsnSessionHandle session)
{
    if (session.obj == nullptr) {
        return;
    }

    reinterpret_cast<Session*>(session.obj)->stop();
}

void mqttsn_gw_session_tick(MqttsnSessionHandle session)
{
    if (session.obj == nullptr) {
        return;
    }

    reinterpret_cast<Session*>(session.obj)->tick();
}

unsigned mqttsn_gw_session_data_from_client(
    MqttsnSessionHandle session,
    const unsigned char* buf,
    unsigned bufLen)
{
    if (session.obj == nullptr) {
        return 0U;
    }

    return static_cast<unsigned>(
        reinterpret_cast<Session*>(session.obj)->dataFromClient(buf, bufLen));

}

unsigned mqttsn_gw_session_data_from_broker(
    MqttsnSessionHandle session,
    const unsigned char* buf,
    unsigned bufLen)
{
    if (session.obj == nullptr) {
        return 0U;
    }

    return static_cast<unsigned>(
        reinterpret_cast<Session*>(session.obj)->dataFromBroker(buf, bufLen));

}

void mqttsn_gw_session_broker_connected(MqttsnSessionHandle session, bool connected)
{
    if (session.obj == nullptr) {
        return;
    }

    reinterpret_cast<Session*>(session.obj)->setBrokerConnected(connected);
}

bool mqttsn_gw_session_add_predefined_topic(
    MqttsnSessionHandle session,
    const char* topic,
    unsigned short topicId)
{
    if (session.obj == nullptr) {
        return false;
    }

    return reinterpret_cast<Session*>(session.obj)->addPredefinedTopic(topic, topicId);
}

bool mqttsn_gw_session_set_topic_id_alloc_range(
    MqttsnSessionHandle session,
    unsigned short minTopicId,
    unsigned short maxTopicId)
{
    if (session.obj == nullptr) {
        return false;
    }

    return reinterpret_cast<Session*>(session.obj)->setTopicIdAllocationRange(minTopicId, maxTopicId);
}

/*===================== Config Object ======================*/

MqttsnConfigHandle mqttsn_gw_config_alloc(void)
{
    MqttsnConfigHandle config;
    config.obj = new Config;
    return config;
}

void mqttsn_gw_config_free(MqttsnConfigHandle config)
{
    std::unique_ptr<Config>(reinterpret_cast<Config*>(config.obj));
}

void mqttsn_gw_config_parse(MqttsnConfigHandle config, const char* str)
{
    if (config.obj == nullptr) {
        return;
    }

    std::stringstream stream;
    stream << str;
    reinterpret_cast<Config*>(config.obj)->read(stream);
}

bool mqttsn_gw_config_read(MqttsnConfigHandle config, const char* filename)
{
    if (config.obj == nullptr) {
        return false;
    }

    std::ifstream stream(filename, std::ios_base::in);
    if (!stream) {
        return false;
    }

    reinterpret_cast<Config*>(config.obj)->read(stream);
    return true;
}

unsigned char mqttsn_gw_config_id(MqttsnConfigHandle config)
{
    if (config.obj == nullptr) {
        return 0U;
    }

    return reinterpret_cast<const Config*>(config.obj)->gatewayId();
}

unsigned short mqttsn_gw_config_advertise_period(MqttsnConfigHandle config)
{
    if (config.obj == nullptr) {
        return 0U;
    }

    return reinterpret_cast<const Config*>(config.obj)->advertisePeriod();
}

unsigned mqttsn_gw_config_retry_period(MqttsnConfigHandle config)
{
    if (config.obj == nullptr) {
        return 0U;
    }

    return reinterpret_cast<const Config*>(config.obj)->retryPeriod();
}

unsigned mqttsn_gw_config_retry_count(MqttsnConfigHandle config)
{
    if (config.obj == nullptr) {
        return 0U;
    }

    return reinterpret_cast<const Config*>(config.obj)->retryCount();
}

const char* mqttsn_gw_config_default_client_id(MqttsnConfigHandle config)
{
    if (config.obj == nullptr) {
        return nullptr;
    }

    return reinterpret_cast<const Config*>(config.obj)->defaultClientId().c_str();
}

unsigned mqttsn_gw_config_pub_only_keep_alive(MqttsnConfigHandle config)
{
    if (config.obj == nullptr) {
        return 0U;
    }

    return reinterpret_cast<const Config*>(config.obj)->pubOnlyKeepAlive();
}

unsigned mqttsn_gw_config_sleeping_client_msg_limit(MqttsnConfigHandle config)
{
    if (config.obj == nullptr) {
        return std::numeric_limits<unsigned>::max();
    }

    return static_cast<unsigned>(
        std::max(
            reinterpret_cast<const Config*>(config.obj)->sleepingClientMsgLimit(),
            static_cast<std::size_t>(std::numeric_limits<unsigned>::max())));
}

unsigned mqttsn_gw_config_available_predefined_topics(MqttsnConfigHandle config)
{
    if (config.obj == nullptr) {
        return 0U;
    }

    return static_cast<unsigned>(
        reinterpret_cast<const Config*>(config.obj)->predefinedTopics().size());
}

unsigned mqttsn_gw_config_get_predefined_topics(
    MqttsnConfigHandle config,
    MqttsnPredefinedTopicInfo* buf,
    unsigned bufLen)
{
    if (config.obj == nullptr) {
        return 0U;
    }

    auto& predefinedTopics = reinterpret_cast<const Config*>(config.obj)->predefinedTopics();
    auto total = std::min(static_cast<unsigned>(predefinedTopics.size()), bufLen);

    std::transform(
        predefinedTopics.begin(), predefinedTopics.begin() + total, buf,
        [](const mqttsn::gateway::Config::PredefinedTopicInfo& info) -> MqttsnPredefinedTopicInfo
        {
            MqttsnPredefinedTopicInfo retInfo;
            retInfo.clientId = info.clientId.c_str();
            retInfo.topic = info.topic.c_str();
            retInfo.topicId = info.topicId;
            return retInfo;
        });
    return total;
}

unsigned mqttsn_gw_config_available_auth_infos(MqttsnConfigHandle config)
{
    if (config.obj == nullptr) {
        return 0U;
    }

    return static_cast<unsigned>(
        reinterpret_cast<const Config*>(config.obj)->authInfos().size());
}

unsigned mqttsn_gw_config_get_auth_infos(
    MqttsnConfigHandle config,
    MqttsnAuthInfo* buf,
    unsigned bufLen)
{
    if (config.obj == nullptr) {
        return 0U;
    }

    auto& authInfos = reinterpret_cast<const Config*>(config.obj)->authInfos();
    auto total = std::min(static_cast<unsigned>(authInfos.size()), bufLen);

    std::transform(
        authInfos.begin(), authInfos.begin() + total, buf,
        [](const mqttsn::gateway::Config::AuthInfo& info) -> MqttsnAuthInfo
        {
            MqttsnAuthInfo retInfo;
            retInfo.clientId = info.clientId.c_str();
            retInfo.username = info.username.c_str();
            retInfo.password = info.password.c_str();
            return retInfo;
        });
    return total;
}

void mqttsn_gw_config_topic_id_alloc_range(
    MqttsnConfigHandle config,
    unsigned short* min,
    unsigned short* max)
{
    if (config.obj == nullptr) {
        return;
    }

    auto range = reinterpret_cast<const Config*>(config.obj)->topicIdAllocRange();
    if (min != nullptr) {
        *min = range.first;
    }

    if (max != nullptr) {
        *max = range.second;
    }
}

const char* mqttsn_gw_config_broker_address(MqttsnConfigHandle config)
{
    if (config.obj == nullptr) {
        return nullptr;
    }

    return reinterpret_cast<const Config*>(config.obj)->brokerTcpHostAddress().c_str();
}

unsigned short mqttsn_gw_config_broker_port(MqttsnConfigHandle config)
{
    if (config.obj == nullptr) {
        return 0U;
    }

    return reinterpret_cast<const Config*>(config.obj)->brokerTcpHostPort();
}

unsigned mqttsn_gw_config_values_count(MqttsnConfigHandle config, const char* key)
{
    if (config.obj == nullptr) {
        return 0U;
    }

    auto& map = reinterpret_cast<const Config*>(config.obj)->configMap();
    auto range = map.equal_range(key);
    return static_cast<unsigned>(std::distance(range.first, range.second));
}

const char* mqttsn_gw_config_get_value(MqttsnConfigHandle config, const char* key, unsigned idx)
{
    if (config.obj == nullptr) {
        return 0U;
    }

    auto& map = reinterpret_cast<const Config*>(config.obj)->configMap();
    auto range = map.equal_range(key);

    unsigned count = 0;
    for (auto iter = range.first; iter != range.second; ++iter) {
        if (count == idx) {
            return iter->second.c_str();
        }

        ++count;
    }
    return nullptr;
}

