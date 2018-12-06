#include "params.h"

#include <assert.h>

namespace quink {

bool Params::SetBoolean(const std::string &key, bool b)
{
    const auto search = mBoolParams.find(key);
    if (search == mBoolParams.end())
        return false;
    return search->second(b);
}

bool Params::SetInt(const std::string &key, int n)
{
    const auto search = mIntParams.find(key);
    if (search == mIntParams.end())
        return false;
    return search->second(n);
}

bool Params::SetAddress(const std::string &key, void *p)
{
    const auto search = mAddrParams.find(key);
    if (search == mAddrParams.end())
        return false;
    return search->second(p);
}

bool Params::SetDouble(const std::string &key, double d)
{
    const auto search = mDoubleParams.find(key);
    if (search == mDoubleParams.end())
        return false;
    return search->second(d);
}

bool Params::SetString(const std::string &key, const std::string &str)
{
    const auto search = mStrParams.find(key);
    if (search == mStrParams.end())
        return false;
    return search->second(str);
}

void Params::RegisterBoolean(const std::string &key, std::function<bool(bool)> callback)
{
    assert(mBoolParams.find(key) == mBoolParams.end());
    mBoolParams[key] = callback;
}

void Params::RegisterInt(const std::string &key, std::function<bool(int)> callback)
{
    assert(mIntParams.find(key) == mIntParams.end());
    mIntParams[key] = callback;
}

void Params::RegisterAddress(const std::string &key, std::function<bool(void*)> callback)
{
    assert(mAddrParams.find(key) == mAddrParams.end());
    mAddrParams[key] = callback;
}

void Params::RegisterDouble(const std::string &key, std::function<bool(double)> callback)
{
    assert(mDoubleParams.find(key) == mDoubleParams.end());
    mDoubleParams[key] = callback;
}

void Params::RegisterString(const std::string &key, std::function<bool(std::string)> callback)
{
    assert(mStrParams.find(key) == mStrParams.end());
    mStrParams[key] = callback;
}

} // namespace

#ifdef TEST_PARAMS
#include <stdio.h>

using namespace quink;
static void Test();

class Foo {
public:
    Foo() {
        Config.RegisterBoolean("bool", [this](bool b) { this->b = b; return true; });
        Config.RegisterInt("int", [this](int n)
                {
                    if (n < 0 || n > 100)
                        return false;
                    this->n = n; return true;
                });
        Config.RegisterAddress("addr", [this](void *p) { this->p = p; return true; });
        Config.RegisterDouble("double", [this](double d) { this->d = d; return true; });
        Config.RegisterString("string", [this](const std::string &str)
                { this->str = str; return true; });
    }

    virtual ~Foo() = default;

    void Dump() {
        printf("b = %d, n = %d, p = %p, d = %.2f, str = %s\n", b, n, p, d, str.c_str());
    }

    bool SetBoolean(const std::string &key, bool b) {
        return Config.SetBoolean(key, b);
    }

    bool SetInt(const std::string &key, int n) {
        return Config.SetInt(key, n);
    }

    bool SetAddress(const std::string &key, void *p) {
        return Config.SetAddress(key, p);
    }

    bool SetDouble(const std::string &key, double d) {
        return Config.SetDouble(key, d);
    }

    bool SetString(const std::string &key, const std::string &str) {
        return Config.SetString(key, str);
    }

protected:
    Params Config;

private:
    bool b = false;
    int n = 0;
    void *p = nullptr;
    double d = 0.0;
    std::string str;
};

class Bar : public Foo {
public:
    Bar() {
        Config.RegisterString("name", [this](const std::string &str)
                {
                    name = str;
                    return true;
                });
    }

    std::string name;
};

static void Test() 
{
    Foo foo;
    foo.Dump();

    bool ret;

    ret = foo.SetBoolean("bool", true);
    assert(ret == true);
    ret = foo.SetBoolean("boo", true);
    assert(ret == false);

    ret = foo.SetInt("int", 123);
    assert(ret == false);

    ret = foo.SetInt("int", 50);
    assert(ret == true);

    foo.SetAddress("addr", &foo);
    foo.SetDouble("double", 3.14);
    foo.SetString("string", "hello");

    foo.Dump();

    Bar *bar = new Bar();
    bar->SetString("name", "foo");
    printf("%s\n", bar->name.c_str());
    bar->SetString("name", "bar");
    printf("%s\n", bar->name.c_str());
    delete bar;
}

int main()
{
    Test();
    return 0;
}
#endif
