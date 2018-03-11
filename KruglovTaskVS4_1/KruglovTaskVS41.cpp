//Напишите программу – «телефонную книжку».
//Записи(имя и телефон) должны хранится в каком - либо STL - контейнере(vector или list), 
//причем крайне желательно, чтобы от типа контейнера не зависело ничего, кроме одной строки 
//в программе – объявления этого контейнера(указание: используйте typedef)


#include "stdafx.h"

using namespace std;

class PhoneBook {
	string name, phoneNum;
public:
	PhoneBook() :name("Empty"), phoneNum("Empty") {}
	PhoneBook(const PhoneBook &r) :name(r.getName()), phoneNum(r.getPhone()) {}
	PhoneBook(const string &n, const string &p) :name(n), phoneNum(p) {}
	~PhoneBook() {}

	PhoneBook& operator=(const PhoneBook &r) {
		setName(r.getName());
		setPhone(r.getPhone());
		return *this;
	}

	string getName() const { return name; }
	void setName(const string &n) { name = n; }
	string getPhone() const { return phoneNum; }
	void setPhone(const string &n) { phoneNum = n; }

	string toString() const {
		return "Name: " + getName() + "; Phone: " + getPhone() + "\n";
	}
};

typedef vector<PhoneBook> PhoneBookDB;

PhoneBookDB db;
PhoneBookDB::iterator iter;

// Command handlers:

string help() { // Show help
	return
		"help - show help\n"
		"reset - reset iterator to first record\n"
		"curr - show current record\n"
		"next - move to next record\n"
		"prev - move to previous record\n"
		"nextn <n> - skip several records\n"
		"prevn <n> - skip several records backwards\n"
		"before <name> <phone> - insert a record before the current\n"
		"after <name> <phone> - insert a record after the current\n"
		"back <name> <phone> - insert a record at the end of the database\n"
		"change <name> <phone> - change the current record\n";
}

template<class T> string modifyRecord(T it) { // Modify a record pointed to by a specified iterator
	string name, phone;
	cout << "Enter name % number:\n";
	cin >> name >> phone;	// Input the new name and phone
	PhoneBook rec(name, phone);
	*it = rec;
	// Reset the iterator to the beginning of the database because
	// in the general case it will become deprecated (non-dereferenceable)
	iter = db.begin();

	return "New val: " + rec.toString();
}

string reset() { // Reset the iterator to the beginning of the database
	iter = db.begin();
	return iter->toString();
}
string curr() {	// Show current record
	return iter->toString();
}
string next() {	// Move to next record and show it
	if (++iter == db.end())
	{
		--iter;
		return "it was last record.\n";
	}
	else
		return iter->toString();
}
string prev() {	// Move to previous record and show it
	if (iter == db.begin())
		cout << "it was first record.\n";
	else
	{
		--iter;
		return iter->toString();
	}
}
string nextn() { // Skip N records and show the Nth
	int pos;
	cout << "Enter position:\n";
	cin >> pos; // Read N
	advance(iter, pos);
	return iter->toString();
}
string prevn() { // Skip N records backwards
	int pos;
	cout << "Enter position:\n";
	cin >> pos;
	advance(iter, pos);
	return iter->toString();
}

// A command handler: called by transform() (see below)
string cmdHandler(string cmd) {
	try {
		if (cmd == "help") {
			return help();
		}
		else if (cmd == "reset") {
			return reset();
		}
		else if (cmd == "curr") {
			return curr();
		}
		else if (cmd == "next") {
			return next();
		}
		else if (cmd == "prev") {
			return prev();
		}
		else if (cmd == "nextn") {
			return nextn();
		}
		else if (cmd == "prevn") {
			return prevn();
		}
		else if (cmd == "before") {
			// Insert a record before the current: 
			// use an insert iterator into a position preceding the current.
			return modifyRecord(inserter(db, --iter));
		}
		else if (cmd == "after") {
			// Same: insert into the current position
			return modifyRecord(inserter(db, iter));
		}
		else if (cmd == "back") {
			// Insert to the end of the database (push back)
			return modifyRecord(back_inserter<PhoneBookDB>(db));
		}
		else if (cmd == "change") {
			// Simply modify the current record
			return modifyRecord(iter);
		}
		else {
			return "Invalid command! Type 'help' for a list of commands\n";
		}
	}
	catch (...) {
		return "Past-end iterator dereferenced!\n";
	}
}

int main() {
	int const N = 5;
	string temp[N][2] = {	"qwe", "123",
							"asd", "456",
							"zxc", "789",
							"rty", "234",
							"ljkasd", "223344"};

	for (int i = 0; i < N; i++)
	{
		PhoneBook rec(temp[i][0], temp[i][1]);
		db.push_back(rec);
	}

	cout << help();
	iter = db.begin();

	// Read strings from cin until EOF, call cmdHandler(str) for each and
	// output the result of cmdHandler to cout.
	transform(istream_iterator<string>(cin), istream_iterator<string>(),
		ostream_iterator<string>(cout), cmdHandler);

	return 0;
}


