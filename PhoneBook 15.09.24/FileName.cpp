#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

class Contact {
private:
    char* fullName;
    string homePhone;
    string workPhone;
    string mobilePhone;
    string additionalInfo;

public:
    Contact(const char* name, const string& home, const string& work,
        const string& mobile, const string& info) {
        fullName = new char[strlen(name) + 1];
        strcpy(fullName, name);
        homePhone = home;
        workPhone = work;
        mobilePhone = mobile;
        additionalInfo = info;
    }
    Contact(const Contact& other) {
        fullName = new char[strlen(other.fullName) + 1];
        strcpy(fullName, other.fullName);
        homePhone = other.homePhone;
        workPhone = other.workPhone;
        mobilePhone = other.mobilePhone;
        additionalInfo = other.additionalInfo;
    }
    ~Contact() {
        delete[] fullName;
    }
    const char* getName() const { return fullName; }
    const string& getHomePhone() const { return homePhone; }
    const string& getWorkPhone() const { return workPhone; }
    const string& getMobilePhone() const { return mobilePhone; }
    const string& getAdditionalInfo() const { return additionalInfo; }

    void saveToFile(ofstream& ofs) const {
        size_t nameLen = strlen(fullName);
        ofs.write((char*)&nameLen, sizeof(nameLen));
        ofs.write(fullName, nameLen);
        ofs << homePhone << endl << workPhone << endl
            << mobilePhone << endl << additionalInfo << endl;
    }
    static Contact loadFromFile(ifstream& ifs) {
        size_t nameLen;
        ifs.read((char*)&nameLen, sizeof(nameLen));
        char* name = new char[nameLen + 1];
        ifs.read(name, nameLen);
        name[nameLen] = '\0';
        delete[] name;
        return;
    }
    void display() const {
        cout << "ФИО: " << fullName << endl
            << "Домашний телефон: " << homePhone << endl
            << "Рабочий телефон: " << workPhone << endl
            << "Мобильный телефон: " << mobilePhone << endl
            << "Дополнительная информация: " << additionalInfo << endl;
    }
};

class PhoneBook {
private:
    vector<Contact> contacts;

public:
    void addContact(const Contact& contact) {
        contacts.push_back(contact);
    }
    void removeContact(const string& name) {
        contacts.erase(remove_if(contacts.begin(), contacts.end(),
            [&](const Contact& c) { return string(c.getName()) == name; }),
            contacts.end());
    }
    void findContact(const string& name) const {
        for (const auto& contact : contacts) {
            if (string(contact.getName()) == name) {
                contact.display();
                return;
            }
        }
        cout << "Абонент с именем " << name << " не найден\n";
    }
    void showAllContacts() const {
        if (contacts.empty()) {
            cout << "Телефонная книга пуста\n";
            return;
        }

        for (const auto& contact : contacts) {
            contact.display();
            cout << "------------\n";
        }
    }
    void saveToFile(const string& filename) const {
        ofstream ofs(filename, ios::binary);
        if (!ofs) {
            cout << "Не удалось открыть файл для записи\n";
            return;
        }

        size_t size = contacts.size();
        ofs.write((char*)&size, sizeof(size));
        for (const auto& contact : contacts) {
            contact.saveToFile(ofs);
        }
    }
    void loadFromFile(const string& filename) {
        ifstream ifs(filename, ios::binary);
        if (!ifs) {
            cout << "Не удалось открыть файл для чтения\n";
            return;
        }

        size_t size;
        ifs.read((char*)&size, sizeof(size));
        for (size_t i = 0; i < size; ++i) {
            contacts.push_back(Contact::loadFromFile(ifs));
        }
    }
};

int main() {
    PhoneBook phoneBook;
    phoneBook.addContact(Contact("Гондер Жаклен Азимович", "3646563", "453757537", "46847337", "инфо"));
    phoneBook.showAllContacts();
    phoneBook.findContact("Гондер Жаклен Азимович");
    phoneBook.removeContact("Гондер Жаклен Азимович");
    phoneBook.saveToFile("contacts.dat");
    phoneBook.loadFromFile("contacts.dat");
    phoneBook.showAllContacts();
    return 0;
}