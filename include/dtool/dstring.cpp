#include "dstring.h"

String::CharProxy::CharProxy(std::string& v) : vec(v) {}

String::CharProxy& String::CharProxy::operator=(const std::string& str) {
    if (str.empty() || str.size() > 4) {
        throw std::invalid_argument("Invalid UTF-8 character assignment");
    }
    unsigned char c = str[0];
    size_t len = 1;
    if ((c & 0b10000000) == 0) len = 1;
    else if ((c & 0b11100000) == 0b11000000) len = 2;
    else if ((c & 0b11110000) == 0b11100000) len = 3;
    else if ((c & 0b11111000) == 0b11110000) len = 4;
    else throw std::invalid_argument("Invalid UTF-8 sequence");
    if (str.size() != len)
        throw std::invalid_argument("Input string is not a valid UTF-8 character");
    vec = str;
    return *this;
}

String::CharProxy::operator const std::string&() const {
    return vec;
}

// String 类方法实现
void String::pop_back() {
    if (Vec.empty()) throw std::out_of_range("String is empty, cannot pop_back");
    Vec.pop_back();
}

void String::push_back(const std::string& str) {
    this->operator+=(str);
}

void String::clear() {
    Vec.clear();
}

void String::swap(String& other) {
    Vec.swap(other.Vec);
}

String::CharProxy String::front() {
    if (Vec.empty()) throw std::out_of_range("String is empty, cannot access front");
    return CharProxy(Vec[0]);
}

String::CharProxy String::back() {
    if (Vec.empty()) throw std::out_of_range("String is empty, cannot access back");
    return CharProxy(Vec[Vec.size() - 1]);
}

String::CharProxy String::operator[](int i) {
    if (i < 0 || i >= (int)Vec.size()) throw std::out_of_range("Index out of range");
    return CharProxy(Vec[i]);
}

String String::operator+(const char* str) const {
	String result;
	result+=str;
	return result;
}

String String::operator+(const String& other) const {
    String result;
    result.Vec.insert(result.Vec.end(), Vec.begin(), Vec.end());
    result.Vec.insert(result.Vec.end(), other.Vec.begin(), other.Vec.end());
    return result;
}

String String::operator+(const std::string& str) const {
    String result;
    result.Vec.insert(result.Vec.end(), Vec.begin(), Vec.end());
    for (size_t i = 0; i < str.size();) {
        unsigned char c = str[i];
        size_t len = 1;
        if ((c & 0b10000000) == 0) len = 1;
        else if ((c & 0b11100000) == 0b11000000) len = 2;
        else if ((c & 0b11110000) == 0b11100000) len = 3;
        else if ((c & 0b11111000) == 0b11110000) len = 4;
        else throw std::runtime_error("Invalid UTF-8 sequence");
        if (i + len > str.size()) throw std::runtime_error("Truncated UTF-8 character");
        result.Vec.push_back(str.substr(i, len));
        i += len;
    }
    return result;
}

String operator+(const std::string& str, const String& other){
	String result=String(str)+other;
	return result;
}

String& String::operator+=(const String& other) {
	Vec.insert(Vec.end(), other.Vec.begin(), other.Vec.end());
	return *this;
}

String& String::operator+=(const std::string& other){
	*this=*this+other;
	return *this;
}
String& String::operator+=(const char* str) {
	std::string s = str;
	this->operator+=(s);
	return *this;
}

String& String::operator=(std::string& str) {
    clear();
    this->operator+=(str);
    return *this;
}
String& String::operator=(const char* str) {
	clear();
	this->operator+=(static_cast<std::string>(str));
	return *this;
}

std::istream& operator>>(std::istream& is, String& str) {
    std::string input;
    is >> input;
    str = input;
    return is;
}

std::ostream& operator<<(std::ostream& os, const String& str) {
    for (const auto& s : str.Vec) os << s;
    return os;
}

// 常用接口实现
size_t String::size() const {
    return Vec.size();
}

bool String::empty() const {
    return Vec.empty();
}

std::string String::substr(size_t pos, size_t len) const {
    if (pos >= Vec.size()) throw std::out_of_range("Position out of range");
    std::string result;
    for (size_t i = pos; i < pos + len && i < Vec.size(); i++) result += Vec[i];
    return result;
}

size_t String::find(const std::string& str, size_t pos) const {
    for (size_t i = pos; i < Vec.size(); i++) {
        if (Vec[i] == str) return i;
    }
    return std::string::npos;
}

void String::replace(size_t pos, size_t len, const String& str) {
    if (pos >= Vec.size()) throw std::out_of_range("Position out of range");
    size_t end = std::min(pos + len, Vec.size());
    Vec.erase(Vec.begin() + pos, Vec.begin() + end);
    Vec.insert(Vec.begin() + pos, str.Vec.begin(), str.Vec.end());
}

void String::insert(size_t pos, const std::string& str) {
    if (pos >= Vec.size()) throw std::out_of_range("Position out of range");
    Vec.insert(Vec.begin() + pos, str);
}

void String::erase(size_t pos, size_t len) {
    if (pos >= Vec.size()) throw std::out_of_range("Position out of range");
    size_t end = std::min(pos + len, Vec.size());
    Vec.erase(Vec.begin() + pos, Vec.begin() + end);
}

const char* String::c_str() const {
    cached_result.clear();
	for (const auto& s : Vec) cached_result += s;
    return cached_result.c_str();
}

// 迭代器类的实现
String::iterator::iterator(typename std::vector<std::string>::iterator iter) : it(iter) {}

String::CharProxy String::iterator::operator*() {
    return CharProxy(*it); // 使用 CharProxy
}

String::iterator& String::iterator::operator++() {
    ++it;
    return *this;
}

String::iterator String::iterator::operator++(int) {
    iterator temp = *this;
    ++(*this);
    return temp;
}

String::iterator& String::iterator::operator--() {
    --it;
    return *this;
}

String::iterator String::iterator::operator--(int) {
    iterator temp = *this;
    --(*this);
    return temp;
}

bool String::iterator::operator!=(const iterator& other) const {
    return it != other.it;
}

bool String::iterator::operator==(const iterator& other) const {
    return it == other.it;
}

String::iterator String::iterator::operator+(int n) const {
    iterator temp = *this;
    temp.it += n;
    return temp;
}

String::iterator String::iterator::operator-(int n) const {
    iterator temp = *this;
    temp.it -= n;
    return temp;
}

int String::iterator::operator-(const iterator& other) const {
    return it - other.it;
}

typename std::vector<std::string>::iterator String::iterator::get_it() const {
    return it;
}

// `begin()` 和 `end()` 实现
String::iterator String::begin() {
    return iterator(Vec.begin());
}

String::iterator String::end() {
    return iterator(Vec.end());
}

// operator std::string() 实现
String::operator std::string() const {
    std::string result;
    for (const auto& s : Vec) result += s;
    return result;
}