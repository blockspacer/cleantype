
# `cleantype` : Readable C++ Types and Lambda signatures / Compiler Decipherer

`cleantype`is a small header only library which offer a *consistent naming scheme accross compilers*, at *run-time* and *compile-time*. It can also output the *signature of lambda* functions.

The included tool `ct_compiler_decipher` simplifies the template noise in your compiler output : just pipe ( `|` ) your build tool to it.

It can be seeen as a developper friendly `typeid` alternative.

<!--
![Build Status Travis](https://travis-ci.org/pthom/cleantype.svg?branch=master)
![Build Status AppVeyor](https://ci.appveyor.com/api/projects/status/github/pthom/cleantype)
-->

<table>
    <tr>
      <td>
        Travis <a href="https://travis-ci.org/pthom/cleantype/"> <img src="https://travis-ci.org/pthom/cleantype.svg?branch=master" /> </a>
     </td>
     <td>
        Windows <a href="https://ci.appveyor.com/project/pthom/cleantype"> <img src="https://ci.appveyor.com/api/projects/status/s6wmhg5q9l69w7aq/branch/master?svg=true" /> </a>
    </td>
     <td>
        License <a href="https://www.boost.org/LICENSE_1_0.txt"> <img src="https://img.shields.io/badge/license-boost%201.0-blue.svg" /> </a>
    </td>
    <td>
        Try this online!
        <a href="https://mybinder.org/v2/gh/pthom/cleantype/master?filepath=notebooks%2Fcleantype%2Fcleantype.ipynb"> 
            <img src="https://mybinder.org/badge_logo.svg" /> 
        </a>
    </tr>
</table>    

In C++, [typeid.name()](https://en.cppreference.com/w/cpp/language/typeid) is able to display the type of variables.
However it has several limitations:

* `const`, `volatile`, `&&`qualifiers are ignored
* if cannot identify the signature of lambdas functions
+ it cannot identify the return type of a function with an auto specifier
* the returned name if often unreadable : for example `std::set<std::string>` becomes
 ````
 std::set<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >
 ````

This library tries to overcomes some of these limitations. It is composed mainly of C++11 / C++14 functions. It also contains some macros in order to be able to display rvalue reference type, as well as variables names. Macros are prepended with a suffix 'm_'.

Note: this library is heavily [tested](https://github.com/pthom/cleantype/tree/master/src/include/cleantype/tests), with clang, gcc and msvc. However, it should be considered alpha state.

# Installation and usage
`cleantype`is a small header only library, so you just need to clone it and add it to your path.

Then, include [cleantype/cleantype.hpp](src/include/cleantype/cleantype.hpp) (this file includes a comprehensive API doc)

# About this manual

This manual is written using [cling](https://root.cern.ch/cling), [xeus cling](https://xeus-cling.readthedocs.io/en/latest/) and [jupyter notebook](https://jupyter.org/). Cling enables a Read-Eval-Print-Loop (REPL) development mode with C++. This approach benefits a lot from having a good type introspection, which is the aim of this library. 

The code that you read in this manual is real live code that can be executed inside jupyter notebook. 
You can try it directly inside [binder](https://mybinder.org/) : click on the "launch binder" at the top of this page.<br/>
Notes:
* Beware, it require about 2 minutes to load; but then you will be able to run the code live from your browser!
* Inside the notebook, click on the "Run" button in order to execute each cell (in order)
* You can modify and run the code as you desire inside binder!
* Note that there is a limitation in cling that *requires that you add two ";" after each lambda function definition*

The "#pragma cling add_include_path" is specific to cling. Beside this, everything is standard C++.


```c++
// Special macro to change the include path (this is required for the binder demo)
#pragma cling add_include_path("./include")
#include <cleantype/cleantype.hpp>

// The includes below are not required, they are just used for the purpose of this manual
#include <future> 
#include <numeric>
#include <fplus/fplus.hpp>
```


```c++
// This macro is used later in this manual in order to display commands and their results
#define run_show(cmd) \
{  \
    std::cout << #cmd << "\n"; \
    std::cout << cmd << "\n\n"; \
}
```

# Friendly type names for variables (clean type, name and value)

 ## Readable type names 
                 
* `cleantype::clean<T...>()` is a function that will return a string containing
   a readable type, for a given type or pack of types
   Use it with "cleantype::clean<decltype(var)>()"<br/>
   Note: It will add a reference by default so that "int v = 5; cleantype::clean(v)" will return "int&". 
   Use the macro CT_cleantype_clean() if you want to avoid this

* `cleantype::clean<T...>(t...)` is a an easier version, using an instance of the type.<br/>
    Notes:
     * It will add a reference. Use the macro CT_cleantype_clean() if you want to avoid this
     * It is not able to output correctly r-value references. For this, use `cleantype::clean<decltype(var)>()`

* `cleantype::show_details(T && v)` is a function that will return a string containing
   the readable type of a variable, as well as its content

* `CT_cleantype_clean(var)` is a macro that will also return the full type,
   but, it is able to also correctly display rvalue reference types.

*  `CT_show_details(var)` is a macro that will return a string containing the name,
   type and content of a variable (in this case, the underlying type of 'var'
   has to have an 'ostream & operator<<')

* `CT_show_details_cont` (macro) is a version of CT_show_details for complex containers
   like "std::map". "cont" stands for "container".

### Examples


```c++
// Lets define a function with an auto return type : what is its return type?
auto my_range(int nb)
{
    std::list<int> l(nb);
    std::iota(l.begin(), l.end(), 1);
    return l;
}
auto v = my_range(5);
```


```c++
run_show(     cleantype::clean(v)                   )
run_show(     cleantype::clean<decltype(v)>()       )
run_show(     cleantype::show_details(v)            )
run_show(     CT_cleantype_clean(v)                 )
run_show(     CT_show_details(v)                    )
```

    cleantype::clean(v)
    std::list<int> &
    
    cleantype::clean<decltype(v)>()
    std::list<int>
    
    cleantype::show_details(v)
    std::list<int> & = [1, 2, 3, 4, 5]
    
    CT_cleantype_clean(v)
    std::list<int>
    
    CT_show_details(v)
    [std::list<int>] v = [1, 2, 3, 4, 5]
    


### Example with rvalue references
Fix this !


```c++
{
    auto foo = [](auto && v){
        std::cout << CT_show_details(v);
    };;
    int a = 1;
    foo(a);
}
```

    [int &] v = 1

### Examples with arguments pack


```c++
std::cout << cleantype::clean(1, "Hello") << std::endl;
std::cout << cleantype::clean<std::string, int, int &&, char &&>() << std::endl;
```

    int, char const( &)[6]
    std::string, int, int &&, char &&


### Configuration of the clean types

You can customize the suppressions and replacements inside [cleantype/cleantype_configuration.hpp](src/include/cleantype/cleantype_configuration.hpp)

## Full type names
* `cleantype::full<T...>()` is a function that will return a string containing
   the full type. It also works with packs of types. Use it with "cleantype::full<decltype(var)>()"<br/>
   It will add a reference by default so that "int v = 5; cleantype::full(v)" will return "int&". 
   Use the macro CT_cleantype_full() if you want to avoid this

* `cleantype::full<T...>(t...)` is a an easier version, using an instance of the type.<br/>
   Notes:
     * It will add a reference by default so that
        int v = 5; cleantype::full(v) will return "int&"
        => use the macro CT_cleantype_full() if you want to avoid this
     * It is not able to output correctly r-value references
         For this, use `cleantype::full<decltype(var)>()`

* `cleantype::show_details_full(T && v)` is a function that will return a string containing
   the full type of a variable, as well as its content

* `CT_cleantype_full(var)` is a macro that will also return the full type,
   but, it is able to also correctly display rvalue reference types.

*  `CT_show_details_full(var)` is a macro that will return a string containing the name,
   type and content of a variable (in this case, the underlying type of 'var'
   has to have an 'ostream & operator<<')

* `CT_show_details_full_cont` is a version of CT_show_details_full for complex containers
   like "std::map". "cont" stands for "container".




```c++
run_show(     cleantype::full(v)                         )
run_show(     cleantype::full<decltype(v)>()             )
run_show(     cleantype::show_details_full(v)            )
run_show(     CT_cleantype_full(v)                       )
run_show(     CT_show_details_full(v)                    )
```

    cleantype::full(v)
    std::__cxx11::list<int, std::allocator<int> > &
    
    cleantype::full<decltype(v)>()
    std::__cxx11::list<int, std::allocator<int> >
    
    cleantype::show_details_full(v)
    [std::__cxx11::list<int, std::allocator<int> > &] = [1, 2, 3, 4, 5]
    
    CT_cleantype_full(v)
    std::__cxx11::list<int, std::allocator<int> >
    
    CT_show_details_full(v)
    [std::__cxx11::list<int, std::allocator<int> >] v = [1, 2, 3, 4, 5]
    


### Full type versus readable type :
Full types can quickly become unreadable, especially with templated types, such as in the STL.


```c++
std::set<std::string> my_set { "Hello", "There"};
run_show(     cleantype::show_details_full(my_set)               )
run_show(     cleantype::show_details(my_set)                    )
```

    cleantype::show_details_full(my_set)
    [std::set<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > > &] = [Hello, There]
    
    cleantype::show_details(my_set)
    std::set<std::string> & = [Hello, There]
    


## Display the content of complex containers

* `CT_show_details_cont` (macro) is a version of CT_show_details for complex containers
   like "std::map". "cont" stands for "container".
* `CT_show_details_full_cont` enables to display the full type and content

These version are required for certains more complex containers, like "std::map". "cont" stands for "container".


```c++
std::map<std::string, int> my_map {{{"a", 1}, {"b", 2}, {"c", 3} }};
run_show(     CT_show_details_cont(my_map)                    )
run_show(     CT_show_details_full_cont(my_map)               )
```

    CT_show_details_cont(my_map)
    [std::map<std::string, int>] my_map = [(a, 1), (b, 2), (c, 3)]
    
    CT_show_details_full_cont(my_map)
    [std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<const std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int> > >] my_map = [(a, 1), (b, 2), (c, 3)]
    


# Identify the signature of non generic lambdas

* `cleantype::lambda_clean<LambdaFn>(LambdaFn fn)` is a function that will return a string containing
   the readable signature of a non generic lambda
* `cleantype::lambda_clean<LambdaFn>(LambdaFn fn)` is a function that will return a string containing
   the full signature of a non generic lambda

*  `CT_show_details_lambda(var)` is a macro that will return a string containing the
   readable signature of a lambda and its name

*  `CT_show_details_lambda_full(var)` is a macro that will return a string containing the
   full signature of a lambda and its name

It is not alway easy to guess the return type of lambda. See the lambda below for example : it's return type is not easy to guess:


```c++
int start = 5;
// what is the return type of this lambda ?
auto mystery_lambda = [&start](int end) {    
    return fplus::overlapping_pairs_cyclic( fplus::numbers(start, end) );
};;
```


```c++
// Let's see
run_show(            cleantype::lambda_clean(mystery_lambda)          );
run_show(            CT_show_details_lambda(mystery_lambda)           );
```

    cleantype::lambda_clean(mystery_lambda)
    lambda: (int) -> std::vector<std::pair<int, int>>
    
    CT_show_details_lambda(mystery_lambda)
    [lambda: (int) -> std::vector<std::pair<int, int>>] mystery_lambda
    



#### Note
If we try to get the type of this lambda via `CT_cleantype_full`, we do not get much information...


```c++
std::cout << cleantype::full<decltype(mystery_lambda)>();
```

    (lambda at input_line_18:4:23)

This is because "mystery_lambda" is actually a instance of a hidden class. We are actually looking for the signature of the operator() of this class. `type_lambda_clean` is able to extract the type of this operator and to display it in a readable way.

# Identify the signature of generic lambdas

* `cleantype::lambda_generic_clean<Args...>(LambdaFn fn)` is a function that will return a string containing
   the readable signature of a generic lambda. You will need to specify the type of the auto parameters
* `cleantype::lambda_generic_full<Args...>(LambdaFn fn)` is a function that will return a string containing
   the full signature of a generic lambda.
*  `CT_type_lamda_generic_fromparams_XXX(lambda, arg1, arg2, ...)` is a macro that will return a string containing the
    signature of a generic lambda where you do not specify the args type, instead you give example of these types.
    (XXX is the number of params of the lambda, and can vary from 1 to 5).

Note: this is an advanced feature, and it might fail on certain compilers, such as gcc (in which case, you will get no output).

### Example


```c++
auto add = [](auto a, auto b) {
    return a + b; 
};;
```

This lambda ("add") is a generic lambda (which means that at least one of its argument is specified with an "auto" type). It's behaviour is comparable to a template function.
So, if we try to get the signature via "cleantype::lambda_clean" we will run into an error. The solution is to provide the types of the input parameters:


```c++
// extract the lambda type using actual types
std::cout << cleantype::lambda_generic_clean<int, double>(add) << std::endl;
```

    lambda: (int, double) -> double


It can also be done by providing some example parameters : use `CT_type_lamda_generic_fromparams_XXX`, where X is the number of parameters of the lambda.


```c++
// extract the lambda type using example params
std::cout << CT_type_lamda_generic_fromparams_2(add, 1u, -2);
```

    lambda: (unsigned int, int) -> unsigned int

This second version is useful when you are lost in a forest of "auto" variables deep in the call stack, and you do not know the return type of the lambda, and you do not even know the type of the input parameters: in that case, if you have a working call example, then you can use it.

# Identify the return type of an `auto` function

Work in progress...


```c++
auto auto_fonction(int b) {
  std::vector<int> range = fplus::numbers(0, b);
  return fplus::pairs_to_map_grouped( fplus::overlapping_pairs_cyclic(range) );
};
```

# The zoo of type qualifiers
`cleantype`handles quite well `const`, `volatile`, references (`&`), rvalue references (`&&`), and pointers (`*`). See below a demonstration 


```c++
auto foo = [](auto && x) {
    run_show(     CT_cleantype_clean(x)                                );
    run_show(     cleantype::clean<decltype(x)>()                      );
};;

#define test_call_foo(var, title)                                        \
    std::cout << " *********** " << title << " ***********\n";           \
    std::cout << "Bare variable (before the call)\n";                    \
    run_show(     cleantype::clean<decltype(var)>()                    );\
    run_show(     CT_cleantype_clean(var)                              );\
    std::cout << "Now inside foo (pass by universal reference)\n";       \
    foo(var);
```


```c++
{
    int a = 5;  
    test_call_foo(a, "Calling with an int lvalue (transformed to a ref by the call)");
    
    int const &  b = a; 
    test_call_foo(b, "Calling with a const lvalue reference");
    
    test_call_foo(42, "Calling with an rvalue reference");
    
    int * const c = &a;
    test_call_foo(c, "Calling with a const pointer");

    int const * d = &a;
    test_call_foo(d, "Calling with a pointer to const");

    int volatile vol = 42;
    test_call_foo(vol, "Calling with a volatile int");
    
    int const e = 5;  
    test_call_foo(e, "Calling with a const");

}
```

     *********** Calling with an int lvalue (transformed to a ref by the call) ***********
    Bare variable (before the call)
    cleantype::clean<decltype(a)>()
    int
    
    CT_cleantype_clean(a)
    int
    
    Now inside foo (pass by universal reference)
    CT_cleantype_clean(x)
    int &
    
    cleantype::clean<decltype(x)>()
    int &
    
     *********** Calling with a const lvalue reference ***********
    Bare variable (before the call)
    cleantype::clean<decltype(b)>()
    const int &
    
    CT_cleantype_clean(b)
    const int &
    
    Now inside foo (pass by universal reference)
    CT_cleantype_clean(x)
    const int &
    
    cleantype::clean<decltype(x)>()
    const int &
    
     *********** Calling with an rvalue reference ***********
    Bare variable (before the call)
    cleantype::clean<decltype(42)>()
    int
    
    CT_cleantype_clean(42)
    int
    
    Now inside foo (pass by universal reference)
    CT_cleantype_clean(x)
    int &&
    
    cleantype::clean<decltype(x)>()
    int &&
    
     *********** Calling with a const pointer ***********
    Bare variable (before the call)
    cleantype::clean<decltype(c)>()
    int * const
    
    CT_cleantype_clean(c)
    int * const
    
    Now inside foo (pass by universal reference)
    CT_cleantype_clean(x)
    int * const &
    
    cleantype::clean<decltype(x)>()
    int * const &
    
     *********** Calling with a pointer to const ***********
    Bare variable (before the call)
    cleantype::clean<decltype(d)>()
    const int *
    
    CT_cleantype_clean(d)
    const int *
    
    Now inside foo (pass by universal reference)
    CT_cleantype_clean(x)
    const int * &
    
    cleantype::clean<decltype(x)>()
    const int * &
    
     *********** Calling with a volatile int ***********
    Bare variable (before the call)
    cleantype::clean<decltype(vol)>()
    volatile int
    
    CT_cleantype_clean(vol)
    volatile int
    
    Now inside foo (pass by universal reference)
    CT_cleantype_clean(x)
    volatile int &
    
    cleantype::clean<decltype(x)>()
    volatile int &
    
     *********** Calling with a const ***********
    Bare variable (before the call)
    cleantype::clean<decltype(e)>()
    const int
    
    CT_cleantype_clean(e)
    const int
    
    Now inside foo (pass by universal reference)
    CT_cleantype_clean(x)
    const int &
    
    cleantype::clean<decltype(x)>()
    const int &
    

