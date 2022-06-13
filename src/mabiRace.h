#ifndef MABIRACE_H_100
#define MABIRACE_H_100

#include <string>
#include <list>
using namespace std;

static void _appendPickUp(list<string>& picks, string pick) {
	for (auto it = picks.begin(); it != picks.end(); ++it) {
		if (*it == pick)
			return;
	}
	picks.push_back(pick);
}

static void _appendPickUp(list<string>& picks, list<string>& otherPicks) {
	for (auto it = otherPicks.begin(); it != otherPicks.end(); ++it) {
		_appendPickUp(picks, *it);
	}
}

class mabiRace
{
public:
	mabiRace() = default;
	~mabiRace() = default;
	void setRaceId(string raceId) { m_raceId = raceId; }
	void setClassName(string className) { m_className = className; }
	void setLocalName(wstring localName) { m_localName = localName; }
	void appendPickUps(string pickUp) { _appendPickUp(m_pickUps, pickUp); }
	void appendPickUps(list<string>& otherPickUps) { _appendPickUp(m_pickUps, otherPickUps); }

protected:
	string m_raceId;
	string m_className;
	wstring m_localName;
	list<string> m_pickUps;
};

#endif // !MABIRACE_H_100