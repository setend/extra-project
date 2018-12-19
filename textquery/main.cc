#include <iostream>
#include "textquery.h"
#include "query.h"
#include <ctype.h>
#include <stdlib.h>
using namespace std;

string get_word(string &str, int &i)
{
        int k=i;
        while(i<str.size()&&isalnum(str[i])) ++i;
        return str.substr(k,i-k);
}

Query parse_str(string str)
{
        Query ans;
        int last = 0;
        for(int i = 0; i<str.size();)
        {
                Query t;
                if(str[i] == '~') { Query x(get_word(str,++i)); t=~x;}
                else if(str[i] == '|') { last = 1; ++i;continue;}
                else if(str[i] == '&') { last = 2; ++i; continue;}
                else { Query x(get_word(str,++i)); t = x;}
                if(last == 0) ans = t;
                else if( last == 1) ans = ans | t;
                else ans = ans&t;
                }
                return ans;
}

int main(int argc, char **argv)
{
        if(argc != 3) 
        {
                cout<<"you must input with correct format like : ./textquery filename word"<<endl;
                return 1;
        }
        TextQuery t;
        if(!t.read_file(argv[1]))
        {
                cout<<"fail get the file"<<endl;
                return 0;
        }
        Query ans_query = parse_str(argv[2]);
        cout<<ans_query<<endl;
        set<TextQuery::line_no> ans = ans_query.eval(t);
        
        for(auto it = ans.begin();it != ans.end(); ++it)
        {
                cout<<*it<<"  "<<t.text_line(*it)<<endl;
        }
        return 0;
}
                

