#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
using namespace std;
stack <bool> run;
int It, function_counter=0, goto_function=-1;
string code;
vector <pair<string, bool>> data_bool;
vector <pair<string, int>> data_int;
vector <pair<string, char>> data_char;
const enum value_type {
	CHAR,INT,BOOL,NOT_FOUND
};
const enum operation {
	PLUS,MINUS,MULTIPLICATION,DIVISION,MODULO,AND,OR,XOR,EQUALS,NOTEQUALS,GREATER,LESSER,GREATEROREQUAL,LESSEROREQUAL
};
const map<string, operation> operation_translate{ {"0000",PLUS},{"0001",MINUS},{"0010", MULTIPLICATION},{"0011",DIVISION},{"0100",MODULO},{"0101",AND},{"0110",OR},{"0111",XOR},{"1000",EQUALS},{"1001",NOTEQUALS},{"1010",GREATER},{"1011",LESSER},{"1100",GREATEROREQUAL},{"1101",LESSEROREQUAL} };
namespace util {
	int bin_to_dec(string s) {
		int result = 0;
		for (int a = s.length() - 1, b=1; a >= 0; a--, b*=2) {
			if (s[a] == '1') { result += b; }
		}
		return result;
	}
	string int_to_str(int i) {
		if (i == 0) { return "0"; }
		string result = "";
		bool negative = 0;
		if (i < 0) { negative = 1; i *= -1; }
		while (i > 0) {
			result = (char)((i % 10) + 48) + result;
			i /= 10;
		}
		if (negative) { result = '-' + result; }
		return result;
	}
	void mem_state() {
		cout << "\n\nIt: "<<It<<" Booleans: ";
		for (int a = 0; a < data_bool.size(); a++) {
			cout << data_bool[a].first + ": " << data_bool[a].second << "   ";
		}
		cout << "\nInts: ";
		for (int a = 0; a < data_int.size(); a++) {
			cout << data_int[a].first + ": " << data_int[a].second << "   ";
		}
		cout << "\nChars: ";
		for (int a = 0; a < data_char.size(); a++) {
			cout << data_char[a].first + ": " << data_char[a].second << "   ";
		}
		cout << "\n\n";
	}
}
int exp_handler() {
	int l, r;
	const string l_type = code.substr(It, 3);
	It += 3;
	if (l_type == "000") { l = exp_handler(); }
	else if(l_type=="001"){
		const string name = code.substr(It, 8);
		It += 8;
		for (int a = 0; a < data_bool.size(); a++) {
			if (data_bool[a].first == name) {
				if (data_bool[a].second == true) {
					l = 1;
				}
				else {
					l = 0;
				}
				goto value_found;
			}
		}
		for (int a = 0; a < data_int.size(); a++) {
			if (data_int[a].first == name) {
				l = data_int[a].second;
				goto value_found;
			}
		}
		for (int a = 0; a < data_char.size(); a++) {
			if (data_char[a].first == name) {
				l = (int)data_char[a].second;
				goto value_found;
			}
		}
		value_found:
		if(0){}
	}
	else if (l_type == "010") {
		const string value = code.substr(It, 1);
		It++;
		if (value == "1") { l = 1; }
		else if (value == "0") { l = 0; }
	}
	else if (l_type == "011") {
		const string value = code.substr(It, 17);
		It += 17;
		l = (value[0] == '1' ? -1 : 1)*util::bin_to_dec(value.substr(1, 16));
	}
	else if (l_type == "100") {
		const string value = code.substr(It, 8);
		It += 8;
		l = util::bin_to_dec(value);
	}
	const string op = code.substr(It, 4);
	It += 4;
	const operation op_ = operation_translate.find(op)->second;
	const string r_type = code.substr(It, 3);
	It += 3;
	if (r_type == "000") { r = exp_handler(); }
	else if (r_type == "001") {
		const string name = code.substr(It, 8);
		It += 8;
		for (int a = 0; a < data_bool.size(); a++) {
			if (data_bool[a].first == name) {
				if (data_bool[a].second == true) {
					r = 1;
				}
				else {
					r = 0;
				}
				goto value_found2;
			}
		}
		for (int a = 0; a < data_int.size(); a++) {
			if (data_int[a].first == name) {
				r = data_int[a].second;
				goto value_found2;
			}
		}
		for (int a = 0; a < data_char.size(); a++) {
			if (data_char[a].first == name) {
				r = (int)data_char[a].second;
				goto value_found2;
			}
		}
		value_found2:
		if (0) {}
	}
	else if (r_type == "010") {
		const string value = code.substr(It, 1);
		It++;
		if (value == "1") { r = 1; }
		else if (value == "0") { r = 0; }
	}
	else if (r_type == "011") {
		const string value = code.substr(It, 17);
		It += 17;
		r = (value[0] == '1' ? -1 : 1)*util::bin_to_dec(value.substr(1, 16));
	}
	else if (r_type == "100") {
		const string value = code.substr(It, 8);
		It += 8;
		r = util::bin_to_dec(value);
	}
	switch (op_) {
	case PLUS:
		return l + r;
	case MINUS:
		return l - r;
	case MULTIPLICATION:
		return l * r;
	case DIVISION:
		return l / r;
	case MODULO:
		return l % r;
	case AND:
		return (l != 0 && r != 0 ? 1 : 0);
	case OR:
		return (l != 0 || r != 0 ? 1 : 0);
	case XOR:
		return ((l != 0 && r == 0) || (l == 0 && r != 0) ? 1 : 0);
	case EQUALS:
		return (l == r ? 1 : 0);
	case NOTEQUALS:
		return (l != r ? 1 : 0);
	case GREATER:
		return (l > r ? 1 : 0);
	case LESSER:
		return (l < r ? 1 : 0);
	case GREATEROREQUAL:
		return (l >= r ? 1 : 0);
	case LESSEROREQUAL:
		return (l <= r ? 1 : 0);
	}
}
namespace function_handlers {
	void declare_function_handler() {
		const string type = code.substr(It, 2);
		It += 2;
		const string given_value = code.substr(It, 1);
		It += 1;
		const string name = code.substr(It, 8);
		It += 8;
		if (given_value == "1") {
			if (type == "01") {
				if (code.substr(It, 1) == "1") {
					if (run.top()==1 && goto_function < 0) {
						data_bool.push_back(make_pair(name, true));
					}
				}
				else {
					if (run.top()==1 && goto_function < 0) {
						data_bool.push_back(make_pair(name, false));
					}
				}
				It += 1;
			}
			else if (type == "10") {
				const string value = code.substr(It, 17);
				It += 17;
				if (run.top() == 1 && goto_function < 0) {
					data_int.push_back(make_pair(name, (value[0] == '1' ? -1 : 1)*util::bin_to_dec(value.substr(1, 16))));
				}
			}
			else if (type == "11") {
				const string value = code.substr(It, 8);
				It += 8;
				if (run.top() == 1 && goto_function < 0) {
					data_char.push_back(make_pair(name, (char)util::bin_to_dec(value)));
				}
			}
		}
		else {
			if (type == "01"&&run.top()==1 && goto_function < 0) {
				data_bool.push_back(make_pair(name, false));
			}
			else if (type == "10"&&run.top()==1 && goto_function < 0) {
				data_int.push_back(make_pair(name, 0));
			}
			else if (type == "11"&&run.top()==1 && goto_function < 0) {
				data_char.push_back(make_pair(name, NULL));
			}
		}
	}
	void print_function_handler() {
		const string type = code.substr(It, 2);
		It += 2;
		if (type == "01") {
			const string name = code.substr(It, 8);
			string value = "";
			value_type vt=NOT_FOUND;
			It += 8;
			for (int a = 0; a < data_bool.size(); a++) {
				if (data_bool[a].first == name) {
					if (data_bool[a].second == true) {
						value = "true";
					}
					else {
						value = "false";
					}
					vt = BOOL;
					goto value_found;
				}
			}
			for (int a = 0; a < data_int.size(); a++) {
				if (data_int[a].first == name) {
					value = util::int_to_str(data_int[a].second);
					vt = INT;
					goto value_found;
				}
			}
			for (int a = 0; a < data_char.size(); a++) {
				if (data_char[a].first == name) {
					value = data_char[a].second;
					vt = CHAR;
					goto value_found;
				}
			}
			value_found:
			if (vt == NOT_FOUND) {
				if (run.top() == 1 && goto_function < 0) {
					cout << "ERROR 1; undeclared variable";
				}
			}
			else{
				if (run.top() == 1 && goto_function < 0) {
					cout << value;
				}
			}
		}
		else if (type == "00") {
			const string msg_len = code.substr(It, 8);
			It += 8;
			const string msg = code.substr(It, 8 * util::bin_to_dec(msg_len));
			It += 8 * util::bin_to_dec(msg_len);
			string character = "";
			for (int a = 0; a < util::bin_to_dec(msg_len);a++) {
				character = msg.substr(a * 8, 8);
				if (run.top() == 1 && goto_function < 0) {
					cout << (char)util::bin_to_dec(character);
				}
			}
		}
		else if (type == "10") {
			int out = exp_handler();
			if (run.top() == 1 && goto_function < 0) {
				cout << out;
			}
		}
	}
	void input_function_handler() {
		const string name = code.substr(It, 8);
		It += 8;
		value_type vt = NOT_FOUND;
		for (int a = 0; a < data_bool.size(); a++) {
			if (data_bool[a].first == name) {
				vt = BOOL;
				goto name_found;
			}
		}
		for (int a = 0; a < data_int.size(); a++) {
			if (data_int[a].first == name) {
				vt = INT;
				goto name_found;
			}
		}
		for (int a = 0; a < data_char.size(); a++) {
			if (data_char[a].first == name) {
				vt = CHAR;
				goto name_found;
			}
		}
		name_found:
		if (vt == NOT_FOUND) {
			if (run.top() == 1) {
				cout << "ERROR 1; undeclared variable";
			}
		}
		else if (vt == BOOL && run.top()==1 && goto_function < 0) {
			bool in;
			cin >> in;
			for (int a = 0; a < data_bool.size(); a++) {
				if (data_bool[a].first == name) {
					data_bool[a].second = in;
					break;
				}
			}
		}
		else if (vt == CHAR&&run.top()==1 && goto_function < 0) {
			char in;
			cin >> in;
			for (int a = 0; a < data_char.size(); a++) {
				if (data_char[a].first == name) {
					data_char[a].second = in;
					break;
				}
			}
		}
		else if(run.top()==1 && goto_function < 0){
			int in;
			cin >> in;
			for (int a = 0; a < data_int.size(); a++) {
				if (data_int[a].first == name) {
					data_int[a].second = in;
					break;
				}
			}
		}
	}
	void if_function_handler() {
		if (run.top()==0) { 
			run.push(0);
			exp_handler();
		}
		else {
			if (exp_handler()) { run.push(1); }
			else { run.push(0); }
		}
	}
	void goto_function_handler() {
		const string num = code.substr(It, 16);
		It += 16;
		if (run.top() == 1 && goto_function<0) {
			goto_function = util::bin_to_dec(num);
			It = 0;
			function_counter = 0;
			while (!run.empty()) { run.pop(); }
		}
	}
	void function_handler() {
		const string function = code.substr(It, 4);
		It += 4;
		//cout << "\n" << function << ' ' << It <<' '<<run.top()<< '\n';//Debug for functions 0100 0101 0110 0111
		function_counter++;
		if (function_counter == goto_function) {
			goto_function = -1;
		}
		if (function == "0001") {
			declare_function_handler();
		}
		else if (function == "0010") {
			print_function_handler();
		}
		else if (function == "0011") {
			input_function_handler();
		}
		else if (function == "0100") {
			if_function_handler();
		}
		else if (function == "0101") {
			run.pop();
		}
		else if (function == "0110") {
			bool last = run.top();
			run.pop();
			if (run.top() == 0) { run.push(0); }
			else { run.push(!last); }
		}
		else if (function == "0111") {
			goto_function_handler();
		}
		//util::mem_state();//Debug for memory state of user declared variables
	}
}
int main() {
	fstream file;
	string path;
	code = "";
	cin >> path;
	file.open(path);
	{
		string s = "";
		while (getline(file, s)) {
			if (s[0] == '#') { continue; }
			for (int a = 0; a < s.length(); a++) {
				if (s[a] == '1' || s[a] == '0') {
					code += s[a];
				}
			}
		}
	}
	//cout << '\n'<<code<<'\n';//Shows the code that is actually executed, so only '1' and '0'
	file.close();
	It = 0;
	while (It < code.length()) {
		if (It == 0) { run.push(1); }
		function_handlers::function_handler();
	}
}