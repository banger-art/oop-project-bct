#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <cctype>

using namespace std;

class Me {
protected:
    string name;
    int age;
    string clas;
    string address;
    string gender;

public:
    Me() {
        string input;

        cout << "========== PERSONAL DETAILS ==========\n";

        cout << "Enter your name: ";
        getline(cin, name);

        cout << "Enter your age: ";
        getline(cin, input);
        age = stoi(input);

        cout << "Enter your class: ";
        getline(cin, clas);

        cout << "Enter your address: ";
        getline(cin, address);

        cout << "Enter your gender: ";
        getline(cin, gender);
    }

    void displayMe() {
        cout << "\n========== PERSONAL DETAILS ==========\n";
        cout << "Name    : " << name << endl;
        cout << "Age     : " << age << endl;
        cout << "Class   : " << clas << endl;
        cout << "Address : " << address << endl;
        cout << "Gender  : " << gender << endl;
    }
};

struct Slot {
    int startTime;
    int endTime;
    string activity;
};

int parseTime(string input) {
    if (input.size() != 5) return -1;
    if (input[2] != ':') return -1;

    if (!isdigit(input[0]) || !isdigit(input[1]) || !isdigit(input[3]) || !isdigit(input[4])) {
        return -1;
    }

    int h = stoi(input.substr(0, 2));
    int m = stoi(input.substr(3, 2));

    if (h < 0 || h > 23) return -1;
    if (m < 0 || m > 59) return -1;

    return h * 60 + m;
}

int promptTime(string label) {
    string input;
    int minutes;

    while (true) {
        cout << label << " (HH:MM, e.g. 09:00, must have the colon and 2 digits each side): ";
        getline(cin, input);
        minutes = parseTime(input);

        if (minutes != -1) {
            break;
        }
        cout << "That's not a valid HH:MM time, try again.\n";
    }

    return minutes;
}

string formatTime(int minutes) {
    int h = minutes / 60;
    int m = minutes % 60;
    string hh = (h < 10 ? "0" : "") + to_string(h);
    string mm = (m < 10 ? "0" : "") + to_string(m);
    return hh + ":" + mm;
}

string toLower(string s) {
    for (int i = 0; i < (int)s.size(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

string formatHours(int totalMinutes) {
    double hours = totalMinutes / 60.0;
    int wholeHours = (int)(hours + 0.0001);

    string out;
    if (hours == wholeHours) {
        out = to_string(wholeHours);
    } else {

        int tenths = (int)(hours * 10 + 0.5);
        int whole = tenths / 10;
        int frac = tenths % 10;
        out = to_string(whole) + "." + to_string(frac);
    }

    out += " hrs";
    return out;
}

class Plan : public Me {
protected:

    vector<Slot> schedule[7];

    string shortNote;

    string days[7] = {
        "Sunday",
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday"
    };

    int chooseDay() {
        string input;

        cout << "\n========== DAYS ==========\n";
        for (int i = 0; i < 7; i++) {
            cout << i + 1 << ". " << days[i] << endl;
        }
        cout << "0. Cancel\n";
        cout << "Choose a day: ";
        getline(cin, input);

        int choice = stoi(input);

        if (choice == 0) return -1;
        if (choice >= 1 && choice <= 7) return choice - 1;

        cout << "Invalid choice!\n";
        return -1;
    }

    void printDay(int dayIndex) {
        cout << "\n" << days[dayIndex] << endl;
        cout << "---------------------------\n";

        if (schedule[dayIndex].empty()) {
            cout << "(no slots planned)\n";
            return;
        }

        for (int i = 0; i < (int)schedule[dayIndex].size(); i++) {
            Slot s = schedule[dayIndex][i];
            cout << i + 1 << ". "
                 << formatTime(s.startTime)
                 << " - "
                 << formatTime(s.endTime)
                 << " : "
                 << s.activity
                 << endl;
        }
    }

    void saveToFile() {

        ofstream fout("plan.txt");

        if (!fout) {
            cout << "Error opening file!\n";
            return;
        }

        fout << "========== PERSONAL DETAILS ==========\n";
        fout << "Name    : " << name << endl;
        fout << "Age     : " << age << endl;
        fout << "Class   : " << clas << endl;
        fout << "Address : " << address << endl;
        fout << "Gender  : " << gender << endl;

        fout << "\n========== WEEKLY SCHEDULE ==========\n";

        for (int d = 0; d < 7; d++) {

            fout << "\n" << days[d] << endl;
            fout << "---------------------------\n";

            if (schedule[d].empty()) {
                fout << "(no slots planned)\n";
                continue;
            }

            for (int i = 0; i < (int)schedule[d].size(); i++) {
                Slot s = schedule[d][i];
                fout << formatTime(s.startTime)
                     << " - "
                     << formatTime(s.endTime)
                     << " : "
                     << s.activity
                     << endl;
            }
        }

        fout << "\n========== SHORT NOTE ==========\n";
        if (shortNote.empty()) {
            fout << "(no note written yet)\n";
        } else {
            fout << shortNote << endl;
        }

        fout.close();
    }

public:
    Plan() {
        cout << "\nHello " << name << "!\n";
        cout << "Let's manage your weekly schedule.\n";
    }

    void addSlot() {
        int dayIndex = chooseDay();
        if (dayIndex == -1) return;

        string input;
        Slot s;

        cout << "\nAdding to " << days[dayIndex] << endl;

        cout << "Enter Activity: ";
        getline(cin, s.activity);

        s.startTime = promptTime("Start Time");
        s.endTime = promptTime("End Time");

        schedule[dayIndex].push_back(s);
        saveToFile();
        cout << "Slot added and saved.\n";
    }

    void viewSchedule() {
        string input;
        cout << "\n1. View full week\n2. View a single day\nChoice: ";
        getline(cin, input);

        if (input == "2") {
            int dayIndex = chooseDay();
            if (dayIndex == -1) return;
            printDay(dayIndex);
        } else {
            cout << "\n========== WEEKLY SCHEDULE ==========\n";
            for (int d = 0; d < 7; d++) {
                printDay(d);
            }
        }
    }

    void editSlot() {
        int dayIndex = chooseDay();
        if (dayIndex == -1) return;

        printDay(dayIndex);
        if (schedule[dayIndex].empty()) return;

        string input;
        cout << "\nWhich slot number do you want to edit? ";
        getline(cin, input);
        int idx = stoi(input) - 1;

        if (idx < 0 || idx >= (int)schedule[dayIndex].size()) {
            cout << "Invalid slot number!\n";
            return;
        }

        Slot &s = schedule[dayIndex][idx];

        cout << "Leave blank to keep current value.\n";

        cout << "Activity [" << s.activity << "]: ";
        getline(cin, input);
        if (!input.empty()) s.activity = input;

        cout << "Start Time [" << formatTime(s.startTime) << "] (HH:MM): ";
        getline(cin, input);
        if (!input.empty()) {
            int t = parseTime(input);
            if (t != -1) s.startTime = t;
            else cout << "Invalid time format (need HH:MM), keeping previous value.\n";
        }

        cout << "End Time [" << formatTime(s.endTime) << "] (HH:MM): ";
        getline(cin, input);
        if (!input.empty()) {
            int t = parseTime(input);
            if (t != -1) s.endTime = t;
            else cout << "Invalid time format (need HH:MM), keeping previous value.\n";
        }

        saveToFile();
        cout << "Slot updated and saved.\n";
    }

    void deleteSlot() {
        int dayIndex = chooseDay();
        if (dayIndex == -1) return;

        printDay(dayIndex);
        if (schedule[dayIndex].empty()) return;

        string input;
        cout << "\nWhich slot number do you want to delete? ";
        getline(cin, input);
        int idx = stoi(input) - 1;

        if (idx < 0 || idx >= (int)schedule[dayIndex].size()) {
            cout << "Invalid slot number!\n";
            return;
        }

        cout << "Deleted: " << schedule[dayIndex][idx].activity << endl;
        schedule[dayIndex].erase(schedule[dayIndex].begin() + idx);
        saveToFile();
    }

    void searchActivities() {
        string keyword;
        cout << "\nEnter keyword to search for: ";
        getline(cin, keyword);

        string keyLower = toLower(keyword);
        bool found = false;

        cout << "\n========== SEARCH RESULTS ==========\n";

        for (int d = 0; d < 7; d++) {
            for (int i = 0; i < (int)schedule[d].size(); i++) {
                Slot s = schedule[d][i];
                if (toLower(s.activity).find(keyLower) != string::npos) {
                    found = true;
                    cout << days[d] << " | "
                         << formatTime(s.startTime) << " - " << formatTime(s.endTime)
                         << " : " << s.activity << endl;
                }
            }
        }

        if (!found) {
            cout << "No matching activities found.\n";
        }
    }

    void checkReminders() {
        cout << "\n========== SMART REMINDER ==========\n";

        int dayIndex = chooseDay();
        if (dayIndex == -1) return;

        int nowMin = promptTime("Enter current time");

        cout << "\nChecking " << days[dayIndex] << " at " << formatTime(nowMin) << "...\n";

        bool doingSomething = false;
        int nextIdx = -1;
        int nextStartMin = -1;

        for (int i = 0; i < (int)schedule[dayIndex].size(); i++) {
            Slot s = schedule[dayIndex][i];
            int startMin = s.startTime;
            int endMin = s.endTime;

            if (startMin <= nowMin && nowMin < endMin) {
                doingSomething = true;
                cout << "Right now: " << s.activity
                     << " (until " << formatTime(s.endTime) << ")\n";
            }
            else if (startMin > nowMin) {
                if (nextIdx == -1 || startMin < nextStartMin) {
                    nextIdx = i;
                    nextStartMin = startMin;
                }
            }
        }

        if (!doingSomething) {
            cout << "Nothing scheduled right now.\n";
        }

        if (nextIdx != -1) {
            Slot s = schedule[dayIndex][nextIdx];
            int minutesUntil = nextStartMin - nowMin;
            cout << "Reminder: \"" << s.activity << "\" starts at "
                 << formatTime(s.startTime) << " (in " << minutesUntil << " minutes)\n";
        } else {
            cout << "No more activities left today.\n";
        }
    }

    void weeklyStatistics() {
        map<string, int> totalMinutes;
        map<string, string> displayName;

        for (int d = 0; d < 7; d++) {
            for (int i = 0; i < (int)schedule[d].size(); i++) {
                Slot s = schedule[d][i];

                int duration = s.endTime - s.startTime;
                if (duration < 0) duration += 24 * 60;

                string key = toLower(s.activity);
                totalMinutes[key] += duration;

                if (displayName.find(key) == displayName.end()) {
                    displayName[key] = s.activity;
                }
            }
        }

        cout << "\n========== WEEKLY STATISTICS ==========\n";

        if (totalMinutes.empty()) {
            cout << "(no activities logged yet, add some slots first)\n";
            return;
        }

        int maxMinutes = 0;
        for (map<string, int>::iterator it = totalMinutes.begin(); it != totalMinutes.end(); it++) {
            if (it->second > maxMinutes) maxMinutes = it->second;
        }

        const int maxBarWidth = 20;

        for (map<string, int>::iterator it = totalMinutes.begin(); it != totalMinutes.end(); it++) {
            int minutes = it->second;

            int barLen = (int)((double)minutes / maxMinutes * maxBarWidth + 0.5);
            if (barLen == 0 && minutes > 0) barLen = 1;

            string bar(barLen, '#');

            cout << displayName[it->first] << "\n";
            cout << bar << " " << formatHours(minutes) << "\n\n";
        }
    }

    void manageNote() {
        string input;

        while (true) {
            cout << "\n========== SHORT NOTE ==========\n";

            if (shortNote.empty()) {
                cout << "(no note written yet)\n";
            } else {
                cout << shortNote << "\n";
            }

            cout << "\n1. Write / Edit note\n";
            cout << "2. Clear note\n";
            cout << "0. Back to main menu\n";
            cout << "Choice: ";
            getline(cin, input);

            if (input == "1") {
                cout << "\nType your note below (up to 6 lines).\n";
                cout << "Just press Enter on an empty line when you're finished.\n";

                string note;
                int lineCount = 0;

                while (lineCount < 6) {
                    string line;
                    getline(cin, line);

                    if (line.empty()) break;

                    if (!note.empty()) note += "\n";
                    note += line;
                    lineCount++;
                }

                if (lineCount == 6) {
                    cout << "(reached the 6 line limit, note saved as is)\n";
                }

                shortNote = note;
                saveToFile();
                cout << "Note saved.\n";
            }
            else if (input == "2") {
                shortNote = "";
                saveToFile();
                cout << "Note cleared.\n";
            }
            else if (input == "0") {
                break;
            }
            else {
                cout << "Invalid choice!\n";
            }
        }
    }

    void viewNote() {
        cout << "\n========== SHORT NOTE ==========\n";
        if (shortNote.empty()) {
            cout << "(no note written yet)\n";
        } else {
            cout << shortNote << "\n";
        }
    }

    void run() {
        string input;

        while (true) {
            cout << "\n========== MENU ==========\n";
            cout << "1. Add slot\n";
            cout << "2. View schedule\n";
            cout << "3. Edit slot\n";
            cout << "4. Delete slot\n";
            cout << "5. Search activities\n";
            cout << "6. Smart reminder (check current time)\n";
            cout << "7. Weekly statistics\n";
            cout << "8. Write / Edit short note\n";
            cout << "9. View short note\n";
            cout << "10. Exit\n";
            cout << "Choice: ";
            getline(cin, input);

            int choice = stoi(input);

            if (choice == 1) addSlot();
            else if (choice == 2) viewSchedule();
            else if (choice == 3) editSlot();
            else if (choice == 4) deleteSlot();
            else if (choice == 5) searchActivities();
            else if (choice == 6) checkReminders();
            else if (choice == 7) weeklyStatistics();
            else if (choice == 8) manageNote();
            else if (choice == 9) viewNote();
            else if (choice == 10) {

                saveToFile();
                cout << "Schedule saved. Goodbye!\n";
                break;
            }
            else {
                cout << "Invalid choice!\n";
            }
        }
    }
};

int main() {

    Plan p;

    p.run();

    return 0;
}