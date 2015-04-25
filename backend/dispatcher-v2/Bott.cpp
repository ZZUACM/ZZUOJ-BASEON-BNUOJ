/* 
 * File:   Bott.cpp
 * Author: 51isoft
 * 
 * Created on 2014年1月19日, 上午1:04
 */

#include "Bott.h"

const string Bott::RAW_FILES_DIRECTORY = "raw_files/";
const string Bott::CHA_RAW_FILES_DIRECTORY = "cha_raw_files/";
const string Bott::RESULTS_DIRECTORY = "results/";
const string Bott::CHA_RESULTS_DIRECTORY = "cha_results/";
const string Bott::EXTENTION = ".bott";

const string Bott::SOURCE_CODE_BEGIN = "__SOURCE-CODE-BEGIN-LABLE__";
const string Bott::SOURCE_CODE_END = "__SOURCE-CODE-END-LABLE__";
const string Bott::COMPILE_INFO_BEGIN = "__COMPILE-INFO-BEGIN-LABLE__";
const string Bott::COMPILE_INFO_END = "__COMPILE-INFO-END-LABLE__";
const string Bott::DATA_DETAIL_BEGIN = "__DATA-DETAIL-BEGIN-LABLE__";
const string Bott::DATA_DETAIL_END = "__DATA-DETAIL-END-LABLE__";
const string Bott::CHALLENGE_DETAIL_BEGIN = "__CHALLENGE-DETAIL-BEGIN-LABLE__";
const string Bott::CHALLENGE_DETAIL_END = "__CHALLENGE-DETAIL-END-LABLE__";

Bott::Bott()
{
	//ctor
}

Bott::~Bott()
{
	//dtor
}

string Bott::parseUntil(string end)
{
	string res = "", tmps;
	getline(fin, tmps);
	getline(fin, tmps);
	while (tmps != end) {
		res += tmps + "\n";
		getline(fin, tmps);
	}
	return res;
}

Bott::Bott(string filename)
{
	string name, value;
	infos.clear();
	fin.open(filename.c_str(), fstream::in);
	while (fin >> name) {
		if (name == SOURCE_CODE_BEGIN)
			src = parseUntil(SOURCE_CODE_END);
		else if (name == COMPILE_INFO_BEGIN)
			ce_info = parseUntil(COMPILE_INFO_END);
		else if (name == DATA_DETAIL_BEGIN)
			data_detail = parseUntil(DATA_DETAIL_END);
		else if (name == CHALLENGE_DETAIL_BEGIN)
			cha_detail = parseUntil(CHALLENGE_DETAIL_END);
		else {
			getline(fin, value);
			infos[name] = value;
		}
	}
	fin.close();
	if (infos.count("<type>"))
		type = stringToInt(infos["<type>"]);
	if (infos.count("<runid>"))
		runid = trim(infos["<runid>"]);
	if (infos.count("<cha_id>"))
		cha_id = trim(infos["<cha_id>"]);
	if (infos.count("<language>"))
		language = trim(infos["<language>"]);
	if (infos.count("<pid>"))
		pid = trim(infos["<pid>"]);
	if (infos.count("<testcases>"))
		number_of_testcases = trim(infos["<testcases>"]);
	if (infos.count("<time_limit>"))
		time_limit = trim(infos["<time_limit>"]);
	if (infos.count("<case_limit>"))
		case_limit = trim(infos["<case_limit>"]);
	if (infos.count("<memory_limit>"))
		memory_limit = trim(infos["<memory_limit>"]);
	if (infos.count("<special>"))
		spj = trim(infos["<special>"]);
	if (infos.count("<vname>"))
		vname = trim(infos["<vname>"]);
	if (infos.count("<vid>"))
		vid = trim(infos["<vid>"]);
	if (infos.count("<memory_used>"))
		memory_used = trim(infos["<memory_used>"]);
	if (infos.count("<time_used>"))
		time_used = trim(infos["<time_used>"]);
	if (infos.count("<result>"))
		result = trim(infos["<result>"]);
	if (infos.count("<data_type>"))
		data_type = trim(infos["<data_type>"]);
	if (infos.count("<data_lang>"))
		data_lang = trim(infos["<data_lang>"]);
	if (infos.count("<challenge_result>"))
		cha_result = trim(infos["<challenge_result>"]);
}

void Bott::toFile()
{

	FILE *fp = fopen(out_filename.c_str(), "w");
	if (type == NEED_JUDGE || type == DO_PRETEST || type == DO_TESTALL
	    || type == DO_INTERACTIVE) {
		fprintf(fp, "<type> %d\n", type);
		fprintf(fp, "%s\n", SOURCE_CODE_BEGIN.c_str());
		fprintf(fp, "%s\n", src.c_str());
		fprintf(fp, "%s\n", SOURCE_CODE_END.c_str());
		fprintf(fp, "<runid> %s\n", runid.c_str());
		fprintf(fp, "<language> %s\n", language.c_str());
		fprintf(fp, "<pid> %s\n", pid.c_str());
		fprintf(fp, "<testcases> %s\n", number_of_testcases.c_str());
		fprintf(fp, "<time_limit> %s\n", time_limit.c_str());
		fprintf(fp, "<case_limit> %s\n", time_limit.c_str());
		fprintf(fp, "<memory_limit> %s\n", memory_limit.c_str());
		fprintf(fp, "<special> %s\n", spj.c_str());
		fprintf(fp, "<vname> %s\n", vname.c_str());
		fprintf(fp, "<vid> %s\n", vid.c_str());
	} else if (type == DO_CHALLENGE) {
		fprintf(fp, "<type> %d\n", type);
		fprintf(fp, "%s\n", SOURCE_CODE_BEGIN.c_str());
		fprintf(fp, "%s\n", src.c_str());
		fprintf(fp, "%s\n", SOURCE_CODE_END.c_str());
		fprintf(fp, "<cha_id> %s\n", cha_id.c_str());
		fprintf(fp, "<language> %s\n", language.c_str());
		fprintf(fp, "<pid> %s\n", pid.c_str());
		fprintf(fp, "<data_type> %s\n", data_type.c_str());
		fprintf(fp, "<data_lang> %s\n", data_lang.c_str());
		fprintf(fp, "%s\n", DATA_DETAIL_BEGIN.c_str());
		fprintf(fp, "%s\n", data_detail.c_str());
		fprintf(fp, "%s\n", DATA_DETAIL_END.c_str());
		fprintf(fp, "<time_limit> %s\n", case_limit.c_str());
		fprintf(fp, "<case_limit> %s\n", case_limit.c_str());
		fprintf(fp, "<memory_limit> %s\n", memory_limit.c_str());
		fprintf(fp, "<special> %s\n", spj.c_str());
	}
	fclose(fp);
}
