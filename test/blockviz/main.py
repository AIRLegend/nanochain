import sys
import os
import json

from flask import Flask
from flask import render_template
from nodestub.client import Client


FILEPATH = os.path.dirname(__file__)
sys.path.append(os.path.join(FILEPATH, '..'))

app = Flask(__name__)

def get_blocks():
    client = Client(endpoint="tcp://localhost:5557")
    payload = {"OP": 4, "payload": {"hash": ""}}
    res = client.request(payload)
    res = res.decode("utf-8")
    return json.loads(res)["data"]


@app.route('/')
def root():
    r = get_blocks()
    return render_template('blocks.html', blocks=r)
