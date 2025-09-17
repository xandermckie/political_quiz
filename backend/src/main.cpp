#include "quiz.h"
#include <iostream>
#include <vector>

int main() {
    Quiz quiz;
    std::vector<Question> questions = quiz.getQuestions();
    std::vector<int> answers;
    
    std::cout << "Political Ideology Quiz\n";
    std::cout << "For each statement, enter your response:\n";
    std::cout << "1 - Strongly Disagree\n";
    std::cout << "2 - Disagree\n";
    std::cout << "3 - Neutral\n";
    std::cout << "4 - Agree\n";
    std::cout << "5 - Strongly Agree\n\n";
    
    // Get user answers
    for (size_t i = 0; i < questions.size(); i++) {
        int answer;
        std::cout << "Question " << (i+1) << "/" << questions.size() << ": " << questions[i].text << "\n";
        std::cout << "Your answer (1-5): ";
        std::cin >> answer;
        
        // Validate input
        while (answer < 1 || answer > 5) {
            std::cout << "Please enter a number between 1 and 5: ";
            std::cin >> answer;
        }
        
        answers.push_back(answer);
        std::cout << std::endl;
    }
    
    // Calculate scores and determine ideology
    std::map<std::string, double> scores = quiz.calculateScores(answers);
    Result result = quiz.determineIdeology(scores);
    
    // Generate HTML results page
    quiz.generateHTMLResult(scores, result);
    
    std::cout << "Quiz completed! Your results have been saved to '../frontend/results.html'\n";
    std::cout << "Your ideology: " << result.ideology << " (" << round(result.score) << "% match)\n";
    
    return 0;
}