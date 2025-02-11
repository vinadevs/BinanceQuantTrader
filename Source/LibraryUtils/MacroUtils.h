/*#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************/ 

#pragma once

/*************************************************************************************************/

#define PRINT_IF_ERROR(res) \
    if (!static_cast<bool>(res)) { \
        std::cout << __FILE__ << "(" << __LINE__ << "): msg=" << res.errmsg << std::endl; \
    }

#define BREAK_IF_ERROR_EXIT_FAILURE(res) \
    PRINT_IF_ERROR(res); \
    if (!res) { \
        return EXIT_FAILURE; \
    }

#define BREAK_IF_ERROR_EXIT_FALSE(res) \
    PRINT_IF_ERROR(res); \
    if (!res) { \
        return false; \
    }

/*************************************************************************************************/

#define ENABLE_COPY_AND_ENABLE_MOVE(class_name) \
	class_name(const class_name&) = default; \
	class_name& operator=(const class_name&) = default; \
	class_name(class_name&&) = default; \
	class_name& operator=(class_name&&) = default;

#define DISABLE_COPY_AND_MOVE(class_name) \
	class_name(const class_name&) = delete; \
	class_name& operator=(const class_name&) = delete; \
	class_name(class_name&&) = delete; \
	class_name& operator=(class_name&&) = delete;

#define DISABLE_COPY_AND_ENABLE_MOVE(class_name) \
	class_name(const class_name&) = delete; \
	class_name& operator=(const class_name&) = delete; \
	class_name(class_name&&) = default; \
	class_name& operator=(class_name&&) = default;

/*************************************************************************************************/

#define DECLARE_SINGLETON(class_name) \
public: \
    DISABLE_COPY_AND_MOVE(class_name) \
    static class_name& GetInstance() { \
        static class_name instance; \
        return instance; \
    } \
private: \
    class_name() = default; \
    ~class_name() = default;

/*************************************************************************************************/

#define ESCAPSE_STRING_WITH_QUOTE(str) "\"" + str + "\""

#define DELETE_POINTER_CPP(x)		 { if(x) { delete (x); (x)=NULL;} }
#define DELETE_POINTER_CPP_ARRAY(x)  { if(x){ delete [] (x); (x)=NULL;} }
#define DELETE_POINTER_C(x)          { if (x) { free(x); x = NULL;} }
