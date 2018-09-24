/*****************************************************************************/
/*                              STL Libraries                                */
/*****************************************************************************/
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <list>
#include <stack>
#include <set>
#include <sstream>
#include <type_traits>
#include "c11_type_traits.h"
#ifndef __PPRINTSTL_H__
#define __PPRINTSTL_H__

namespace pprint
{

	template<
		typename Ty,
		bool as_type = std_c11::is_associative<Ty>::value,
		bool it_type = std_c11::is_iterable<Ty>::value,
		bool pr_type = std_c11::is_pair<Ty>::value
	>
		struct brackets
	{
		// Any other case is an Error
	};
	template<typename Ty>
	struct brackets<
		Ty,
		true,  //Associative
		true,  //Iterable
		false  //But not a Pair
	>
	{
		static const char open = '{';
		static const char close = '}';
	};
	template<typename Ty>
	struct brackets<
		Ty,
		false, //Not Associative
		true,  //But Iterable
		false  //But not a Pair
	>
	{
		static const char open = '[';
		static const char close = ']';
	};
	template<typename Ty>
	struct brackets<
		Ty,
		false, //Neither Associative
		false, //Nor Iterable
		false  //Nor a Pair
	>
	{
		static const char open = '<';
		static const char close = '>';
	};
	template<typename Ty>
	struct brackets<
		Ty,
		false, //Neither Associative (actually don't care)
		false, //Nor Iterable
		true   //But a Pair
	>
	{
		static const char open = '(';
		static const char close = ')';
	};
	enum TYPE_PROPERTY : uint32_t {
		IS_NONE=0,
		IS_ITERABLE=1,
		IS_FUNDAMENTAL=2,
		IS_STRING=4,
		IS_SMART_PTR=8
	};

	constexpr enum TYPE_PROPERTY operator |(const enum TYPE_PROPERTY selfValue, const enum TYPE_PROPERTY inValue)
	{
		return (enum TYPE_PROPERTY)(uint32_t(selfValue) | uint32_t(inValue));
	}
	constexpr enum TYPE_PROPERTY operator |=( enum TYPE_PROPERTY &selfValue, const enum TYPE_PROPERTY inValue)
	{
		selfValue = (enum TYPE_PROPERTY)(uint32_t(selfValue) | uint32_t(inValue));
		return selfValue;
	}
	template<typename Ty>
	constexpr TYPE_PROPERTY get_type_property()
	{
		
		return std::is_convertible<Ty, std::string>::value ? IS_STRING : 
			std_c11::is_iterable<typename std::remove_cv<typename std::remove_reference<Ty>::type>::type>::value ?IS_ITERABLE :
			std::is_fundamental<typename std::remove_cv<typename std::remove_reference<Ty>::type>::type>::value ? IS_FUNDAMENTAL :
			std_c11::is_smart_ptr<Ty>::value ? IS_SMART_PTR:
			IS_NONE;
	}
	template<
		typename Ty,
		TYPE_PROPERTY
	>
		struct pprint {
		std::string   operator()(const Ty& data, size_t depth = 0)
		{
			
			static_assert(get_type_property<Ty>() == IS_NONE);
			auto type_name = STRINGIZE_NX(Ty);
			std::string strm;
			strm << type_name;
			return strm;
		}
	};
	template<typename Ty>
	struct pprint<
		Ty,
		IS_ITERABLE
	> {
		std::string   operator()(const Ty& data, size_t depth = 0)
		{
			std::string strm;
			strm += "\n";
			strm += std::string(depth, ' ');
			strm += std::string(1, brackets<Ty>::open);
			for (typename Ty::const_iterator it = data.begin();
				it != data.end();
				it++)
			{
				strm += pprint<typename Ty::value_type, get_type_property<Ty::value_type>()>()(*it, depth + 1);
				if (std::distance(it, data.end()) > 1)
				{
					strm += ",";

				}
			}
			strm += std::string(1, brackets<Ty>::close);
			return strm;
		}
	};
	//Smart Pointer
	template<typename Ty>
	struct pprint<
		Ty,
		IS_SMART_PTR
	> {
		std::string   operator()(const Ty& data, size_t depth = 0)
		{
			std::string strm;
			strm = std::string(1, brackets<Ty>::open)
			+ pprint< Ty::element_type, get_type_property< Ty::element_type>()>()(*data, depth + 1)
			+ std::string(1, brackets<Ty>::close);
			return strm;
		}
	};
	//std::string / C Type String
	template<typename Ty>
	struct pprint<
		Ty,
		IS_STRING
	> {
		const std::string   operator()(const std::string& data, size_t depth = 0)
		{
			std::string strm = "\"" + data + "\"";
			return strm;
		}
	};
	// Fundamental Data Types
	template<typename Ty>
	struct pprint<
		Ty,
		IS_FUNDAMENTAL
	> {
		std::string  operator()(const Ty& data, size_t depth = 0)
		{
			std::stringstream st;
			st << data;
			return st.str();
		}
	};
	template<
		typename TyK,
		typename TyV>
		struct pprint<
		std::pair<TyK, TyV>,
		IS_NONE> {
		std::string   operator()(const std::pair<TyK, TyV>& data, size_t depth = 0)
		{
			std::string strm = std::string("\n");
			
			strm += std::string(depth, ' ')
				+ std::string("(")
				+ pprint<TyK, get_type_property<TyK>()>()(data.first, depth)
				+ " :";
			strm += pprint<TyV, get_type_property<TyV>()>()(data.second, strm.size())
				+ ")";
			return strm;
		}
	};
};
#endif
