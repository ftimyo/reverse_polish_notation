#include<iostream>
#include<regex>
#include<string>
#include<list>
#include<stack>
#include<map>

using namespace std;


class token:public string {
public:
	inline token(const string& s):string{s}{}
	inline token(string&& s):string{std::move(s)}{}
	inline token() = default;
	inline token(const token&) = default;
	inline token(token&&) = default;
	inline token& operator= (const token&) = default;
	inline token& operator= (token&&) = default;
	inline ~token() = default;
};

class tokens:public list<token> {
	public:
		inline void get_tokens_from_string(string);
		inline bool check_token(const token&, const regex&) const;
		void infix_to_postfix();
	protected:
		const static regex all_regex ;
		const static regex op_regex;
		const static regex var_regex;
		const static token op_add;
		const static token op_sub;
		const static token op_mul;
		const static token op_div;
		const static token op_open;
		const static token op_close;
		const static token op_rem;
		enum PRIO {LOW, MED, HIGH};
		static map<token,PRIO> prio;
};

const token tokens::op_add = token{"+"};
const token tokens::op_sub = token{"-"};
const token tokens::op_mul = token{"*"};
const token tokens::op_div = token{"/"};
const token tokens::op_rem = token{"%"};
const token tokens::op_open = token{"("};
const token tokens::op_close = token{")"};
map<token,tokens::PRIO> tokens::prio = {
	{tokens::op_add, tokens::LOW},
	{tokens::op_sub, tokens::LOW},
	{tokens::op_mul, tokens::MED},
	{tokens::op_div, tokens::MED},
	{tokens::op_rem, tokens::MED},
	{tokens::op_open, tokens::HIGH},
	{tokens::op_close, tokens::HIGH}
};

const regex tokens::all_regex = regex{"(_*[[:alpha:]]+[[:alnum:]_]*)|([()+*-/%])"};
const regex tokens::op_regex = regex{"([()+*-/])"};
const regex tokens::var_regex = regex{"(_*[[:alpha:]]+[[:alnum:]_]*)"};

inline bool tokens::check_token(const token& t, const regex& e) const
{
	return regex_match(t, e);
}

void tokens::get_tokens_from_string(string s)
{
	smatch m;
	clear();
	while (regex_search(s, m, all_regex)) {
		push_back(m.str());
		s = m.suffix().str();
	}
}

void tokens::infix_to_postfix()
{
	stack<token> filo;
	tokens postfix;
	tokens::iterator p = begin();

	while (p != end()) {
		token& t = *p++;
		if (check_token(t, var_regex)) {
			postfix.push_back(t);
			continue;
		}
		if (filo.empty()) {
			filo.push(t);
			continue;
		}
		while (!filo.empty()) {
			token& top = filo.top();

			if ((t == op_close) && (top != op_open)) {
				postfix.push_back(top);
				filo.pop();
				if (filo.empty()) {
					cerr<<"unmatched \""<<op_close<<"\""<<endl;
				}
				continue;
			} else if ((t == op_close)) {
				filo.pop();
				break;
			} else if ((prio[t] <= prio[top]) && (top != op_open)) {
				postfix.push_back(top);
				filo.pop();
				continue;
			} else {
				break;
			}

		}
		if (t != op_close) {
			filo.push(t);
		}
	}
	while (!filo.empty()) {
		token& top = filo.top();
		if (top == op_open) {
			cerr<<"unmatched \""<<op_open<<"\""<<endl;
			filo.pop();
			continue;
		}
		postfix.push_back(top);
		filo.pop();
	}
	std::swap(*this, postfix);
}


int main()
{
	string line;
	tokens tks;
	getline(cin, line);
	tks.get_tokens_from_string(line);
	tks.infix_to_postfix();
	for (auto& x: tks) {
		cout << x << " ";
	}
	cout<<endl;
	return 0;
}
