#ifndef TEXTQUERY_H
#define TEXTQUERY_H
#include <vector>
#include <set>
#include <map>

using std::vector;
using std::set;
using std::map;
using std::string;
class TextQuery
{
public:
      typedef vector<string>::size_type line_no;
        bool read_file(const string& t)
        {
               if(!store_file(t))
                       return false;
               build_map();
               return true;
        }
        set<line_no> run_query(const string&) const;
        string text_line(line_no) const;
        int size() const{ return Lines_of_text.size();}

private:

        bool store_file(const string&);
        void build_map();
        vector<std::string> Lines_of_text;
        map<string,set<line_no>> word_map;
};


#endif //TEXTQUERY_H
