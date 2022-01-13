// SFINAE Test 
#include <stdio.h>
#include <memory>
#include <type_traits>
// #include <experimental/type_traits>


#define detect_idiom_name(structname, funcname) structname ## _ ## funcname
#define detect_idiom_bool(structname, funcname) structname ## _ ## funcname ## _v

// *************************************************************
// Detection idiom https://blog.tartanllama.xyz/detection-idiom/
// *************************************************************
#define check_support(func) \
    template <class... Ts> \
    using void_t = void; \
    \
    template <class Ts, class=void> \
    struct detect_idiom_name(supports, func) : std::false_type{}; \
    \
    /* general case, when the derived class doesn't have the searched for function */\
    template <class Ts> \
    struct detect_idiom_name(supports, func) <Ts, \
        void_t<decltype(std::declval<Ts>().func())>> \
    : std::true_type{}; \
    /* when the expression inside the decltype expression is valid then the template 
       argument will match => the whole type declaration matches => the template specialization 
       has std::true_type as base class */ \
    template <class ...V> \
    inline static constexpr bool detect_idiom_bool(supports, func) = \
        detect_idiom_name(supports, func)<V...>::value;

template <class D>
class MixIn {

    check_support(print_derived_specific)

    // *****************************************************************
    // Implementation version with experimental namespace implementation
    // *****************************************************************
    // Please comment out the "check_support" line above and the "#include <experimental/type_traits"
    // template <class T>
    // using print_derived_specific_t = decltype(std::declval<T>().print_derived_specific());

    // template <class V>
    // static constexpr bool supports_print_derived_specific_v = 
    //                     std::experimental::is_detected<print_derived_specific_t, V>::value;

    // *****************************************************************

    public:
        // functionality mixed in with calls to template class if it
        // has "print_derived_specific" implemented, otherwise call
        // default implementation
        void print_general() {
           // static_assert(supports_print_derived_specific_v<D> == true,
           //      "Derived doesn't have print_derived_specific member.");
            
           if constexpr(supports_print_derived_specific_v<D>) {
              puts("***");
              puts("print_general");
              get_derived().print_derived_specific();
              puts("***");
           } else {
               puts("***\nNo specific, just print general.\n***");
           }
        }

        // functionality mixed in without any dependency on the template class
        void print_other() {
            puts("print_other");
        }
    private:
        D &get_derived() {
            return static_cast<D &>(*this);
        }
};

class Derived: public MixIn<Derived>
{
public:
    void print() {
        puts("print");
    }
};

class DerivedAgain: public MixIn<DerivedAgain>
{
public:
    void print_derived_specific() {
        puts("print_derived_specific");
    }
};

int main(int argc, char* argv[]){
    puts("x:");
    Derived x;
    x.print(); // functionality offered by the class
    x.print_other(); // mixed-in functionality
    x.print_general(); // Doesn't work, but code compiles if not used

    puts("\ny:");
    DerivedAgain y;
    y.print_general(); // mixed-in functionality that calls 
                       // specific functionality from derived
    return 0;
}
