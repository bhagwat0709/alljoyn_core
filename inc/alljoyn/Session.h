#ifndef _ALLJOYN_SESSION_H
#define _ALLJOYN_SESSION_H
/**
 * @file
 * AllJoyn session related data types.
 */

/******************************************************************************
 * Copyright 2011-2013, Qualcomm Innovation Center, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 ******************************************************************************/

#include <qcc/platform.h>
#include <alljoyn/TransportMask.h>

namespace ajn {

/**
 * SessionPort identifies a per-BusAttachment receiver for incoming JoinSession requests.
 * SessionPort values are bound to a BusAttachment when the attachment calls
 * BindSessionPort.
 *
 * NOTE: Valid SessionPort values range from 1 to 0xFFFF.
 */
typedef uint16_t SessionPort;

/** Invalid SessionPort value used to indicate that BindSessionPort should choose any available port */
const SessionPort SESSION_PORT_ANY = 0;

/** SessionId uniquely identifies an AllJoyn session instance */
typedef uint32_t SessionId;

/**
 * SessionOpts contains a set of parameters that define a Session's characteristics.
 */
class SessionOpts {
  public:
    /** Traffic type */
    typedef enum {
        TRAFFIC_MESSAGES       = 0x01,   /**< Session carries message traffic */
        TRAFFIC_RAW_UNRELIABLE = 0x02,   /**< Session carries an unreliable (lossy) byte stream */
        TRAFFIC_RAW_RELIABLE   = 0x04    /**< Session carries a reliable byte stream */
    } TrafficType;
    TrafficType traffic; /**< holds the Traffic type for this SessionOpt*/

    /**
     * Multi-point session capable.
     * A session is multi-point if it can be joined multiple times to form a single
     * session with multi (greater than 2) endpoints. When false, each join attempt
     * creates a new point-to-point session.
     */
    bool isMultipoint;

    /**@name Proximity */
    // {@
    typedef uint8_t Proximity;
    static const Proximity PROXIMITY_ANY      = 0xFF;
    static const Proximity PROXIMITY_PHYSICAL = 0x01;
    static const Proximity PROXIMITY_NETWORK  = 0x02;
    Proximity proximity;
    // @}

    /** Allowed Transports  */
    TransportMask transports;

    /**
     * Construct a SessionOpts with specific parameters.
     *
     * For Windows 8 see also these sample file(s): @n
     * cpp/AllJoynStreaming/src/MediaSink.cc @n
     * cpp/AllJoynStreaming/src/MediaSource.cc @n
     * cpp/AllJoynStreaming/tests/csharp/MediaPlayerApp/MainPage.xaml.cs @n
     * cpp/AllJoynStreaming/tests/csharp/MediaServerApp/MainPage.xaml.cs @n
     * cpp/AllJoynStreaming/tests/javascript/PlayerApp/js/alljoyn.js @n
     * cpp/AllJoynStreaming/tests/javascript/ServerApp/js/alljoyn.js @n
     * cpp/Basic/Basic_Client/BasicClient/MainPage.xaml.cpp @n
     * cpp/Basic/Basic_Service/BasicService/MainPage.xaml.cpp @n
     * cpp/Basic/Name_Change_Client/NameChangeClient/MainPage.xaml.cpp @n
     * cpp/Basic/Signal_Consumer_Client/SignalConsumerClient/MainPage.xaml.cpp @n
     * cpp/Basic/Signal_Service/SignalService/MainPage.xaml.cpp @n
     * cpp/Chat/Chat/MainPage.xaml.cpp @n
     * cpp/Secure/Secure/MainPage.xaml.cpp @n
     * csharp/Basic/Basic_Client/BasicClient/MainPage.xaml.cs @n
     * csharp/Basic/Basic_Service/BasicService/MainPage.xaml.cs @n
     * csharp/Basic/Name_Change_Client/NameChangeClient/MainPage.xaml.cs @n
     * csharp/Basic/Signal_Consumer_Client/SignalConsumerClient/MainPage.xaml.cs @n
     * csharp/Basic/Signal_Service/SignalService/MainPage.xaml.cs @n
     * csharp/BusStress/BusStress/Common/StressOperation.cs @n
     * csharp/chat/chat/MainPage.xaml.cs @n
     * csharp/FileTransfer/Client/Common/FileTransferBusObject.cs @n
     * csharp/Secure/Secure/Common/Client.cs @n
     * csharp/Secure/Secure/Common/Service.cs @n
     * csharp/Sessions/Sessions/MainPage.xaml.cs @n
     * javascript/Basic/Basic_Client/BasicClient/js/BasicClient.js @n
     * javascript/Basic/Basic_Service/BasicService/js/BasicService.js @n
     * javascript/Basic/Name_Change_Client/NameChangeClient/js/NameChangeClient.js @n
     * javascript/Basic/Signal_Consumer_Client/SignalConsumerClient/js/SignalConsumerClient.js @n
     * javascript/Basic/Signal_Service/SignalService/js/SignalService.js @n
     * javascript/chat/chat/js/alljoyn.js @n
     * javascript/README.txt @n
     * javascript/Secure/Secure/js/Client.js @n
     * javascript/Secure/Secure/js/Service.js @n
     *
     * @param traffic       Type of traffic.
     * @param isMultipoint  true iff session supports multipoint (greater than two endpoints).
     * @param proximity     Proximity constraint bitmask.
     * @param transports    Allowed transport types bitmask.
     *
     */
    SessionOpts(SessionOpts::TrafficType traffic, bool isMultipoint, SessionOpts::Proximity proximity, TransportMask transports) :
        traffic(traffic),
        isMultipoint(isMultipoint),
        proximity(proximity),
        transports(transports)
    { }

    /**
     * Construct a default SessionOpts
     *
     * For Windows 8 see also these sample file(s): @n
     * cpp/Chat/Chat/MainPage.xaml.cpp @n
     * csharp/chat/chat/MainPage.xaml.cs @n
     */
    SessionOpts() : traffic(TRAFFIC_MESSAGES), isMultipoint(false), proximity(PROXIMITY_ANY), transports(TRANSPORT_ANY) { }

    /**
     * Determine whether this SessionOpts is compatible with the SessionOpts offered by other
     *
     * Compatibility means that the SessionOpts share at least one of each
     *  - transport type
     *  - traffic type
     *  - proximity type
     *
     * Note that multipoint support is not a condition of compatibility
     *
     * @param other  Options to be compared against this one.
     * @return true iff this SessionOpts can use the option set offered by other.
     */
    bool IsCompatible(const SessionOpts& other) const;

    /**
     * Compare SessionOpts
     *
     * @param other the SessionOpts being compared against
     * @return true if all of the SessionOpts parameters are the same
     *
     */
    bool operator==(const SessionOpts& other) const
    {
        return (traffic == other.traffic) && (isMultipoint == other.isMultipoint) && (proximity == other.proximity) && (transports == other.transports);
    }

    /**
     * Rather arbitrary less-than operator to allow containers holding SessionOpts
     * to be sorted.
     * Traffic takes precedence when sorting SessionOpts.
     *
     * #TRAFFIC_MESSAGES \< #TRAFFIC_RAW_UNRELIABLE \< #TRAFFIC_RAW_RELIABLE
     *
     * If traffic is equal then Proximity takes next level of precedence.
     *
     * PROXIMITY_PHYSICAL \< PROXIMITY_NETWORK \< PROXIMITY_ANY
     *
     * last transports.
     *
     * #TRANSPORT_LOCAL \< #TRANSPORT_BLUETOOTH \< #TRANSPORT_WLAN \< #TRANSPORT_WWAN \< #TRANSPORT_ANY
     *
     *
     * @param other the SessionOpts being compared against
     * @return true if this SessionOpts is designated as less than the SessionOpts
     *         being compared against.
     */
    bool operator<(const SessionOpts& other) const
    {
        if ((traffic < other.traffic) ||
            ((traffic == other.traffic) && !isMultipoint && other.isMultipoint) ||
            ((traffic == other.traffic) && (isMultipoint == other.isMultipoint) && (proximity < other.proximity)) ||
            ((traffic == other.traffic) && (isMultipoint == other.isMultipoint) && (proximity == other.proximity) && (transports < other.transports))) {
            return true;
        }
        return false;
    }
};


}

#endif
