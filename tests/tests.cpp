#include <gtest/gtest.h>
#include "../src/main.h"

TEST(RuleCheck, ConstructorCheck) {
  Rule r1("A->aAb");
  Rule r2("R->irkaAB");
  Rule r3("B->");

  ASSERT_EQ(r1.left, 'A');
  ASSERT_EQ(r1.right, "aAb");
  ASSERT_EQ(r2.left, 'R');
  ASSERT_EQ(r2.right, "irkaAB");
  ASSERT_EQ(r3.left, 'B');
  ASSERT_EQ(r3.right, "");
}

TEST(EarleyCheck, BracketSequences) {
  std::unordered_set<char> nonterminals = {'S'};
  std::unordered_set<char> terminals = {'(', ')'};
  std::vector<Rule> rules = {Rule("S->(S)S"), Rule("S->")};
  char start_nonterminal = 'S';

  std::vector<std::string> requests = {"", "()()", "(())()()", "()((()))", ")", "()())"};
  InputData data{nonterminals.size(), terminals.size(), rules.size(),
                 nonterminals, terminals, rules, start_nonterminal,
                 requests.size(), requests};

  std::ofstream of;
  of.open("log_test_brackets");
  std::ostream logger(nullptr);
  std::streambuf *buf;
  buf = of.rdbuf();
  logger.rdbuf(buf);

  auto answers = GetAnswers(data, logger);
  std::vector<std::string> expected_answers = {"Yes", "Yes", "Yes", "Yes", "No", "No"};
  ASSERT_EQ(answers, expected_answers);
}

TEST(EarleyCheck, SomeGrammar) {
  std::unordered_set<char> nonterminals = {'A', 'B', 'C', 'T'};
  std::unordered_set<char> terminals = {'a', 'b', 'c'};
  std::vector<Rule> rules = {Rule("T->ABC"), Rule("A->aA"), Rule("B->Bb"), Rule("C->cC"),
                             Rule("A->"), Rule("B->"), Rule("C->")};
  char start_nonterminal = 'T';

  std::vector<std::string> requests = {"", "aab", "bbccc", "cc", "ac", "aabc", "aacb", "ba", "cabb"};
  InputData data{nonterminals.size(), terminals.size(), rules.size(),
                 nonterminals, terminals, rules, start_nonterminal,
                 requests.size(), requests};

  std::ofstream of;
  of.open("log_test_some_grammar");
  std::ostream logger(nullptr);
  std::streambuf *buf;
  buf = of.rdbuf();
  logger.rdbuf(buf);

  auto answers = GetAnswers(data, logger);
  std::vector<std::string> expected_answers = {"Yes", "Yes", "Yes", "Yes", "Yes", "Yes", "No", "No", "No"};
  ASSERT_EQ(answers, expected_answers);
}