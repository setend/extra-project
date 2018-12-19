#ifndef QUERY_H

#define QUERY_H
#include "textquery.h"
#include <iostream>
#include <set>
using std::set;

class Query;
class Query_base{
        friend class Query;

protected:
        typedef TextQuery::line_no line_no;
        virtual ~Query_base(){};

private:
        virtual set<line_no> eval(const TextQuery&) const = 0;
        virtual std::ostream& display(std::ostream& = std::cout) const = 0;
};

class WordQuery:public Query_base{
        friend class Query;
        WordQuery(const std::string& t):query_word(t) {}

        set<line_no> eval(const TextQuery& t)  const
        {
                return t.run_query(query_word);
        }
        std::ostream& display(std::ostream &os) const
        {
                os<<query_word;
                return os;
        }

        const std::string query_word;

};


class Query{
        friend Query operator | (const Query&, const Query&);
        friend Query operator & (const Query&, const Query&);
        friend Query operator ~ (const Query&);

public:

        typedef TextQuery::line_no line_no;
        Query(const std::string &t):q(new WordQuery(t)),use(new std::size_t(1)) {}
        Query():q(0),use(0){}
        Query(const Query& t):q(t.q),use(t.use) { ++*use;}
        Query& operator =(const Query& t) { q = t.q; use = t.use; ++*use;}
        ~Query() { decr_use();}
        set<line_no> eval(const TextQuery& c) const { return q->eval(c);}
        std::ostream& display(std::ostream& os) const { return q->display(os);}


private:
        Query(Query_base *t):q(t),use(new std::size_t(1)) {}
        Query_base *q;
        std::size_t *use;
        void decr_use()
        {
                if(q && use)
                if( --*use == 0 ) {
                        delete q; delete use;
                }
        }
};

inline std::ostream& operator << (std::ostream& os,const Query &q)
{
        return q.display(os);
}

class NotQuery:public Query_base{
        friend Query operator ~(const Query&);
        NotQuery(const Query& t):query(t){}
        set<line_no> eval(const TextQuery&) const;
        std::ostream& display(std::ostream &os) const
        {
                os << "~"<<query;
                return os;
        }
        const Query query;
};

class BinaryQuery:public Query_base{
protected:
        BinaryQuery(const Query &l,const Query& r,const std::string & op):rhs(r),lhs(l),oper(op) {}
        std::ostream& display(std::ostream &os) const
        {       return os<<"("<<lhs<<" "<<oper<<" "<<rhs<<")";}

        const Query lhs,rhs;
        const std::string oper;
};

class OrQuery:public BinaryQuery{
        friend Query operator | (const Query&,const Query&);
        OrQuery(const Query &lh, const Query &rh):BinaryQuery(lh,rh,"|"){}

        set<line_no> eval(const TextQuery&) const;
};

class AndQuery:public BinaryQuery{
        friend Query operator & ( const Query&,const Query&);
        AndQuery(const Query &lh,const Query &rh):BinaryQuery(lh,rh,"&"){}
        set<line_no> eval(const TextQuery&) const;
};


inline Query operator | (const Query& lh,const Query &rh)
{
        return new OrQuery(lh,rh);
}
inline Query operator & (const Query &lh, const Query &rh)
{
        return new AndQuery(lh,rh);
}
inline Query operator ~ (const Query &t)
{
        return new NotQuery(t);
}


#endif //QUERY_H
