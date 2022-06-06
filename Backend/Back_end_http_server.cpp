// Back_end_http_server.cpp: определяет точку входа для приложения.
//

#include "Back_end_http_server.h"



bool SERVER_LOGIC::is_log_exist(CSR _login)
{
	bool is = false;
	std::ifstream in(PATH_SITE + "logins.txt");
	if (!in.is_open())
	{
		throw "Error: open logins.txt";
	}
	else
	{
		std::string login, pass, basket;
		while (!in.eof())
		{
			in >> login >> pass>>basket;
			if (login == _login)
			{
				is = true;
				break;
			}
		}
	}
	in.close();
	return is;
}

bool SERVER_LOGIC::is_log_pass_correct(CSR _login, CSR _pass)
{
	bool is = false;
	std::ifstream in(PATH_SITE + "logins.txt");
	if (!in.is_open())
	{
		throw "Error: open logins.txt";
	}
	else
	{
		std::string login, pass, basket;
		while (!in.eof())
		{
			in >> login >> pass>> basket;
			if (login == _login && pass == _pass)
			{
				is = true;
				break;
			}
		}
	}
	in.close();
	return is;
}

void SERVER_LOGIC::add_new_account(CSR _login, CSR _pass)
{
	std::ofstream out(PATH_SITE + "logins.txt",std::ios_base::app);
	if (!out.is_open())
	{
		throw "Error: open logins.txt";
	}
	else
	{
		out << _login <<" "<< _pass <<" "<<0 << std::endl;
	}
	out.close();
}

int SERVER_LOGIC::autification(std::string IP, CSR login, CSR pass)
{
	if (!is_log_pass_correct(login, pass))
		return -1;

	if (this->autifications.find(IP) == this->autifications.end())
	{
		autifications[IP] = login;
		return 1;
	}
	else
	{
		return 0;
	}

}

std::pair <bool, std::string > SERVER_LOGIC::is_logining(CSR IP)
{
	auto node = autifications.find(IP);
	if (node == autifications.end())
	{
		return { false, std::string("") };
	}
	else
		return { true , node->second };
}

void SERVER_LOGIC::disautification(CSR IP)
{
	auto node = autifications.find(IP);
	if (node == autifications.end())
	{
		throw "Error with disautification";
	}
	autifications.erase(node);
}

bool SERVER_LOGIC::new_account(CSR login, CSR pass)
{
	if (is_log_exist(login))
		return false;
	else
		 add_new_account(login, pass);
	return true;
}

std::string SERVER_LOGIC::get_basket(CSR _login)
{
	std::ifstream in(PATH_SITE + "logins.txt");
	std::string login, pass, basket;
		while (!in.eof())
		{
			in >> login >> pass>> basket;
			if (login == _login)
			{
				return basket;
			}
		}
		in.close();
	return "";
}
std::string SERVER_LOGIC::get_nickname(CSR IP)
{
	auto nick = autifications.find(IP);
	if(nick==autifications.end())
	{
		return "";
	}
	else
		return nick->second;
}
void SERVER_LOGIC::add_basket_elem(CSR _login ,int _basket_item)
{
	std::string before;
	int crit1=0, crit2=0;
	std::fstream in(PATH_SITE + "logins.txt", ios_base::in | ios_base::out);
	std::fstream data(PATH_SITE + "data.txt", ios_base::in | ios_base::out);
	std::string login, pass, basket;
		while (!in.eof() &&crit1<10000)
		{
			in >> login >> pass >> basket;
			if(login!=before)
			{
			if (login == _login)
			{
				
				data<< login <<" "<< pass<<" "<< basket<<_basket_item<<"\n";
			}
			else
			{
			data<< login <<" "<< pass<<" "<< basket<<"\n";
			}
			}
			before = login;
			crit1++;
		}
		data.close();
		in.close();
		in.open(PATH_SITE + "logins.txt", ios_base::in | ios_base::out);
		data.open(PATH_SITE + "data.txt", ios_base::in | ios_base::out);
		while (!data.eof()&&crit2<10000)
		{
			data >> login >> pass >> basket;
			if(login!=before)
			in << login <<" "<< pass<<" "<< basket<<"\n";
			crit2++;
			before = login;
		}
		in.close();
		data.close();
}