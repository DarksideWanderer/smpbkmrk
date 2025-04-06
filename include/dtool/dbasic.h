#pragma once
#include<bits/stdc++.h>
template<typename... Args>
std::istream& InPut(Args&...x){return (std::cin>>...>>x);}
template<typename... Args>
std::ostream& OutPut(const Args&... x){return(std::cout<<...<<x);}
template<typename...Args>
std::ostream& ErrPut(const Args&...x){return (std::cerr<<...<<x);}
template<typename T>
void clearContainer(T&x){T y;x.swap(y);}