#include <gtest/gtest.h>
#include <fstream>

#include "Json.hpp"
// Ïðîâåðêà íà ïóñòîé îáúåêò
TEST(Json, NullJson)
	{
		Json json{};
		EXPECT_EQ(json.is_object(), false);
		EXPECT_EQ(json.is_array(), false);
		EXPECT_EQ(json.is_empty(), true);
	}

TEST(Json, ExampleJson)
	{
		Json object = Json::parse(R"(
			{
				"lastname" : "Ivanov",
				"firstname" : "Ivan",
				"age" : 25,
				"islegal" : false,
				"marks" : [
					4,5,5,5,2,3
				],
				"address" : {
					"city" : "Moscow",
					"street" : "Vozdvijenka"
				}
			}
		)");

		EXPECT_EQ(std::any_cast<std::string>(object["lastname"]), "Ivanov");
		EXPECT_EQ(std::any_cast<bool>(object["islegal"]), false);
		EXPECT_EQ(std::any_cast<double>(object["age"]), 25);

		Json &marks = *std::any_cast<Json *>(object["marks"]);
		EXPECT_EQ(std::any_cast<double>(marks[0]), 4);
		EXPECT_EQ(std::any_cast<double>(marks[1]), 5);

		Json &address = *std::any_cast<Json *>(object["address"]);
		EXPECT_EQ(std::any_cast<std::string>(address["city"]), "Moscow");
		EXPECT_EQ(std::any_cast<std::string>(address["street"]), "Vozdvijenka");
	}

TEST(Json, ParseFile)
	{
		auto json = Json::parseFile("../tests/TestJson.json");

		EXPECT_EQ(json.is_object(), true);
		EXPECT_EQ(json.is_array(), false);
		EXPECT_EQ(json.is_empty(), false);
	}
// Ïðîâåðêà íà íåïðàâèëüíûé ïóòü
TEST(Json, ParseFileException)
	{
		EXPECT_THROW(
			Json::parseFile("_file_do_not_exists_"),
			WrongJson
		);
	}
// Ïðîâåðêà íà äîáàâëåíèå îáúåêòà
TEST(Json, AddObject)
	{
		Json json{ "{}" };
		json.AddObject("key", std::string("value"));

		EXPECT_EQ(std::any_cast<std::string>(json["key"]), "value");
	}
// Ïðîâåðêà íà ïóñòûå îáúåêòû
TEST(Json, OperatorArrayTypeException)
	{
		Json json{ "[]" };

		EXPECT_THROW(
			json["test"],
			JsonNoContainer
		);
	}
// Ïðîâåðêà íà ïóñòûå îáúåêòû
TEST(Json, OperatorObjectTypeException)
	{
		Json json{ "{}" };

		EXPECT_THROW(
			json[0],
			JsonNoContainer
		);
	}
// Ïðîâåðêà íà ïóñòîé ìàññèâ
TEST(Json, OperatorArrayExceptionSize)
	{
		Json json{ "[]" };

		EXPECT_THROW(
			json[999],
			JsonNoContainer
		);
	}
// Ïðîâåðêà íà íåïðàâèëüíûé ââîä êëþ÷à
TEST(Json, OperatorObjectTypeExceptionNoKey)
	{
		Json json{ "{'key':1}" };

		EXPECT_THROW(
			json["__definitely_not_existing_key__"],
			JsonWrongKey
		);
	}
// Ïðîâåðêà íà íåïðàâèëüíûé ââîä json ñòðîêè
TEST(Json, NoJson)
	{
		EXPECT_THROW(
			Json json{ "false" },
			WrongJson
		);
	}
