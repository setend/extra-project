#include "textquery.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

bool TextQuery::store_file(const string& t)
{
        ifstream ifs(t.c_str());
        if(!ifs) 
        {
                cout<<"can't open the file : "<<t << endl;

                return false;
        }
        if(!Lines_of_text.empty())
        {
                Lines_of_text.clear();
        }
        string str;
        while(getline(ifs,str))
                        Lines_of_text.push_back(str);
        return true;
}

void TextQuery::build_map()
{
        for(int i=0;i<Lines_of_text.size();++i)
        {
                stringstream ss(Lines_of_text[i]);
                string word;
                while(ss>>word) word_map[word].insert(i);
         }
}

set<TextQuery::line_no> TextQuery::run_query(const string& str) const
{
        if(word_map.count(str)) return word_map.find(str)->second;
        return set<line_no>();
}

string TextQuery::text_line(line_no n) const
{
        if( n < Lines_of_text.size()) return Lines_of_text[n];
        return "";
}

