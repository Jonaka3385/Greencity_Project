import paho.mqtt.client as mqtt
from influxdb import InfluxDBClient
import json
import base64

mqtt_server     = "10.20.111.210"
mqtt_port       = 1883
mqtt_user       = "group06"
mqtt_password   = "aeLoad9Aew3shaemaoca"
mqtt_topic      = "v3/sennet@ttn/devices/+/up"
mqtt_client     = None

influx_host     = "localhost"
influx_port     = 8086
influx_user     = "root"
influx_password = ""
influx_database = "GreenCity"
influx_client   = None

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
        client.subscribe(mqtt_topic)
    else:
        print("Failed to connect, return code %d\n", rc)

def on_disconnect(client, userdata, rc):
    print("Disconnecting...")
    if rc == 0:
        print("Disconnected OK Returned code=", rc)
    else:
        print("Disconnected INVALID Returned code=", rc)

def on_message(client, userdata, msg):
    global influx_client
    json_msg = json.loads(msg.payload.decode())
    print(f"Received Msg in `{msg.topic}` topic: {json.dumps(json_msg, indent=2)}")

    device_id = json_msg["end_device_ids"]["device_id"]
    f_port = json_msg["uplink_message"]["f_port"]
    print(f"Device ID: {device_id} Port: {f_port}")

    frm_payload = json_msg["uplink_message"]["frm_payload"]
    print(f"Received payload: {frm_payload}")

    try:
        json_data = [{
                        'measurement' : 'bodenfeuchtigkeit',
                        'time'      : json_msg['received_at'],
                        'tags'      : {
                                        'device_id' : json_msg['end_device_ids']['device_id'],
                                    },
                        'fields'    : {
                                        'frequency' : int(json_msg['uplink_message']['settings']['frequency']),
                                        'bodenfeuchtigkeit' : int(json_msg['uplink_message']['decoded_payload']['bodenfeuchtigkeit'][0]),
                                    },
                    }]
    except Exception:
        print("Couldn't extract payload information from MQTT Message")
    print(json_data)

    try:
        influx_client.write_points(json_data)
    except Exception:
        print("Couldn't write to InfluxDB")

mqtt_client = mqtt.Client()
mqtt_client.username_pw_set(mqtt_user, mqtt_password)
mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message
mqtt_client.connect(mqtt_server, mqtt_port)

influx_client = InfluxDBClient(
    host=influx_host,
    port=influx_port,
    username=influx_user,
    password=influx_password,
    database=influx_database
)

try:
    mqtt_client.loop_forever()
except KeyboardInterrupt:
    mqtt_client.disconnect()
    print("KeyboardInterrupt!")
