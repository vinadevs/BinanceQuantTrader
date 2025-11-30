To run this python program, you only need to install gRPC tools.
The strategy_parent_order_pb2 and strategy_parent_order_pb2_grpc files are generated from your .proto file — they are not installed via pip.

1. Install gRPC for Python
pip install grpcio grpcio-tools
2. Generate strategy_parent_order_pb2.py and strategy_parent_order_pb2_grpc.py
python -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. strategy_parent_order.proto
