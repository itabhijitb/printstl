#include <type_traits>
#ifndef __C11_TYPE_TRAITS_H__
#define __C11_TYPE_TRAITS_H__
#define STRINGIZE_NX(A) #A
namespace std_c11
{
	template<typename T>
	class is_associative
	{

		typedef char Yes[1];
		typedef char No[2];

		template<typename U> static Yes  &func(typename U::key_type);
		template<typename> static No  &func(...);

	public:
		typedef is_associative type;
		static bool const value = sizeof(func<T>(0)) == sizeof(Yes);
	};
	template <typename T>
	class is_iterable
	{
		typedef char Yes[1];
		typedef char No[2];

		template<typename U> static Yes  &func(typename U::iterator*);
		template<typename> static No  &func(...);

	public:
		typedef is_iterable type;
		static bool const value = sizeof(func<T>(0)) == sizeof(Yes);
	};
	template <typename T>
	struct is_pair {
		static const bool value = false;
	};
	template <typename TyK, typename TyV>
	struct is_pair < std::pair<TyK, TyV> > {
		static const bool value = true;
	};

                                     
	template <typename T>                                                     
	class is_smart_ptr                                            
	{                                                                         
		typedef char yes;                                                
		typedef long no;                                                 
		template <typename C> static yes test_deref(decltype(std::declval<C>().operator*())*);
		template <typename C> static no test_deref(...);
		template <typename C> static yes test_arrow(decltype(std::declval<C>().operator->()));
		template <typename C> static no test_arrow(...);
	public:                                                                   
		static constexpr bool value = (sizeof(test_deref<T>(nullptr)) == sizeof(yes) && 
			sizeof(test_arrow<T>(nullptr)) == sizeof(yes)  );
	};

}
#endif
