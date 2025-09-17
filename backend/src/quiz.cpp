#include "quiz.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <algorithm>

Quiz::Quiz() {
    // Initialize questions
    questions = {
        {"The government should play a larger role in the economy.", "Economic", 1.0},
        {"Military action is often necessary to protect national interests.", "Diplomatic", 1.0},
        {"Individual freedoms should be protected above all else.", "Civil", 1.0},
        {"Society should uphold traditional values and morals.", "Societal", 1.0},
        {"Taxes should be increased on the wealthy to fund social programs.", "Economic", 1.0},
        {"International cooperation is more important than national sovereignty.", "Diplomatic", 1.0},
        {"Security is more important than privacy.", "Civil", 1.0},
        {"Progressive values benefit society.", "Societal", 1.0},
        {"Free market competition leads to better outcomes for all.", "Economic", 1.0},
        {"Diplomacy is usually better than military action.", "Diplomatic", 1.0},
        {"The right to free speech should be absolute.", "Civil", 1.0},
        {"Religion should play a role in government.", "Societal", 1.0}
    };
}

std::vector<Question> Quiz::getQuestions() {
    return questions;
}

std::map<std::string, double> Quiz::calculateScores(const std::vector<int>& answers) {
    std::map<std::string, double> scores = {
        {"Economic", 0.0}, {"Diplomatic", 0.0}, {"Civil", 0.0}, {"Societal", 0.0}
    };
    std::map<std::string, int> count = {
        {"Economic", 0}, {"Diplomatic", 0}, {"Civil", 0}, {"Societal", 0}
    };
    
    for (size_t i = 0; i < questions.size(); i++) {
        std::string axis = questions[i].axis;
        double answerValue = (answers[i] - 2) / 2.0; // Convert to -1 to 1 range
        scores[axis] += answerValue * questions[i].weight;
        count[axis]++;
    }
    
    // Normalize scores
    for (auto& pair : scores) {
        if (count[pair.first] > 0) {
            pair.second = (pair.second / count[pair.first] + 1) / 2.0 * 100;
        }
    }
    
    return scores;
}

Result Quiz::determineIdeology(const std::map<std::string, double>& scores) {
    // Define ideological positions
    std::vector<std::pair<std::string, std::vector<double>>> ideologies = {
        {"Libertarian", {80, 70, 90, 80}},
        {"Conservative", {60, 40, 40, 20}},
        {"Liberal", {70, 80, 70, 85}},
        {"Socialist", {20, 90, 60, 70}},
        {"Centrist", {50, 50, 50, 50}},
        {"Authoritarian", {40, 30, 10, 15}},
        {"Libertarian Socialist", {30, 85, 80, 90}},
        {"Anarchist", {10, 90, 95, 95}}
    };
    
    std::vector<std::string> descriptions = {
        "Libertarians emphasize individual liberty, limited government, and free markets.",
        "Conservatives value tradition, authority, and stability with cautious change.",
        "Liberals support individual rights, democracy, and social progress with government intervention.",
        "Socialists advocate for social ownership, economic equality, and redistribution of wealth.",
        "Centrists take a balanced approach, incorporating ideas from multiple political traditions.",
        "Authoritarians prioritize order, hierarchy, and state power over individual freedoms.",
        "Libertarian Socialists combine opposition to state power with support for economic equality.",
        "Anarchists reject all forms of compulsory government and hierarchy."
    };
    
    // Calculate distances to each ideology
    std::vector<std::pair<double, int>> distances;
    for (int i = 0; i < ideologies.size(); i++) {
        double distance = 0;
        distance += pow(scores.at("Economic") - ideologies[i].second[0], 2);
        distance += pow(scores.at("Diplomatic") - ideologies[i].second[1], 2);
        distance += pow(scores.at("Civil") - ideologies[i].second[2], 2);
        distance += pow(scores.at("Societal") - ideologies[i].second[3], 2);
        distances.push_back({sqrt(distance), i});
    }
    
    // Find the closest ideology
    std::sort(distances.begin(), distances.end());
    int closestIdx = distances[0].second;
    
    // Calculate match percentage (0-100%)
    double maxDistance = sqrt(pow(100, 2) * 4); // Maximum possible distance
    double matchPercent = (1 - distances[0].first / maxDistance) * 100;
    
    return {ideologies[closestIdx].first, descriptions[closestIdx], matchPercent};
}

void Quiz::generateHTMLResult(const std::map<std::string, double>& scores, const Result& result) {
    std::ofstream htmlFile("../frontend/results.html");
    
    htmlFile << R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Quiz Results</title>
    <link rel="stylesheet" href="styles/results.css">
</head>
<body>
    <div class="container">
        <h1>Your Political Ideology Results</h1>
        
        <div class="result-box">
            <div class="ideology">)" << result.ideology << R"(</div>
            <div class="match">Match: )" << round(result.score) << R"(%</div>
            <div class="description">)" << result.description << R"(</div>
        </div>
        
        <h2>Detailed Scores</h2>
        <div class="axis-container">)";
    
    for (const auto& score : scores) {
        htmlFile << R"(
            <div class="axis">
                <span class="axis-name">)" << score.first << R"(:</span>
                <div class="bar-container">
                    <div class="bar" style="width: )" << score.second << R"(%;"></div>
                </div>
                <span class="percentage">)" << round(score.second) << R"(%</span>
            </div>)";
    }
    
    htmlFile << R"(
        </div>
        
        <a href="quiz.html" class="retake-btn">Retake Quiz</a>
    </div>
</body>
</html>)";
    
    htmlFile.close();
}