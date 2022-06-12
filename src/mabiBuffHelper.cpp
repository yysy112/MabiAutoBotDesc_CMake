#include "mabiBuffHelper.h"

mabiBuffHelper mabiBuffHelper::operator+ (const mabiBuffHelper& other){
    mabiBuffHelper result;
    auto thisBuffList = getBuffs();
    auto otherBuffList = other.getBuffs();
    for (const auto& buffIterator : thisBuffList){
        string _buffKey = buffIterator.first;
        int _thisBuffVal = buffIterator.second;
        int _otherBuffVal = 0;
        for (const auto& otherBuffIterator : otherBuffList){
            if (otherBuffIterator.first == _buffKey){
                _otherBuffVal = otherBuffIterator.second;
            }
        }
        result.appendBuff(make_pair(_buffKey, _thisBuffVal + _otherBuffVal));
    }
    string thisConditions = getConditions();
    string otherConditions = other.getConditions();
    if (thisConditions.length() > 0 && otherConditions.length() > 0){
        int _index = otherConditions.find_first_of(':');
        auto conditionKey = otherConditions.substr(0, _index);
        int __index = thisConditions.find_first_of(':');
        auto thisConditionKey = thisConditions.substr(0, __index);
        if (conditionKey == thisConditionKey) {
            int otherConditionVal = atoi(otherConditions.substr(_index + 1).c_str());
            int thisConditionVal = atoi(m_Conditions.second.substr(__index + 1).c_str());
            thisConditionVal += otherConditionVal;
            string resConditions = thisConditionKey + ":" + to_string(thisConditionVal);
            result.setCondition(resConditions);
        }
        else {
            result.setCondition(thisConditions + ";" + otherConditions);
        }
    }
    else{
        string resConditions = thisConditions.length() > 0 ? thisConditions : otherConditions;
        result.setCondition(resConditions);
    }
    return result;
}

void mabiBuffHelper::operator+= (const mabiBuffHelper& other){
    auto otherBuffList = other.getBuffs();
    for (auto& buffIterator : m_buffs){
        string _buffKey = buffIterator.first;
        for (const auto& otherBuffIterator : otherBuffList){
            if (otherBuffIterator.first == _buffKey){
                buffIterator.second += otherBuffIterator.second;
                break;
            }
        }
    }
    string otherConditions = other.getConditions();
    if (otherConditions.length() > 0){
        int _index = otherConditions.find_first_of(':');
        auto conditionKey = otherConditions.substr(0, _index);
        int __index = m_Conditions.second.find_first_of(':');
        auto thisConditionKey = m_Conditions.second.substr(0, __index);
        if (conditionKey == thisConditionKey) {
            int otherConditionVal = atoi(otherConditions.substr(_index + 1).c_str());
            int thisConditionVal = atoi(m_Conditions.second.substr(__index + 1).c_str());
            thisConditionVal += otherConditionVal;
            string resConditions = thisConditionKey + ":" + to_string(thisConditionVal);
            setCondition(resConditions);
        }
        else {
            string resConditions = m_Conditions.second.length() > 0 ? m_Conditions.second + ";" + otherConditions : otherConditions;
            setCondition(resConditions);
        }
    }
}

wstring mabiBuffHelper::toWString(unordered_map<string,wstring>& buffToStr){
    wstring result = L"";
    for (const auto& buffIterator : m_buffs){
        string _buffKey = buffIterator.first;
        int _buffVal = buffIterator.second;
        if (_buffVal > 0){
            wstringstream wss;
            wss << buffToStr[_buffKey] << L" " << _buffVal << L" 增加&#xA;";
            result += wss.str();
        }
        else if (_buffVal < 0){
            wstringstream wss;
            wss << buffToStr[_buffKey] << L" " << _buffVal << L" 减少&#xA;";
            result += wss.str();
        }
    }
    if (m_Conditions.second.length() > 0){
        auto conditionVec = stringSplit(m_Conditions.second, ';');
        for (const auto& conditionIterator : conditionVec){
            int _index = conditionIterator.find_first_of(':');
            string _conditionKey = conditionIterator.substr(0, _index);
            string _conditionVal = conditionIterator.substr(_index + 1);
            wstringstream wss;
            wss << buffToStr[_conditionKey] << L" " << to_wide_string(_conditionVal) << L" 增加&#xA;";
            result += wss.str();
        }
    }
    return result;
}
