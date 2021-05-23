import zmq
import json


class Client:
    def __init__(self, endpoint="tcp://localhost:5556"):
        self.endpoint = endpoint

    def request(self, payload):
        context = zmq.Context()
        socket = context.socket(zmq.REQ)
        socket.connect(self.endpoint)

        # print("Sending block")
        raw_data = json.dumps(payload)
        socket.send_string(raw_data)

        message = socket.recv()
        print("Received reply")
        print(message)

        socket.close()
