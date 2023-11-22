#ifndef DEAUTHENTICATION
#define DEAUTHENTICATION

#include <esp_wifi.h>
#include <Arduino.h>
#include "string.h"

#include "wifiUtil.h"

/* This definition is used to bypass the limitations on the esp32.
*/
// esp_err_t esp_wifi_80211_tx(wifi_interface_t ifx, const void *buffer, int len, bool en_sys_seq);
extern "C" int ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3){
  return 0;
}

void deauthenticateTarget(wifi_AP* targetAP);

#endif