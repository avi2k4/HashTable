//
//  main.cpp
//  HashTable
//
//  Created by Avighnash Kumar on 4/4/23.
//

#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>

#define NAME_SIZE 25

class Student {
public:
    int id;
    char * first_name;
    char * last_name;
    double gpa;
    
    Student(int id, char * first_name, char * last_name, float gpa) {
        this->id = id;
        this->first_name = first_name;
        this->last_name = last_name;
        this->gpa = gpa;
    }
};

struct Node {
    Student * student;
    Node * next;
};

class HashTable {
private:
    static const int maxCollisionCount = 3;
    int size;
    Node ** table;
    
    int hash(int key) {
        return key % this->size;
    }
    
    void deleteLL(struct Node * current) {
        if (current == NULL) {
            return;
        }
        
        this->deleteLL(current->next);
        delete(current);
    }
    
    void rehash() {
        
    }
    
public:
    HashTable(int size) {
        this->size = size;
        this->table = new Node * [size];
        
        for (int i = 0; i < this->size; i++) {
            this->table[i] = NULL;
        }
    }
    
    bool insert(Student * student) {
        int location = this->hash(student->id);
        bool doesExist = this->table[location] != NULL;
        
        Node * node = new Node();
        node->student = student;
        node->next = NULL;
        
        int collisionCount = 0;
        
        if (!doesExist) {
            this->table[location] = node;
        }
        else {
            collisionCount += 1;
            Node * current = this->table[location];
            
            while (current != NULL) {
                if (current->next == NULL) {
                    current->next = node;
                    break;
                }
                
                current = current->next;
                collisionCount += 1;
            }
        }
        
        return collisionCount >= this->maxCollisionCount;
    }
    
    Node * get(int id) {
        int location = this->hash(id);
        bool doesExist = this->table[location] != NULL;
        
        if (!doesExist) {
            return NULL;
        }
        
        Node * current = this->table[location];
        
        while (current != NULL) {
            if (current->student->id == id) {
                return current;
            }
            
            current = current->next;
        }
        
        return current;
    }
    
    // return bool of existing
    bool del(int id) {
        int location = this->hash(id);
        bool doesExist = this->table[location] != NULL;
        
        if (!doesExist) {
            return false;
        }
        
        Node * current = this->table[location];
        
        // If only node, that means only 1
        if (current->next == NULL) {
            delete current;
            this->table[location] = NULL;
        }
        else {
            
        }

        return true;
    }
    
    void print() {
        for (int i = 0; i < this->size; i++) {
            if (this->table[i] != NULL) {
                Node * current = this->table[i];
                
                while (current != NULL) {
                    Student * student = current->student;
                    
                    std::cout << "---------------------------" << std::endl;
                    std::cout << "ID: " << student->id << std::endl;
                    std::cout << "Name: " << student->first_name << " " << student->last_name << std::endl;
                    std::cout << "GPA: " << std::setprecision(2) << std::fixed << student->gpa << std::endl;
                    std::cout << "---------------------------" << std::endl;

                    current = current->next;
                }
            }
        }
    }
    
    void deleteAll() {
        for (int i = 0; i < this->size; i++) {
            this->deleteLL(this->table[i]);
            this->table[i] = NULL;
        }
    }
};

float randomGPA() {
    return static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / 4));
}

void generateRandomStudentsAndInsert(HashTable * table, int count) {
    std::vector<char *> first_names;
    std::vector<char *> last_names;
    
    std::ifstream first_names_file("first_names.txt");
    std::ifstream last_names_file("last_names.txt");
    
    char first_name[NAME_SIZE];
    while (!first_names_file.eof()) {
        std::cout << "bro" << std::endl;
        first_names_file >> first_name;
        std::cout << first_name << std::endl;
        first_names.push_back(first_name);
    }
    
    char last_name[NAME_SIZE];
    while (last_names_file >> last_name) {
        last_names.push_back(last_name);
    }
    
    char * chosen_first_name = first_names[std::rand() % first_names.size()];
    char * chosen_last_name = last_names[std::rand() % last_names.size()];

    for (int id = 0; id < count; id++) {
        table->insert(new Student(id, chosen_first_name, chosen_last_name, randomGPA()));
    }
}

int main() {
    HashTable * table = new HashTable(10);
    
    std::cout << "\nWelcome to your student hashtable! Here are the commands:" << std::endl;
    std::cout << "[1] to add a student" << std::endl;
    std::cout << "[2] to delete a student, by id" << std::endl;
    std::cout << "[3] to print all students" << std::endl;
    std::cout << "[4] to generate random students" << std::endl;
    std::cout << "[5] to quit" << std::endl;
    
    while (true) {
        std::cout << "\nPerform a command" << std::endl;

        int input;
        std::cin >> input;

        if (input == 1) {
            std::cout << "Enter the id:" << std::endl;
            int id;
            std::cin >> id;
            
            std::cout << "Enter the first name:" << std::endl;
            char first_name[NAME_SIZE];
            std::cin >> first_name;
            
            std::cout << "Enter the last name:" << std::endl;
            char last_name[NAME_SIZE];
            std::cin >> last_name;
            
            std::cout << "Enter the GPA:" << std::endl;
            float gpa;
            std::cin >> gpa;
            
            table->insert(new Student(id, first_name, last_name, gpa));
            std::cout << "Added student with id: " << id << std::endl;
        }
        else if (input == 2) {
            std::cout << "Enter the id of the student you would like to delete:" << std::endl;
            int id_input;
            std::cin >> id_input;
            
            if (table->del(id_input)) {
                std::cout << "Student with id " << id_input << " successfully deleted!" << std::endl;
            }
            else {
                std::cout << "Student doesn't exist" << std::endl;
            }
        }
        else if (input == 3) {
            table->print();
        }
        else if (input == 4) {
            std::cout << "Enter the number of students you would like to generate:" << std::endl;
            int generate_input;
            std::cin >> generate_input;
            
            generateRandomStudentsAndInsert(table, generate_input);
        }
        else if (input == 5) {
            break;
        }
    }
    
    
    table->print();
    
    return 0;
}
