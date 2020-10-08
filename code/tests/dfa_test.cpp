///////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief Testing module for DisjointSet classes.
///
/// © Sergey Shershakov 2020.
///
/// This code is for educational purposes of the course "Algorithms and Data 
/// Structures" provided by the School of Software Engineering of the Faculty 
/// of Computer Science at the Higher School of Economics.
///
/// When altering code, a copyright line must be preserved.
///////////////////////////////////////////////////////////////////////////////


#include <gtest/gtest.h>

#include <map>

#include "fsa/dfa.hpp"


TEST(Dfa, simplest)
{
//    size_t s1 = sizeof(std::map<char, int>);

//    int a = 0;
}

typedef Dfa<int, char> IntCharDfa;

TEST(Dfa, defCreate)
{
    IntCharDfa dfa;
    EXPECT_EQ(0, dfa.getStatesNum());
    EXPECT_EQ(0, dfa.getSymbolsNum());
    EXPECT_EQ(0, dfa.getTransNum());
    EXPECT_EQ(0, dfa.getFinStatesNum());
}

TEST(Dfa, makeExample1)
{
    IntCharDfa dfa{0,                           // init state
                   { {0, '1', 0}, {0, '0', 1},  // trans table
                     {1, '0', 1}, {1, '1', 2},
                     {2, '0', 2}, {2, '1', 2}
                   },
                   { 2 }                        // fin states
                  };

    EXPECT_EQ(3, dfa.getStatesNum());
    EXPECT_EQ(2, dfa.getSymbolsNum());
    EXPECT_EQ(6, dfa.getTransNum());
    EXPECT_EQ(0, dfa.getInitState());
    EXPECT_EQ(1, dfa.getFinStatesNum());

    // get trans
    IntCharDfa::TState d;
    EXPECT_TRUE(dfa.getTrans(0, '1', d));
    EXPECT_EQ(0, d);
    EXPECT_TRUE(dfa.getTrans(2, '0', d));
    EXPECT_EQ(2, d);
}


// Player

//typedef Dfa<int, char> IntCharDfa;
typedef DfaPlayer<int, char> IntCharDfaPlayer;


TEST(DfaPlayer, replay1)
{
    IntCharDfa dfa{0,                           // init state
                   { {0, '1', 0}, {0, '0', 1},  // trans table
                     {1, '0', 1}, {1, '1', 2},
                     {2, '0', 2}, {2, '1', 2}
                   },
                   { 2 }                        // fin states
                  };
    IntCharDfaPlayer player(dfa);
    EXPECT_EQ(IntCharDfaPlayer::Result::Ok, player.play({'0', '1'}));
    EXPECT_EQ(IntCharDfaPlayer::Result::Ok, player.play({'0', '0', '1'}));
    EXPECT_EQ(IntCharDfaPlayer::Result::Ok, player.play({'1', '0', '1', '0'}));

    EXPECT_EQ(IntCharDfaPlayer::Result::NonFinState, player.play({'1', '0', '0'}));
    EXPECT_EQ(3, player.getCurPos());   // out of the range [0; 3) = [0, 1, 2]
    EXPECT_EQ(1, player.getCurState());
    EXPECT_EQ('0', player.getLastSymbol());
    //player.g

}


