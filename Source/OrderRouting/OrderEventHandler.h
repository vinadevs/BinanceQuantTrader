/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/

#pragma once

#include "dlldefine.h"

namespace OrderManagement {
	class BinanceNewOrder;
	class BinanceCancelOrder;
	class BinanceReplaceOrder;
	class BinanceQueryOrder;
}

namespace tinyxml2 {
	class XMLDocument;
}

namespace OrderRouting {
	
	// -An order event handler is responsible for managing and responding to various events
	// related to order processing and execution from internal or external sources.
	class DLL_CLASS_ORDER_ROUTING_EXPORTS OrderEventHandler
	{
	public:
		OrderEventHandler(const tinyxml2::XMLDocument* orderEventHandlerCfgXml = nullptr);

		////-----------------------------------------------------------------------------------------------------------
		/* -List of post trade callbacks for derived class to implement to handle exchange/simulator acks
			when an order is sent to exchange/simulator successfully and exchange/simulator sends back an ack message
		   -They should be called from trader when trader receives ack from exchange/simulator */

		// when a new order is sent to exchange successfully and exchange sends back an ack message
		virtual void OnNewConfirmedOrderAck(const OrderManagement::BinanceNewOrder* newOder) {}
		// when a cancel order is sent to exchange successfully and exchange sends back an ack message
		virtual void OnCancelConfirmedOrderAck(const OrderManagement::BinanceCancelOrder* cancelOrder) {}
		// when a replace order is sent to exchange successfully and exchange sends back an ack message
		virtual void OnReplaceConfirmedOrderAck(const OrderManagement::BinanceReplaceOrder* replaceOrder) {}
		// when a query order is sent to exchange successfully and exchange sends back an ack message
		virtual void OnQueryOrderAck(const OrderManagement::BinanceQueryOrder* queryOrder) {}
		// when an order is fully filled and exchange sends back an ack message
		virtual void OnOrderFilledAck(const OrderManagement::BinanceNewOrder* filledOrder) {}
		// when an order partially filled and exchange sends back an ack message
		virtual void OnOrderPartiallyFilledAck(const OrderManagement::BinanceNewOrder* partiallyFilledOrder) {}
		// when an order is expired and exchange sends back an ack message
		virtual void OnOrderExpiredAck(const OrderManagement::BinanceNewOrder* expiredOrder) {}
		// when an order is rejected and exchange sends back an ack message
		virtual void OnOrderRejectedAck(const OrderManagement::BinanceNewOrder* rejectedOrder) {}
		// when an order is cancelled and exchange sends back an ack message
		virtual void OnOrderCancelledAck(const OrderManagement::BinanceCancelOrder* cancelledOrder) {}
		// when an order is replaced and exchange sends back an ack message
		virtual void OnOrderReplacedAck(const OrderManagement::BinanceReplaceOrder* replacedOrder) {}

		// For future strading
		// when an order is opening and exchange sends back an ack message
		virtual void OnOrderOpeningPositionAck(const OrderManagement::BinanceNewOrder* openingOrder) {}
		// when an order is closed and exchange sends back an ack message
		virtual void OnOrderClosedPositionAck(const OrderManagement::BinanceNewOrder* closedOrder) {}
		// when an order is liquidated and exchange sends back an ack message
		virtual void OnOrderLiquidatedPositionAck(const OrderManagement::BinanceNewOrder* liquidatedOrder) {}
		// when an order is margin called and exchange sends back an ack message
		virtual void OnOrderMarginCalledPositionAck(const OrderManagement::BinanceNewOrder* marginCalledOrder) {}
		//-----------------------------------------------------------------------------------------------------------
	};
}
