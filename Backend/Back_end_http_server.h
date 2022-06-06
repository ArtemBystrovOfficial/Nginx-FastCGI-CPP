
#pragma once

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <limits>

using namespace std;

using CSR = const std::string&;

const std::string PATH_SITE = "/var/www/html/";


class SERVER_LOGIC
{
	// IP -> NICKNAME
	std::map <std::string, std::string> autifications;

	bool is_log_exist(CSR login);

	bool is_log_pass_correct(CSR login, CSR pass);

	void add_new_account(CSR , CSR );

public:

	std::string get_nickname(CSR IP);
	
	void add_basket_elem(CSR,int _basket_item);

	std::string get_basket(CSR);

	std::pair <bool, std::string> is_logining(CSR);

	int autification(std::string IP, CSR login, CSR pass);

	void disautification(CSR IP);

	bool new_account(CSR login, CSR pass);

};

struct HTML_CONV
{
public:

	static std::string get_template(CSR template_path)
	{
		std::fstream fileInput(template_path, std::ios_base::in);
		std::string out;
		std::string line;
		if (!fileInput.is_open())
		{
			throw "error open template.txt";
		}

		while (getline(fileInput, line)) { 
			out += line;
		}

		fileInput.close();
		return out;
	}

	static bool past_elem_from_id_to_doc(CSR document, CSR id, CSR past_elem, CSR new_doc)
	{
		std::fstream fileInput(document, std::ios_base::in);
		std::fstream fileOutput(new_doc, std::ios_base::out);
		if (!fileInput.is_open())
		{
			throw "error open index.html";
		}
		std::string line;
		bool is_find=false;
		while (getline(fileInput, line)) { // I changed this, see below
			fileOutput << line<<std::endl;

			if (line.find(id, 0) != string::npos)
			{
					fileOutput << past_elem;
					is_find = true;
			}
		}
		fileOutput.close();
		fileInput.close();
		return is_find;
	}

private:

};

