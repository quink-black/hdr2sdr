#pragma once

#include <stdint.h>
#include <functional>
#include <map>
#include <string>

namespace quink {

class Params {
public:
    bool SetBoolean(const std::string &key, bool b);
    bool SetInt(const std::string &key, int n);
    bool SetAddress(const std::string &key, void *p);
    bool SetDouble(const std::string &key, double d);
    bool SetString(const std::string &key, const std::string &str);

    // These methods can be protected, but then you're forced to use inheritance.
    // Composition is more flexible and simple.
    void RegisterBoolean(const std::string &key, std::function<bool(bool)> callback);
    void RegisterInt(const std::string &key, std::function<bool(int)> callback);
    void RegisterAddress(const std::string &key, std::function<bool(void*)> callback);
    void RegisterDouble(const std::string &key, std::function<bool(double)> callback);
    void RegisterString(const std::string &key, std::function<bool(std::string)> callback);

private:
    std::map<std::string, std::function<bool(bool)>> mBoolParams;
    std::map<std::string, std::function<bool(int)>> mIntParams;
    std::map<std::string, std::function<bool(void*)>> mAddrParams;
    std::map<std::string, std::function<bool(double)>> mDoubleParams;
    std::map<std::string, std::function<bool(const std::string&)>> mStrParams;
};

}
