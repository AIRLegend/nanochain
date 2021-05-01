import zmq
import time
import json
import netops as ops

from threading import Thread


class NodeServer:
    def __init__(self,
                 data_path,
                 port=5556,
                 proc_time=5):
        self.port = port
        self.proc_time = proc_time
        self.should_run = False
        self.run_thread = None
        with open(data_path, 'r') as file:
            self.raw_data = file.read()
            self.blockchain = json.loads(self.raw_data)["payload"]

    def start(self):
        self.should_run = True
        self.run_thread = Thread(target=self.loop)
        self.run_thread.start()

    def stop(self):
        self.should_run = False
        self.run_thread.join()
        self.run_thread = None

    def message_handle(self, msg):
        message = json.loads(msg)
        result = None

        if int(message["OP"]) is ops.OpType.OP_MEMPOOL_REQUEST.value:
            print("> Mempool request")
            result = self.blockchain[0]["txs"]
        elif int(message["OP"]) is ops.OpType.OP_BLOCK_REQ.value:
            print("> Mempool request")
            result = self.blockchain

        return result


    def loop(self):
        context = zmq.Context()
        socket = context.socket(zmq.REP)
        socket.bind(f"tcp://*:{self.port}")

        while self.should_run:
            #  Wait for next request from client
            message = socket.recv().decode('utf-8')
            print("Received request: %s" % message)

            #  Do some 'work'
            # time.sleep(self.proc_time)

            res = self.message_handle(message)
            res_status = ops.MessageStatus.BAD
            if res is not None:
                res_status = ops.MessageStatus.OK

            response = json.dumps({"status": res_status.value, "payload": res})
            print(f"\t Sending: {response}")

            #  Send reply back to client
            socket.send_string(response)

        socket.close()
        socket.unbind()
