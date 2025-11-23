#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************

import zmq
import json

###############################################
# Callback Handlers
###############################################

def OnIndividualBookTickerChange(msg):
    print("[OnIndividualBookTickerChange]", msg)

def OnTradeChange(msg):
    print("[OnTradeChange]", msg)

def OnIndividualMarketTickerChange(msg):
    print("[OnIndividualMarketTickerChange]", msg)

def OnMiniTickerChange(msg):
    print("[OnMiniTickerChange]", msg)

def OnAggregateTradeChange(msg):
    print("[OnAggregateTradeChange]", msg)

def OnKlineCandleStickChange(msg):
    print("[OnKlineCandleStickChange]", msg)

def OnAllMarketTickersChange(msg):
    print("[OnAllMarketTickersChange]", msg)

def OnAllMiniTickersChange(msg):
    print("[OnAllMiniTickersChange]", msg)

def OnAllPartDepthChange(msg):
    print("[OnAllPartDepthChange]", msg)

def OnAllDiffDepthChange(msg):
    print("[OnAllDiffDepthChange]", msg)


###############################################
# MessageType → Callback Mapping
###############################################

MESSAGE_CALLBACKS = {
    "IndividualBookTicker": OnIndividualBookTickerChange,
    "Trade": OnTradeChange,
    "IndividualMarketTicker": OnIndividualMarketTickerChange,
    "MiniTicker": OnMiniTickerChange,
    "AggregateTrade": OnAggregateTradeChange,
    "KlineCandleStick": OnKlineCandleStickChange,
    "AllMarketTickers": OnAllMarketTickersChange,
    "AllMiniTickers": OnAllMiniTickersChange,
    "AllPartDepth": OnAllPartDepthChange,
    "AllDiffDepth": OnAllDiffDepthChange,
}


###############################################
# Dispatcher
###############################################

def dispatch_json_message(json_str: str):
    try:
        message = json.loads(json_str)
    except Exception as e:
        print("JSON decode error:", e, "| raw =", json_str)
        return

    msg_type = message.get("MessageType")
    msg_symbol = message.get("Symbol")
    if not msg_type:
        print("Missing MessageType in message:", message)
        return

    callback = MESSAGE_CALLBACKS.get(msg_type)
    if not callback:
        print(f"Unknown MessageType='{msg_type}', message:", message)
        return

    callback(message)


###############################################
# ZeroMQ Subscriber
###############################################

def main():
    context = zmq.Context()
    socket = context.socket(zmq.SUB)
    socket.connect("tcp://localhost:5554")

    topic_name = "AlgosToPythonClient"
    socket.setsockopt_string(zmq.SUBSCRIBE, topic_name)

    poller = zmq.Poller()
    poller.register(socket, zmq.POLLIN)

    print("Listening for messages... Ctrl+C to exit.")

    try:
        while True:
            socks = dict(poller.poll(500))  # 500ms poll interval

            if socket in socks:
                # Receive two frames: topic + payload
                topic_frame = socket.recv_string()
                payload_frame = socket.recv_string()

                # Dispatch payload
                dispatch_json_message(payload_frame)

    except KeyboardInterrupt:
        print("\nInterrupted by user, shutting down...")

    finally:
        socket.close()
        context.term()
        print("ZeroMQ socket closed.")

if __name__ == "__main__":
    main()
