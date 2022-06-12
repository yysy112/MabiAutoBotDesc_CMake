#ifndef MABIBUFFHELPER_H_100
#define MABIBUFFHELPER_H_100

#include <list>
#include <unordered_map>
#include <cassert>
#include <fstream>
#include <codecvt>
#include <regex>
#include <iostream>
#include <list>
#include <sstream>
#include <vector>
#include <iomanip>
#include "mabiAutoBot.h"

using namespace std;

static vector<string> stringSplit(const string& str, char delim) {
    stringstream ss(str);
    string item;
    vector<string> elems;
    while (getline(ss, item, delim)) {
        if (!item.empty()) {
            elems.push_back(item);
        }
    }
    return elems;
}

static wstring to_wide_string(const string& input)
{
    wstring_convert<codecvt_utf8<wchar_t> > converter;
    return converter.from_bytes(input);
}

static string to_normal_string(const wstring& input)
{
    wstring_convert<codecvt_utf8<wchar_t> > converter;
    return converter.to_bytes(input);
}

class mabiBuffHelper{
public:
    mabiBuffHelper() = default;
    ~mabiBuffHelper() = default;
    void setId(int _id) {m_id.second = _id;}
    void setLayer(int layer) {m_layer.second = layer;}
    void setOverLapped(int overlapped) {m_overlapped.second = overlapped;}
    void setCondition(string condition) {m_Conditions.second = condition;}
    void appendBuff(pair<string, int> buffPair) {m_buffs.push_back(buffPair);}
    list<pair<string, int> > getBuffs() const {return m_buffs;}
    
    mabiBuffHelper operator+ (const mabiBuffHelper& other);
    void operator+= (const mabiBuffHelper& other);
    wstring toWString(unordered_map<string,wstring>& buffToStr);
    bool isOverlapped() {return m_overlapped.second == 1;}
    string getConditions() const {return m_Conditions.second;}
private:
    pair<string,int> m_id = pair<string,int>("Id", 0);
    pair<string,int> m_layer = pair<string,int>("Layer", 0);
    pair<string,int> m_overlapped = pair<string,int>("Overlapped", 0);
    list<pair<string, int> > m_buffs;
    pair<string,string> m_Conditions = pair<string,string>("Conditions", "");
};

#endif // !MABIBUFFHELPER_H_100
