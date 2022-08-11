
#ifndef ropto_Optional_hpp
#define ropto_Optional_hpp

#include <type_traits>

namespace ropto
{
    template<class T>
    class optional
    {
    private:
        T __value;
        bool _has_value;
    public:
        optional(): __value{}, _has_value{false} {}
        optional(const T& value): __value{value}, _has_value{true} {}
        
        
        explicit operator bool() const
        {
            return _has_value;
        }
        
        optional<T>& operator=(const T& value)
        {
            _has_value = true;
            __value = value;
        }
        
        optional(const optional<T>& value)
        {
            _has_value = value._has_value;
            if (_has_value)
                __value = value.__value;
        }
        
        optional<T>& operator=(optional<T> value)
        {
            swap(value);
            return * this;
        }
        
        void swap(optional<T>& value)
        {
            std::swap(_has_value, value._has_value);
            std::swap(__value,    value.__value);
        }
        
        T value() const
        {
            return __value;
        }
        
        T& value_or(const T& defaults) const
        {
            return _has_value ? __value : defaults;
        }
    };
}

#endif
