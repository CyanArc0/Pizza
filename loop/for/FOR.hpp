#pragma once
#define $included$loop$for$FOR 3.0

#include<utility>

#define FOR($begin,$end) for(auto[_iterator_,_sentinel_]=std::make_pair(($begin),($end));_iterator_!=_sentinel_;++_iterator_)