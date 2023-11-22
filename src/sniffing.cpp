#include "sniffing.h"

// std::map<String, wifi_AP*> discoveredWifiAPs;

void setupPCAP(){// PCAP file header
  uint32_t magic_num = 0xa1b2c3d4; //The default magic number. With this magic number, every packet needs timestamp in seconds and Microseconds.
  uint16_t version_major = 2; /* major version number */
  uint16_t version_minor = 4; /* minor version number */
  uint32_t thiszone = 0; /* GMT to local correction */
  uint32_t sigfigs = 0; /* accuracy of timestamps */
  uint32_t snaplen = 65535; /* max length of captured packets, in octets */
  uint32_t network = 105; /* data link type */
  serialout_32bit(magic_num);
  serialout_16bit(version_major);
  serialout_16bit(version_minor);
  serialout_32bit(thiszone);
  serialout_32bit(sigfigs);
  serialout_32bit(snaplen);
  serialout_32bit(network);
}

void promiscuousModeCallBack(void* buffer, wifi_promiscuous_pkt_type_t type){

  wifi_promiscuous_pkt_t* packet = (wifi_promiscuous_pkt_t*) buffer;
  frame_control_t* frameControl = &((wifi_management_frame_t*) packet->payload)->mac_header.frameControl;
  // Management packet
  if (frameControl->type == WIFI_PKT_MGMT) {
    int8_t subtype = frameControl->subtype;
    // 8 : Beacon frame
    if (subtype == 8) {
      wifi_beacon_frame_t* frame = (wifi_beacon_frame_t*) packet->payload;
      // Adds AP to know APs, logs the probe request if new AP.
      if (addAP(frame, packet->rx_ctrl)) {
        serialPacket(packet->rx_ctrl.sig_len, packet->payload);
      }
    }
    // 11 : Authentication frame, 0 and 1 : Association request and response
    if (subtype == 11 || subtype == 0 || subtype == 1) {
      serialPacket(packet->rx_ctrl.sig_len, packet->payload);
    }
  }
  // Data packet
  else if (frameControl->type == WIFI_PKT_DATA) {
    // EAPOL (key) packets
    if (packet->payload[32] == 0x88 && packet->payload[33] == 0x8e) {
      serialPacket(packet->rx_ctrl.sig_len, packet->payload);
    }
  }
}



bool addAP(wifi_beacon_frame_t* beaconFrame, const wifi_pkt_rx_ctrl_t& packetMetadataHeader) {
  // Retrieves SSID from payload  
  char SSIDbuf[32];
  int16_t ssidLength = ((beaconFrame->ssidLength << 8) | (beaconFrame->ssidLength >> 8)) & 0xFF;
  strncpy(SSIDbuf, (char*)beaconFrame->body, ssidLength);
  SSIDbuf[ssidLength] = '\0';
  String* SSID = new String(SSIDbuf);
  // Checks if SSID already in know APs. 
  // If so, update channel and rssi.
  // Else, adds it.
  auto it = discoveredWifiAPs.find(*SSID);
  if (it != discoveredWifiAPs.end()) {
    discoveredWifiAPs[*SSID]->channel = channel;
    discoveredWifiAPs[*SSID]->signal_power = packetMetadataHeader.rssi;
    return false;
  } else {
    wifi_AP* ap = new wifi_AP;
    ap->macAddr = beaconFrame->macHeader.senderMacAddr;
    ap->channel = channel;
    ap->signal_power = packetMetadataHeader.rssi;
    
    // Pick SSID from buffer and create a String from it.
    ap->SSID = SSID;
    discoveredWifiAPs[*(ap->SSID)] = ap;
    return true;
  }
}

