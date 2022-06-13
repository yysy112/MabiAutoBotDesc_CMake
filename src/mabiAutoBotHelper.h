#ifndef MABIAUTOBOTHELPER_H_100
#define MABIAUTOBOTHELPER_H_100

#include "tinyxml2.h"
#include "mabiBuffHelper.h"

using namespace tinyxml2;
using namespace std;

class mabiAutoBotHelper {
public:
	mabiAutoBotHelper();
	~mabiAutoBotHelper();

private:
	void _loadResources();
	void _loadLibrary();
	void _parseRace();
    void _parseAutoBot();
    void _parseBuff();
    void _parseGroupBuff();
	void _parseBotSetBuff();
	void _saveToXml(XMLDocument* dataBase, int flag);
	void _boxAndCoupons();
	string m_resourceDir = RESOURCE_DIR;
	XMLDocument* m_itemDb;
	XMLDocument* m_itemEtc;
	XMLDocument* m_itemScript;
	XMLDocument* m_race;
	XMLDocument* m_autoBot;
	unordered_map<string, wstring> m_itemDbContent;
	unordered_map<string, wstring> m_itemEtcContent;
	unordered_map<string, wstring> m_itemScriptContent;
	unordered_map<string, wstring> m_raceContent;
	unordered_map<string, wstring> m_botPickUpContent;
	unordered_map<string, wstring> m_botBuffContent;
	unordered_map<string, wstring> m_botNameContent;
	unordered_map<string, mabiAutobot> m_autoBots;
    unordered_map<int, mabiAutobot> m_autoBots_itemdb;
	unordered_map<int, mabiAutobot> m_itemIdToAutoBots;
    unordered_map<int, mabiBuffHelper> m_buffHelpers;
    unordered_map<int, int> m_botIdToBuffId;
    unordered_map<string, int> m_botStrIdToBuffId;
	unordered_map<string, wstring> m_boxAndCoupons;
};

#endif // !MABIAUTOBOTHELPER_H_100
