#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************

#!/usr/bin/env python3

from dataclasses import dataclass
import grpc
import strategy_parent_order_pb2
import strategy_parent_order_pb2_grpc


@dataclass
class ParentOrder:
    symbol: str
    price: float
    quantity: float
    strategy: str
    start_time: str
    end_time: str
    side: str
    child_order_type: str
    parent_order_type: str
    parent_order_id: str
    original_parent_order_id: str


def main():
    # Example NEW parent order
    order = ParentOrder(
        symbol="BNBUSDT",
        price=636.0,
        quantity=1000,
        strategy="VWAPStrategy",
        start_time="2025-11-26 10:00:00",
        end_time="2025-11-26 11:00:00",
        side="BUY",
        child_order_type="LIMIT",
        parent_order_type="NEW",
        parent_order_id="123456",               # NEW order → no ID yet
        original_parent_order_id=""             # root = empty for NEW orders
    )

    channel = grpc.insecure_channel("localhost:50061")
    stub = strategy_parent_order_pb2_grpc.StrategyOrderServiceStub(channel)

    request = strategy_parent_order_pb2.StrategyParentOrderRequest(
        symbol=order.symbol,
        price=order.price,
        quantity=order.quantity,
        strategy=order.strategy,
        start_time=order.start_time,
        end_time=order.end_time,
        side=order.side,
        child_order_type=order.child_order_type,
        parent_order_type=order.parent_order_type,
        parent_order_id=order.parent_order_id,
        original_parent_order_id=order.original_parent_order_id,
    )

    response = stub.SubmitParentOrder(request)

    print("Response:", response.status, response.error_message)


if __name__ == "__main__":
    main()
