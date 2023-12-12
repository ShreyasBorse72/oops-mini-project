#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

class Person {
protected:
    string name;

public:
    Person(const string& n) : name(n) {}

    static bool isValidName(const string& n) {
        for (char c : n) {
            if (!isalpha(c) && c != ' ') {
                return false;
            }
        }
        return true;
    }

    virtual void displayInfo(const string& className) const {
        cout << "Class: " << className << endl;
        cout << "Name: " << name << endl;
    }
};

class Member : public Person {
private:
    int age;
    string membershipType;
    string assignedTrainer;

public:
    Member(const string& n, int a, const string& m) : Person(n), age(a), membershipType(m), assignedTrainer("") {}

    void assignTrainer(const string& trainer) {
        assignedTrainer = trainer;
    }

    void displayInfo(const string& className) const override {
        Person::displayInfo(className);
        cout << "Age: " << age << endl;
        cout << "Membership Type: " << membershipType << endl;
        cout << "Assigned Trainer: " << assignedTrainer << endl;
    }
};

class Trainer : public Person {
private:
    string specialization;
    string type;
    bool available;

public:
    Trainer(const string& n, const string& s, const string& t) : Person(n), specialization(s), type(t), available(true) {
        if (t != "basic" && t != "premium" && t != "advance") {
            cout << "Invalid trainer type. Defaulting to basic." << endl;
            type = "basic";
        }
    }

    bool isAvailable(const string& membershipType) const {
        if (type == "basic" && (membershipType == "basic3months" || membershipType == "advance6months" || membershipType == "premium1year")) {
            return available;
        } else if (type == "advance" && (membershipType == "advance6months" || membershipType == "premium1year")) {
            return available;
        } else if (type == "premium" && membershipType == "premium1year") {
            return available;
        }
        return false;
    }

    void setAvailability(bool status) {
        available = status;
    }

    string getSpecialization() const {
        return specialization;
    }

    string getType() const {
        return type;
    }

    void displayInfo(const string& className) const override {
        Person::displayInfo(className);
        cout << "Specialization: " << specialization << endl;
        cout << "Type: " << type << endl;
        cout << "Availability: " << (available ? "Available" : "Not Available") << endl;
    }
};

int getInputInt() {
    int input;
    while (true) {
        cin >> input;
        if (cin.fail() || input <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid positive integer: ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return input;
        }
    }
}

int main() {
    vector<Member> members;
    vector<Trainer> trainers;
    int basicTrainers = 0;
    int advanceTrainers = 0;
    int premiumTrainers = 0;
    int choice;

    do {
        cout << "\n1. Add Trainer\n2. Add Member\n3. Display Information\n4. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name, specialization, type;

                cout << "Enter trainer's name: ";
                cin >> ws;
                getline(cin, name);

                while (!Person::isValidName(name)) {
                    cout << "Invalid input for name. Enter a name with alphabetic characters only: ";
                    getline(cin, name);
                }

                cout << "Enter trainer's specialization (weightlifting, cardio, workout): ";
                cin >> specialization;

                cout << "Enter trainer's type (basic, premium, advance): ";
                cin >> type;

                if (type != "basic" && type != "premium" && type != "advance") {
                    cout << "Invalid trainer type. Defaulting to basic." << endl;
                    type = "basic";
                }

                if (type == "basic") {
                    basicTrainers++;
                } else if (type == "advance") {
                    advanceTrainers++;
                } else if (type == "premium") {
                    premiumTrainers++;
                }

                trainers.push_back(Trainer(name, specialization, type));
                break;
            }
            case 2: {
                string name;
                int age;

                cout << "Enter member's name: ";
                cin >> ws;
                getline(cin, name);

                while (!Person::isValidName(name)) {
                    cout << "Invalid input for name. Enter a name with alphabetic characters only: ";
                    getline(cin, name);
                }

                cout << "Enter member's age: ";
                age = getInputInt();

                cout << "Enter membership type (basic3months, advance6months, premium1year): ";
                string membershipType;
                cin >> membershipType;

                bool trainerAssigned = false;
                for (auto& trainer : trainers) {
                    if (trainer.isAvailable(membershipType)) {
                        if ((membershipType == "basic3months" && trainer.getType() == "basic") ||
                            (membershipType == "advance6months" && trainer.getType() == "advance") ||
                            (membershipType == "premium1year" && trainer.getType() == "premium")) {

                            trainer.setAvailability(false);
                            members.push_back(Member(name, age, membershipType));
                            members.back().assignTrainer(trainer.getSpecialization() + " (" + trainer.getType() + ")");
                            trainerAssigned = true;
                            break;
                        }
                    }
                }

                if (!trainerAssigned) {
                    cout << "No trainers available for the entered membership type." << endl;
                    cout << "Available trainers - Basic: " << basicTrainers << ", Advance: " << advanceTrainers << ", Premium: " << premiumTrainers << endl;
                }
                break;
            }
            case 3:
                cout << "\nTrainers Information:" << endl;
                for (const auto& trainer : trainers) {
                    trainer.displayInfo("Trainer");
                    cout << endl;
                }

                cout << "\nMembers Information:" << endl;
                for (const auto& member : members) {
                    member.displayInfo("Member");
                    cout << endl;
                }
                break;
        }
    } while (choice != 4);

    return 0;
}