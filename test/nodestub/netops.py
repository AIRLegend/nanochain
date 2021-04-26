from enum import Enum


class OpType(Enum):
    OP_NULL = 0
    OP_BROADCAST_TX = 1
    OP_BLOCK_SEND = 3
    OP_BLOCK_REQ = 4
    OP_BLOCK_ANNOUNCE = 5


class MessageStatus(Enum):
    BAD = 0
    OK = 1

