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


#pragma once

#include "comms/CompileControl.h"

CC_DISABLE_WARNINGS()
#include <QtCore/QVariantMap>
CC_ENABLE_WARNINGS()

namespace mqttsn
{

namespace cc_plugin
{

namespace protocol
{

namespace field
{

QVariantMap createProps_gwId();
QVariantMap createProps_duration();
QVariantMap createProps_durationOpt();
QVariantMap createProps_radius();
QVariantMap createProps_gwAdd();
QVariantMap createProps_flags();
QVariantMap createProps_flagsOpt();
QVariantMap createProps_protocolId();
QVariantMap createProps_clientId();
QVariantMap createProps_returnCode();
QVariantMap createProps_willTopic();
QVariantMap createProps_willMsg();
QVariantMap createProps_topicId();
QVariantMap createProps_topicIdOpt();
QVariantMap createProps_msgId();
QVariantMap createProps_topicName();
QVariantMap createProps_topicNameOpt();
QVariantMap createProps_data();

}  // namespace field

}  // namespace protocol

}  // namespace cc_plugin

}  // namespace mqttsn


