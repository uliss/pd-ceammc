/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include <cerrno>
#include <chrono>
#include <csignal>
#include <iostream>
#include <map>
#include <string>
#include <thread>

using namespace std;

enum Cmd {
    CMD_OK = 0,
    CMD_ERR = 1,
    CMD_INF = 2,
    CMD_INF_NO_INT = 3,
    CMD_INF_NO_TERM = 4,
    CMD_STDOUT = 5,
    CMD_STDERR = 6,
    CMD_NO_NEWLINE = 7,
    CMD_BIG_OUTPUT = 8,
    CMD_HUGE_OUTPUT = 9,
    CMD_STDIN = 10,
    CMD_STDIN_INF = 11
};

typedef map<Cmd, string> CmdMap;
static CmdMap cmd_list = {
    { CMD_OK, "normal exit" },
    { CMD_ERR, "error exit 100" },
    { CMD_INF, "infinite loop" },
    { CMD_INF_NO_INT, "infinite loop, no interrupt" },
    { CMD_INF_NO_TERM, "infinite loop, no terminate" },
    { CMD_STDOUT, "output test to stdout" },
    { CMD_STDERR, "output test to stderr" },
    { CMD_NO_NEWLINE, "stdout no new line" },
    { CMD_BIG_OUTPUT, "stdout big output" },
    { CMD_HUGE_OUTPUT, "stdout huge output" },
    { CMD_STDIN, "stdin waiting" },
    { CMD_STDIN_INF, "stdin infinite" }
};

static void usage(const char* name)
{
    cerr << "Usage: " << name << " COMMAND" << endl;
    cerr << "  command list:\n";
    for (auto& kv : cmd_list)
        cerr << "    " << kv.first << " - " << kv.second << endl;
}

static void run_inf()
{
    std::this_thread::sleep_for(std::chrono::hours(1));
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        usage(argv[0]);
        return -1;
    }

    errno = 0;
    const auto n = strtol(argv[1], NULL, 10);
    if (errno) {
        perror("error");
        return -1;
    }

    switch (n) {
    case CMD_OK:
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        return 0;
    case CMD_ERR:
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        exit(100);
    case CMD_INF:
        run_inf();
        return 0;

    case CMD_INF_NO_INT: {
        signal(SIGINT, SIG_IGN);
        run_inf();
        return CMD_INF_NO_INT;
    }
    case CMD_INF_NO_TERM: {
        signal(SIGTERM, SIG_IGN);
        run_inf();
        return CMD_INF_NO_TERM;
    }
    case CMD_STDOUT: {
        cout << "stdout test\n";
        return 0;
    }
    case CMD_STDERR: {
        cerr << "stderr test\n";
        return 0;
    }
    case CMD_STDIN: {
        cerr << "waiting for single user input...\n";
        std::string data;
        std::getline(cin, data);
        cout << "got: " << data << endl;
        return 0;
    }
    case CMD_STDIN_INF: {
        cerr << "waiting for infinite user input...\n";
        std::string data;
        while (std::getline(cin, data)) {
            if (cin.eof())
                break;
            cout << "got: " << data << endl;
        }
        return 0;
    }
    case CMD_NO_NEWLINE: {
        cout << "no newline";
        return 0;
    }
    case CMD_BIG_OUTPUT: {
        for (int i = 0; i < 100; i++)
            cout << std::string(100, '1') << endl;
        return 0;
    }
    case CMD_HUGE_OUTPUT: {
        for (int i = 0; i < 1000; i++)
            cout << std::string(100, '2') << endl;
        return 0;
    }
    default:
        cerr << "Unknown command: " << n << endl;
        usage(argv[0]);
        return -1;
        break;
    }

    return 0;
}
