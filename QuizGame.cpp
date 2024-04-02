#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>

using namespace std;


struct Question {
    string questionText;
    vector<string> options;
    int correctOptionIndex;
};


void displayMenu() {
    cout << "Welcome to the Quiz Game!\n";
    cout << "1. Register\n";
    cout << "2. Start Quiz\n";
    cout << "3. Exit\n";
    cout << "Choose an option: ";
}


string registerUser() {
    string username;
    cout << "Enter your username: ";
    cin >> username;
    return username;
}

vector<Question> loadQuestionsFromFile(const string& filename) {
    vector<Question> questions;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return questions;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) {
           
            continue;
        }

        Question question;
        question.questionText = line;

        
        for (int i = 0; i < 4; ++i) {
            if (!getline(file, line)) {
                cerr << "Error: Unexpected end of file" << endl;
                return questions;
            }
            question.options.push_back(line);
        }

        
        char correctAnswer;
        if (!(file >> correctAnswer)) {
            cerr << "Error: Unexpected end of file" << endl;
            return questions;
        }
        question.correctOptionIndex = toupper(correctAnswer) - 'A';

        
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        questions.push_back(question);
    }

    file.close();
    return questions;
}


void conductQuiz(vector<Question>& questions, map<string, int>& users) {
    string username;
    cout << "Enter your username: ";
    cin >> username;
    int score = 0;

    
    for (auto& question : questions) {
        cout << question.questionText << endl;
        for (const auto& option : question.options) {
            cout << option << endl;
        }
        char userChoice;
        cout << "Enter your choice (A/B/C/D): ";
        cin >> userChoice;
        int userChoiceIndex = toupper(userChoice) - 'A';
        if (userChoiceIndex == question.correctOptionIndex) {
            cout << "Correct!\n";
            score++;
        }
        else {
            cout << "Incorrect!\n";
        }
    }

   
    cout << "Quiz ended. Your score: " << score << "/" << questions.size() << endl;

    
    users[username] += score;
}



void displayScores(const map<string, int>& users) {
    cout << "----- Leaderboard -----\n";
    for (const auto& user : users) {
        cout << user.first << ": " << user.second << " points\n";
    }
}

int main() {
    map<string, int> users;
    vector<Question> questions = loadQuestionsFromFile("Questions.txt");
    int choice;

    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            users[registerUser()] = 0;
            break;
        case 2:
            if (users.empty()) {
                cout << "No users registered yet. Please register first.\n";
            }
            else {
                conductQuiz(questions, users);
            }
            break;
        case 3:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);

    displayScores(users);

    return 0;
}
