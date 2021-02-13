# adri_ntpTime

based on esp8266 framework

<pre>
Librairies
<details>
adri_ntpTime                    = https://github.com/AdriLighting/adri_ntpTime

esp8266/arduino                 = https://github.com/esp8266/Arduino/tree/master/libraries

TimeLib                         = https://github.com/PaulStoffregen/Time
</details>
Dependency Graph
<details>
|-- [adri_ntpTime] 1.0.0
|   |-- [ESP8266WiFi] 1.0
|   |-- [TimeLib] 1.6</details>
</pre>

<hr>

#### examples

adriNtpTime_1
<details>
platformio.ini
<details>
<pre>
[env:nodemcuv2]
platform=espressif8266
board=nodemcuv2
framework=arduino
lib_ignore=WiFi101
board_build.filesystem=littlefs
board_build.ldscript=eagle.flash.4m3m.ld
lib_extra_dirs= ${env.lib_extra_dirs}
upload_speed=921600
[platformio]
src_dir= ${env.src_dir}

</pre>
</details>
<pre>
Librairies
<details>
adri_ntpTime                    = https://github.com/AdriLighting/adri_ntpTime
adri_timer                      = https://github.com/AdriLighting/adri_timer
adri_wifiConnect                = https://github.com/AdriLighting/adri_wifiConnect
adri_tools_v2                   = https://github.com/AdriLighting/adri_tools_v2

esp8266/arduino                 = https://github.com/esp8266/Arduino/tree/master/libraries

TimeLib                         = https://github.com/PaulStoffregen/Time
</details>
Dependency Graph
<details>
|-- [adri_ntpTime] 1.0.0
|   |-- [TimeLib] 1.6
|   |-- [ESP8266WiFi] 1.0
|-- [adri_timer] 1.0.0
|-- [adri_wifiConnect] 1.0.0
|   |-- [ESP8266WebServer] 1.0
|   |   |-- [ESP8266WiFi] 1.0
|   |-- [ESP8266WiFi] 1.0
|   |-- [ESP8266mDNS] 1.2
|   |   |-- [ESP8266WiFi] 1.0
|   |-- [adri_tools_v2] 1.0.0
|   |   |-- [ESP8266WiFi] 1.0
|   |   |-- [LittleFS(esp8266)] 0.1.0
|   |-- [LittleFS(esp8266)] 0.1.0
|   |-- [adri_timer] 1.0.0
|-- [LittleFS(esp8266)] 0.1.0</details>
</pre>

</details>
<hr>