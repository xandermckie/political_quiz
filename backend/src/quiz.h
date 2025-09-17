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

struct Result {
    std::string ideology;
    std::string description;
    double score;
};

class Quiz {
private:
    std::vector<Question> questions;
    
public:
    Quiz();
    std::vector<Question> getQuestions();
    std::map<std::string, double> calculateScores(const std::vector<int>& answers);
    Result determineIdeology(const std::map<std::string, double>& scores);
    void generateHTMLResult(const std::map<std::string, double>& scores, const Result& result);
};

#endif