#include "jlcxx/jlcxx.hpp"

#include "glasskey.h"

using namespace gk;

JLCXX_MODULE define_julia_module(jlcxx::Module& mod)
{
    mod.method("init", &init);
    mod.method("start", &start);
    mod.method("stop", &start);

    mod.add_type<Color>("Color")
        .constructor<std::float_t, std::float_t, std::float_t>();

        
    mod.method("from_bytes", &Color::from_bytes);
    mod.set_const("Black", Colors::Black);

    mod.add_type<Rect>("Rect")
        .constructor<Index, Index, Size, Size>()
        .method("move", &Rect::move)
        .method("translate", &Rect::translate)
        .method("clip", &Rect::clip);

}