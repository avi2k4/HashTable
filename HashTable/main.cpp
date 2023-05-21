//
//  main.cpp
//  HashTable
//
//  Created by Avighnash Kumar on 4/4/23.
//

#include <iostream>

class Student {
public:
    int id;
    int age;
    
    Student(int id, int age) {
        this->id = id;
        this->age = age;
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
    
    void print() {
        for (int i = 0; i < this->size; i++) {
            if (this->table[i] != NULL) {
                Node * current = this->table[i];
                
                while (current != NULL) {
                    std::cout << current->student->id << " at position " << i << std::endl;
                    current = current->next;
                }
            }
        }
    }
    
    void del() {
        for (int i = 0; i < this->size; i++) {
            this->deleteLL(this->table[i]);
            this->table[i] = NULL;
        }
    }
};

int main() {
    HashTable * t = new HashTable(10);
    
    Student * s1 = new Student(1234, 4);
    t->insert(s1);
    Student * s2 = new Student(13, 13);
    t->insert(s2);
    Student * s3 = new Student(9, 1);
    t->insert(s3);
    Student * s4 = new Student(100, 2);
    t->insert(s4);
    Student * s5 = new Student(502, 3);
    t->insert(s5);
    t->print();
        
    t->del();

    t->print();
    
    return 0;
}
