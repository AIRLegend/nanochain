import zmq
import time

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

    def start(self):
        self.should_run = True
        self.run_thread = Thread(target=self.loop, args=(self,))
        self.run_thread.start()

    def stop(self):
        self.should_run = False
        self.run_thread.join()
        self.run_thread = None

    def loop(self):
        context = zmq.Context()
        socket = context.socket(zmq.REP)
        socket.bind(f"tcp://*:{self.port}")

        while self.should_run:
            #  Wait for next request from client
            message = socket.recv()
            print("Received request: %s" % message)

            #  Do some 'work'
            time.sleep(self.proc_time)

            #  Send reply back to client
            socket.send_string(self.raw_data)

        socket.close()
        socket.unbind()
