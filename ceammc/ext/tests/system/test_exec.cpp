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
    OK = 0,
    ERR = 1,
    INF = 2,
    INF_NO_INT = 3,
    INF_NO_TERM = 4,
    STDOUT = 5,
    STDERR = 6,
    NO_NEWLINE = 7,
    BIG_OUTPUT = 8,
    HUGE_OUTPUT = 9,
    STDIN = 10,
    STDIN_INF = 11
};

typedef map<Cmd, string> CmdMap;
static CmdMap cmd_list = {
    { OK, "normal exit" },
    { ERR, "error exit 100" },
    { INF, "infinite loop" },
    { INF_NO_INT, "infinite loop, no interrupt" },
    { INF_NO_TERM, "infinite loop, no terminate" },
    { STDOUT, "output test to stdout" },
    { STDERR, "output test to stderr" },
    { NO_NEWLINE, "stdout no new line" },
    { BIG_OUTPUT, "stdout big output" },
    { HUGE_OUTPUT, "stdout huge output" },
    { STDIN, "stdin waiting" },
    { STDIN_INF, "stdin infinite" }
};

static void usage(const char* name)
{
    cerr << "Usage: " << name << " COMMAND" << endl;
    cerr << "  command list:\n";
    for (auto& kv : cmd_list)
        cerr << "    " << kv.first << " - " << kv.second << endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        usage(argv[0]);
        return -1;
    }

    int n = strtol(argv[1], NULL, 10);
    if (errno) {
        perror("error:");
        return -1;
    }

    switch (n) {
    case OK:
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        return 0;
    case ERR:
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        exit(100);
    case INF:
        while (true)
            ;
        return 0;

    case INF_NO_INT: {
        signal(SIGINT, SIG_IGN);
        while (true)
            ;

        return INF_NO_INT;
    }
    case INF_NO_TERM: {
        signal(SIGTERM, SIG_IGN);
        while (true)
            ;

        return INF_NO_TERM;
    }
    case STDOUT: {
        cout << "stdout test\n";
        return 0;
    }
    case STDERR: {
        cerr << "stderr test\n";
        return 0;
    }
    case STDIN: {
        cerr << "waiting for single user input...\n";
        std::string data;
        std::getline(cin, data);
        cout << "got: " << data << endl;
        return 0;
    }
    case STDIN_INF: {
        cerr << "waiting for infinite user input...\n";
        std::string data;
        while (std::getline(cin, data)) {
            if (cin.eof())
                break;
            cout << "got: " << data << endl;
        }
        return 0;
    }
    case NO_NEWLINE: {
        cout << "no newline";
        return 0;
    }
    case BIG_OUTPUT: {
        for (int i = 0; i < 100; i++)
            cout << std::string(100, '1') << endl;
        return 0;
    }
    case HUGE_OUTPUT: {
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
