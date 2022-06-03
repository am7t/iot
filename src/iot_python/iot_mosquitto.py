import paho.mqtt.client as mqtt  # import the client1
broker_address = "test.mosquitto.org"
print("creating new instance")
client = mqtt.Client("P1")  # create new instance
# The callback for when connection with the server is success


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("connected OK Returned code=", rc)
    else:
        print("Bad connection Returned code=", rc)

# The callback for when a PUBLISH message is received from the server.


def on_message(client, userdata, msg):
    global next_reading
    print("received: " + msg.topic+" "+str(msg.payload))


# Register connect callback
client.on_connect = on_connect
# Registed publish message callback
client.on_message = on_message

print("connecting to broker")
client.connect(broker_address)  # connect to broker
print("Subscribing to topic", "ece-jecc/test_value")
client.subscribe("ece-jecc/test_value")
print("Publishing message to topic", "ece-jecc/test_value")
client.publish("ece-jecc/test_value", "OFF")

try:
    client.loop_forever()
except KeyboardInterrupt:
    pass
