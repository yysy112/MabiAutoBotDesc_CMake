#include "mabiAutoBotHelper.h"

#ifdef __APPLE__
inline void strupr(char* s)
{
    while (*s != '\0')
    {
        if (*s >= 'a' && *s <= 'z')
            *s -= 32;
        s++;
    }
}
#endif

mabiAutoBotHelper::mabiAutoBotHelper()
{
	m_itemDb = new XMLDocument();
	m_itemEtc = new XMLDocument();
	m_race = new XMLDocument();
	m_autoBot = new XMLDocument();
	m_itemScript = new XMLDocument();
	_loadResources();
	_loadLibrary();
	_parseRace();
    _parseBuff();
	_parseAutoBot();
    _parseGroupBuff();
	_parseBotSetBuff();
	_saveToXml(m_itemDb, 0);
	_saveToXml(m_itemEtc, 1);
	_saveToXml(m_itemScript, 2);
	_boxAndCoupons();
}

mabiAutoBotHelper::~mabiAutoBotHelper()
{
	delete m_itemDb;
	delete m_itemEtc;
	delete m_race;
	delete m_autoBot;
	delete m_itemScript;
}

/// 加载官方资源文件，如果是 utf-16 格式需先手动转成 utf-8
void mabiAutoBotHelper::_loadResources()
{
	m_itemDb->LoadFile((m_resourceDir + "ITEMDB.xml").c_str());
	assert( m_itemDb->Error() == false);

	m_itemEtc->LoadFile((m_resourceDir + "ITEMDB_ETC.xml").c_str());
	assert(m_itemEtc->Error() == false);

	m_itemScript->LoadFile((m_resourceDir + "ITEMDB_SCRIPT.xml").c_str());
	assert(m_itemScript->Error() == false);

	m_race->LoadFile((m_resourceDir + "RACE.xml").c_str());
	assert(m_race->Error() == false);

	m_autoBot->LoadFile((m_resourceDir + "AUTOBOTDESC.xml").c_str());
	assert(m_autoBot->Error() == false);
}

/// 加载翻译文本
void mabiAutoBotHelper::_loadLibrary()
{
	ifstream ifs;
	string _tmpStr;
	ifs.open((m_resourceDir + "ItemDB.china.txt").c_str(), ios::in);
	assert(ifs.is_open() == true);
	while (getline(ifs, _tmpStr)) {
		auto _index = _tmpStr.find_first_of('\t');
		auto _key = _tmpStr.substr(0, _index);
		auto _value = _tmpStr.substr(_index+1);
		m_itemDbContent.insert(make_pair(_key, to_wide_string(_value)));
	}
	ifs.close();

	ifs.open((m_resourceDir + "itemdb_etc.china.txt").c_str(), ios::in);
	assert(ifs.is_open() == true);
	while (getline(ifs, _tmpStr)) {
		auto _index = _tmpStr.find_first_of('\t');
		auto _key = _tmpStr.substr(0, _index);
		auto _value = _tmpStr.substr(_index + 1);
		m_itemEtcContent.insert(make_pair(_key, to_wide_string(_value)));
	}
	ifs.close();

	ifs.open((m_resourceDir + "itemdb_script.china.txt").c_str(), ios::in);
	assert(ifs.is_open() == true);
	while (getline(ifs, _tmpStr)) {
		auto _index = _tmpStr.find_first_of('\t');
		auto _key = _tmpStr.substr(0, _index);
		auto _value = _tmpStr.substr(_index + 1);
		m_itemScriptContent.insert(make_pair(_key, to_wide_string(_value)));
	}
	ifs.close();

	ifs.open((m_resourceDir + "Race.china.txt").c_str(), ios::in);
	assert(ifs.is_open() == true);
	while (getline(ifs, _tmpStr)) {
		auto _index = _tmpStr.find_first_of('\t');
		auto _key = _tmpStr.substr(0, _index);
		auto _value = _tmpStr.substr(_index + 1);
		m_raceContent.insert(make_pair(_key, to_wide_string(_value)));
	}
	ifs.close();

	ifs.open((m_resourceDir + "autobot_pick.txt").c_str(), ios::in);
	assert(ifs.is_open() == true);
	while (getline(ifs, _tmpStr)) {
		auto _index = _tmpStr.find_first_of('=');
		auto _key = _tmpStr.substr(0, _index);
		auto _value = _tmpStr.substr(_index + 1);
		m_botPickUpContent.insert(make_pair(_key, to_wide_string(_value)));
	}
	ifs.close();

	ifs.open((m_resourceDir + "autobot_buff.txt").c_str(), ios::in);
	assert(ifs.is_open() == true);
	while (getline(ifs, _tmpStr)) {
		auto _index = _tmpStr.find_first_of('=');
		auto _key = _tmpStr.substr(0, _index);
		auto _value = _tmpStr.substr(_index + 1);
		m_botBuffContent.insert(make_pair(_key, to_wide_string(_value)));
	}
	ifs.close();

	ifs.open((m_resourceDir + "autobot_name.txt").c_str(), ios::in);
	assert(ifs.is_open() == true);
	while (getline(ifs, _tmpStr)) {
		auto _index = _tmpStr.find_first_of('=');
		auto _key = _tmpStr.substr(0, _index);
		auto _value = _tmpStr.substr(_index + 1);
		m_botNameContent.insert(make_pair(_key, to_wide_string(_value)));
	}
	ifs.close();
}

/// 解析种族文件，ExtraData 记录了拾取物品种类
void mabiAutoBotHelper::_parseRace()
{
	regex reg("\\d+");
	auto root = m_race->RootElement();
	assert(root != nullptr);
	auto raceList = root->FirstChildElement("RaceList");
	assert(raceList != nullptr);
	auto _race = raceList->FirstChildElement();
	assert(_race != nullptr);
	while (_race != nullptr) {
		string _tmpVal = _race->Attribute("StringID");
		if (_tmpVal.find("autobot") != string::npos) {
			mabiAutobot _bot;
			smatch match;
			string _toMatch = _race->Attribute("LocalName");
			regex_search(_toMatch, match, reg);
			if (m_raceContent.find(match[0]) != m_raceContent.end()) {
				auto _localName = m_raceContent[match[0]];
				char* _className = const_cast<char*>(_race->Attribute("ClassName"));
#ifdef __APPLE__
                strupr(_className);
                string __className = _className;
				_bot.setClassName(__className);
#else
                _bot.setClassName(string(strupr(_className)));
#endif
				_bot.setLocalName(_localName);
				string _pickUps = _race->Attribute("ExtraData");
				wstring pickUps = L"";
				for (const auto& pickKey : m_botPickUpContent) {
					if (_pickUps.find(pickKey.first) != string::npos) {
						pickUps += pickKey.second + L" ";
					}
				}
				_bot.setPickUps(pickUps);
				m_autoBots.insert(make_pair(string(_race->Attribute("ClassName")), _bot));
			}
		}
		_race = _race->NextSiblingElement();
	}
	cout << m_autoBots.size() << " autobots found." << endl;
}

/// 解析机械包文件
void mabiAutoBotHelper::_parseAutoBot() {
	auto root = m_autoBot->RootElement();
	assert(root != nullptr);
	auto autoBotList = root->FirstChildElement("AutoBots");
	assert(autoBotList != nullptr);
	auto _autoBotIterator = autoBotList->FirstChildElement();
	assert(_autoBotIterator != nullptr);
	while (_autoBotIterator != nullptr) {
		auto _raceName = const_cast<char*>(_autoBotIterator->Attribute("Race"));
        strupr(_raceName);
		if (m_autoBots.find(_raceName) == m_autoBots.end() && _autoBotIterator->IntAttribute("BufferId") <= 0) { // 组合包
			mabiAutobot _autoBot;
			string _groupStr = _autoBotIterator->Attribute("SummonGroup");
			_autoBot.setGroup(_groupStr);
			if (_groupStr.length() > 0) {
				auto _groupVec = stringSplit(_groupStr.c_str(), '/');
                _autoBot.setBuffId(0);
                _autoBot.setGroupBuffId(0);
				wstring _pickUps;
				for (const auto& _iterator : _groupVec) {
					if (_iterator.length() > 0) {
						for (const auto& __iterator : m_autoBots_itemdb) {
							if (__iterator.first == atoi(_iterator.c_str())) {
								_pickUps += __iterator.second.pickUp();
								break;
							}
						}
					}
				}
				_autoBot.setPickUps(_pickUps);
				string _summonTime = _autoBotIterator->Attribute("SummonLimitTime");
				_autoBot.setTime(_summonTime);
				string _summonWeight = _autoBotIterator->Attribute("SummonWeight");
				_autoBot.setWeight(_summonWeight);
				int _botId = _autoBotIterator->IntAttribute("Id");
				_autoBot.setBotId(_botId);
				m_autoBots_itemdb.insert(make_pair(_botId, _autoBot));
			}
			else {
				cout << "failed to parse the autoBot with ID : " << _autoBotIterator->Attribute("Id") << endl;
			}
			_autoBotIterator = _autoBotIterator->NextSiblingElement();
			continue;
		}
		else { // 单包
//            string _groupStr = _autoBotIterator->Attribute("GroupStringId");
//            string __groupBuffId = _autoBotIterator->Attribute("GroupBufferId");
//            if (_groupStr.length() == 0 && __groupBuffId.length() != 0){
//                cout << _autoBotIterator->GetLineNum() << " has no groupStrId but a groupBuffId." << endl;
//            }
//            if (_groupStr.length() != 0 && __groupBuffId.length() == 0){
//                cout << _autoBotIterator->GetLineNum() << " has no groupBuffId but a groupStrId." << endl;
//            }
            auto _autoBot = m_autoBots[_raceName];
            char* _stringIdChar =const_cast<char*>(_autoBotIterator->Attribute("StringId"));
            strupr(_stringIdChar);
            string _stringId = _stringIdChar;
            int _botId = _autoBotIterator->IntAttribute("Id");
            _autoBot.setBotId(_botId);
            int _buffId = _autoBotIterator->IntAttribute("BufferId");
            _autoBot.setBuffId(_buffId);
            m_botStrIdToBuffId.insert(make_pair(_stringId, _buffId));
            if (_buffId > 0){   // 记录botid到buffid的映射
                m_botIdToBuffId.insert(make_pair(_botId, _buffId));
                if (m_buffHelpers.find(_buffId) != m_buffHelpers.end()){
                    auto _buffStr = m_buffHelpers[_buffId].toWString(m_botBuffContent);
                    _autoBot.setBuff(_buffStr);
                }
                else{
                    cout << "failed to get the single bag buff, autobot id = " << _botId << endl;
                }
            }
//            if (m_bufferParse.find(_buffId) != m_bufferParse.end()){
//                wstring _buffValue = m_bufferParse[_buffId];
//                _autoBot.setBuff(_buffValue);
//            }
//            else{
//                cout << "failed to parse the buffer: id = " << _buffId << ", at line " << _autoBotIterator->GetLineNum() << endl;
//            }
            int _groupBuffId = _autoBotIterator->IntAttribute("GroupBufferId");
            _autoBot.setGroupBuffId(_groupBuffId);
            string _summonTime =_autoBotIterator->Attribute("SummonLimitTime");
            _autoBot.setTime(_summonTime);
            string _summonWeight = _autoBotIterator->Attribute("SummonWeight");
            _autoBot.setWeight(_summonWeight);
            if (string(_autoBotIterator->Attribute("StringId")).find("/group/") != string::npos){  // 组合包，group记录为SummonGroup值
                string _summonGroup = _autoBotIterator->Attribute("SummonGroup");
                _autoBot.setGroup(_summonGroup);
            }
            else{
                char* _groupChar = const_cast<char*>(_autoBotIterator->Attribute("GroupStringId"));
                strupr(_groupChar);
                string _groupStr = _groupChar;
                _autoBot.setGroup(_groupStr);
            }
            m_autoBots_itemdb.insert(make_pair(_botId, _autoBot));
			_autoBotIterator = _autoBotIterator->NextSiblingElement();
		}
	}
}

/// 解析buff, 生成 m_buffHelpers ：buffid 与 buff 的映射
void mabiAutoBotHelper::_parseBuff(){
    list<XMLElement*> groupBufferList;
    auto root = m_autoBot->RootElement();
    assert(root != nullptr);
    auto bufferList = root->FirstChildElement("Buffers");
    assert(bufferList != nullptr);
    auto _bufferIterator = bufferList->FirstChildElement();
    assert(_bufferIterator != nullptr);
    while (_bufferIterator != nullptr) {
        mabiBuffHelper _buffHelper;
        auto curAttribute = _bufferIterator->FirstAttribute();
        while (curAttribute != nullptr){
            const char* _attName = curAttribute->Name();
            if (strcmp(_attName, "Id") == 0){
                _buffHelper.setId(curAttribute->IntValue());
            }
            else if (strcmp(_attName, "Layer") == 0){
                int _layerFlag = strcmp(curAttribute->Value(), "1.Single") == 0 ? 0 : 1;
                _buffHelper.setLayer(_layerFlag);
            }
            else if (strcmp(_attName, "Overlapped") == 0){
                int _overlapFlag = strcmp(curAttribute->Value(), "false") == 0 ? 0 : 1;
                _buffHelper.setOverLapped(_overlapFlag);
            }
            else if (strcmp(_attName, "Conditions") == 0){
                string _condition = _bufferIterator->Attribute("Conditions");
                _buffHelper.setCondition(_condition);
            }
            else {
                _buffHelper.appendBuff(make_pair(_attName, curAttribute->IntValue()));
            }
            curAttribute = curAttribute->Next();
        }
        m_buffHelpers.insert(make_pair(_bufferIterator->IntAttribute("Id"), _buffHelper));
        _bufferIterator = _bufferIterator->NextSiblingElement();
    }
}

void mabiAutoBotHelper::_parseGroupBuff(){
    for (auto& autoBotIterator : m_autoBots_itemdb){
        auto& _autoBot = autoBotIterator.second;
        auto _groupStr = _autoBot.groupStr();
        // 单包
        if(_autoBot.buffId() > 0){
            vector<int> _groupIds;
            auto groupVec = stringSplit(_groupStr, '&');
            for (const auto& groupVecIterator : groupVec){
                char* _groupVecIteratorChar = const_cast<char*>(groupVecIterator.c_str());
                strupr(_groupVecIteratorChar);
                for (const auto& strIdToBuffIdIterator : m_botStrIdToBuffId){
                    if (strIdToBuffIdIterator.first.find(_groupVecIteratorChar) != string::npos){
                        _groupIds.push_back(strIdToBuffIdIterator.second);
                    }
                }
            }
            if (_groupIds.size() != groupVec.size()){
                cout << "failed to parse the summon group of " << autoBotIterator.first << endl;
                cout << "size of groupIds : " << _groupIds.size() << endl;
                cout << "size of groupVec : " << groupVec.size() << endl;
            }
            int _groupBuffId = _autoBot.groupBuffId();
            if (m_buffHelpers.find(_groupBuffId) == m_buffHelpers.end()){
                if (_groupIds.size() > 0){
                    auto _groupBuff = m_buffHelpers[_groupIds[0]];
                    for (int index = 1; index < _groupIds.size(); ++index){
                        if (m_buffHelpers.find(_groupIds[index]) == m_buffHelpers.end()){
                            cout << "failed to find the buff : " << _groupIds[index] << endl;
                        }
                        else{
                            _groupBuff += m_buffHelpers[_groupIds[index]];
                        }
                    }
                    wstring _groupBuffStr = _groupBuff.toWString(m_botBuffContent);
                    _autoBot.setGroupBuff(_groupBuffStr);
                }
                else{
                    cout << "failed to find the group buff : " << _groupBuffId << endl;
                }
            }
            else{
                auto _groupBuff = m_buffHelpers[_groupBuffId];
                wstring _groupBuffStr;
                if (_groupBuff.isOverlapped()){
                    for (const auto& groupPart : _groupIds){
                        if (m_buffHelpers.find(groupPart) == m_buffHelpers.end()){
                            cout << "failed to find the buff : " << groupPart <<endl;
                        }
                        else{
                            _groupBuff += m_buffHelpers[groupPart];
                        }
                    }
                }
                _groupBuffStr = _groupBuff.toWString(m_botBuffContent);
                _autoBot.setGroupBuff(_groupBuffStr);
            }
        }
    }
}

void mabiAutoBotHelper::_parseBotSetBuff()
{
	auto root = m_autoBot->RootElement();
	assert(root != nullptr);
	auto autoBotList = root->FirstChildElement("AutoBots");
	assert(autoBotList != nullptr);
	auto _autoBotIterator = autoBotList->FirstChildElement();
	assert(_autoBotIterator != nullptr);
	while (_autoBotIterator != nullptr) {
		if (string(_autoBotIterator->Attribute("StringId")).find("/group/") != string::npos) {  // 组合包，group记录为SummonGroup值
			int _botId = _autoBotIterator->IntAttribute("Id");
			auto& _autoBot = m_autoBots_itemdb[_botId];
			auto _groupStr = _autoBot.groupStr();
			auto _groupVec = stringSplit(_groupStr.c_str(), '/');
			vector<int> _groupIds;
			for (const auto _groupPart : _groupVec) {
				if (_groupPart.length() == 0) {
					continue;
				}
				int _botId = atoi(_groupPart.c_str());
				_groupIds.push_back(_botId);
			}
			if (_groupVec.size() <= 0){
			    wcout << "failed to parse autoBot : " << _autoBot.botName() << endl;
			    continue;
			}
			auto _autoBotPart = m_autoBots_itemdb[_groupIds[1]];
			wstring _groupBuffStr = _autoBotPart.groupBuff();
			_autoBot.setBuff(_groupBuffStr);
		}
		_autoBotIterator = _autoBotIterator->NextSiblingElement();
	}
}

void mabiAutoBotHelper::_saveToXml(XMLDocument* dataBase, int flag)
{
	auto root = dataBase->RootElement();
	assert(root != nullptr);
	auto itemDbIterator = root->FirstChildElement();
	assert(itemDbIterator != nullptr);
	regex reg("\\d+");
	while (itemDbIterator != nullptr) {
		auto _name0Attr = itemDbIterator->FindAttribute("Text_Name0");
		if (_name0Attr != nullptr) {
			string _name0Str = itemDbIterator->Attribute("Text_Name0");
			if (_name0Str.find("Bag Box") != string::npos || _name0Str.find("Bag Coupon") != string::npos) {
				string _id = itemDbIterator->Attribute("Text_Name1");
				smatch match;
				regex_search(_id, match, reg);
				wstring _textName;
				if (flag == 0) {
					_textName = m_itemDbContent[match[0]];
				}
				else if (flag == 1){
					_textName = m_itemEtcContent[match[0]];
				}
				else {
					_textName = m_itemScriptContent[match[0]];
				}
				string itemdbId = itemDbIterator->Attribute("ID");
				m_boxAndCoupons.insert(make_pair(itemdbId, _textName));
			}
		}
		auto _xmlAttr = itemDbIterator->FindAttribute("XML");
		if (_xmlAttr != nullptr) {
			string _xmlStr = itemDbIterator->Attribute("XML");
			if (_xmlStr.find("autobot") != string::npos) {
				auto _xmlStrVec = stringSplit(_xmlStr, ' ');
				string _bagShapeStr = _xmlStrVec.at(1);
				string _bagShape = _bagShapeStr.substr(10, _bagShapeStr.size() - 11);
				int _index = _bagShape.find_first_of('/');
				string _bagX = _bagShape.substr(0, _index);
				string _bagY = _bagShape.substr(_index + 1);
				double __bagX = atoi(_bagX.c_str());
				double __bagY = atoi(_bagY.c_str());
				double _volumn = __bagX * __bagY;
				auto _autoBotStr = _xmlStrVec.at(2);
				int _botId = atoi(_autoBotStr.substr(9, _autoBotStr.size() - 10).c_str());
				string _invX = itemDbIterator->Attribute("Inv_XSize");
				string _invY = itemDbIterator->Attribute("Inv_YSize");
				double __invX = atoi(_invX.c_str());
				double __invY = atoi(_invY.c_str());
				double _invSize = __invX * __invY;
				if (m_autoBots_itemdb.find(_botId) != m_autoBots_itemdb.end()) {
					auto _autoBot = m_autoBots_itemdb[_botId];
					_autoBot.setVolume(_bagX + " x " + _bagY + " (贴包 + " + _bagX + ")");
					double _ratio = _volumn / _invSize;
					double _ratioCal = (_volumn + __bagX) / _invSize;
					stringstream ss;
					ss << setprecision(5) << _ratio << " (" << _ratioCal << ")";
					_autoBot.setRatio(ss.str());
					string textNameId = itemDbIterator->Attribute("Text_Name1");
					
					smatch match;
					regex_search(textNameId, match, reg);
					if (flag == 0) {
						_autoBot.setBotName(m_itemDbContent[match[0]]);
					}
					else if (flag == 1){
						_autoBot.setBotName(m_itemEtcContent[match[0]]);
					}
					else {
						_autoBot.setBotName(m_itemScriptContent[match[0]]);
					}

					auto xmlId = itemDbIterator->Attribute("ID");
					_autoBot.setItemDbId(xmlId);

					XMLDocument toSave;
					auto declaration = toSave.NewDeclaration("xml version=\"1.0\" encoding=\"utf-8\"");
					toSave.InsertFirstChild(declaration);
					XMLElement* element = toSave.NewElement("tooltip");
					toSave.InsertEndChild(element);

					XMLElement* _root = toSave.RootElement();

					XMLElement* bagSection = toSave.NewElement("section");
					bagSection->SetAttribute("title", "包裹空间");
					string bagStr = "&lt;color=3&gt;占据空间: " + _invX + " x " + _invY +  "&#xA;包裹容量: " + _bagX + " x " + _bagY + " (贴包 + " + _bagX + ")" + "&#xA;每格物品栏扩展比: " + ss.str() + "&lt;/color&gt;";
					bagSection->SetAttribute("content", bagStr.c_str());
					_root->InsertEndChild(bagSection);

					XMLElement* summonSection = toSave.NewElement("section");
					string _summonTitle = "召唤加成 (召唤栏位 " + _autoBot.weight() + ")";
					summonSection->SetAttribute("title", _summonTitle.c_str());
					string summonStr = "&#xA;&lt;color=3&gt;" + to_normal_string(_autoBot.buff()) + "&lt;/color&gt;&#xA;&lt;color=7&gt;拾取:&lt;/color&gt;&lt;color=3&gt;" + to_normal_string(_autoBot.pickUp()) + "&lt;/color&gt;&#xA;&#xA;&lt;color=7&gt;召唤时长: " + _autoBot.summonTime() + "分钟&lt;/color&gt;";
					summonSection->SetAttribute("content", summonStr.c_str());
					_root->InsertEndChild(summonSection);

					auto _groupStr = _autoBot.groupStr();
					if (_groupStr.length() > 0 && _autoBot.buffId() > 0) {
						XMLElement* groupSection = toSave.NewElement("section");
						groupSection->SetAttribute("title", "机械包组合");
						auto _groupVec = stringSplit(_groupStr, '&');
						string groupStr = "";
						for (const auto& groupPartIterator : _groupVec) {
							auto _groupPartIterator = groupPartIterator.substr(1, groupPartIterator.length() - 2);
							groupStr += to_normal_string(m_botNameContent[_groupPartIterator]) + " ";
						}
						groupStr += "&#xA;&lt;color=7&gt;组合buff:&lt;/color&gt;&#xA;&lt;color=3&gt;" + to_normal_string(_autoBot.groupBuff()) + "&lt;/color&gt;";
						groupSection->SetAttribute("content", groupStr.c_str());
						_root->InsertEndChild(groupSection);
					}
					toSave.SetBOM(true);
					string filePath = m_resourceDir + "result/data/db/tooltipextension/" + xmlId + ".xml";
					toSave.SaveFile(filePath.c_str());
					m_itemIdToAutoBots.insert(make_pair(atoi(xmlId), _autoBot));
				}
			}
		}
		itemDbIterator = itemDbIterator->NextSiblingElement();
	}
}

void mabiAutoBotHelper::_boxAndCoupons()
{
	for (const auto& iterator : m_boxAndCoupons) {
        for (const auto& autoBotIterator : m_itemIdToAutoBots){
            auto _botNameStr = to_normal_string(autoBotIterator.second.botName());
            int _index = _botNameStr.size() - 6;
            auto _textNameStr = to_normal_string(iterator.second);
            auto __botNameStr = _botNameStr.substr(0, _index);
            auto __textNameStr = _textNameStr.substr(0, _index);
            if (__botNameStr == __textNameStr){
                string _filePath = m_resourceDir + "result/data/db/tooltipextension/" + autoBotIterator.second.itemDbId() + ".xml";
                XMLDocument _doc;
                _doc.LoadFile(_filePath.c_str());
                _doc.SetBOM(true);
                string __filePath = m_resourceDir + "result/data/db/tooltipextension/" + iterator.first + ".xml";
                _doc.SaveFile(__filePath.c_str());
                break;
            }
        }
	}
}
