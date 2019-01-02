// This file is part of cleantype: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
#pragma once

#include <cleantype/cleantype.hpp>

namespace cleantype
{
    namespace internal
    {
        // _impl_lamda_to_ptr cannot be extracted into a separate function!
        // because its return type will be unknown to the compiler...
        // ("error: cannot deduce return type 'auto' from returned value of type '<overloaded function type>'")
        //
        // template<typename... Args, typename Lambda>
        // auto _impl_lamda_to_ptr()
        // {
        //     if constexpr((sizeof...(Args) > 0))
        //         #ifdef _MSC_VER
        //             return &Lambda::operator() < Args... > ;
        //         #else
        //             return &Lambda::template operator() < Args... > ;
        //         #endif
        //     else
        //         return &Lambda::operator(); // if you have an error here, your lambda is generic! Add template params for its input types!
        // }


        template <typename... Args, typename Lambda> std::string impl_lambda_generic(Lambda fn, bool clean_type)
        {
            std::string mem_fn_type;
            {
                if constexpr((sizeof...(Args) > 0))
                {
                    #ifdef _MSC_VER
                        auto as_ptr = &Lambda::operator() < Args... > ;
                    #else
                        auto as_ptr = &Lambda::template operator() < Args... > ;
                    #endif
                    auto as_mem_fn = std::mem_fn(as_ptr);
                    mem_fn_type = cleantype::internal::_impl_typeid_hana_no_holder<decltype(as_mem_fn)>();
                }
                else
                {
                    auto as_ptr = &Lambda::operator(); // if you have an error here, your lambda is generic! Add template params for its input types!
                    auto as_mem_fn = std::mem_fn(as_ptr);
                    mem_fn_type = cleantype::internal::_impl_typeid_hana_no_holder<decltype(as_mem_fn)>();
                }
            }

            std::string final_type = cleantype::internal::_mem_fn_to_lambda_type(mem_fn_type, clean_type);
            return final_type;
        }


    } // namespace internal

    template <typename... Args, typename Lambda> std::string lambda_generic_clean(Lambda fn)
    {
        return internal::impl_lambda_generic<Args...>(fn, true);
    }

    template <typename... Args, typename Lambda> std::string lambda_generic_full(Lambda fn)
    {
        return internal::impl_lambda_generic<Args...>(fn, false);
    }


    //////////////////////////////
    // Start of public API
    //////////////////////////////

    template <typename... Args, typename Lambda> std::string lambda_generic_clean(Lambda fn);
    template <typename... Args, typename Lambda> std::string lambda_generic_full(Lambda fn);

} // namespace cleantype

// CT_type_lambda_generic_fromparams_
#define CT_type_lambda_generic_fromparams_1(fn, arg1) cleantype::lambda_generic_clean<decltype(arg1)>(fn)
#define CT_type_lambda_generic_fromparams_2(fn, arg1, arg2) cleantype::lambda_generic_clean<decltype(arg1), decltype(arg2)>(fn)
#define CT_type_lambda_generic_fromparams_3(fn, arg1, arg2, arg3) cleantype::lambda_generic_clean<decltype(arg1), decltype(arg2), decltype(arg3)>(fn)
#define CT_type_lambda_generic_fromparams_4(fn, arg1, arg2, arg3, arg4) cleantype::lambda_generic_clean<decltype(arg1), decltype(arg2), decltype(arg3), decltype(arg4)>(fn)
#define CT_type_lambda_generic_fromparams_5(fn, arg1, arg2, arg3, arg4, arg5) cleantype::lambda_generic_clean<decltype(arg1), decltype(arg2), decltype(arg3), decltype(arg4), decltype(arg5)>(fn)
