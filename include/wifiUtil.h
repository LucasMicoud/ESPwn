/* wifiUtil.h
 * Contains utility structures used to facilitate wifi frames analysis.
*/

#ifndef WIFI_FRAMES
#define WIFI_FRAMES

#include "inttypes.h"

/* Wifi AP type. Contains informations about the WIFI APs.
 */

/* Mac adress type, stored as an array of 6 bytes.
 */
typedef struct {
    uint8_t mac[6];
} __attribute__((packed)) mac_addr_t;

typedef struct {
  mac_addr_t macAddr;
  String* SSID;
  int8_t channel;
  int8_t signal_power;
} __attribute__((packed)) wifi_AP;

/* Frame control type. 
 * Order is important for automatic affectation with casting.
 */
typedef struct {
    uint8_t version : 2;
    uint8_t type : 2;
    uint8_t subtype : 4;
    uint8_t DSStatusFlag : 2;
    uint8_t moreFragmentsFlag : 1;
    uint8_t retryFlag : 1;
    uint8_t powerManagementFlag : 1;
    uint8_t moreDataFlag : 1;
    uint8_t protectedFlag : 1;
    uint8_t HTCOrderFlag : 1;
} __attribute__((packed)) frame_control_t;

/* Wifi frame header. 
 */
typedef struct {
  frame_control_t frameControl;
  uint16_t duration;
  mac_addr_t destinationMacAddr;
  mac_addr_t senderMacAddr;
  mac_addr_t bssid;
  uint16_t seqctl;
} __attribute__((packed)) wifi_mac_header_t;

/* Wifi management frame. 
 */
typedef struct {
  wifi_mac_header_t mac_header;
  unsigned char body[];
} __attribute__((packed)) wifi_management_frame_t;

/* Wifi Beacon frame. SSID is present as the ssid_length first bytes of body.
 * Body contains the other optionnal parameters.
 */
typedef struct {
  wifi_mac_header_t macHeader;
  int64_t timestamp;
  int16_t beaconInterval;
  int16_t capabilityInfo;
  int16_t ssidLength;
  unsigned char body[];
} __attribute__((packed)) wifi_beacon_frame_t;

#endif //WIFI_FRAMES

