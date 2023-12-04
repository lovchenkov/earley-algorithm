#include "main.h"

int main() {
  InputData data = ReadInput();

  std::ofstream of;
  of.open("log");
  std::ostream logger(nullptr);
  std::streambuf *buf;
  buf = of.rdbuf();
  logger.rdbuf(buf);

  auto answers = GetAnswers(data, logger);
  PrintAnswers(answers);
}
