/* 
 * File:   ACdreamJudger.cpp
 * Author: 51isoft
 * 
 * Created on 2014年8月13日, 下午9:52
 */

#include "ACdreamJudger.h"

ACdreamJudger::ACdreamJudger(JudgerInfo * _info):VirtualJudger(_info)
{
	socket->sendMessage(CONFIG->GetJudge_connect_string() + "\nACdream");

	language_table["1"] = "2";
	language_table["2"] = "1";
}

ACdreamJudger::~ACdreamJudger()
{
}

/**
 * Login to ACdream
 */
void ACdreamJudger::login()
{
	prepareCurl();
	curl_easy_setopt(curl, CURLOPT_URL, "http://acdream.info/login");
	string post =
	    (string) "username=" + info->GetUsername() + "&password=" +
	    escapeURL(info->GetPassword());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post.c_str());
	performCurl();

	string html = loadAllFromFile(tmpfilename);
	//cout<<ts;
	if (html != "") {
		throw Exception("Login failed!");
	}
}

/**
 * Submit a run
 * @param bott      Bott file for Run info
 * @return Submit status
 */
int ACdreamJudger::submit(Bott * bott)
{
	prepareCurl();
	curl_easy_setopt(curl, CURLOPT_URL, "http://acdream.info/submit");
	string post =
	    (string) "pid=" + bott->Getvid() + "&lang=" +
	    escapeURL(bott->Getlanguage()) + "&code=" +
	    escapeURL(bott->Getsrc());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post.c_str());

	try {
		performCurl();
	} catch(Exception & e) {
		return SUBMIT_OTHER_ERROR;
	}

	string html = loadAllFromFile(tmpfilename);
	if (html != "")
		return SUBMIT_OTHER_ERROR;
	return SUBMIT_NORMAL;
}

/**
 * Get result and related info
 * @param bott  Original Bott info
 * @return Result Bott file
 */
Bott *ACdreamJudger::getStatus(Bott * bott)
{
	time_t begin_time = time(NULL);

	Bott *result_bott;
	while (true) {
		// check wait time
		if (time(NULL) - begin_time > info->GetMax_wait_time()) {
			throw
			    Exception
			    ("Failed to get current result, judge time out.");
		}

		prepareCurl();
		curl_easy_setopt(curl, CURLOPT_URL,
				 ((string) "http://acdream.info/status?name=" +
				  escapeURL(info->GetUsername()) + "&pid=" +
				  bott->Getvid() + "&lang=" +
				  bott->Getlanguage()).c_str());
		performCurl();

		string html = loadAllFromFile(tmpfilename);
		string status;
		string runid, result, time_used, memory_used;

		// get first row
		if (!RE2::PartialMatch
		    (html, "(?s)<tbody>(<tr.*?</tr>)", &status)) {
			throw Exception("Failed to get status row.");
		}
		// get result
		if (!RE2::PartialMatch(status,
				       "(?s)<td rid=\"([0-9]*)\".*?>(.*?)</td>",
				       &runid, &result)) {
			throw Exception("Failed to get current result.");
		}
		result = convertResult(trim(result));
		if (isFinalResult(result)) {
			if (!RE2::PartialMatch(status,
					       "(?s)>([0-9]*) MS<.*?>([0-9]*) KB<",
					       &time_used, &memory_used)) {
				throw
				    Exception
				    ("Failed to parse details from status row.");
			}
			result_bott = new Bott;
			result_bott->Settype(RESULT_REPORT);
			result_bott->Setresult(convertResult(result));
			result_bott->Settime_used(trim(time_used));
			result_bott->Setmemory_used(trim(memory_used));
			result_bott->Setremote_runid(trim(runid));
			break;
		}
	}
	return result_bott;
}

/**
 * Convert result text to local ones, keep consistency
 * @param result Original result
 * @return Converted local result
 */
string ACdreamJudger::convertResult(string result)
{
	if (result.find("Time Limit Exceeded") != string::npos)
		return "Time Limit Exceed";
	if (result.find("Memory Limit Exceeded") != string::npos)
		return "Memory Limit Exceed";
	if (result.find("Output Limit Exceeded") != string::npos)
		return "Output Limit Exceed";
	if (result.find("Compilation Error") != string::npos)
		return "Compile Error";
	if (result.find("Dangerous Code") != string::npos)
		return "Restricted Function";
	if (result.find("System Error") != string::npos)
		return "Judge Error";
	return trim(result);
}

/**
 * Get compile error info
 * @param bott      Result bott file
 * @return Compile error info
 */
string ACdreamJudger::getCEinfo(Bott * bott)
{

	prepareCurl();
	curl_easy_setopt(curl, CURLOPT_URL, "http://acdream.info/status/CE");
	string post = (string) "rid=" + bott->Getremote_runid();
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post.c_str());
	performCurl();

	string info = loadAllFromFile(tmpfilename);
	return trim(info);
}
