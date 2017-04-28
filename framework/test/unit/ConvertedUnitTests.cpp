

#include "gtest/gtest.h"

#include "cppmicroservices/detail/Log.h"

//#include "TestingMacros.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <regex>
#include <thread>
#include <vector>

using namespace cppmicroservices;


TEST(LogTest, testDefaultLogMessages)
{
	std::stringstream temp_buf;
	auto clog_buf = std::clog.rdbuf();
	std::clog.rdbuf(temp_buf.rdbuf());
	detail::LogSink sink(&std::clog, true);
	DIAG_LOG(sink) << "Testing " << 1 << 2 << 3 << ", Testing " << std::scientific << 1.0 << static_cast<void*>(nullptr) << 2.0 << 3.0 << "\n";
	sink.Log(std::string("blaaaaaaaaaaaaaaaaaah\n"));
	
	std::clog.rdbuf(clog_buf);

	
	
	EXPECT_NE(std::string::npos, temp_buf.str().find(std::string("blaaaaaaaaaaaaaaaaaah\n")));
	
	EXPECT_NE(std::string::npos, temp_buf.str().find(std::string(__FUNCTION__)));
			  
	EXPECT_NE(std::string::npos, temp_buf.str().find(std::string(__FILE__)));
	
}

TEST(LogTest,testNullSink)
{
	// A null (i.e. disabled logger) shouldn't throw when used.
	
	detail::LogSink sink_null(nullptr);
	
	ASSERT_NO_THROW(sink_null.Log(std::string("Don't log me")));
	
	ASSERT_NO_THROW(DIAG_LOG(sink_null) << "Shouldn't see this...");
	
}


//TEST(LogTest,testEmptyStream)
//{
	
//}

TEST(LogTest, testLogRedirection)
{
	std::ofstream filestream("foo.txt", std::ofstream::trunc);
	std::ostringstream stringstream;
	std::stringstream test_log_output;
	test_log_output << "Testing..." << 1 << " " << 2 << " " << 12 << "\n";
	
	detail::LogSink sink_stringstream(&stringstream, true);
	DIAG_LOG(sink_stringstream) << test_log_output.str();

	EXPECT_EQ(stringstream.str().empty(), false);
	
	EXPECT_NE(std::string::npos, stringstream.str().find(test_log_output.str()));
	
	
	detail::LogSink sink_file(&filestream, true);
	DIAG_LOG(sink_file) << test_log_output.str();
	filestream.flush();
	std::ifstream test_file("foo.txt");
	std::stringstream test_output_stream;
	test_file >> test_output_stream.rdbuf();

	EXPECT_EQ(test_output_stream.str().empty(),false);
	
	EXPECT_NE(std::string::npos,test_output_stream.str().find(test_log_output.str()));
	
	test_file.close();
	std::remove("foo.txt");
	
	// A null (i.e. disabled logger) shouldn't throw when used.
	detail::LogSink sink_null(nullptr, true);
	
	ASSERT_NO_THROW(DIAG_LOG(sink_null) << test_log_output.str());
	
	// Redirect cerr instead of cout since cout is used to output test information
	
	std::ostringstream local_cerr_buffer;
	auto cerr_buffer = std::cerr.rdbuf();
	std::cerr.rdbuf(local_cerr_buffer.rdbuf());
	
	detail::LogSink sink_redirected_cerr(&std::cerr, true);
	DIAG_LOG(sink_redirected_cerr) << test_log_output.str();
	std::cerr.rdbuf(cerr_buffer);

	EXPECT_EQ(local_cerr_buffer.str().empty(),false);
	EXPECT_NE(std::string::npos,local_cerr_buffer.str().find(test_log_output.str()));
	
}

#ifdef US_ENABLE_THREADING_SUPPORT
TEST(LogTest,testLogMultiThreaded)
{
	std::stringstream stringstream;
	detail::LogSink sink(&stringstream, true);
	
	std::size_t num_threads(100);
	std::vector<std::thread> threads;
	for (std::size_t i = 0; i < num_threads; ++i)
	{
		threads.push_back(std::thread([&sink, &num_threads]()
		{
			for (std::size_t i = 0; i < num_threads; ++i)
			{
				DIAG_LOG(sink) << "MACRO: START foo\n";
				sink.Log(std::string("foo bar boo baz\n"));
				DIAG_LOG(sink) << "MACRO: END foo\n";
			}
		}));
	}

	for (auto& t : threads) t.join();
	
	// instead of testing for all three log lines in one regular expressions, test for each line
	// as logging a single line is expected to be thread safe. It is NOT guaranteed that all three
	// log lines will appear in the correct order.

	std::ptrdiff_t expected_num_matches(num_threads * num_threads);
#if defined (US_HAVE_REGEX)
	std::string func_name(__FUNCTION__);
	std::string file_name = std::regex_replace(std::string(__FILE__), std::regex("\\\\"), std::string("\\\\"));
	std::string logpreamble("In (" + func_name + "::<lambda(\\w+)>::)*operator(\\s)?\\(\\) at " + file_name + ":(\\d+) :");
	std::regex reg_expr_start(logpreamble + std::string(" MACRO: START foo\n"));
	std::regex reg_expr_middle("foo bar boo baz\n");
	std::regex reg_expr_end(logpreamble + std::string(" MACRO: END foo\n"));
	
	const std::string stream(stringstream.str());
	auto regex_iter_end = std::sregex_iterator();
	
	auto regex_iter_begin = std::sregex_iterator(stream.begin(), stream.end(), reg_expr_start);
	std::ptrdiff_t num_found = std::distance(regex_iter_begin, regex_iter_end);
	
	EXPECT_EQ(num_found, expected_num_matches);
	
	regex_iter_begin = std::sregex_iterator(stream.begin(), stream.end(), reg_expr_middle);
	num_found = std::distance(regex_iter_begin, regex_iter_end);
	
	EXPECT_EQ(num_found, expected_num_matches);
	
	regex_iter_begin = std::sregex_iterator(stream.begin(), stream.end(), reg_expr_end);
	num_found = std::distance(regex_iter_begin, regex_iter_end);
	
	EXPECT_EQ(num_found, expected_num_matches);
	
	#else
	// support compilers w/o c++11 regex support...
	// the regex approach is more strict in checking there is no
	// log splicing however this suboptimal approach will do for now.
	std::ptrdiff_t total_expected_matches = expected_num_matches * 3;
	std::vector<std::string> log_lines;
	std::string line;
	while (std::getline(stringstream, line, '\n'))
	{
		log_lines.push_back(line);
	}

	EXPECT_EQ(static_cast<std::ptrdiff_t>(log_lines.size()), total_expected_matches);
	
	
	std::ptrdiff_t count = std::count_if(log_lines.begin(), log_lines.end(), [](const std::string& s) { return (std::string::npos != s.find(std::string("MACRO: START foo"))); });

	EXPECT_EQ(count, expected_num_matches);

#endif
}
#endif


