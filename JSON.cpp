#include "JSON.h"
#include <iostream>

#ifdef _DEBUG
#include "Lost/Assert.h"
#endif

template <typename Out>
void split(const std::string& s, char delim, Out result) {
	std::istringstream iss(s);
	std::string item;
	while (std::getline(iss, item, delim)) {
		*result++ = item;
	}
}

std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

bool IsStringInt(std::string& s)
{
	for (auto c : s)
	{
		if (c < 48 || c > 57 || c == '.')
			return false;
	}
	return true;
}

bool IsStringFloat(std::string& s)
{
	for (auto c : s)
	{
		if ((c < 48 || c > 57) && c != '.' && c != '-')
			return false;
	}
	return true;
}

JSONArray::JSONArray()
{
	m_IDSpecifiers = new std::vector<JSONIDSpecifier>();
	m_IntObjects = std::vector<int>();
	m_FloatObjects = std::vector<float>();
	m_BoolObjects = std::vector<bool>();
	m_StringObjects = std::vector<std::string>();
	m_JsonObjects = std::vector<JSONObject*>();
	m_JsonArrays = std::vector<JSONArray*>();
}

JSONArray::~JSONArray()
{
	for (auto p : m_JsonObjects)
		delete p;
	m_JsonObjects.clear();
	for (auto p : m_JsonArrays)
		delete p;
	m_JsonArrays.clear();
	m_IDSpecifiers->clear();
	delete m_IDSpecifiers;
}

void JSONArray::removeObject(int id)
{
	int type = m_IDSpecifiers->at(id).Type;
	int ID = m_IDSpecifiers->at(id).ID;

	switch (type)
	{
	case JSON_TYPE_INT:			m_IntObjects.erase(m_IntObjects.begin() + ID); break;
	case JSON_TYPE_FLOAT:		m_FloatObjects.erase(m_FloatObjects.begin() + ID); break;
	case JSON_TYPE_BOOL:		m_BoolObjects.erase(m_BoolObjects.begin() + ID); break;
	case JSON_TYPE_STRING:		m_StringObjects.erase(m_StringObjects.begin() + ID); break;
	case JSON_TYPE_JSON_OBJECT: delete m_JsonObjects[ID]; m_JsonObjects.erase(m_JsonObjects.begin() + ID); break;
	case JSON_TYPE_JSON_ARRAY:  delete m_JsonArrays[ID]; m_JsonArrays.erase(m_JsonArrays.begin() + ID); break;
	}

	for (int i = 0; i < m_IDSpecifiers->size(); i++)
	{
		if (m_IDSpecifiers->at(i).Type == type && m_IDSpecifiers->at(i).ID > ID)
			m_IDSpecifiers->at(i).ID--;
	}

	m_IDSpecifiers->erase(m_IDSpecifiers->begin() + id);
}

void JSONArray::addInt(int intObject)
{
	m_IntObjects.push_back(intObject);
	m_IDSpecifiers->push_back(JSONIDSpecifier{ JSON_TYPE_INT, (int)m_IntObjects.size() - 1 });
}

void JSONArray::addFloat(float floatObject)
{
	m_FloatObjects.push_back(floatObject);
	m_IDSpecifiers->push_back(JSONIDSpecifier{ JSON_TYPE_FLOAT, (int)m_FloatObjects.size() - 1 });
}

void JSONArray::addString(std::string stringObject)
{
	m_StringObjects.push_back(stringObject);
	m_IDSpecifiers->push_back(JSONIDSpecifier{ JSON_TYPE_STRING, (int)m_StringObjects.size() - 1 });
}

void JSONArray::addBool(bool boolObject)
{
	m_BoolObjects.push_back(boolObject);
	m_IDSpecifiers->push_back(JSONIDSpecifier{ JSON_TYPE_BOOL, (int)m_BoolObjects.size() - 1 });
}

void JSONArray::addJSONObject(JSONObject* jsonObject)
{
	m_JsonObjects.push_back(jsonObject);
	m_IDSpecifiers->push_back(JSONIDSpecifier{ JSON_TYPE_JSON_OBJECT, (int)m_JsonObjects.size() - 1 });
}

void JSONArray::addJSONArray(JSONArray* jsonArray)
{
	m_JsonArrays.push_back(jsonArray);
	m_IDSpecifiers->push_back(JSONIDSpecifier{ JSON_TYPE_JSON_ARRAY, (int)m_JsonArrays.size() - 1 });
}

void JSONArray::setInt(int id, int intObject)
{
	removeObject(id);
	m_IntObjects.push_back(intObject);
	if (id < m_IDSpecifiers->size())
		m_IDSpecifiers->emplace(m_IDSpecifiers->begin() + id, JSONIDSpecifier{ JSON_TYPE_INT, (int)m_IntObjects.size() - 1 });
	else
		m_IDSpecifiers->push_back(JSONIDSpecifier{ JSON_TYPE_INT, (int)m_IntObjects.size() - 1 });
}

void JSONArray::setFloat(int id, float floatObject)
{
	removeObject(id);
	m_FloatObjects.push_back(floatObject);
	if (id < m_IDSpecifiers->size())
		m_IDSpecifiers->emplace(m_IDSpecifiers->begin() + id, JSONIDSpecifier{ JSON_TYPE_INT, (int)m_FloatObjects.size() - 1 });
	else
		m_IDSpecifiers->push_back(JSONIDSpecifier{ JSON_TYPE_FLOAT, (int)m_FloatObjects.size() - 1 });
}

void JSONArray::setString(int id, std::string stringObject)
{
	removeObject(id);
	m_StringObjects.push_back(stringObject);
	if (id < m_IDSpecifiers->size())
		m_IDSpecifiers->emplace(m_IDSpecifiers->begin() + id, JSONIDSpecifier{ JSON_TYPE_STRING, (int)m_StringObjects.size() - 1 });
	else
		m_IDSpecifiers->push_back(JSONIDSpecifier{ JSON_TYPE_STRING, (int)m_StringObjects.size() - 1 });
}

void JSONArray::setBool(int id, bool boolObject)
{
	removeObject(id);
	m_BoolObjects.push_back(boolObject);
	if (id < m_IDSpecifiers->size())
		m_IDSpecifiers->emplace(m_IDSpecifiers->begin() + id, JSONIDSpecifier{ JSON_TYPE_BOOL, (int)m_BoolObjects.size() - 1 });
	else
		m_IDSpecifiers->push_back(JSONIDSpecifier{ JSON_TYPE_BOOL, (int)m_BoolObjects.size() - 1 });
}

void JSONArray::setJSONObject(int id, JSONObject* jsonObject)
{
	removeObject(id);
	m_JsonObjects.push_back(jsonObject);
	if (id < m_IDSpecifiers->size())
		m_IDSpecifiers->emplace(m_IDSpecifiers->begin() + id, JSONIDSpecifier{ JSON_TYPE_JSON_OBJECT, (int)m_JsonObjects.size() - 1 });
	else
		m_IDSpecifiers->push_back(JSONIDSpecifier{ JSON_TYPE_JSON_OBJECT, (int)m_JsonObjects.size() - 1 });
}

void JSONArray::setJSONArray(int id, JSONArray* jsonArray)
{
	removeObject(id);
	m_JsonArrays.push_back(jsonArray);
	if (id < m_IDSpecifiers->size())
		m_IDSpecifiers->emplace(m_IDSpecifiers->begin() + id, JSONIDSpecifier{ JSON_TYPE_JSON_ARRAY, (int)m_JsonArrays.size() - 1 });
	else
		m_IDSpecifiers->push_back(JSONIDSpecifier{ JSON_TYPE_JSON_ARRAY, (int)m_JsonArrays.size() - 1 });
}

JSONArray* JSONArray::createCopy() const
{
	JSONArray* copy = new JSONArray();

	//(*copy) = (*this);
	copy->m_BoolObjects = m_BoolObjects;
	copy->m_FloatObjects = m_FloatObjects;
	*copy->m_IDSpecifiers = *m_IDSpecifiers;
	copy->m_IntObjects = m_IntObjects;
	copy->m_JsonArrays = m_JsonArrays;
	copy->m_JsonObjects = m_JsonObjects;
	copy->m_StringObjects = m_StringObjects;

	std::vector<JSONIDSpecifier>* specifiers = getSpecifiers();
	std::vector<int> objectIDs = {};
	std::vector<int> arrayIDs = {};

	for (JSONIDSpecifier& specifier : *specifiers)
	{
		if (specifier.Type == JSON_TYPE_JSON_OBJECT)
			objectIDs.push_back(specifier.ID);
		else if (specifier.Type == JSON_TYPE_JSON_ARRAY)
			arrayIDs.push_back(specifier.ID);
	}

	for (int id : objectIDs)
	{
		JSONObject* child = m_JsonObjects[id]->createCopy();
		copy->m_JsonObjects[id] = child;
	}

	for (int id : arrayIDs)
	{
		JSONArray* child = m_JsonArrays[id]->createCopy();
		copy->m_JsonArrays[id] = child;
	}

	return copy;
}

int& JSONArray::getInt(int id)
{
	if (m_IDSpecifiers->at(id).Type == JSON_TYPE_INT)
		return m_IntObjects.at(m_IDSpecifiers->at(id).ID);
}

float& JSONArray::getFloat(int id)
{
	if (m_IDSpecifiers->at(id).Type == JSON_TYPE_FLOAT)
		return m_FloatObjects.at(m_IDSpecifiers->at(id).ID);
}

std::string& JSONArray::getString(int id)
{
	if (m_IDSpecifiers->at(id).Type == JSON_TYPE_STRING)
		return m_StringObjects.at(m_IDSpecifiers->at(id).ID);
}

bool JSONArray::getBool(int id)
{
	if (m_IDSpecifiers->at(id).Type == JSON_TYPE_BOOL)
		return m_BoolObjects.at(m_IDSpecifiers->at(id).ID);
	return NULL;
}

JSONObject* JSONArray::getJSONObject(int id)
{
	if (m_IDSpecifiers->at(id).Type == JSON_TYPE_JSON_OBJECT)
		return m_JsonObjects.at(m_IDSpecifiers->at(id).ID);
	return nullptr;
}

JSONArray* JSONArray::getJSONArray(int id)
{
	if (m_IDSpecifiers->at(id).Type == JSON_TYPE_JSON_ARRAY)
		return m_JsonArrays.at(m_IDSpecifiers->at(id).ID);
	return nullptr;
}

std::string JSONArray::exportString(int indentAddition)
{
	std::string FinalOutput = "";
	for (int i = 0; i < indentAddition - 1; i++)
		FinalOutput += "	";
	FinalOutput += "[\n";

	for (int i = 0; i < m_IDSpecifiers->size(); i++)
	{
		for (int i = 0; i < indentAddition + 1; i++)
			FinalOutput += "	";

		switch (m_IDSpecifiers->at(i).Type)
		{
		case JSON_TYPE_INT:
			FinalOutput += std::to_string(m_IntObjects.at(m_IDSpecifiers->at(i).ID));
			break;
		case JSON_TYPE_FLOAT:
			FinalOutput += std::to_string(m_FloatObjects.at(m_IDSpecifiers->at(i).ID));
			break;
		case JSON_TYPE_BOOL:
			if (m_BoolObjects.at(m_IDSpecifiers->at(i).ID))
				FinalOutput += "true";
			else
				FinalOutput += "false";
			break;
		case JSON_TYPE_STRING:
			FinalOutput += "\"" + m_StringObjects.at(m_IDSpecifiers->at(i).ID) + "\"";
			break;
		case JSON_TYPE_JSON_OBJECT:
			FinalOutput += m_JsonObjects.at(m_IDSpecifiers->at(i).ID)->exportString(indentAddition + 1);
			break;
		case JSON_TYPE_JSON_ARRAY:
			FinalOutput += m_JsonArrays.at(m_IDSpecifiers->at(i).ID)->exportString(indentAddition + 1);
			break;
		}

		if (i != m_IDSpecifiers->size() - 1)
			FinalOutput += ",\n";
		else
			FinalOutput += "\n";
	}

	for (int i = 0; i < indentAddition; i++)
		FinalOutput += "	";
	FinalOutput += "]";

	return FinalOutput;
}

void JSONArray::saveToFile(std::string loc)
{
	//std::string exportData = exportString();
	//std::cin >> exportData;
	std::ofstream Output(loc);
	Output << exportString();
	Output.close();
}

void JSONArray::loadDataFromString(std::string text)
{
	text = text.substr(1, text.size() - 2);

	std::vector<std::string> arrayValues;
	std::string listObject = "";

	int totalScopesOut = 0;
	bool inString = false;
	for (int i = 0; i < text.size(); i++)
	{
		char currentChar = text.at(i);
		if (currentChar == '\"')
			inString = !inString;
		if (currentChar == '[' || currentChar == '{')
			totalScopesOut++;
		if (currentChar == ']' || currentChar == '}')
			totalScopesOut--;

		if (currentChar != ',' || totalScopesOut > 0 || inString)
			listObject += currentChar;
		else
		{
			arrayValues.push_back(listObject);
			listObject = "";
		}
	}

	if (totalScopesOut == 0)
		arrayValues.push_back(listObject);

	for (int i = 0; i < arrayValues.size(); i++)
	{
		int ObjectType = 0;
		std::string& objectData = arrayValues.at(i);
		if (objectData.at(0) == '\"')
		{
			ObjectType = JSON_TYPE_STRING;
			addString(objectData.substr(1, objectData.size() - 2));
		}
		if (IsStringInt(objectData))
		{
			ObjectType = JSON_TYPE_INT;
			addInt(std::stoi(objectData));
		}
		else if (IsStringFloat(objectData))
		{
			ObjectType = JSON_TYPE_FLOAT;
			addFloat(std::stof(objectData));
		}
		else if (objectData.at(0) == 'f')
		{
			ObjectType = JSON_TYPE_BOOL;
			addBool(false);
		}
		else if (objectData.at(0) == 't')
		{
			ObjectType = JSON_TYPE_BOOL;
			addBool(true);
		}
		if (objectData.at(0) == '[')
		{
			ObjectType = JSON_TYPE_JSON_ARRAY;
			JSONArray* subArray = new JSONArray();
			subArray->loadDataFromString(objectData);
			addJSONArray(subArray);
		}
		if (objectData.at(0) == '{')
		{
			ObjectType = JSON_TYPE_JSON_OBJECT;
			JSONObject* subArray = new JSONObject();
			subArray->loadDataFromString(objectData);
			addJSONObject(subArray);
		}
	}	
}

JSONObject::JSONObject()
{
	data = new JSONArray();
}

JSONObject::~JSONObject()
{
	delete data;
}

void JSONObject::removeObject(std::string name)
{
	int idRemoved = objectNames[name];
	data->removeObject(idRemoved);
	for (int i = 0; i < namesList.size(); i++)
	{
		if (objectNames[namesList.at(i)] > idRemoved)
			objectNames[namesList.at(i)] = objectNames[namesList.at(i)] - 1;
	};
	namesList.erase(namesList.begin() + idRemoved);
}

void JSONObject::setInt(std::string name, int intObject)
{
	if (objectNames.count(name) > 0)
		data->setInt(objectNames[name], intObject);
	else
	{
		data->addInt(intObject);
		objectNames[name] = data->size() - 1;
		namesList.push_back(name);
	}
}

void JSONObject::setFloat(std::string name, float floatObject)
{
	if (objectNames.count(name) > 0)
		data->setFloat(objectNames[name], floatObject);
	else
	{
		data->addFloat(floatObject);
		objectNames[name] = data->size() - 1;
		namesList.push_back(name);
	}
}

void JSONObject::setString(std::string name, std::string stringObject)
{
	if (objectNames.count(name) > 0)
	{
		data->setString(objectNames[name], stringObject);
	}
	else
	{
		data->addString(stringObject);
		objectNames[name] = data->size() - 1;
		namesList.push_back(name);
	}
}

void JSONObject::setBool(std::string name, bool boolObject)
{
	if (objectNames.count(name) > 0)
		data->setBool(objectNames[name], boolObject);
	else
	{
		data->addBool(boolObject);
		objectNames[name] = data->size() - 1;
		namesList.push_back(name);
	}
}

void JSONObject::setJSONObject(std::string name, JSONObject* jsonObject)
{
	if (objectNames.count(name) > 0)
		data->setJSONObject(objectNames[name], jsonObject);
	else
	{
		data->addJSONObject(jsonObject);
		objectNames[name] = data->size() - 1;
		namesList.push_back(name);
	}
}

void JSONObject::setJSONArray(std::string name, JSONArray* jsonArray)
{
	if (objectNames.count(name) > 0)
		data->setJSONArray(objectNames[name], jsonArray);
	else
	{
		data->addJSONArray(jsonArray);
		objectNames[name] = data->size() - 1;
		namesList.push_back(name);
	}
}

#ifdef NDEBUG
// Release mode

int& JSONObject::getInt(std::string name)
{
	return data->getInt(objectNames[name]);
}

float& JSONObject::getFloat(std::string name)
{
	return data->getFloat(objectNames[name]);
}

std::string& JSONObject::getString(std::string name)
{	
	return data->getString(objectNames[name]);
}

bool JSONObject::getBool(std::string name)
{
	return data->getBool(objectNames[name]);
}

JSONObject* JSONObject::getJSONObject(std::string name)
{
	return data->getJSONObject(objectNames[name]);
}

JSONArray* JSONObject::getJSONArray(std::string name)
{
	return data->getJSONArray(objectNames[name]);
}

#else 
// Debug mode

int& JSONObject::getInt(std::string name)
{
	if (objectNames.count(name) == 0)
		lost::lassert("Tried to access int value in JSONObject at: " + name + "\nBut no value was found");
	return data->getInt(objectNames[name]);
}

float& JSONObject::getFloat(std::string name)
{
	if (objectNames.count(name) == 0)
		lost::lassert("Tried to access float value in JSONObject at: " + name + "\nBut no value was found");
	return data->getFloat(objectNames[name]);
}

std::string& JSONObject::getString(std::string name)
{
	if (objectNames.count(name) == 0)
		lost::lassert("Tried to access string value in JSONObject at: " + name + "\nBut no value was found");
	return data->getString(objectNames[name]);
}

bool JSONObject::getBool(std::string name)
{
	if (objectNames.count(name) == 0)
		lost::lassert("Tried to access bool value in JSONObject at: " + name + "\nBut no value was found");
	return data->getBool(objectNames[name]);
}

JSONObject* JSONObject::getJSONObject(std::string name)
{
	if (objectNames.count(name) == 0)
		lost::lassert("Tried to access JSONObject value in JSONObject at: " + name + "\nBut no value was found");
	return data->getJSONObject(objectNames[name]);
}

JSONArray* JSONObject::getJSONArray(std::string name)
{
	if (objectNames.count(name) == 0)
		lost::lassert("Tried to access JSONArray value in JSONObject at: " + name + "\nBut no value was found");
	return data->getJSONArray(objectNames[name]);
}

#endif // NDEBUG

JSONObject* JSONObject::createCopy() const
{
	JSONObject* copy = new JSONObject();

	copy->namesList = namesList;
	copy->objectNames = objectNames;
	copy->data->m_BoolObjects = data->m_BoolObjects;
	copy->data->m_FloatObjects = data->m_FloatObjects;
	*copy->data->m_IDSpecifiers = *data->m_IDSpecifiers;
	copy->data->m_IntObjects = data->m_IntObjects;
	copy->data->m_JsonArrays = data->m_JsonArrays;
	copy->data->m_JsonObjects = data->m_JsonObjects;
	copy->data->m_StringObjects = data->m_StringObjects;

	std::vector<JSONIDSpecifier>* specifiers = data->getSpecifiers();
	std::vector<int> objectIDs = {};
	std::vector<int> arrayIDs = {};

	for (JSONIDSpecifier& specifier : *specifiers)
	{
		if (specifier.Type == JSON_TYPE_JSON_OBJECT)
			objectIDs.push_back(specifier.ID);
		else if (specifier.Type == JSON_TYPE_JSON_ARRAY)
			arrayIDs.push_back(specifier.ID);
	}

	for (int id : objectIDs)
	{
		JSONObject* child = data->m_JsonObjects[id]->createCopy();
		copy->data->m_JsonObjects[id] = child;
	}

	for (int id : arrayIDs)
	{
		JSONArray* child = data->m_JsonArrays[id]->createCopy();
		copy->data->m_JsonArrays[id] = child;
	}

	return copy;
}

std::string JSONObject::exportString(int indentAddition)
{
	std::string FinalExport = "";
	for (int i = 0; i < indentAddition; i++)
		FinalExport += "	";
	FinalExport = "{\n";

	for (int i = 0; i < namesList.size(); i++)
	{
		std::string DataString = "";

		for (int i = 0; i < indentAddition + 1; i++)
			DataString += "	";

		DataString += "\"" + namesList.at(i) + "\" : ";
		int dataType = data->getSpecifiers()->at(objectNames[namesList.at(i)]).Type;

		switch (dataType)
		{
		case JSON_TYPE_INT:
			DataString += std::to_string(data->getInt(objectNames[namesList.at(i)]));
			break;
		case JSON_TYPE_FLOAT:
			DataString += std::to_string(data->getFloat(objectNames[namesList.at(i)]));
			break;
		case JSON_TYPE_BOOL:
			if (data->getBool(objectNames[namesList.at(i)]))
				DataString += "true";
			else
				DataString += "false";
			break;
		case JSON_TYPE_STRING:
			DataString += "\"" + data->getString(objectNames[namesList.at(i)]) + "\"";
			break;
		case JSON_TYPE_JSON_OBJECT:
			DataString += data->getJSONObject(objectNames[namesList.at(i)])->exportString(indentAddition + 1);
			break;
		case JSON_TYPE_JSON_ARRAY:
			DataString += data->getJSONArray(objectNames[namesList.at(i)])->exportString(indentAddition + 1);
			break;
		}

		if (i != namesList.size() - 1)
			DataString += ",\n";
		else
			DataString += "\n";

		FinalExport += DataString;
	}

	for (int i = 0; i < indentAddition; i++)
		FinalExport += "	";

	return FinalExport + "}";
}

void JSONObject::saveToFile(std::string loc)
{
	std::ofstream Output(loc);
	Output << exportString();
	Output.close();
}

int JSONObject::getType(std::string name)
{
	return data->getSpecifiers()->at(objectNames[name]).Type;
}

void JSONObject::loadDataFromString(std::string text)
{
	text = text.substr(1, text.size() - 2);

	std::vector<std::string> arrayValues;
	std::string listObject = "";

	int totalScopesOut = 0;
	bool inString = false;
	for (int i = 0; i < text.size(); i++)
	{
		char currentChar = text.at(i);
		if (currentChar == '\"')
			inString = !inString;
		if (currentChar == '[' || currentChar == '{')
			totalScopesOut++;
		if (currentChar == ']' || currentChar == '}')
			totalScopesOut--;

		if (currentChar != ',' || totalScopesOut > 0 || inString)
			listObject += currentChar;
		else
		{	
			arrayValues.push_back(listObject);
			listObject = "";
		}
	}

	if (totalScopesOut == 0)
		arrayValues.push_back(listObject);

	for (int i = 0; i < arrayValues.size(); i++)
	{
		if (arrayValues.at(i).size() >= 3)
		{
			int ObjectType = 0;
			std::string& objectFullData = arrayValues.at(i);
			std::string ObjectName;
			ObjectName = split(objectFullData, '\"').at(1);
			std::string ObjectData = "";
			bool awaitingData = false;
			bool hitData = false;
			for (int i = 0; i < objectFullData.size(); i++)
			{
				if (hitData)
					ObjectData += objectFullData.at(i);
				if (objectFullData.at(i) == ':')
					hitData = true;
			}

			if (ObjectData.at(0) == '\"')
			{
				ObjectType = JSON_TYPE_STRING;
				setString(ObjectName, ObjectData.substr(1, ObjectData.size() - 2));
			}
			if (IsStringInt(ObjectData))
			{
				ObjectType = JSON_TYPE_INT;
				setInt(ObjectName, std::stoi(ObjectData));
			}
			else if (IsStringFloat(ObjectData))
			{
				ObjectType = JSON_TYPE_FLOAT;
				setFloat(ObjectName, std::stof(ObjectData));
			}
			else if (ObjectData.at(0) == 'f')
			{
				ObjectType = JSON_TYPE_BOOL;
				setBool(ObjectName, false);
			}
			else if (ObjectData.at(0) == 't')
			{
				ObjectType = JSON_TYPE_BOOL;
				setBool(ObjectName, true);
			}
			if (ObjectData.at(0) == '[')
			{
				ObjectType = JSON_TYPE_JSON_ARRAY;
				JSONArray* subArray = new JSONArray();
				subArray->loadDataFromString(ObjectData);
				setJSONArray(ObjectName, subArray);
			}
			if (ObjectData.at(0) == '{')
			{
				ObjectType = JSON_TYPE_JSON_OBJECT;
				JSONObject* subArray = new JSONObject();
				subArray->loadDataFromString(ObjectData);
				setJSONObject(ObjectName, subArray);
			}
		}
	}
}

JSONObject* LoadJSONObject(std::string fileLocation)
{
	JSONObject* FinalOutput = new JSONObject();

	std::ifstream t(fileLocation);
	std::stringstream buffer;
	buffer << t.rdbuf();
	t.close();
	std::string output = buffer.str();

	//[!] ADD VERIFICATION THAT THE JSON FILE IS VALID HERE!

	std::string processedString = "";

	bool inString = false;
	for (int i = 0; i < output.size(); i++)
	{
		if (output.at(i) == '\"')
			inString = !inString;

		if (output.at(i) != '\n' && output.at(i) != ' ' && output.at(i) != '	' || inString)
		{
			processedString += output.at(i);
		}
	}

	FinalOutput->loadDataFromString(processedString);

	return FinalOutput;
}

JSONArray* LoadJSONArray(std::string fileLocation)
{
	JSONArray* FinalOutput = new JSONArray();

	std::ifstream t(fileLocation);
	std::stringstream buffer;
	buffer << t.rdbuf();
	t.close();
	std::string output = buffer.str();

	//[!] ADD VERIFICATION THAT THE JSON FILE IS VALID HERE!

	std::string processedString = "";

	bool inString = false;
	for (int i = 0; i < output.size(); i++)
	{
		if (output.at(i) == '\"')
			inString = !inString;

		if (output.at(i) != '\n' && output.at(i) != ' ' && output.at(i) != '	' || inString)
		{
			processedString += output.at(i);
		}
	}

	FinalOutput->loadDataFromString(processedString);

	return FinalOutput;
}

JSONObject* LuaStackToJSONObject(lua_State* luaState)
{
	JSONObject* jsonReturn = new JSONObject();

	lua_pushnil(luaState);
	while (true) {

		// Check if it's iterated over the whole table
		if (!lua_next(luaState, -2))
			break;

		std::string key;
		int index = 0;
		switch (lua_type(luaState, -2))
		{
		case LUA_TSTRING:
			key = lua_tostring(luaState, -2);
			break;
		case LUA_TNUMBER:
			key = std::to_string((int)lua_tonumber(luaState, -2) - 1);
			break;
		}

		float value = 0;
		switch (lua_type(luaState, -1))
		{
		case LUA_TSTRING:
			jsonReturn->setString(key, lua_tostring(luaState, -1));
			lua_pop(luaState, 1);
			break;
		case LUA_TBOOLEAN:
			jsonReturn->setBool(key, lua_toboolean(luaState, -1));
			lua_pop(luaState, 1);
			break;
		case LUA_TNUMBER:
			value = lua_tonumber(luaState, -1);

			if (lua_isinteger(luaState, -1))
				jsonReturn->setInt(key, (int)value);
			else
				jsonReturn->setFloat(key, value);

			lua_pop(luaState, 1);
			break;
		case LUA_TTABLE:
			jsonReturn->setJSONObject(key, LuaStackToJSONObject(luaState));
			// Honestly I have absolutely no idea why this doesn't need to be lua_pop'ed
			break;
		default:
			// This should NEVER happen
			fprintf(stdout, "Type not found: %i\n", lua_type(luaState, -1));
			lua_pop(luaState, 1);
			break;
		}
	}
	lua_pop(luaState, 1);

	return jsonReturn;
}

static void SubJSONObjectToLuaStack(JSONObject* jsonObject, lua_State* luaState, std::string objectName)
{
	std::vector<JSONIDSpecifier>* specifiers = jsonObject->getData()->getSpecifiers();

	lua_newtable(luaState);

	for (auto& [name, id] : jsonObject->getObjectList())
	{
		int type = (*specifiers)[id].Type;
		switch (type)
		{
		case JSON_TYPE_INT:
			lua_pushinteger(luaState, jsonObject->getInt(name));
			break;
		case JSON_TYPE_FLOAT:
			lua_pushnumber(luaState, jsonObject->getFloat(name));
			break;
		case JSON_TYPE_BOOL:
			lua_pushboolean(luaState, jsonObject->getBool(name));
			break;
		case JSON_TYPE_STRING:
			lua_pushstring(luaState, jsonObject->getString(name).c_str());
			break;
		case JSON_TYPE_JSON_OBJECT:
			SubJSONObjectToLuaStack(jsonObject->getJSONObject(name), luaState, name);
			break;
		case JSON_TYPE_JSON_ARRAY:
			break;
		}
		lua_setfield(luaState, -2, name.c_str());
	}

	lua_setfield(luaState, -2, objectName.c_str());
}

void JSONObjectToLuaTable(JSONObject* jsonObject, lua_State* luaState, std::string name)
{
	if (jsonObject)
	{
		std::vector<JSONIDSpecifier>* specifiers = jsonObject->getData()->getSpecifiers();

		lua_newtable(luaState);

		for (auto& [name, id] : jsonObject->getObjectList())
		{
			int type = (*specifiers)[id].Type;
			switch (type)
			{
			case JSON_TYPE_INT:
				lua_pushinteger(luaState, jsonObject->getInt(name));
				break;
			case JSON_TYPE_FLOAT:
				lua_pushnumber(luaState, jsonObject->getFloat(name));
				break;
			case JSON_TYPE_BOOL:
				lua_pushboolean(luaState, jsonObject->getBool(name));
				break;
			case JSON_TYPE_STRING:
				lua_pushstring(luaState, jsonObject->getString(name).c_str());
				break;
			case JSON_TYPE_JSON_OBJECT:
				SubJSONObjectToLuaStack(jsonObject->getJSONObject(name), luaState, name);
				break;
			case JSON_TYPE_JSON_ARRAY:
				break;
			}
			lua_setfield(luaState, -2, name.c_str());
		}

		lua_setglobal(luaState, name.c_str());
	}
}
