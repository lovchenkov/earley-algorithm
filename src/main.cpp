#include "main.h"
#include <cstring>

int main(int argc, char* argv[]) {
  bool need_to_log = (argc > 1 &&  std::strcmp(argv[1], "v"));

  GrammarAndRequests data = Read();

  std::ofstream of;
  std::ostream logger(nullptr);

  if (need_to_log) {
  std::streambuf *buf;
    of.open("log");
    buf = of.rdbuf();
    logger.rdbuf(buf);
  }

  auto answers = GetAnswers(data, logger, need_to_log);
  PrintAnswers(answers);
}
