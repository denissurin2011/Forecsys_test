#include <iostream>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Base {
 public:
    Base() : name ("input.txt") {}
    Base(const char* name1) : name(name1) {}

    ~Base() {}

    int separate (const string& s, string& id, string &country) {
        size_t ind = 0;
        string count;
        int check_format = 1;
        for (;ind < s.size(); ++ind) {
            if (isdigit(s[ind])) {
                id += s[ind];
            } else {
                if (s[ind] != ';') {
                    check_format = 0;
                }
                break;
            }
        }
        ++ind;
        for (;ind < s.size(); ++ind) {
            if (isdigit(s[ind])) {
                count += s[ind];
            } else {
                if (s[ind] != ';') {
                    check_format = 0;
                }
                break;
            }
        }
        ++ind;
        for (;ind < s.size(); ++ind) {
            if (isletter(s[ind])) {
                country += s[ind];
            } else {
                if (s[ind] != ';') {
                    check_format = 0;
                }
                break;
            }
        }
        if (check_format) {
            check_format = stoi(count);;
        }
        return check_format;
    }

    bool isletter(char c) {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            return true;
        }
        return false;
    }

    void read(){
        freopen(name, "rt", stdin);
        string s;
        while (cin >> s) {
            string id;
            string country;
            int count = separate(s, id, country);
            if (!count) {
                continue;
            }
            add(id, count, country);
        }
    }

    virtual void add(const string& id, int count, const string& country) {}

    virtual void print() {}

 private:
    const char* name;
};

class Solution_Without_Additional_Libraries : Base{
public:
    struct Vertex {
        bool leaf;
        Vertex* next[27 * 2 + 10];
        int id;
        long long count;
        Vertex() : leaf(false), next(), id(0), count(0) {}
    };

    Solution_Without_Additional_Libraries() {
        bor = new Vertex();
        name = "input.txt";
    }

    Solution_Without_Additional_Libraries(const char* name1) {
        bor = new Vertex();
        name = name1;
    }

    ~Solution_Without_Additional_Libraries() {
        erase(bor);
    }

    void run () {
        read();
        print();
    }

private:
    Vertex *bor;
    const char* name;

    void print() override{
        string s;
        dfs(bor, s);
    }

    char make_char(int n) {
        if (n < 27) {
            int a = 'A' + n;
            return (char) a;
        } else {
            int a = 'a' - 27 + n;
            return (char) a;
        }
    }

    void dfs(Vertex* bor, string &s) {
        const int alph = 54;
        if (bor->leaf) {
            cout << s << ';' << bor->count << ';' << bor->id << endl;
        }
        for (size_t i = 0; i < alph; ++i) {
            if (bor->next[i]) {
                char c = make_char(i);
                s += c;
                dfs(bor->next[i], s);
                s = s.substr(1);
            }
        }
        return;
    }

    void add(const string& id, int count, const string& country) override{
        int v = 0;
        Vertex* bor_head = bor;
        for (size_t i = 0; i < country.size(); ++i) {
            char c;
            if (isupper(country[i])) {
                c = country[i] - 'A';
            } else {
                c = country[i] - 'a' + 27;
            }
            if (bor->next[c] == nullptr) {
                Vertex* bor1 = new Vertex();
                bor->next[c] = bor1;
            }
            bor = bor->next[c];
        }
        bor->count += count;
        bor->leaf = true;
        Vertex* bor_term = bor;
        bool check_new = false;
        for (size_t i = 0; i < id.size(); ++i) {
            char c = id[i] - '0' + 27 * 2;
            if (!bor->next[c]) {
                Vertex* bor1 = new Vertex();
                bor->next[c] = bor1;
                check_new = 1;
            }
            bor = bor->next[c];
        }
        bor[v].leaf = true;
        if (check_new) {
            bor_term->id++;
        }
        bor = bor_head;
    }

    void erase (Vertex* bor) {
        for (size_t i = 0; i < 64; ++i) {
            if (bor->next[i]) {
                erase(bor->next[i]);
            }
        }
        delete bor;
    }
};

class Usual_Solution : Base {
 public:
    Usual_Solution() : name("input.txt") {}
    Usual_Solution(const char* name1) {
        name = name1;
    }

    ~Usual_Solution() {}

    void run () {
        read();
        print();
    }

private:
    void add(const string& id, int count, const string& country) override {
        data[country].first += count;
        data[country].second.insert(id);
    }

    void print() override {
        for (const auto& i : data) {
            cout << i.first << ';' << i.second.first << ';' << i.second.second.size() << endl;
        }
    }
    const char* name;
    unordered_map <string, pair<int, unordered_set<string>>> data;
};

int main(int argc, char *argv[]) {
    Usual_Solution a;
    a.run();
    return 0;
}
