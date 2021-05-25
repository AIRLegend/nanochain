from client import Client
from server import NodeServer


import json
import time


def main():
    # with open('../resources/balance_req.json', 'r') as file:
        # data = json.load(file)

    # client = Client(endpoint="tcp://localhost:5557")
    # client.request(data)
    server = NodeServer('../resources/blockchain_test.json')
    server.start()
    time.sleep(900)
    server.stop()




if __name__ == "__main__":
    main()
