#pragma once

#include <fstream>
#include <string>
#include <list>

class ILoggable {
public:
    virtual std::string stringToLog() = 0;
};

class Observer {
protected:
    Observer();
public:
    virtual ~Observer();
    virtual void update(ILoggable* iloggable) = 0;
};

class Subject {
private:
    std::list<Observer*> observers;
public:
    Subject();
    ~Subject();
    virtual void attach(Observer* observer);
    virtual void detach(Observer* observer);
    virtual void notify(ILoggable* iloggable);
};

class LogObserver : Observer {
    std::ofstream file;
    const std::string DEFAULT_FILENAME = "gamelog.txt";
    std::list<Subject*> subjects;
public:
    LogObserver();
    LogObserver(std::string filename);
    ~LogObserver();
    void attachSubject(Subject* subject);
    void update(ILoggable* iloggable) override;
};

extern LogObserver* logObserver;
