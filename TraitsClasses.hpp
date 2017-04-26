#ifndef TRAITSCLASSES_HPP
#define TRAITSCLASSES_HPP

struct color_rgb_tag {};
struct color_mono_tag {};

template<typename T, template<typename> class colorT>
struct color_traits {
    typedef typename colorT<T>::color_space_category color_space_category;
};

#endif // TRAITSCLASSES_HPP
