//
// Created by shawnfeng on 2021-08-10.
//

#include <fcntl.h>
#include <poll.h>
#include <ulog/helper/async_rotating_file.h>
#include <ulog/ulog.h>
#include <unistd.h>

#include <CLI11/CLI11.hpp>

int main(int argc, char **argv) {
  CLI::App app(
      "Write the standard input data to the file\n"
      "\n"
      "Example: \n"
      "  your_program | log_rotate --file_path /tmp/example/log.txt "
      "--file_size=100000 --file_number=3\n");
  app.get_formatter()->column_width(40);

  std::string file_path;
  app.add_option("--file_path", file_path, "File path to record log, if the directory does not exist it will be created automatically")
      ->required();

  int file_size;
  app.add_option("--file_size", file_size, "Size of each file")->required();

  int file_number;
  app.add_option("--file_number", file_number, "Maximum number of files")
      ->required();

  int fifo_size;
  app.add_option("--fifo_size", fifo_size, "Fifo size")->default_val(32768);

  bool out_stdout{false};
  app.add_flag("--stdout", out_stdout, "Should print to stdout");

  CLI11_PARSE(app, argc, argv);

  ulog::AsyncRotatingFile async_rotate(fifo_size, file_path, file_size,
                                       file_number, 0, out_stdout);
  // Set O_NONBLOCK flag
  {
    int flag = fcntl(STDIN_FILENO, F_GETFL);
    if (flag < 0) {
      perror("fcntl");
      return -1;
    }
    fcntl(STDIN_FILENO, F_SETFL, flag | O_NONBLOCK);
  }

  pollfd fds{.fd = STDIN_FILENO, .events = POLLIN};
  char buffer[10 * 1024];
  while (poll(&fds, 1, -1) >= 0) {
    auto n = read(STDIN_FILENO, buffer, sizeof(buffer));
    if (n == 0) return -1;
    if (n > 0) async_rotate.InPacket(buffer, n);
  }
  async_rotate.Flush();

  return 0;
}
