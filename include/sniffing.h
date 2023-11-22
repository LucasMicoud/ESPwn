#ifndef SNIFFING
#define SNIFFING

#include <map>

#include "esp_wifi.h"
#include "inttypes.h"
#include "serialUtil.h"

#include "wifiUtil.h"

/* Operating mode of the ESP. 
 * false : target discovery mode.
 * true : attack (capture) mode.
 */
extern bool attackMode;

/* Active or passive mode.
 * false : deauth will only target selected AP.
 * true : deauth will target every AP.
 */
extern bool activeMode;

/* Current channel used by the ESP.
*/
extern uint8_t channel;

/* Map of registered WIFI APs, keyed by their SSID.
 */
extern std::map<String, wifi_AP*> discoveredWifiAPs;

/* Sets up the pcap file by writing file header.
 */
void setupPCAP();

/* Sniffing callback function. Gets call for every packet received.
 */ 
void promiscuousModeCallBack(void* buf, wifi_promiscuous_pkt_type_t type);

/* Registers an access point in the discoveredWifiAPs from a Beacon frame.
 * Returns true if the AP is newly discovered, false if it was already known.
 * In the latter case, channel and rssi are updated. 
 */
bool addAP(wifi_beacon_frame_t* beaconFrameHeader, const wifi_pkt_rx_ctrl_t& packetHeader);



#endif // SNIFFING