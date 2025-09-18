#ifndef QUIZ_H
#define QUIZ_H

#include <string>
#include <vector>
#include <map>

struct Question {
    std::string text;
    std::string axis;
    double weight;
};

struct Axis {
    std::string name;
    std::string description;
};

struct Ideology {
    std::string name;
    std::string description;
    std::map<std::string, double> scores;
};

struct Result {
    std::string ideology;
    std::string description;
    double score;
};

class Quiz {
private:
    std::vector<Question> questions;
    std::vector<Axis> axes;
    std::vector<Ideology> ideologies;
    
    std::vector<Question> loadQuestionsFromFile(const std::string& filename);
    std::vector<Axis> loadAxesFromFile(const std::string& filename);
    std::vector<Ideology> loadIdeologiesFromFile(const std::string& filename);
    
public:
    Quiz(const std::string& questionFile = "questions.txt", 
         const std::string& axisFile = "config/axes.txt",
         const std::string& ideologyFile = "config/ideologies.txt");
    
    std::vector<Question> getQuestions();
    std::vector<Axis> getAxes();
    std::map<std::string, double> calculateScores(const std::vector<int>& answers);
    Result determineIdeology(const std::map<std::string, double>& scores);
    void generateHTMLResult(const std::map<std::string, double>& scores, const Result& result);
};

#endif