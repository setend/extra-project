#include "query.h"
#include <algorithm>
#include <iterator>

using namespace std;


set<TextQuery::line_no> NotQuery::eval(const TextQuery& t) const
{
        set<line_no> has = query.eval(t);
        set<line_no> ans;
        for(int i=0 ;i<t.size();++i)
        {
                if(!has.count(i)) ans.insert(i);
        }
        return ans;
}

set<TextQuery::line_no> OrQuery::eval(const TextQuery& t) const
{
        set<line_no> rh = rhs.eval(t);
        set<line_no> lh = lhs.eval(t);
        lh.insert(rh.begin(),rh.end());
        return lh;
}

set<TextQuery::line_no> AndQuery::eval(const TextQuery &t) const
{
        set<line_no> a = rhs.eval(t);
        set<line_no> b = lhs.eval(t);
        set<line_no> ret;
        set_intersection(a.begin(),a.end(),b.begin(),b.end(),inserter(ret,ret.begin()));
        return ret;
}
