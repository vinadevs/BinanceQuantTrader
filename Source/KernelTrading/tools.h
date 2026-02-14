/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#ifndef _TOOLS_HPP_
#define _TOOLS_HPP_

#include "dlldefine.h"
#include "double_type.h"

#include <string>
#include <vector>


namespace binapi {

	/*************************************************************************************************/

	std::size_t DLL_CLASS num_fractions_from_double_type(const double_type& v);

	double_type DLL_CLASS adjust_to_step(const double_type& v, const double_type& s, bool increase = false);

	std::vector<std::string> DLL_CLASS split_string(const std::string& str, const char* sep);
	std::string DLL_CLASS join_string(const std::vector<std::string>& vec, const char* sep);

	/*************************************************************************************************/

	double_type DLL_CLASS percents_diff(const double_type& a, const double_type& b);
	double_type DLL_CLASS percents_add(const double_type& v, const double_type& p);
	double_type DLL_CLASS percents_sub(const double_type& v, const double_type& p);
	double_type DLL_CLASS percents_val_by_percent(const double_type& v, const double_type& p);

	/*************************************************************************************************/

	bool DLL_CLASS is_my_orderid(const std::string& client_order_id);
	bool DLL_CLASS is_my_orderid(const char* client_order_id);

	/*************************************************************************************************/

} // ns binapi

#endif // _TOOLS_HPP_
