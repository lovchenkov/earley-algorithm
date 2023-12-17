#include "earley.cpp"
#include <fstream>
#include <sstream>

GrammarAndRequests Read() {
  GrammarAndRequests data;

  std::cin >> data.nonterminals_count >> data.terminals_count >> data.rules_count;

  for (int i = 0; i < data.nonterminals_count; ++i) {
    char nonterm;
    std::cin >> nonterm;
    data.nonterminals.insert(nonterm);
  }
  for (int i = 0; i < data.terminals_count; ++i) {
    char term;
    std::cin >> term;
    data.terminals.insert(term);
  }

  for (int i = 0; i < data.rules_count; ++i) {
    std::string str;
    std::cin >> str;
    Rule rule(str);
    data.rules.push_back(rule);
  }

  std::cin >> data.start_nonterminal;

  std::cin >> data.requests_count;

  for (int i = 0; i < data.requests_count; ++i) {
    std::string word;
    std::cin >> word;
    data.requests.push_back(word);
  }
  return data;
}

std::vector<std::string> GetAnswers(const GrammarAndRequests& data, std::ostream& logger,
                                    bool need_to_log) {
  EarleyChecker checker(data, need_to_log);
  std::vector<std::string> answers;
  for (int i = 0; i < data.requests_count; ++i) {
    answers.push_back(checker.isWordInGrammar(data.requests[i], logger) ? "Yes" : "No");
  }
  return answers;
}

void PrintAnswers(std::vector<std::string>& answers) {
  for (auto& ans : answers) std::cout << ans << '\n';
}