////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief      Main module for smoke-test-project for DFA.
/// \author     Sergey Shershakov
/// \version    0.1.0
/// \date       8.10.2020
/// \copyright  © Sergey Shershakov 2020.
///             This code is for educational purposes of the course "Algorithms
///             and Data Structures" provided by the Faculty of Computer Science
///             at the Higher School of Economics.
///
/// When altering code, a copyright line must be preserved.
///
////////////////////////////////////////////////////////////////////////////////


#include <iostream>

#include "dfa.hpp"

typedef Dfa<int, char> IntCharDfa;
typedef DfaPlayer<int, char> IntCharDfaPlayer;


// Create custom EventListener for a player to track changes in an automaton.

class IntCharDfaPlayerEventListener : public IntCharDfaPlayer::IEventListener {
public:
    virtual void onStateChanging(int /*preS*/, int /*newS*/) override
    {
        //int a = 0;
    }

    virtual void onTransFired(int s, char a, int d) override
    {
        std::cout << s << " --[" << a << "]--> " << d << '\n';
        //std::cout << '(' << s << ')' << "-[" << a << "]->" << '(' << d << ')'
        //          << '\n';
    }
}; // class IntCharDfaPlayerEventListener

void test1()
{
    IntCharDfa dfa{0,                           // init state
                   { {0, '1', 0}, {0, '0', 1},  // trans table
                     {1, '0', 1}, {1, '1', 2},
                     {2, '0', 2}, {2, '1', 2}
                   },
                   { 2 }                        // fin states
                  };
    IntCharDfaPlayerEventListener cb;
    IntCharDfaPlayer player(dfa, &cb);

    IntCharDfaPlayer::Result res = player.play({'1', '0', '1', '0'});
}

int main()
{
    std::cout << "Let's do some deterministic automata!\n";

    test1();

    std::cout << "\n\nBye-bye!\n\n";
    return 0;
}

