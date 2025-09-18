#include "quiz.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

std::vector<Question> Quiz::loadQuestionsFromFile(const std::string& filename) {
    std::vector<Question> questions;
    std::ifstream file(filename);
    std::string line;
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open questions file: " << filename << std::endl;
        return questions;
    }
    
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;
        
        size_t firstColon = line.find(':');
        size_t secondColon = line.find(':', firstColon + 1);
        
        if (firstColon != std::string::npos && secondColon != std::string::npos) {
            std::string axis = line.substr(0, firstColon);
            std::string weightStr = line.substr(firstColon + 1, secondColon - firstColon - 1);
            std::string text = line.substr(secondColon + 1);
            
            // Trim whitespace
            text.erase(0, text.find_first_not_of(" \t"));
            text.erase(text.find_last_not_of(" \t") + 1);
            
            try {
                double weight = std::stod(weightStr);
                questions.push_back({text, axis, weight});
            } catch (...) {
                std::cerr << "Warning: Invalid weight in line: " << line << std::endl;
            }
        }
    }
    
    file.close();
    return questions;
}

std::vector<Axis> Quiz::loadAxesFromFile(const std::string& filename) {
    std::vector<Axis> axes;
    std::ifstream file(filename);
    std::string line;
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open axes file: " << filename << std::endl;
        // Return default axes
        return {
            {"Economic", "Measures your views on economic policy and wealth distribution"},
            {"Diplomatic", "Measures your views on foreign policy and international relations"},
            {"Civil", "Measures your views on individual rights and civil liberties"},
            {"Societal", "Measures your views on social issues and cultural values"}
        };
    }
    
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;
        
        size_t colonPos = line.find(':');
        
        if (colonPos != std::string::npos) {
            std::string name = line.substr(0, colonPos);
            std::string description = line.substr(colonPos + 1);
            
            // Trim whitespace
            name.erase(0, name.find_first_not_of(" \t"));
            name.erase(name.find_last_not_of(" \t") + 1);
            description.erase(0, description.find_first_not_of(" \t"));
            description.erase(description.find_last_not_of(" \t") + 1);
            
            axes.push_back({name, description});
        }
    }
    
    file.close();
    return axes;
}

std::vector<Ideology> Quiz::loadIdeologiesFromFile(const std::string& filename) {
    std::vector<Ideology> ideologies;
    std::ifstream file(filename);
    std::string line;
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open ideologies file: " << filename << std::endl;
        return ideologies;
    }
    
    Ideology currentIdeology;
    bool readingIdeology = false;
    
    while (std::getline(file, line)) {
        // Skip empty lines
        if (line.empty()) continue;
        
        // Check for ideology header
        if (line[0] == '[' && line[line.size() - 1] == ']') {
            if (readingIdeology) {
                ideologies.push_back(currentIdeology);
            }
            
            currentIdeology = Ideology();
            currentIdeology.name = line.substr(1, line.size() - 2);
            readingIdeology = true;
        }
        // Check for description
        else if (line.find("Description:") == 0) {
            currentIdeology.description = line.substr(12);
            // Trim whitespace
            currentIdeology.description.erase(0, currentIdeology.description.find_first_not_of(" \t"));
            currentIdeology.description.erase(currentIdeology.description.find_last_not_of(" \t") + 1);
        }
        // Check for score line
        else if (line.find(':') != std::string::npos) {
            size_t colonPos = line.find(':');
            std::string axis = line.substr(0, colonPos);
            std::string scoreStr = line.substr(colonPos + 1);
            
            // Trim whitespace
            axis.erase(0, axis.find_first_not_of(" \t"));
            axis.erase(axis.find_last_not_of(" \t") + 1);
            scoreStr.erase(0, scoreStr.find_first_not_of(" \t"));
            scoreStr.erase(scoreStr.find_last_not_of(" \t") + 1);
            
            try {
                double score = std::stod(scoreStr);
                currentIdeology.scores[axis] = score;
            } catch (...) {
                std::cerr << "Warning: Invalid score in line: " << line << std::endl;
            }
        }
    }
    
    // Add the last ideology
    if (readingIdeology) {
        ideologies.push_back(currentIdeology);
    }
    
    file.close();
    return ideologies;
}

Quiz::Quiz(const std::string& questionFile, const std::string& axisFile, const std::string& ideologyFile) {
    questions = loadQuestionsFromFile(questionFile);
    axes = loadAxesFromFile(axisFile);
    ideologies = loadIdeologiesFromFile(ideologyFile);
    
    std::cout << "Loaded " << questions.size() << " questions, " 
              << axes.size() << " axes, and " 
              << ideologies.size() << " ideologies." << std::endl;
}

std::vector<Question> Quiz::getQuestions() {
    return questions;
}

std::vector<Axis> Quiz::getAxes() {
    return axes;
}

std::map<std::string, double> Quiz::calculateScores(const std::vector<int>& answers) {
    std::map<std::string, double> scores;
    std::map<std::string, int> count;
    
    // Initialize scores for all axes
    for (const auto& axis : axes) {
        scores[axis.name] = 0.0;
        count[axis.name] = 0;
    }
    
    for (size_t i = 0; i < questions.size(); i++) {
        std::string axis = questions[i].axis;
        double answerValue = (answers[i] - 2) / 2.0; // Convert to -1 to 1 range
        
        // Only process if axis exists
        if (scores.find(axis) != scores.end()) {
            scores[axis] += answerValue * questions[i].weight;
            count[axis]++;
        }
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
    if (ideologies.empty()) {
        return {"Unknown", "No ideologies defined", 0.0};
    }
    
    // Calculate distances to each ideology
    std::vector<std::pair<double, int>> distances;
    for (int i = 0; i < ideologies.size(); i++) {
        double distance = 0;
        
        for (const auto& axis : axes) {
            double ideologyScore = ideologies[i].scores[axis.name];
            double userScore = scores.at(axis.name);
            distance += pow(userScore - ideologyScore, 2);
        }
        
        distances.push_back({sqrt(distance), i});
    }
    
    // Find the closest ideology
    std::sort(distances.begin(), distances.end());
    int closestIdx = distances[0].second;
    
    // Calculate match percentage (0-100%)
    double maxDistance = sqrt(pow(100, 2) * axes.size()); // Maximum possible distance
    double matchPercent = (1 - distances[0].first / maxDistance) * 100;
    
    return {ideologies[closestIdx].name, ideologies[closestIdx].description, matchPercent};
}

void Quiz::generateHTMLResult(const std::map<std::string, double>& scores, const Result& result) {
    // Try to create the results file in the current directory instead of ../frontend/
    std::ofstream htmlFile("quiz_results.html");
    
    if (!htmlFile.is_open()) {
        // If that fails, try creating it in a temporary location
        std::cerr << "Warning: Could not create results file in current directory. Trying temp location..." << std::endl;
        htmlFile.open("C:\\temp\\quiz_results.html");
    }
    
    if (!htmlFile.is_open()) {
        std::cerr << "Error: Could not create results file anywhere!" << std::endl;
        return;
    }
    
    // Rest of the HTML generation code remains the same...
    htmlFile << R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Quiz Results</title>
    <style>
        body { font-family: Arial, sans-serif; line-height: 1.6; margin: 0; padding: 20px; background-color: #f5f5f5; color: #333; }
        .container { max-width: 800px; margin: 0 auto; background: white; padding: 30px; border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }
        h1 { color: #2c3e50; text-align: center; }
        .result-box { background: #e8f4f8; padding: 20px; border-radius: 8px; margin-bottom: 20px; text-align: center; }
        .ideology { font-size: 24px; font-weight: bold; color: #2980b9; }
        .match { font-size: 18px; margin: 10px 0; }
        .description { font-style: italic; margin-top: 10px; }
        .axis-container { margin: 20px 0; }
        .axis { margin: 10px 0; }
        .axis-name { display: inline-block; width: 120px; }
        .bar-container { display: inline-block; width: 300px; height: 20px; background: #eee; border-radius: 10px; overflow: hidden; vertical-align: middle; }
        .bar { height: 100%; background: #3498db; }
        .percentage { display: inline-block; width: 50px; text-align: right; margin-left: 10px; }
        .retake-btn { display: block; width: 200px; margin: 30px auto 0; padding: 10px; background: #3498db; color: white; border: none; border-radius: 5px; font-size: 16px; cursor: pointer; text-align: center; text-decoration: none; }
        .retake-btn:hover { background: #2980b9; }
    </style>
</head>
<body>
    <div class="container">
        <h1>Your Political Ideology Results</h1>
        
        <div class="result-box">
            <div class="ideology">)" << result.ideology << R"(</div>
            <div class="match">Match: )" << std::round(result.score) << R"(%</div>
            <div class="description">)" << result.description << R"(</div>
        </div>
        
        <h2>Detailed Scores</h2>
        <div class="axis-container">)";
    
    for (const auto& axis : axes) {
        double score = scores.at(axis.name);
        htmlFile << R"(
            <div class="axis">
                <span class="axis-name">)" << axis.name << R"(:</span>
                <div class="bar-container">
                    <div class="bar" style="width: )" << score << R"(%;"></div>
                </div>
                <span class="percentage">)" << std::round(score) << R"(%</span>
                <div class="axis-description">)" << axis.description << R"(</div>
            </div>)";
    }
    
    htmlFile << R"(
        </div>
        
        <a href="#" onclick="window.location.reload();" class="retake-btn">Retake Quiz</a>
    </div>
</body>
</html>)";
    
    htmlFile.close();
    std::cout << "Results saved to: quiz_results.html" << std::endl;
}