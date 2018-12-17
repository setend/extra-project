#include <iostream>
#include "textquery.h"

using namespace std;

int main()
{
        TextQuery t;
        if(!t.read_file("main.cc"))
        {
                cout<<"fail get the file"<<endl;
                return 0;
        }
        auto ans = t.run_query("#include");
        
        for(auto it = ans.begin();it != ans.end(); ++it)
        {
                cout<<*it<<"  "<<t.text_line(*it)<<endl;
        }
        return 0;
}
                

