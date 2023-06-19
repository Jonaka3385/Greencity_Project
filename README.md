# GreenCity
![gif](./Image/giphy.gif)

## Projektidee und Motivation
Durch den Klimawandel wird es in deutschen Städten immer heißer. Grünflächen und Stadtbäume senken die Temperaturen messbar, doch durch die extremen Sommer sind sie von Trockenheit Bedrohnt. Mit dem Projekt GreenCity soll die Wasserversorgung der Stadtbäume aufgezeichnet werden, damit bei austrocknungsgefahr gezielt  bewässert werden kann. Dadurch kann Personal und Wasser eingespart werden.


## Aufbau
Auf der Physikalischenebene wird ein Arduino verwendet, der die Bodenfeuchtigkeit misst und sie über das Lorawan im The Thinks Network(TTN) versendet. Im TTN werden die Empfangenen Daten decodiert und weiter an den MQTT-Broker Übergeben. Die Influx Daten Bank abonniert die entsprechenden Sensoren und speichert die Daten, über Grafana werden sie dann Dargestellt.


## Hardware 

### Arduino 
Als Basis wird das Modular System von WisBlock "RAK Wireless" verwendet.

LPWAN Module:     RAK4631 WisBlock
Basis Bord:       RAK19007 WisBlock  
Adapter Module:   RAK1920 WisBlock 

### Sensor 
Zur Bodenfeuchtigkeitsmessung wird der Bodenfeuchtesensor Hygrometer Modul V2.0 kapazitiv verwendet. Dieser Sensor gibt eine elektronische spannung zurück, diese Spanung haben wir durch Probemessungen mit trocken boden bis überflutten boden auf zwei zahlen für max und min Festgelegt.


## Software 

### Entwicklungsumgebung
Als Entwicklungsumgebung wurde Visual Studio Code verwendet. Zusätzlich wird das PlatformIO PlugIn benötigt, in welchem die NRF52 Plattform installiert und die RAK4631 Cores hinzugefügt werden mussten. 

### Programmteile
Die platformio.ini ist die Konfigurationsdatei des Projekts. 
Die main Dateien sind der Kern des Projekts. In diesen wird alles gestartet und verwaltet. 
Die main.cpp kümmert sich um das setup, die Initialisierung und startet die Prozess-Schleife. Die main.h Datei ist für das Definieren aller benötigten Pins und Timeout-Zeiten da und included die benötigten Packages. 
Die lorawan.cpp ist, wie der Name schon sagt, für die LoraWan Kommunikation zuständig und sendet die Daten des Sensors und kümmert sich um Errors. 

### Verwendete Packages
Als Packages nutzen wir die Arduino und Adafruit_TinyUSB Libraries, das SPI und Wire Interface und die SX126x-Arduino und LoRaWan-RAK4630 API. 


## Sendeinterwall 
Das Sendeinterwall ist auf einmal pro Minute aus Vorführungsgründen eingestellt. Jedoch würde im Realbetrieb eine Naricht pro Stunde genügen.

## Payload

{
  "name": "as.up.data.forward",
  "time": "2023-06-19T01:18:07.173546833Z",
  "identifiers": [
    {
      "device_ids": {
        "device_id": "eui-ac1f09fffe08fba1",
        "application_ids": {
          "application_id": "sennet"
        },
        "dev_eui": "AC1F09FFFE08FBA1",
        "join_eui": "0000000000000000",
        "dev_addr": "260B160B"
      }
    }
  ],
  "data": {
    "@type": "type.googleapis.com/ttn.lorawan.v3.ApplicationUp",
    "end_device_ids": {
      "device_id": "eui-ac1f09fffe08fba1",
      "application_ids": {
        "application_id": "sennet"
      },
      "dev_eui": "AC1F09FFFE08FBA1",
      "join_eui": "0000000000000000",
      "dev_addr": "260B160B"
    },
    "correlation_ids": [
      "as:up:01H38KP2A20XXGM395XZK03GNQ",
      "gs:conn:01H2Y5NSJZJ4Y7BQ6N7FCTNJW8",
      "gs:up:host:01H2Y5NSK4YY8BRE79RBQCWJYG",
      "gs:uplink:01H38KP23F3BH1QGXCGG2FCH9E",
      "ns:uplink:01H38KP23GE7XDHEEP90612YAC",
      "rpc:/ttn.lorawan.v3.GsNs/HandleUplink:01H38KP23GAZ3JJC6YBKMRQ8T7",
      "rpc:/ttn.lorawan.v3.NsAs/HandleUplink:01H38KP2A1ED2ZQJ1APTT7A7SD"
    ],
    "received_at": "2023-06-19T01:18:07.169568427Z",
    "uplink_message": {
      "session_key_id": "AYjRK3cQSDEbmQ8PR0sE+w==",
      "f_port": 2,
      "f_cnt": 16,
      "frm_payload": "Pg==",
      "decoded_payload": {
        "bodenfeuchtigkeit": [
          62
        ]
      },
      "rx_metadata": [
        {
          "gateway_ids": {
            "gateway_id": "stadtbonn-swb-gateway-02",
            "eui": "7076FF005605099C"
          },
          "time": "2023-06-19T01:18:06.906Z",
          "timestamp": 4227130812,
          "rssi": -117,
          "channel_rssi": -117,
          "snr": 0.5,
          "location": {
            "latitude": 50.710253,
            "longitude": 7.120836,
            "source": "SOURCE_REGISTRY"
          },
          "uplink_token": "CiYKJAoYc3RhZHRib25uLXN3Yi1nYXRld2F5LTAyEghwdv8AVgUJnBC8y9PfDxoMCM7ZvqQGEMD8x8kDIODMhaWDiFAqDAjO2b6kBhCA7YGwAw==",
          "channel_index": 3,
          "gps_time": "2023-06-19T01:18:06.906Z",
          "received_at": "2023-06-19T01:18:06.959577664Z"
        },
        {
          "gateway_ids": {
            "gateway_id": "stadtbonn-swb-gateway-06",
            "eui": "7076FF0056050692"
          },
          "time": "2023-06-19T01:18:06.906Z",
          "timestamp": 1683178396,
          "rssi": -106,
          "channel_rssi": -106,
          "snr": 8,
          "location": {
            "latitude": 50.70335162180902,
            "longitude": 7.103377282619477,
            "altitude": 30,
            "source": "SOURCE_REGISTRY"
          },
          "uplink_token": "CiYKJAoYc3RhZHRib25uLXN3Yi1nYXRld2F5LTA2Eghwdv8AVgUGkhCch82iBhoMCM7ZvqQGEKX1rc8DIOCygKr+ulAqDAjO2b6kBhCA7YGwAw==",
          "channel_index": 3,
          "gps_time": "2023-06-19T01:18:06.906Z",
          "received_at": "2023-06-19T01:18:05.048971990Z"
        }
      ],
      "settings": {
        "data_rate": {
          "lora": {
            "bandwidth": 125000,
            "spreading_factor": 8,
            "coding_rate": "4/5"
          }
        },
        "frequency": "867700000",
        "timestamp": 4227130812,
        "time": "2023-06-19T01:18:06.906Z"
      },
      "received_at": "2023-06-19T01:18:06.960451403Z",
      "consumed_airtime": "0.082432s",
      "version_ids": {
        "band_id": "EU_863_870"
      },
      "network_ids": {
        "net_id": "000013",
        "tenant_id": "ttn",
        "cluster_id": "eu1",
        "cluster_address": "eu1.cloud.thethings.network"
      }
    }
  },
  "correlation_ids": [
    "as:up:01H38KP2A20XXGM395XZK03GNQ",
    "gs:conn:01H2Y5NSJZJ4Y7BQ6N7FCTNJW8",
    "gs:up:host:01H2Y5NSK4YY8BRE79RBQCWJYG",
    "gs:uplink:01H38KP23F3BH1QGXCGG2FCH9E",
    "ns:uplink:01H38KP23GE7XDHEEP90612YAC",
    "rpc:/ttn.lorawan.v3.GsNs/HandleUplink:01H38KP23GAZ3JJC6YBKMRQ8T7",
    "rpc:/ttn.lorawan.v3.NsAs/HandleUplink:01H38KP2A1ED2ZQJ1APTT7A7SD"
  ],
  "origin": "ip-10-100-14-67.eu-west-1.compute.internal",
  "context": {
    "tenant-id": "CgN0dG4="
  },
  "visibility": {
    "rights": [
      "RIGHT_APPLICATION_TRAFFIC_READ"
    ]
  },
  "unique_id": "01H38KP2A50W9BFAHGG9BERC1E"
}

## Daten auswertung 
Die Messdaten sind keine absoluten Werte, sondern abhängig von der Boden zusammensetzung. Zusetzlich ist zubeachten, dass die Ausagekraft über die hinreichende Versorgung eine Pflanze mit Wasser abhängig 
von der Durchwurzelungstiefe. Wir Empfelen deshalb diesen spezifischen Aufbau nur für Pflanzen mit einer maximalen Wurzeltiefe von 20 cm.


## Quellen
https://www.nccs.admin.ch/nccs/de/home/sektoren/wasserwirtschaft/auswirkungen-auf-den-wasserhaushalt/bodenfeuchteundverdunstung.html
https://www.dwd.de/DE/klimaumwelt/ku_beratung/landwirtschaft/agrar_modelle/ambav.pdf?__blob=publicationFile&v=2
https://github.com/RAKWireless/WisBlock/tree/master/examples
https://www.thethingsindustries.com/docs/integrations/payload-formatters/javascript/uplink/
https://test-wetterstation.de/arduino-moisture-sensor

