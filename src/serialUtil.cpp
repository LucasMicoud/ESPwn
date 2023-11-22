#include "serialUtil.h"

void serialout_32bit(uint32_t input){
  uint8_t val[4];

  //shift bits to the right to get every chunk of bytes
  val[0] = input;
  val[1] = input >> 8;
  val[2] = input >> 16;
  val[3] = input >> 24;
  Serial.write(val, 4);
}

//convert 16 bit input to 2 bytes output to serial port
void serialout_16bit(uint16_t input){
  uint8_t val[2];

  //shift bits to the right to get every chunk of bytes
  val[0] = input;
  val[1] = input >> 8;
  Serial.write(val, 2);
}

//Parser method to send PCAP formated packets to serial port. (PCAP needs timestamps in sec, microsec, payload length and payload).
void serialPacket(uint32_t len, uint8_t* payload_buf){

  // Define packet length for the pcap. Removes FCS so that wireshark does not get confused.
  uint32_t incl_len = len - FCS_LENGTH; // number of octets of packet saved in file
  uint32_t orig_len = len - FCS_LENGTH; // actual length of packet
  
  //if received packet is greater than snaplen, limit it, otherwise malforms the wireshark output.
  if(incl_len > SNAPLEN){
      incl_len = SNAPLEN;
  }
  
  // Retrieves time for the packet
  uint32_t time_sec = millis() * 1000; //current timestamp 
  uint32_t time_usec = (unsigned int)(micros() - millis() * 1000);

  //send data to serial port in the order time in seconds > time in usec > incl_len > orig_len > payload
  serialout_32bit(time_sec);
  serialout_32bit(time_usec);
  serialout_32bit(incl_len);
  serialout_32bit(orig_len);
  Serial.write(payload_buf, incl_len);
}