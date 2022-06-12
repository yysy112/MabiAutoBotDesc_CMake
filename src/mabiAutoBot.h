#ifndef MABIAUTOBOT_H_100
#define MABIAUTOBOT_H_100

#include "mabiRace.h"
using namespace std;

class mabiAutobot : public mabiRace
{
public:
	mabiAutobot() = default;
	~mabiAutobot() = default;
    void setBotId(int botId) { m_botId = botId; }
    void setSize(string& size) { m_size = size; }
    void setVolume(string volume) { m_volume = volume; }
    void setBotName(wstring botName) { m_botName = botName; }
    void setBuffId(int buffId) { m_buffId = buffId; }
    void setBuff(wstring& buff) { m_buff = buff; }
    void setGroup(string& group) { m_group = group; }
    void setGroupBuffId(int groupBuffId) { m_groupBuffId = groupBuffId; }
    void setGroupBuff(wstring& groupBuff) { m_groupBuff = groupBuff; }
    void setTime(string time) { m_time = time; }
    void setWeight(string& weight) { m_weight = weight; }
    void setRatio(string ratio) { m_ratio = ratio; }
    void setItemDbId(string id) { m_itemDbId = id; }
    void appendNickName(wstring nickName) { m_nickNames.push_back(nickName); }
    
    int buffId() const {return m_buffId;}
    int groupBuffId() const {return m_groupBuffId;}
    string groupStr() const {return m_group;}
    string weight() const { return m_weight; }
    wstring buff() const { return m_buff; }
    wstring groupBuff() const { return m_groupBuff; }
    wstring pickUp() const{ return m_pickUps; }
    string summonTime() const { return m_time; }
    string itemDbId() const { return m_itemDbId; }
    wstring botName() const { return m_botName; }
    list<wstring> nickNames() const { return m_nickNames; }
protected:
    int m_botId;
    string m_size;
    string m_volume;
    wstring m_botName;
    list<wstring> m_nickNames;
    int m_buffId;
    wstring m_buff;
    string m_group;
    int m_groupBuffId;
    wstring m_groupBuff;
    string m_time;
    string m_weight;
    string m_ratio;
    string m_itemDbId;
};

#endif // !MABIAUTOBOT_H_100
