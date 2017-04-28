
#include "cppmicroservices/Any.h"
#include "gtest/gtest.h"
#include <limits>

using namespace cppmicroservices;

TEST(AnyTest, anyBool)
{
	
	
	Any anyBool = true;
	
	EXPECT_EQ(anyBool.Type(), typeid(bool));
	EXPECT_EQ(any_cast<bool>(anyBool), true);
	
	
	EXPECT_EQ(anyBool.ToString(), "1");
	EXPECT_EQ(anyBool.ToJSON(), "true");
	
	
	anyBool = false;
	
	EXPECT_EQ(anyBool.ToString(), "0");
	EXPECT_EQ(anyBool.ToJSON(), "false");
	
}

TEST(AnyTest, anyInt)
{
	Any anyInt = 13;
	
	EXPECT_EQ(anyInt.Type(), typeid(int));
	EXPECT_EQ(any_cast<int>(anyInt), 13);
	EXPECT_EQ(anyInt.ToString(), "13");
	EXPECT_EQ(anyInt.ToJSON(), "13");
	
}

TEST(AnyTest, anyChar)
{
	Any anyChar = 'a';
	
	EXPECT_EQ(anyChar.Type() , typeid(char));
	EXPECT_EQ(any_cast<char>(anyChar) , 'a');
	EXPECT_EQ(anyChar.ToString() , "a");
	EXPECT_EQ(anyChar.ToJSON() , "a");
}

TEST(AnyTest, anyFloat)
{

	Any anyFloat = 0.2f;
	
	EXPECT_EQ(anyFloat.Type() , typeid(float));
	EXPECT_EQ(any_cast<float>(anyFloat) - 0.2f < std::numeric_limits<float>::epsilon(), true);
	EXPECT_EQ(anyFloat.ToString(), "0.2");
	EXPECT_EQ(anyFloat.ToString(), "0.2");
	
}

TEST(AnyTest, anyDouble)
{
	Any anyDouble = 0.5;
	
	EXPECT_EQ(anyDouble.Type(), typeid(double));
	EXPECT_EQ(any_cast<double>(anyDouble) - 0.5 < std::numeric_limits<double>::epsilon(), true);
	EXPECT_EQ(anyDouble.ToString(), "0.5");
	EXPECT_EQ(anyDouble.ToString() , "0.5");
}

TEST(AnyTest, anyString)
{
	Any anyString = std::string("hello");
	
	EXPECT_EQ(anyString.Type(), typeid(std::string));
	EXPECT_EQ(any_cast<std::string>(anyString) , "hello");
	EXPECT_EQ(anyString.ToString(), "hello");
	EXPECT_EQ(anyString.ToJSON() , "\"hello\"");
}

TEST(AnyTest, anyVector)
{
	std::vector<int> vecInts;
	vecInts.push_back(1);
	vecInts.push_back(2);
	Any anyVectorOfInts = vecInts;
	
	EXPECT_EQ(anyVectorOfInts.Type(), typeid(std::vector<int>));
	EXPECT_EQ(any_cast<std::vector<int> >(anyVectorOfInts) , vecInts);
	EXPECT_EQ(anyVectorOfInts.ToString() , "[1,2]");
	EXPECT_EQ(anyVectorOfInts.ToJSON() , "[1,2]");
}

TEST(AnyTest, anyList)
{
	std::list<int> listInts;
	listInts.push_back(1);
	listInts.push_back(2);
	Any anyListOfInts = listInts;
	
	EXPECT_EQ(anyListOfInts.Type() ,typeid(std::list<int>));
	EXPECT_EQ(any_cast<std::list<int> >(anyListOfInts) , listInts);
	EXPECT_EQ(anyListOfInts.ToString() , "[1,2]");
	EXPECT_EQ(anyListOfInts.ToJSON() , "[1,2]");
}

TEST(AnyTest, anySet)
{
	std::set<int> setInts;
	setInts.insert(1);
	setInts.insert(2);
	Any anySetOfInts = setInts;
	
	EXPECT_EQ(anySetOfInts.Type() , typeid(std::set<int>));
	EXPECT_EQ(any_cast<std::set<int> >(anySetOfInts) , setInts);
	EXPECT_EQ(anySetOfInts.ToString(), "[1,2]");
	EXPECT_EQ(anySetOfInts.ToJSON(), "[1,2]");
}

TEST(AnyTest, vectorOfAnys)
{
	std::vector<Any> vecAny;
	vecAny.push_back(1);
	vecAny.push_back(std::string("hello"));
	Any anyVectorOfAnys = vecAny;
	
	EXPECT_EQ(anyVectorOfAnys.Type() , typeid(std::vector<Any>));
	EXPECT_EQ(anyVectorOfAnys.ToString() , "[1,hello]");
	EXPECT_EQ(anyVectorOfAnys.ToJSON() , "[1,\"hello\"]");
}

TEST(AnyTest, listofAnys)
{
	std::list<Any> listAny;
	listAny.push_back(1);
	listAny.push_back(std::string("hello"));
	Any anyListOfAnys = listAny;
	
	EXPECT_EQ(anyListOfAnys.Type() , typeid(std::list<Any>));
	EXPECT_EQ(anyListOfAnys.ToString() , "[1,hello]");
	EXPECT_EQ(anyListOfAnys.ToJSON() , "[1,\"hello\"]");
	
	std::map<std::string, int> map1;
	map1["one"] = 1;
	map1["two"] = 2;
	Any anyMap1 = map1;
	
	EXPECT_EQ(anyMap1.Type() , typeid(std::map<std::string, int>));
	EXPECT_EQ((any_cast<std::map<std::string, int> >(anyMap1)), map1);
	EXPECT_EQ(anyMap1.ToString() , "{one : 1, two : 2}");
	EXPECT_EQ(anyMap1.ToJSON() , "{\"one\" : 1, \"two\" : 2}");
	
	std::map<int, Any> map2;
	map2[1] = 0.3;
	map2[3] = std::string("bye");
	Any anyMap2 = map2;
	
	EXPECT_EQ(anyMap2.Type() , typeid(std::map<int, Any>));
	EXPECT_EQ(anyMap2.ToString() , "{1 : 0.3, 3 : bye}");
	EXPECT_EQ(anyMap2.ToJSON() , "{\"1\" : 0.3, \"3\" : \"bye\"}");
	
	std::map<std::string, Any> map3;
	map3["number"] = 5;
	std::vector<int> numbers;
	numbers.push_back(9);
	numbers.push_back(8);
	numbers.push_back(7);
	map3["vector"] = numbers;
	map3["map"] = map2;
	Any anyMap3 = map3;
	
	EXPECT_EQ(anyMap3.Type() , typeid(std::map<std::string, Any>));
	EXPECT_EQ(anyMap3.ToString() , "{map : {1 : 0.3, 3 : bye}, number : 5, vector : [9,8,7]}");
	EXPECT_EQ(anyMap3.ToJSON(), "{\"map\" : {\"1\" : 0.3, \"3\" : \"bye\"}, \"number\" : 5, \"vector\" : [9,8,7]}");
	
	
}