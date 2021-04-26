from client import Client

import json


def main():
    with open('../resources/msg_new_block.json', 'r') as file:
        data = json.load(file)

    client = Client(endpoint="tcp://localhost:5556")
    client.request(data)


if __name__ == "__main__":
    main()
