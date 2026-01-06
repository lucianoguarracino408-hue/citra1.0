import zmq
import json
import requests
import logging

logging.basicConfig(level=logging.INFO)

ZMQ_BIND = "tcp://*:5555"
HTTP_COMMAND_URL = "http://localhost:8080/command"

ctx = zmq.Context()
receiver = ctx.socket(zmq.PULL)
receiver.bind(ZMQ_BIND)

logging.info(f"[ZMQ] Bridge escuchando en {ZMQ_BIND}")

while True:
    raw = receiver.recv_string()
    cmd = json.loads(raw)

    logging.info(f"CMD recibido: {cmd}")

    try:
        r = requests.post(HTTP_COMMAND_URL, json=cmd, timeout=1)
        logging.info(f"RobotCore respondió: {r.json()}")
    except requests.RequestException as e:
        logging.error(f"Error enviando comando al RobotCore: {e}")
