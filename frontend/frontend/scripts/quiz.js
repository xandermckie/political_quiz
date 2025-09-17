// Quiz questions
const questions = [
    "The government should play a larger role in the economy.",
    "Military action is often necessary to protect national interests.",
    "Individual freedoms should be protected above all else.",
    "Society should uphold traditional values and morals.",
    "Taxes should be increased on the wealthy to fund social programs.",
    "International cooperation is more important than national sovereignty.",
    "Security is more important than privacy.",
    "Progressive values benefit society.",
    "Free market competition leads to better outcomes for all.",
    "Diplomacy is usually better than military action.",
    "The right to free speech should be absolute.",
    "Religion should play a role in government."
];

let currentQuestion = 0;
let answers = [];

function updateQuestion() {
    const questionText = document.getElementById('question-text');
    questionText.textContent = `${currentQuestion + 1}. ${questions[currentQuestion]}`;
    
    // Update navigation buttons
    document.getElementById('prev-btn').disabled = currentQuestion === 0;
    
    if (currentQuestion === questions.length - 1) {
        document.getElementById('next-btn').style.display = 'none';
        document.getElementById('submit-btn').style.display = 'block';
    } else {
        document.getElementById('next-btn').style.display = 'block';
        document.getElementById('submit-btn').style.display = 'none';
    }
    
    // Clear selected options
    document.querySelectorAll('.option').forEach(option => {
        option.classList.remove('selected');
    });
    
    // Restore previous answer if exists
    if (answers[currentQuestion] !== undefined) {
        const options = document.querySelectorAll('.option');
        options[answers[currentQuestion] - 1].classList.add('selected');
    }
    
    updateProgress();
}

function updateProgress() {
    const progress = ((currentQuestion + 1) / questions.length) * 100;
    document.getElementById('quiz-progress').style.width = `${progress}%`;
}

function selectOption(optionElement, value) {
    // Clear other selections
    const options = optionElement.parentElement.querySelectorAll('.option');
    options.forEach(option => {
        option.classList.remove('selected');
    });
    
    // Select this option
    optionElement.classList.add('selected');
    
    // Save answer
    answers[currentQuestion] = value;
}

function nextQuestion() {
    if (answers[currentQuestion] === undefined) {
        alert('Please select an answer before continuing.');
        return;
    }
    
    if (currentQuestion < questions.length - 1) {
        currentQuestion++;
        updateQuestion();
    }
}

function prevQuestion() {
    if (currentQuestion > 0) {
        currentQuestion--;
        updateQuestion();
    }
}

function submitQuiz() {
    if (answers.length < questions.length) {
        alert('Please answer all questions before submitting.');
        return;
    }
    
    // In a real implementation, this would send data to the C++ backend
    alert("In a complete implementation, this would send your answers to the C++ backend for processing.");
    
    // For demonstration, redirect to a placeholder results page
    window.location.href = "results.html";
}

// Initialize the quiz when page loads
document.addEventListener('DOMContentLoaded', function() {
    updateQuestion();
});