#ifndef MABIRACE_H_100
#define MABIRACE_H_100

#include <string>
using namespace std;

class mabiRace
{
public:
	mabiRace() = default;
	~mabiRace() = default;
	void setRaceId(string& raceId) { m_raceId = raceId; }
	void setClassName(string& className) { m_className = className; }
	void setLocalName(wstring& localName) { m_localName = localName; }
	void setPickUps(wstring& pickUps) { m_pickUps = pickUps; }

protected:
	string m_raceId;
	string m_className;
	wstring m_localName;
	wstring m_pickUps;
};

#endif // !MABIRACE_H_100
