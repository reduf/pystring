# Motivations

Strings are a fundamental structure used in every system languages. C++ offers
the [std::string](https://en.cppreference.com/w/cpp/string/basic_string) from
the STL which is a mutable heap allocated string. Heap allocated strings make
it very easy to have any strings of any size and mutability is arguably the
most natural approach in a language like C++. That said, heap + mutability comes
with issues such as performance. In my experience, the heap + mutability is
almost always a bad decision, because it doesn't cover my usual cases. Indeed,
when dealing with strings, I see 4 cases that comes very often.

The first case was recently solved by [string view](https://en.cppreference.com/w/cpp/header/string_view)
and it's the situation when there is no ownership of the string. It's very cheap,
because it's simply a pointer length tuple.

The second case is the string with a known limit. This can similarly be very
easily solved using `char[]`. Arguably, the better way to solve it would be to
use a `char[]`, `size_t` tuple and not rely on nul termination.

The third case is the string builder. Interestingly, `std::string` is a very
good candidate for this problem. It lack some functionalities, like appending
integers, but [std::stringstream](https://en.cppreference.com/w/cpp/io/basic_stringstream)
is a wrapper on top of `std::string` that provide those functionalities. Other
interesting solutions or improvements exist, for instance a `std::string` with
a stack allocator.

The last case that I often see are strings that do not change, but that are
copied. This is the worst case for `std::string` and there is a very elegant
solution to this case. Immutable heap strings. The immutability allow you to
share those string when combine with a reference count, the immutability also
gives you thread safety and finally the copy boils down to incrementing an
atomic word which is extremely cheap. There is no support for immutable heap
strings, so this is what this library want to offer.

In addition to providing referenced counted heap string, I also want to provide
a better API. For this reason, the API is heavily inspired from Python who
also use reference counted immutable heap string. In my opinion, Python's API
is extremely elegant and easy to use. Moreover, the function can be strict with
respect to the encoding, the same way Python 3 strings are.
