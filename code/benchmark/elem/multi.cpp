
struct yes { };
struct no { };

template <typename x>
struct no_decay { using type = x; };

template <typename ...xs>
struct inherit : xs... { };


template <typename key>
static no lookup(...);

template <typename key>
static yes lookup(no_decay<key>*);

template <typename key, typename ...keys>
using elem = decltype(::lookup<key>((inherit<no_decay<keys>...>*)nullptr));


template <int> struct x;
<% xs = (1..n+1).map { |i| "x<#{i}>" }.join(', ') %>

using r = elem<x<0>, <%= xs %>>;

int main() { }
