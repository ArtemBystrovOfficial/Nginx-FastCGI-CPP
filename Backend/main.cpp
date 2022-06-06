#include <iostream>
#include <fstream>
#include "fcgio.h"
#include <string>
#include <nlohmann/json.hpp>
#include <vector>
#include "Back_end_http_server.h"
#include <locale>

using json = nlohmann::json;

std::string get_http(CSR doc_path, CSR css="")
{
    std::ifstream in(doc_path);
    std::string s="Content-type: text-html\r\n\r\n";
    char b;
    if(css=="")
    {
        while(!in.eof())
        {
             in.get(b);
             s.push_back(b);
        }
    }
    else
    {
        std::ifstream in_s(css);
         while(!in.eof())
        {
             in.get(b);
             s.push_back(b);
        }
        s+="<style>\n";
         while(!in_s.eof())
        {
             in_s.get(b);
             s.push_back(b);
        }
        s+="</style>\n";
        in_s.close();
    }
    in.close();
    return s;
}

std::string way_without_query(const char * str)
{
    std::string s;
    while(*str!='?')
    {
        s.push_back(*str);
        str++;
    }
    return s;
}

std::string char_ref_to_string(const char * str)
{
std::string s;
    while(*(str++)!='\0')
    {
        s.push_back(*str);
    }
    return s;
}

std::vector <std::string> get_values_query(const char * msg,int length)
{
    bool write=false;
    std::string data;
    std::vector <std::string> v;
    while(*msg!='\0' && length--)
    {
        if(*msg =='&')
        {
            write = false;
            v.push_back(data);
            data="";
        }
        if(write)
        {
            data+=*msg;
        }
            if(*msg=='=')
        {
            write = true;
        }
        msg++;
    };
    if(data.size()!=0)
    v.push_back(data);
    return v;
}

int main() {

    setlocale(LC_ALL,"Russian");

    std::streambuf * cin_streambuf = std::cin.rdbuf();
    std::streambuf * cout_streambuf = std::cout.rdbuf();
    std::streambuf * cerr_streambuf = std::cerr.rdbuf();

    FCGX_Request request;

    SERVER_LOGIC srv_log;

    FCGX_Init();
    FCGX_InitRequest(&request,0,0);

    while(FCGX_Accept_r(&request)==0)
    {
        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        fcgi_streambuf cerr_fcgi_streambuf(request.err);

        std::cin.rdbuf(&cin_fcgi_streambuf);
        std::cout.rdbuf(&cout_fcgi_streambuf);
        std::cerr.rdbuf(&cerr_fcgi_streambuf);

        std::string IP = char_ref_to_string(FCGX_GetParam("REMOTE_ADDR", request.envp));
        if(strcmp(FCGX_GetParam("QUERY_STRING", request.envp),"")!=0)
        {
            auto param = get_values_query(FCGX_GetParam("QUERY_STRING", request.envp),strlen(FCGX_GetParam("QUERY_STRING", request.envp)));

            if(strcmp(way_without_query(FCGX_GetParam("REQUEST_URI", request.envp)).c_str(),"/register")==0)
            {
                if(param.size()>=2)
                    {
                        if(srv_log.new_account(param[0],param[1]))
                             cout << get_http("/var/www/html/reg_acc.html").c_str();
                        else
                             cout << get_http("/var/www/html/reg_err.html").c_str();
                    }
            }

            if(strcmp(way_without_query(FCGX_GetParam("REQUEST_URI", request.envp)).c_str(),"/login")==0)
            {
                    if(param.size()>=2)
                    {
                        auto mode = srv_log.autification(IP, param[0],param[1]);
                        if(mode==1)
                             cout << get_http("/var/www/html/log_acc.html").c_str();
                        if(mode==-1)
                             cout << get_http("/var/www/html/log_err.html").c_str();
                        if(mode==0)
                             cout << get_http("/var/www/html/iplog.html").c_str();
                    }
            }

            if(strcmp(way_without_query(FCGX_GetParam("REQUEST_URI", request.envp)).c_str(),"/shop")==0)
            {
                auto [is_connect, nickname] = srv_log.is_logining(IP);
                if(!is_connect)
                {
                    cout << get_http("/var/www/html/brigde_login.html").c_str();
                }
                else
                {
                    if(param.size()==1)
                    {
                        srv_log.add_basket_elem(srv_log.get_nickname(IP),std::atoi(param[0].c_str()));
                    } 
                cout << get_http("/var/www/html/shop_out.html").c_str();
                }
            }

            if(strcmp(way_without_query(FCGX_GetParam("REQUEST_URI", request.envp)).c_str(),"/make")==0)
            {
                auto [is_connect, nickname] = srv_log.is_logining(IP);
                if(!is_connect)
                {
                    cout << get_http("/var/www/html/brigde_login.html").c_str();
                }
                else
                {
                    if(param.size()==2)
                    {
                        int id=0;
                        int i = param[0][0]-'0';
                        int j = param[1][0]-'0';
                        if(i==1 && j==1)id=1;
                        if(i==1 && j==2)id=3;
                        if(i==2 && j==1)id=2;
                        if(i==2 && j==2)id=4;
                        srv_log.add_basket_elem(srv_log.get_nickname(IP),id);
                    } 
                    cout << get_http("/var/www/html/make.html").c_str();
                }
            }
        }
        else
        {
            if(strcmp(FCGX_GetParam("REQUEST_URI", request.envp),"/register")==0)
            {          
                cout << get_http("/var/www/html/register.html").c_str();
            }

            if(strcmp(FCGX_GetParam("REQUEST_URI", request.envp),"/login")==0)
            {
                cout << get_http("/var/www/html/login.html").c_str();
            }

             if(strcmp(FCGX_GetParam("REQUEST_URI", request.envp),"/main")==0)
            {
                auto [is_login, nickname] = srv_log.is_logining(IP);
                if(is_login)
                {
                    HTML_CONV::past_elem_from_id_to_doc("/var/www/html/logining_template.txt","name_login",
                                                        nickname,                                             
                                                        "/var/www/html/logining_data.txt");
                    HTML_CONV::past_elem_from_id_to_doc("/var/www/html/main.html","past_block",
                                                        HTML_CONV::get_template("/var/www/html/logining_data.txt"),
                                                        "/var/www/html/out.html");
                    
                    cout << get_http("/var/www/html/out.html","/var/www/html/styles.css").c_str();
                }
                else
                {
                    HTML_CONV::past_elem_from_id_to_doc("/var/www/html/main.html","past_block",
                                                        HTML_CONV::get_template("/var/www/html/reg_template.txt"),
                                                        "/var/www/html/out.html");


                    cout << get_http("/var/www/html/out.html","/var/www/html/styles.css").c_str();
                }
            }

            if(strcmp(FCGX_GetParam("REQUEST_URI", request.envp),"/shop")==0)
            {
                std::string port;
                std::string from;
                for(int i=1;i<10;i++)
                {
                port="port_";
                from="/var/www/html/products/"+std::to_string(i);
                port+=std::to_string(i);
                from+=".html";
                    if(i==1)
                    {
                        HTML_CONV::past_elem_from_id_to_doc("/var/www/html/shop.html",port,
                                                                HTML_CONV::get_template(from),
                                                                "/var/www/html/shop_out.html");
                    }
                    else
                    {
                        if(i%2==0)
                        {
                        HTML_CONV::past_elem_from_id_to_doc("/var/www/html/shop_out.html",port,
                                                                HTML_CONV::get_template(from),
                                                                "/var/www/html/shop_out1.html");
                        }
                        else
                        {
                        HTML_CONV::past_elem_from_id_to_doc("/var/www/html/shop_out1.html",port,
                                                                HTML_CONV::get_template(from),
                                                                "/var/www/html/shop_out.html");    
                        }
                    }
                }
                cout << get_http("/var/www/html/shop_out.html").c_str();
            }

            if(strcmp(FCGX_GetParam("REQUEST_URI", request.envp),"/local")==0)
            {
                auto [is_login, nickname] = srv_log.is_logining(IP);
                if(is_login)
                {
                auto basket = srv_log.get_basket(nickname);
                int id=1;
                std::string port;
                std::string from;
                
                for(auto i : basket)
                {
                    if(i!='0')
                    {
                    port="port_";
                    from="/var/www/html/products/";
                    from+=i;
                    port+=std::to_string(id);
                    from+=".html";
                        if(id==1)
                        {
                            HTML_CONV::past_elem_from_id_to_doc("/var/www/html/local.html",port,
                                                                    HTML_CONV::get_template(from),
                                                                    "/var/www/html/local_out.html");
                        }
                        else
                        {
                            if(id%2==0)
                            {
                            HTML_CONV::past_elem_from_id_to_doc("/var/www/html/local_out.html",port,
                                                                    HTML_CONV::get_template(from),
                                                                    "/var/www/html/local_out1.html");
                            }
                            else
                            {
                            HTML_CONV::past_elem_from_id_to_doc("/var/www/html/local_out1.html",port,
                                                                    HTML_CONV::get_template(from),
                                                                    "/var/www/html/local_out.html");    
                            }
                        }
                        id++;
                    }
                }
                if(id%2==0)
                    cout << get_http("/var/www/html/local_out.html").c_str();
                else
                    cout << get_http("/var/www/html/local_out1.html").c_str();
                }
                else
                    cout << get_http("/var/www/html/brigde_login.html");
            }

            if(strcmp(FCGX_GetParam("REQUEST_URI", request.envp),"/make")==0)
            {
                cout << get_http("/var/www/html/make.html");
            }

            if(strcmp(FCGX_GetParam("REQUEST_URI", request.envp),"/sport")==0)
            {
                cout << get_http("/var/www/html/sport.html"); 
            }

            if(strcmp(FCGX_GetParam("REQUEST_URI", request.envp),"/dilivery")==0)
            {
                cout << get_http("/var/www/html/dilivery.html"); 
            }

             if(strcmp(FCGX_GetParam("REQUEST_URI", request.envp),"/exit")==0)
            {
                srv_log.disautification(IP);
                cout << get_http("/var/www/html/exit.html"); 
            }

            if(strcmp(FCGX_GetParam("REQUEST_URI", request.envp),"/about")==0)
            {
                cout << get_http("/var/www/html/about.html","/var/www/html/about.css"); 
            }

        }
       // cout <<"Content-type: text-html\r\n\r\n"<<s.c_str();
      //FCGX_GetParam("QUERY_STRING", request.envp);
      //FCGX_GetParam("REMOTE_ADDR", request.envp);
    }
    std::cin.rdbuf(cin_streambuf);
    std::cout.rdbuf(cout_streambuf);
    std::cerr.rdbuf(cerr_streambuf);

    return 0;
    
}
