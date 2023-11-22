# ESPwn

Wifi deauthentication and 4-way handshake sniffing, using esp32.

## Build instruction

By default, Espressif limits the capabilities of the esp32 by only allowing the user to send raw beacon/probe request/probe response/action and non-QoS data frame, which means no deauthentication.

In practice, that limitation is applied by a `ieee80211_raw_frame_sanity_check` function. Redefining it in our cause bypasses it, but causes linking issues as the function is now defined twice.

Build the project in VS Code using platform io. This should result in two redefinition errors during the linking process : one with `main.cpp.o` and one with `libnet80211.a`.

To fix these issues :

- Locate `libnet80211.a`, and make a copy of it (just in case).
- Locate `xtensa-esp32-elf-objcopy.exe` (eg. `C:\Users\[user]\.platformio\packages\toolchain-xtensa-esp32\bin\xtensa-esp32-elf-objcopy.exe`).
- Run `xtensa-esp32-elf-objcopy.exe --weaken-symbol=ieee80211_raw_frame_sanity_check libnet80211.a libnet80211.a2`
- Replace libnet80211.a with libnet80211.a2

Repeat with `main.cpp.o` instead of `libnet80211.a`.

Each time `main.cpp` is recompiled, this part must be repeated.

You are then free to flash and test the program !
