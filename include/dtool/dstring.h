#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

class String {
public:
	String() = default;
	String(const std::string& str) {clear(); this->operator+=(str);}
	String(const char* str) {clear(); this->operator+=(static_cast<std::string>(str));}
	
    struct CharProxy {
        std::string& vec;
        CharProxy(std::string& v);
        CharProxy& operator=(const std::string& str);
        operator const std::string&() const;
    };

    std::vector<std::string> Vec;

    void pop_back();
    void push_back(const std::string& str);
    void clear();
    void swap(String& other);
    
    CharProxy front();
    CharProxy back();
    CharProxy operator[](int i);

    String operator+(const String& other) const;
    String operator+(const std::string& str) const;
    String operator+(const char*str) const;
	String& operator+=(const String& other);
	String& operator+=(const std::string& other);
	String& operator+=(const char*str);
    friend String operator+(const std::string& str, const String& other);

    String& operator=(std::string& str);
    String& operator=(const char*str);
    friend std::istream& operator>>(std::istream& is, String& str);
    friend std::ostream& operator<<(std::ostream& os, const String& str);

    // 常用接口
    size_t size() const;
    bool empty() const;
    std::string substr(size_t pos, size_t len) const;
    size_t find(const std::string& str, size_t pos = 0) const;
    void replace(size_t pos, size_t len, const String& str);
    void insert(size_t pos, const std::string& str);
    void erase(size_t pos, size_t len);
	
	mutable std::string cached_result;
    const char* c_str() const;

    struct iterator {
        typename std::vector<std::string>::iterator it;
        iterator(typename std::vector<std::string>::iterator iter);
        
        CharProxy operator*();
        iterator& operator++();
        iterator operator++(int);
        iterator& operator--();
        iterator operator--(int);
        bool operator!=(const iterator& other) const;
        bool operator==(const iterator& other) const;
        iterator operator+(int n) const;
        iterator operator-(int n) const;
        int operator-(const iterator& other) const;
        typename std::vector<std::string>::iterator get_it() const;
    };

    operator std::string() const;
    iterator begin();
    iterator end();
};