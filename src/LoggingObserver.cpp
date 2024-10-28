#include "LoggingObserver.h"
#include <fstream>

LogObserver* logObserver = new LogObserver();

Observer::Observer()=default;
Observer::~Observer()=default;

Subject::Subject() {
    
}

Subject::~Subject() {
    
}

void Subject::attach(Observer* observer) {
    this->observers.push_back(observer);
}

void Subject::detach(Observer* observer) {
    this->observers.remove(observer);
}

void Subject::notify(ILoggable* iloggable) {
    for (Observer* observer : this->observers) {
        observer->update(iloggable);
    }
}

LogObserver::LogObserver() {
    this->file.open(DEFAULT_FILENAME);
}

void LogObserver::attachSubject(Subject* subject) {
    subject->attach(this);
}

LogObserver::LogObserver(std::string filename) {
    this->file.open(filename);
}

LogObserver::~LogObserver() {
    this->file.close();
}

void LogObserver::update(ILoggable* iloggable) {
    this->file << iloggable->stringToLog() << std::endl;
}
