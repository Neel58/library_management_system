#include <bits/stdc++.h>
using namespace std;

struct book {
    string id, title, author, publisher, status;
    int year;
};

unordered_map<string, book> books;
unordered_map<string, string> issued_books;      // book_id -> username
unordered_map<string, string> user_roles;        // username -> role
unordered_map<string, string> user_passwords;    // username -> password
unordered_map<string, int> user_issued_count;    // username -> count

// In-memory search history: username -> vector of last 5 searches
unordered_map<string, deque<string>> search_history;

string toLower(const string& s) {
    string res = s;
    transform(res.begin(), res.end(), res.begin(), ::tolower);
    return res;
}

string inputNonEmptyLine(const string& prompt) {
    string line;
    while (true) {
        cout << prompt;
        getline(cin, line);
        if (line.empty()) cout << "Input cannot be empty. Please try again.\n";
        else break;
    }
    return line;
}

int inputInt(const string& prompt, int minVal = INT_MIN, int maxVal = INT_MAX) {
    int val;
    string line;
    while (true) {
        cout << prompt;
        getline(cin, line);
        stringstream ss(line);
        if ((ss >> val) && !(ss >> line) && val >= minVal && val <= maxVal) break;
        cout << "Invalid input. Please enter a number";
        if (minVal != INT_MIN && maxVal != INT_MAX) cout << " between " << minVal << " and " << maxVal;
        cout << ".\n";
    }
    return val;
}

void save_books() {
    ofstream fout("books.txt");
    for (auto& p : books) {
        fout << p.second.id << "|" << p.second.title << "|" << p.second.author << "|"
             << p.second.publisher << "|" << p.second.year << "|" << p.second.status << "\n";
    }
}

void load_books() {
    ifstream fin("books.txt");
    if (!fin) return;
    books.clear();
    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string id, title, author, publisher, yearStr, status;
        if (getline(ss, id, '|') && getline(ss, title, '|') && getline(ss, author, '|') &&
            getline(ss, publisher, '|') && getline(ss, yearStr, '|') && getline(ss, status)) {
            book b{id, title, author, publisher, status, stoi(yearStr)};
            books[id] = b;
        }
    }
}

void save_issued() {
    ofstream fout("issued.txt");
    for (auto& p : issued_books)
        fout << p.first << "|" << p.second << "\n";
}

void load_issued() {
    ifstream fin("issued.txt");
    if (!fin) return;
    issued_books.clear();
    user_issued_count.clear();
    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string book_id, username;
        if (getline(ss, book_id, '|') && getline(ss, username)) {
            issued_books[book_id] = username;
            user_issued_count[username]++;
        }
    }
}

void save_users() {
    ofstream fout("users.txt");
    for (auto& p : user_roles)
        fout << p.first << "|" << user_passwords[p.first] << "|" << p.second << "\n";
}

void load_users() {
    ifstream fin("users.txt");
    if (!fin) return;
    user_roles.clear();
    user_passwords.clear();
    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string username, password, role;
        if (getline(ss, username, '|') && getline(ss, password, '|') && getline(ss, role)) {
            user_roles[username] = role;
            user_passwords[username] = password;
        }
    }
}

string truncateStr(const string& s, size_t width) {
    if (s.size() <= width) return s;
    return s.substr(0, width - 3) + "...";
}

void record_search(const string& username, const string& query) {
    auto& dq = search_history[username];
    if (!dq.empty() && dq.back() == query) return;
    if ((int)dq.size() == 5) dq.pop_front();
    dq.push_back(query);
}

void show_search_history(const string& username) {
    auto it = search_history.find(username);
    if (it == search_history.end() || it->second.empty()) {
        cout << "No recent searches.\n";
        return;
    }
    cout << "Recent Searches:\n";
    for (const auto& q : it->second) cout << " - " << q << "\n";
}

void search_books(const string& username) {
    cout << "Search by:\n1. ID\n2. Author\n3. Title\n4. Publisher\n5. Status\nChoice: ";
    int choice = inputInt("", 1, 5);
    cin.ignore();

    cout << "Enter search text: ";
    string query; getline(cin, query);
    record_search(username, query);

    string qLow = toLower(query);
    bool found = false;
    for (auto& p : books) {
        string field;
        switch(choice) {
            case 1: field = p.second.id; break;
            case 2: field = p.second.author; break;
            case 3: field = p.second.title; break;
            case 4: field = p.second.publisher; break;
            case 5: field = p.second.status; break;
        }
        if (toLower(field).find(qLow) != string::npos) {
            found = true;
            cout << "ID: " << p.second.id
                 << "\nTitle: " << p.second.title
                 << "\nAuthor: " << p.second.author
                 << "\nPublisher: " << p.second.publisher
                 << "\nYear: " << p.second.year
                 << "\nStatus: " << p.second.status << "\n\n";
        }
    }
    if (!found) cout << "No matching books found.\n";

    show_search_history(username);
}

void list_all_books() {
    if (books.empty()) {
        cout << "No books in the library.\n";
        return;
    }

    vector<book> blist;
    for (auto& p : books) blist.push_back(p.second);

    cout << "Sort by:\n1. ID\n2. Title\n3. Author\n4. Publisher\n5. Year\n6. Status\nChoice: ";
    int choice = inputInt("", 1, 6);

    switch (choice) {
        case 1: sort(blist.begin(), blist.end(), [](auto& a, auto& b){ return a.id < b.id; }); break;
        case 2: sort(blist.begin(), blist.end(), [](auto& a, auto& b){ return a.title < b.title; }); break;
        case 3: sort(blist.begin(), blist.end(), [](auto& a, auto& b){ return a.author < b.author; }); break;
        case 4: sort(blist.begin(), blist.end(), [](auto& a, auto& b){ return a.publisher < b.publisher; }); break;
        case 5: sort(blist.begin(), blist.end(), [](auto& a, auto& b){ return a.year < b.year; }); break;
        case 6: sort(blist.begin(), blist.end(), [](auto& a, auto& b){ return a.status < b.status; }); break;
    }

    cout << left << setw(12) << "ID"
         << setw(22) << "Title"
         << setw(17) << "Author"
         << setw(17) << "Publisher"
         << setw(6) << "Year"
         << setw(9) << "Status" << "\n";

    cout << string(83, '-') << "\n";

    for (auto& b : blist) {
        cout << left
             << setw(12) << truncateStr(b.id, 12)
             << setw(22) << truncateStr(b.title, 22)
             << setw(17) << truncateStr(b.author, 17)
             << setw(17) << truncateStr(b.publisher, 17)
             << setw(6) << b.year
             << setw(9) << b.status << "\n";
    }
}

void add_book() {
    string id = inputNonEmptyLine("Enter book ID: ");
    if (books.find(id) != books.end()) {
        cout << "Book exists, update? (y/n): ";
        string ans; getline(cin, ans);
        if (toLower(ans) != "y") {
            cout << "Cancelled.\n";
            return;
        }
    }
    string title = inputNonEmptyLine("Enter title: ");
    string author = inputNonEmptyLine("Enter author: ");
    string publisher = inputNonEmptyLine("Enter publisher: ");
    int year = inputInt("Enter publication year: ", 1000, 2100);

    books[id] = {id, title, author, publisher, "available", year};
    cout << "Book added/updated.\n";
}

void issue_book(const string& username) {
    string book_id = inputNonEmptyLine("Enter book ID to issue: ");
    auto it = books.find(book_id);
    if (it == books.end()) { cout << "Book not found.\n"; return; }
    if (it->second.status != "available") { cout << "Book already issued.\n"; return; }
    if (user_issued_count[username] >= 5) {
        cout << "Issue limit reached (5 books max).\n";
        return;
    }
    issued_books[book_id] = username;
    it->second.status = "issued";
    user_issued_count[username]++;
    cout << "Book issued to " << username << ".\n";
}

void return_book(const string& username) {
    string book_id = inputNonEmptyLine("Enter book ID to return: ");
    auto it = books.find(book_id);
    if (it == books.end()) { cout << "Book not found.\n"; return; }
    if (it->second.status != "issued") { cout << "Book not currently issued.\n"; return; }
    if (issued_books[book_id] != username) { cout << "This book was not issued to you.\n"; return; }
    issued_books.erase(book_id);
    it->second.status = "available";
    user_issued_count[username]--;
    cout << "Book returned successfully.\n";
}

void show_stats() {
    int total_books = (int)books.size();
    int available = 0, issued = 0;
    for (auto& p : books) {
        if (p.second.status == "available") available++;
        else if (p.second.status == "issued") issued++;
    }
    int total_users = (int)user_roles.size();

    cout << "\n--- Library Statistics ---\n"
         << "Total Books      : " << total_books << "\n"
         << "Available Books  : " << available << "\n"
         << "Issued Books     : " << issued << "\n"
         << "Total Users      : " << total_users << "\n";
}

void add_user() {
    string username = inputNonEmptyLine("Enter new username: ");
    if (user_roles.find(username) != user_roles.end()) {
        cout << "User exists.\n";
        return;
    }
    string password = inputNonEmptyLine("Enter password: ");
    string role;
    while (true) {
        cout << "Enter role (admin/user): ";
        getline(cin, role);
        role = toLower(role);
        if (role == "admin" || role == "user") break;
        cout << "Invalid role.\n";
    }
    user_roles[username] = role;
    user_passwords[username] = password;
    user_issued_count[username] = 0;
    cout << "User added.\n";
}

string login() {
    while (true) {
        cout << "Username: ";
        string username; getline(cin, username);
        cout << "Password: ";
        string password; getline(cin, password);
        if (user_roles.find(username) != user_roles.end() && user_passwords[username] == password) {
            cout << "Welcome, " << username << " (" << user_roles[username] << ")\n";
            return username;
        }
        cout << "Invalid username or password. Try again.\n";
    }
}

void admin_menu(const string& username) {
    while (true) {
        cout << "\n--- Admin Menu ---\n"
             << "1. Add/Update Book\n"
             << "2. Search Books\n"
             << "3. List All Books\n"
             << "4. Issue Book\n"
             << "5. Return Book\n"
             << "6. Add User\n"
             << "7. Show Statistics\n"
             << "8. Logout\n"
             << "Choice: ";
        int choice = inputInt("", 1, 8);
        switch (choice) {
            case 1: add_book(); break;
            case 2: search_books(username); break;
            case 3: list_all_books(); break;
            case 4: issue_book(username); break;
            case 5: return_book(username); break;
            case 6: add_user(); break;
            case 7: show_stats(); break;
            case 8: return;
            default: cout << "Invalid choice.\n"; break;
        }
    }
}

void user_menu(const string& username) {
    while (true) {
        cout << "\n--- User Menu ---\n"
             << "1. Search Books\n"
             << "2. List All Books\n"
             << "3. Issue Book\n"
             << "4. Return Book\n"
             << "5. Show Statistics\n"
             << "6. Logout\n"
             << "Choice: ";
        int choice = inputInt("", 1, 6);
        switch (choice) {
            case 1: search_books(username); break;
            case 2: list_all_books(); break;
            case 3: issue_book(username); break;
            case 4: return_book(username); break;
            case 5: show_stats(); break;
            case 6: return;
            default: cout << "Invalid choice.\n"; break;
        }
    }
}

int main() {
    load_users();
    if (user_roles.empty()) {
        user_roles["admin"] = "admin";
        user_passwords["admin"] = "admin123";
        user_issued_count["admin"] = 0;
        save_users();
    }
    load_books();
    load_issued();

    cout << "Welcome to the Library Management System\n";

    while (true) {
        string username = login();
        string role = user_roles[username];
        if (role == "admin") admin_menu(username);
        else user_menu(username);
        cout << "Logged out.\n";
    }

    save_books();
    save_issued();
    save_users();

}