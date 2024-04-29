#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <map>
#include <iterator>
#include "Lost/LuaInit.h"

template <typename Out>
void split(const std::string& s, char delim, Out result);

std::vector<std::string> split(const std::string& s, char delim);

enum
{
	JSON_TYPE_INT,
	JSON_TYPE_FLOAT,
	JSON_TYPE_STRING,
	JSON_TYPE_BOOL,
	JSON_TYPE_JSON_OBJECT,
	JSON_TYPE_JSON_ARRAY
};

struct JSONIDSpecifier
{
	int Type = -1;
	int ID;
};

class JSONArray;

class JSONObject
{
private:
	JSONArray* data = nullptr;
	std::vector<std::string> namesList;
	std::map<std::string, int> objectNames;
public:
	JSONObject();
	~JSONObject();

	void removeObject(std::string name);

	void setInt(std::string name, int intObject);
	void setFloat(std::string name, float floatObject);
	void setString(std::string name, std::string stringObject);
	void setBool(std::string name, bool boolObject);
	void setJSONObject(std::string name, JSONObject* jsonObject);
	void setJSONArray(std::string name, JSONArray* jsonArray);

	int getInt(std::string name);
	float getFloat(std::string name);
	std::string getString(std::string name);
	bool getBool(std::string name);
	JSONObject* getJSONObject(std::string name);
	JSONArray* getJSONArray(std::string name);

	std::string exportString(int indentAddition = 0);
	void saveToFile(std::string loc);

	inline std::vector<std::string> getNamesList() { return namesList; };
	inline std::map<std::string, int> getObjectList() { return objectNames; };
	inline JSONArray* getData() { return data; };

	int getType(std::string name);

	void loadDataFromString(std::string text);
};

class JSONArray
{
private:
	std::vector<JSONIDSpecifier>* m_IDSpecifiers;

	std::vector<int> m_IntObjects;
	std::vector<float> m_FloatObjects;
	std::vector<std::string> m_StringObjects;
	std::vector<bool> m_BoolObjects;
	std::vector<JSONObject*> m_JsonObjects;
	std::vector<JSONArray*> m_JsonArrays;
public:
	JSONArray();
	~JSONArray();

	void removeObject(int id);

	void addInt(int intObject);
	void addFloat(float floatObject);
	void addString(std::string stringObject);
	void addBool(bool boolObject);
	void addJSONObject(JSONObject* jsonObject);
	void addJSONArray(JSONArray* jsonArray);

	void setInt(int id, int intObject);
	void setFloat(int id, float floatObject);
	void setString(int id, std::string stringObject);
	void setBool(int id, bool boolObject);
	void setJSONObject(int id, JSONObject* jsonObject);
	void setJSONArray(int id, JSONArray* jsonArray);

	int getInt(int id);
	float getFloat(int id);
	std::string getString(int id);
	bool getBool(int id);
	JSONObject* getJSONObject(int id);
	JSONArray* getJSONArray(int id);

	inline int size() { return m_IDSpecifiers->size(); };
	inline std::vector<JSONIDSpecifier>* getSpecifiers() { return m_IDSpecifiers; };

	std::string exportString(int indentAddition = 0);
	void saveToFile(std::string loc);

	void loadDataFromString(std::string text);
};

JSONObject* LoadJSONObject(std::string fileLocation);
JSONArray* LoadJSONArray(std::string fileLocation);
JSONObject* LuaStackToJSONObject(lua_State* luaState);