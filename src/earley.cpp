#include <string>
#include <cstring>
#include <set>
#include <unordered_set>
#include <vector>
#include <iostream>


struct Rule {
  std::string rule;
  char left;
  std::string right;

  Rule() = default;
  Rule(std::string str) : rule(str) {
    char* rule = const_cast<char*>(str.c_str());
    left = strtok(rule, "->")[0];
    if (str[str.size() - 1] != '>')
      right = strtok(NULL, "->");
    else 
      right = "";
  }
  Rule(const Rule& other) = default;
};

struct InputData {
  int nonterminals_count;
  int terminals_count;
  int rules_count;
  std::unordered_set<char> nonterminals;
  std::unordered_set<char> terminals;
  std::vector<Rule> rules;
  char start_nonterminal;
  int requests_count;
  std::vector<std::string> requests;
};

class EarleyChecker {
 private:
  struct Situation {
    Rule rule;
    int shift;  // dot in [0, rule.right.size()]
    int l;  // prefix what was before this situation

    Situation() = default;
    Situation(const Rule& rule, int shift, int l)
      : rule(rule)
      , shift(shift)
      , l(l) {}
    
    bool operator<(const Situation& rhs) const {  // for std::set:insert to work
      if (rule.left != rhs.rule.left)
        return rule.left < rhs.rule.left;
      if (rule.right != rhs.rule.right)
        return rule.right < rhs.rule.right;
      if (shift != rhs.shift)
        return shift < rhs.shift;
      return l < rhs.l;
      std::cout << "IN COMP\n";
    }

    bool IsInside() const { return shift < rule.right.size(); }
    char GetCurrentSymbol() const {
      return rule.right[shift];
    }

    std::string GetRule() const { return rule.rule; }
  };

  std::unordered_set<char> nonterminals;
  std::unordered_set<char> terminals;
  std::vector<Rule> rules;
  char start_nonterminal;
  std::vector<std::set<Situation>> levels;

  const char start = '@';
  
  void LogSituation(const Situation& situation, std::ostream& logger, int level_index) {
    logger << "ADDED SITUATION " << situation.rule.left
           << "->"
           << situation.rule.right << ' '
           << situation.shift << ' '
           << situation.l << ' '
           << level_index
           << std::endl;

  }

  void Scan(char terminal, int level_index, std::ostream& logger) {
    logger << "ENTERING SCAN ON LEVEL " << level_index << std::endl;
    if (!terminals.count(terminal)) {
      throw std::runtime_error("Word is out of alphabet!\n");
    }
    for (const Situation& situation : levels[level_index]) {
      if (!situation.IsInside()) {
        continue;
      }
      if (situation.GetCurrentSymbol() == terminal) {
        Situation to_add(Rule(situation.GetRule()), situation.shift + 1, situation.l);
        levels[level_index + 1].insert(to_add);
        LogSituation(to_add, logger, level_index + 1);
      }
    }
    logger << "EXITING SCAN\n-------------------------------" << std::endl;
  }

  void Predict(int level_index, std::ostream& logger) {
    logger << "ENTERING PREDICT ON LEVEL " << level_index << std::endl;
    for (const Situation& situation : levels[level_index]) {
      if (!situation.IsInside()) {
        continue;
      }
      char nonterminal_to_predict = situation.GetCurrentSymbol();

      if (nonterminals.count(nonterminal_to_predict)) {
        for (const Rule& rule : rules) {
          if (rule.left == nonterminal_to_predict) {
            Situation to_add(rule.rule, 0, level_index);
            if (!levels[level_index].count(to_add)) {
              levels[level_index].insert(to_add);
              LogSituation(to_add, logger, level_index);
            }
          }
        }
      }
    }
    logger << "EXITING PREDICT\n-------------------------------" << std::endl;
  }

  void Complete(int level_index, std::ostream& logger) {
    logger << "ENTERING COMPLETE ON LEVEL " << level_index << std::endl;
    for (const Situation& situation_to_complete : levels[level_index]) {
      if (situation_to_complete.shift != situation_to_complete.rule.right.size()) {
        continue;
      }
      for (const Situation& situation : levels[situation_to_complete.l]) {
        if (situation.GetCurrentSymbol() == situation_to_complete.rule.left) {
          Situation to_add(situation.GetRule(), situation.shift + 1, situation.l);
          if (!levels[level_index].count(to_add)) {
            levels[level_index].insert(to_add);
            LogSituation(to_add, logger, level_index);
          }
        }
      }
    }
    logger << "EXITING COMPLETE\n-------------------------------" << std::endl;
  }

 public:
  EarleyChecker(const InputData& data)
    : nonterminals(data.nonterminals)
    , terminals(data.terminals)
    , rules(data.rules)
    , start_nonterminal(data.start_nonterminal) {}

  bool isWordInGrammar(const std::string& word, std::ostream& logger) {
    logger << "\nCHECKING WORD " << word << "\n\n";
    levels.resize(word.size() + 1);

    Rule starting_rule(std::string{start} + "->" + start_nonterminal);
    Situation to_add(starting_rule, 0, 0);
    levels[0].insert(to_add);
    LogSituation(to_add, logger, 0);
    
    for (int i = 0; i <= word.size(); ++i) {
      logger << "STARTING ITERATION " << i << std::endl;
      if (i != 0)
        Scan(word[i - 1], i - 1, logger);

      int prev_level_size = -1;
      while (prev_level_size != levels[i].size()) {
        prev_level_size = levels[i].size();
        Complete(i, logger);
        Predict(i, logger);
      }

      logger << "FINIISHING ITERATION " << i << std::endl;
    }
    Situation finish(starting_rule, 1, 0);
    bool result = levels[word.size()].count(finish);
    logger << "FINISHED CHECKING " << word << ", RESULT IS " << (result ? "YES" : "NO") << "\n\n\n";

    levels.clear();
    return result;
  }
};