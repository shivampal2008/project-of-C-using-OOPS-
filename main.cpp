/*
==================================================
   ONLINE EXAMINATION SYSTEM
   C++ OOP Project - 2nd Semester

   OOP Concepts Used:
   1. Classes and Objects
   2. Constructors
   3. Inheritance
   4. Encapsulation
   5. Polymorphism (Virtual Functions)
   6. Function Overloading
   7. File Handling

   Files Used:
   - students.txt   (student accounts)
   - questions.txt  (MCQ questions)
   - results.txt    (exam results)

   Admin Login: username=admin  password=admin123
==================================================
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cctype>
using namespace std;


// ==================================================
//  CLASS 1: User  (Base Class)
//  Concepts: Encapsulation, Polymorphism
// ==================================================
class User {
protected:
    string username;   // protected = accessible in child classes
    string password;

public:
    // Default Constructor
    User() {
        username = "";
        password = "";
    }

    // Parameterized Constructor
    User(string u, string p) {
        username = u;
        password = p;
    }

    // Getter functions (Encapsulation - controlled access)
    string getUsername() { return username; }
    string getPassword() { return password; }

    // Check if login credentials are correct
    bool login(string u, string p) {
        return (username == u && password == p);
    }

    // Pure virtual function => makes User an abstract class
    // Every child class MUST override this (Polymorphism)
    virtual void showMenu() = 0;

    virtual ~User() {}
};


// ==================================================
//  CLASS 2: Question
//  Concepts: Encapsulation, Constructors
// ==================================================
class Question {
private:
    int    id;
    string text;
    string optA, optB, optC, optD;
    char   correctAnswer;

public:
    // Default Constructor
    Question() {
        id = 0;
        correctAnswer = 'A';
    }

    // Parameterized Constructor
    Question(int i, string t, string a, string b,
             string c, string d, char ans) {
        id   = i;
        text = t;
        optA = a;  optB = b;
        optC = c;  optD = d;
        correctAnswer = toupper(ans);
    }

    // Getters
    int  getId()     { return id;            }
    char getAnswer() { return correctAnswer; }

    // Show question to student (no answer shown)
    void display() {
        cout << "\n  Q" << id << ": " << text       << endl;
        cout << "     A. " << optA                  << endl;
        cout << "     B. " << optB                  << endl;
        cout << "     C. " << optC                  << endl;
        cout << "     D. " << optD                  << endl;
    }

    // Show question with answer (admin only)
    void displayWithAnswer() {
        display();
        cout << "     [Correct Answer: " << correctAnswer << "]" << endl;
    }

    // Returns true if student's answer is correct
    bool checkAnswer(char ans) {
        return toupper(ans) == correctAnswer;
    }

    // Convert object to one line for saving in file
    string toFile() {
        string line = "";
        line += to_string(id) + "|";
        line += text          + "|";
        line += optA          + "|";
        line += optB          + "|";
        line += optC          + "|";
        line += optD          + "|";
        line += correctAnswer;
        return line;
    }

    // Read one file line and create a Question object
    static Question fromFile(string line) {
        stringstream ss(line);
        string token;
        vector<string> parts;

        while (getline(ss, token, '|'))
            parts.push_back(token);

        if (parts.size() >= 7)
            return Question(stoi(parts[0]), parts[1],
                            parts[2], parts[3],
                            parts[4], parts[5], parts[6][0]);
        return Question();
    }
};


// ==================================================
//  CLASS 3: Result
//  Concepts: Constructors, Function Overloading
// ==================================================
class Result {
private:
    string studentName;
    int    score;
    int    total;
    double percentage;
    bool   passed;

public:
    // Constructor - calculates result automatically
    Result(string name, int s, int t) {
        studentName = name;
        score       = s;
        total       = t;
        percentage  = (total > 0) ? ((double)score / total) * 100 : 0;
        passed      = (percentage >= 50);  // 50% = passing marks
    }

    // ---- FUNCTION OVERLOADING on display() ----

    // display() Version 1: Full result card
    void display() {
        cout << "\n  +--------------------------------+" << endl;
        cout << "  |          EXAM RESULT           |" << endl;
        cout << "  +--------------------------------+" << endl;
        cout << "  |  Name   : " << left << setw(22) << studentName << "|" << endl;
        cout << "  |  Score  : " << score << " / " << total;
        int pad = 22 - (int)to_string(score).size() - (int)to_string(total).size() - 3;
        cout << string(pad, ' ') << "|" << endl;
        ostringstream pss;
        pss << fixed << setprecision(1) << percentage << "%";
        cout << "  |  Marks  : " << left << setw(22) << pss.str() << "|" << endl;
        string status = passed ? "** PASS **" : "-- FAIL --";
        cout << "  |  Status : " << left << setw(22) << status    << "|" << endl;
        cout << "  +--------------------------------+" << endl;
    }

    // display() Version 2: One-line summary (overloaded)
    void display(bool brief) {
        if (brief)
            cout << "  " << left << setw(20) << studentName
                 << " | " << score << "/" << total
                 << " | " << (passed ? "PASS" : "FAIL") << endl;
        else
            display();  // call version 1
    }

    // Save result to file
    string toFile() {
        return studentName       + "|" +
               to_string(score) + "|" +
               to_string(total) + "|" +
               (passed ? "Pass" : "Fail");
    }
};


// ==================================================
//  CLASS 4: Student  (Inherits from User)
//  Concepts: Inheritance, Polymorphism
// ==================================================
class Student : public User {   // 'public' inheritance from User
private:
    int    id;
    string name;
    int    score;
    bool   attempted;   // has student taken exam?

public:
    // Default Constructor
    Student() : User("", "") {
        id = 0;  name = "";  score = 0;  attempted = false;
    }

    // Parameterized Constructor
    Student(int i, string n, string u, string p) : User(u, p) {
        id        = i;
        name      = n;
        score     = 0;
        attempted = false;
    }

    // Getters and Setters (Encapsulation)
    int    getId()              { return id;        }
    string getName()            { return name;      }
    int    getScore()           { return score;     }
    bool   hasAttempted()       { return attempted; }
    void   setScore(int s)      { score = s;        }
    void   setAttempted(bool a) { attempted = a;    }

    // Override pure virtual function from User (Polymorphism)
    void showMenu() override {
        cout << "\n  +==============================+" << endl;
        cout << "  |  STUDENT MENU                |" << endl;
        cout << "  |  Welcome, " << left << setw(18) << name << "|" << endl;
        cout << "  +==============================+" << endl;
        cout << "  |  1. Start Exam               |" << endl;
        cout << "  |  2. View My Result           |" << endl;
        cout << "  |  3. My Profile               |" << endl;
        cout << "  |  4. Logout                   |" << endl;
        cout << "  +==============================+" << endl;
        cout << "  Enter Choice: ";
    }

    // Show student profile
    void showProfile() {
        cout << "\n  --- My Profile ---"                           << endl;
        cout << "  ID       : " << id                             << endl;
        cout << "  Name     : " << name                           << endl;
        cout << "  Username : " << username                       << endl;
        cout << "  Exam     : " << (attempted ? "Done" : "Not taken yet") << endl;
        if (attempted)
            cout << "  Score    : " << score << endl;
        cout << "  ------------------" << endl;
    }

    // Convert to file line
    string toFile() {
        return to_string(id)             + "|" +
               name                      + "|" +
               username                  + "|" +
               password                  + "|" +
               (attempted ? "1" : "0")  + "|" +
               to_string(score);
    }

    // Create Student object from file line
    static Student fromFile(string line) {
        stringstream ss(line);
        string token;
        vector<string> parts;

        while (getline(ss, token, '|'))
            parts.push_back(token);

        if (parts.size() >= 6) {
            Student s(stoi(parts[0]), parts[1], parts[2], parts[3]);
            s.attempted = (parts[4] == "1");
            s.score     = stoi(parts[5]);
            return s;
        }
        return Student();
    }
};


// ==================================================
//  CLASS 5: Admin  (Inherits from User)
//  Concepts: Inheritance, Polymorphism
// ==================================================
class Admin : public User {   // 'public' inheritance from User
public:
    // Constructor - default admin credentials
    Admin() : User("admin", "admin123") {}

    // Override pure virtual function from User (Polymorphism)
    void showMenu() override {
        cout << "\n  +==============================+" << endl;
        cout << "  |  ADMIN MENU                  |" << endl;
        cout << "  +==============================+" << endl;
        cout << "  |  1. Add Question             |" << endl;
        cout << "  |  2. View All Questions       |" << endl;
        cout << "  |  3. Delete Question          |" << endl;
        cout << "  |  4. View All Results         |" << endl;
        cout << "  |  5. View All Students        |" << endl;
        cout << "  |  6. Logout                   |" << endl;
        cout << "  +==============================+" << endl;
        cout << "  Enter Choice: ";
    }
};


// ==================================================
//  FILE HANDLING FUNCTIONS
//  Read and write to .txt files
// ==================================================

// Load all students from students.txt
vector<Student> loadStudents() {
    vector<Student> list;
    ifstream file("students.txt");
    string line;
    while (getline(file, line))
        if (!line.empty())
            list.push_back(Student::fromFile(line));
    file.close();
    return list;
}

// Save all students to students.txt
void saveStudents(vector<Student>& list) {
    ofstream file("students.txt");
    for (int i = 0; i < (int)list.size(); i++)
        file << list[i].toFile() << "\n";
    file.close();
}

// Load all questions from questions.txt
vector<Question> loadQuestions() {
    vector<Question> list;
    ifstream file("questions.txt");
    string line;
    while (getline(file, line))
        if (!line.empty())
            list.push_back(Question::fromFile(line));
    file.close();
    return list;
}

// Append one result to results.txt
void saveResult(Result& r) {
    ofstream file("results.txt", ios::app);
    file << r.toFile() << "\n";
    file.close();
}


// ==================================================
//  STUDENT OPERATIONS
// ==================================================

// Register a new student
void registerStudent() {
    cout << "\n  === NEW REGISTRATION ===" << endl;
    string name, uname, pwd, cpwd;

    cin.ignore();
    cout << "  Full Name : "; getline(cin, name);
    cout << "  Username  : "; getline(cin, uname);
    cout << "  Password  : "; getline(cin, pwd);
    cout << "  Confirm   : "; getline(cin, cpwd);

    // Validation
    if (name.empty() || uname.empty() || pwd.empty()) {
        cout << "\n  Error: All fields are required!" << endl;
        return;
    }
    if (pwd != cpwd) {
        cout << "\n  Error: Passwords do not match!" << endl;
        return;
    }

    // Check for duplicate username
    vector<Student> students = loadStudents();
    for (int i = 0; i < (int)students.size(); i++) {
        if (students[i].getUsername() == uname) {
            cout << "\n  Error: Username already taken!" << endl;
            return;
        }
    }

    // Auto generate ID starting from 1001
    int newId = students.empty() ? 1001 : students.back().getId() + 1;

    students.push_back(Student(newId, name, uname, pwd));
    saveStudents(students);
    cout << "\n  Registered successfully! You can now login." << endl;
}

// Conduct the exam for a student
void startExam(Student& student) {
    vector<Question> questions = loadQuestions();

    if (questions.empty()) {
        cout << "\n  No questions available. Ask admin to add some." << endl;
        return;
    }

    if (student.hasAttempted()) {
        cout << "\n  You already took the exam!" << endl;
        cout << "  Your Score: " << student.getScore()
             << "/" << questions.size() << endl;
        return;
    }

    // Exam header
    cout << "\n  ================================" << endl;
    cout << "         EXAM STARTS NOW          " << endl;
    cout << "  ================================" << endl;
    cout << "  Total Questions : " << questions.size() << endl;
    cout << "  Passing Marks   : 50%"              << endl;
    cout << "  Press Enter to begin...";
    cin.ignore();
    cin.get();

    int score = 0;

    // Question loop
    for (int i = 0; i < (int)questions.size(); i++) {
        cout << "\n  [Question " << i+1 << " of " << questions.size() << "]";
        questions[i].display();

        // Get valid answer
        char ans;
        while (true) {
            cout << "  Your Answer (A/B/C/D): ";
            cin >> ans;
            ans = toupper(ans);
            if (ans=='A' || ans=='B' || ans=='C' || ans=='D') break;
            cout << "  Invalid! Enter A, B, C or D only." << endl;
        }

        if (questions[i].checkAnswer(ans)) {
            cout << "  Correct! (+1)" << endl;
            score++;
        } else {
            cout << "  Wrong! Correct answer: "
                 << questions[i].getAnswer() << endl;
        }
    }

    // Update student record
    student.setScore(score);
    student.setAttempted(true);

    // Save updated student to file
    vector<Student> all = loadStudents();
    for (int i = 0; i < (int)all.size(); i++)
        if (all[i].getId() == student.getId())
            all[i] = student;
    saveStudents(all);

    // Show and save result
    Result r(student.getName(), score, (int)questions.size());
    r.display();       // Function overload version 1 (full card)
    saveResult(r);

    cout << "\n  Result saved to results.txt" << endl;
}

// Student login and menu session
void studentSession() {
    cout << "\n  === STUDENT LOGIN ===" << endl;

    string uname, pwd;
    cout << "  Username : "; cin >> uname;
    cout << "  Password : "; cin >> pwd;

    vector<Student> students = loadStudents();
    Student* s = nullptr;

    for (int i = 0; i < (int)students.size(); i++)
        if (students[i].login(uname, pwd)) { s = &students[i]; break; }

    if (s == nullptr) {
        cout << "\n  Wrong username or password!" << endl;
        return;
    }

    cout << "\n  Login successful! Welcome, " << s->getName() << endl;

    bool running = true;
    while (running) {

        s->showMenu();   // Polymorphism: calls Student's showMenu()
        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                startExam(*s);
                break;

            case 2:
                if (!s->hasAttempted()) {
                    cout << "\n  You haven't taken the exam yet." << endl;
                } else {
                    int total = (int)loadQuestions().size();
                    Result r(s->getName(), s->getScore(), total);
                    r.display();  // Full card
                }
                break;

            case 3:
                s->showProfile();
                break;

            case 4:
                cout << "\n  Goodbye, " << s->getName() << "!" << endl;
                running = false;
                break;

            default:
                cout << "\n  Invalid choice! Enter 1-4." << endl;
        }
    }
}


// ==================================================
//  ADMIN OPERATIONS
// ==================================================

// Add a new MCQ question
void addQuestion() {
    cout << "\n  === ADD QUESTION ===" << endl;

    // Find next available ID
    vector<Question> questions = loadQuestions();
    int newId = questions.empty() ? 1 : questions.back().getId() + 1;

    string text, a, b, c, d;
    char ans;

    cin.ignore();
    cout << "  Question : "; getline(cin, text);
    cout << "  Option A : "; getline(cin, a);
    cout << "  Option B : "; getline(cin, b);
    cout << "  Option C : "; getline(cin, c);
    cout << "  Option D : "; getline(cin, d);
    cout << "  Correct Answer (A/B/C/D) : "; cin >> ans;
    ans = toupper(ans);

    // Validation
    if (text.empty() || a.empty() || b.empty() || c.empty() || d.empty()) {
        cout << "\n  Error: All fields are required!" << endl;
        return;
    }
    if (ans!='A' && ans!='B' && ans!='C' && ans!='D') {
        cout << "\n  Error: Answer must be A, B, C or D!" << endl;
        return;
    }

    Question q(newId, text, a, b, c, d, ans);
    ofstream file("questions.txt", ios::app);
    file << q.toFile() << "\n";
    file.close();

    cout << "\n  Question #" << newId << " added successfully!" << endl;
}

// Show all questions (with correct answers)
void viewQuestions() {
    vector<Question> questions = loadQuestions();

    if (questions.empty()) {
        cout << "\n  No questions found." << endl;
        return;
    }

    cout << "\n  === ALL QUESTIONS ===" << endl;
    for (int i = 0; i < (int)questions.size(); i++) {
        questions[i].displayWithAnswer();
        cout << "  " << string(35, '-') << endl;
    }
    cout << "  Total: " << questions.size() << " question(s)" << endl;
}

// Delete a question by ID
void deleteQuestion() {
    viewQuestions();

    if (loadQuestions().empty()) return;

    cout << "\n  Enter Question ID to delete (0 = cancel): ";
    int id; cin >> id;
    if (id == 0) { cout << "  Cancelled." << endl; return; }

    vector<Question> questions = loadQuestions();
    bool found = false;

    ofstream file("questions.txt");
    for (int i = 0; i < (int)questions.size(); i++) {
        if (questions[i].getId() == id)
            found = true;
        else
            file << questions[i].toFile() << "\n";
    }
    file.close();

    if (found) cout << "  Question #" << id << " deleted!" << endl;
    else       cout << "  Question #" << id << " not found!" << endl;
}

// Show all results
void viewResults() {
    ifstream file("results.txt");
    if (!file.is_open()) {
        cout << "\n  No results yet." << endl;
        return;
    }

    cout << "\n  === ALL RESULTS ===" << endl;
    cout << "  " << left << setw(22) << "Name"
         << setw(10) << "Score" << "Status" << endl;
    cout << "  " << string(42, '-') << endl;

    string line;
    int count = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string token;
        vector<string> parts;
        while (getline(ss, token, '|'))
            parts.push_back(token);

        if (parts.size() >= 4) {
            cout << "  " << left << setw(22) << parts[0]
                 << setw(10) << (parts[1] + "/" + parts[2])
                 << parts[3] << endl;
            count++;
        }
    }
    file.close();

    if (count == 0) cout << "  No results yet." << endl;
    else            cout << "  Total: " << count << " result(s)" << endl;
}

// Show all registered students
void viewStudents() {
    vector<Student> students = loadStudents();

    cout << "\n  === ALL STUDENTS ===" << endl;
    cout << "  " << left
         << setw(6)  << "ID"
         << setw(20) << "Name"
         << setw(15) << "Username"
         << "Exam Status" << endl;
    cout << "  " << string(55, '-') << endl;

    if (students.empty()) {
        cout << "  No students registered yet." << endl;
        return;
    }

    for (int i = 0; i < (int)students.size(); i++) {
        string status = students[i].hasAttempted()
            ? "Done (" + to_string(students[i].getScore()) + ")"
            : "Pending";
        cout << "  " << left
             << setw(6)  << students[i].getId()
             << setw(20) << students[i].getName()
             << setw(15) << students[i].getUsername()
             << status   << endl;
    }
    cout << "  Total: " << students.size() << " student(s)" << endl;
}

// Admin login and menu session
void adminSession() {
    cout << "\n  === ADMIN LOGIN ===" << endl;

    string uname, pwd;
    cout << "  Username : "; cin >> uname;
    cout << "  Password : "; cin >> pwd;

    Admin admin;

    if (!admin.login(uname, pwd)) {
        cout << "\n  Wrong admin credentials!" << endl;
        return;
    }

    cout << "\n  Admin login successful!" << endl;

    bool running = true;
    while (running) {

        admin.showMenu();   // Polymorphism: calls Admin's showMenu()
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: addQuestion();   break;
            case 2: viewQuestions(); break;
            case 3: deleteQuestion(); break;
            case 4: viewResults();   break;
            case 5: viewStudents();  break;
            case 6:
                cout << "\n  Admin logged out." << endl;
                running = false;
                break;
            default:
                cout << "\n  Invalid choice! Enter 1-6." << endl;
        }
    }
}


// ==================================================
//  MAIN FUNCTION
// ==================================================
int main() {

    cout << "\n  +================================+" << endl;
    cout << "  |   ONLINE EXAMINATION SYSTEM    |" << endl;
    cout << "  |   C++ OOP Project - 2nd Sem    |" << endl;
    cout << "  +================================+" << endl;

    bool running = true;

    while (running) {
        cout << "\n  === MAIN MENU ===" << endl;
        cout << "  1. Student Login"   << endl;
        cout << "  2. Admin Login"     << endl;
        cout << "  3. Register"        << endl;
        cout << "  4. Exit"            << endl;
        cout << "  Enter Choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: studentSession();  break;
            case 2: adminSession();    break;
            case 3: registerStudent(); break;
            case 4:
                cout << "\n  Goodbye!\n" << endl;
                running = false;
                break;
            default:
                cout << "\n  Invalid choice! Enter 1-4." << endl;
        }
    }

    return 0;
}
