////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief      Contains declarations of the types for DFAs.
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


#ifndef DFA_HPP_
#define DFA_HPP_


#include <set>
#include <map>
#include <vector>
#include <tuple>
////#include <cstddef> // size_t



/*! ****************************************************************************
 *  \brief Dfa represents a parametrized deterministic finit state automata.
 *
 *  An automaton made of this type is defined by a transition table.
 *
 *  \tparam State is a data type for representing states. Must be compact enough
 *  to maintain multiple copy-by-value operations.
 *  \tparam Alpha represent elements of the alphabet of an automaton. Must be
 *  compact enough to maintain multiple copy-by-value operations.
 ******************************************************************************/
template<typename State, typename Alpha>
class Dfa {
public:
    // aliases for State and Alpha
    typedef State TState;
    typedef Alpha TAlpha;


    /// Define a set of states type.
    typedef std::set<State> States;

    /// Define a set of alphabet symbols type.
    typedef std::set<Alpha> Alphabet;

    //typedef std::map<Alpha, State>

    /// State-Alpha pair.
    typedef std::pair<State, Alpha> StateAlphaPair;

    /// State-Alpha-State tuple.
    typedef std::tuple<State, Alpha, State> StateAlphaState;

    /// Transition function delta mapping StateAlpha to State.
    typedef std::map<StateAlphaPair, State> TransFunc;

public:

    // Constructors and all.

    ///  Default constructor.
    Dfa() = default;

    /// Inititalizes an automaton with an init state \a init, a set of
    /// transitions \a l and a set of accepting states \a fin.
    Dfa(State init, std::initializer_list<StateAlphaState> l,
        std::initializer_list<State> fin)
    {
        for (const StateAlphaState& t : l)
        {
            addTrans(std::get<0>(t), std::get<1>(t), std::get<2>(t));
        }

        // init state
        addState(init);
        _init = init;

        // accepting states
        for (State s : fin)
        {
            addFinState(s);
        }
    }

public:
    // Modifying the structure of an automaton.

    /// Adds a new state \a s.
    /// \returns the newly added state.
    State addState(State s)
    {
        // for the very first added state set it as the initial (can be changed)
        if (_states.size() == 0)
            _init = s;

        _states.insert(s);

        return s;
    }

    /// Adds a new symbol \a a.
    /// \returns the newly added symbol.
    Alpha addSymbol(Alpha a)
    {
        _alphabet.insert(a);

        return a;
    }

    /// Adds a new transition from the state \a s to the state \a d labeled with
    /// the symbol \a a.
    /// Adds states \a s and \a d and the symbol \a a to the definition of the
    /// automata if there were no corresponding object added previously.
    void addTrans(State s, Alpha a, State d)
    {
        addState(s);
        addState(d);
        addSymbol(a);

        _transTable.insert({{s, a}, d});
    }

    /// Adds a new accepting state \a s. The state is also added to a set of
    /// states.
    /// \returns the newly added state.
    State addFinState(State s)
    {
        _states.insert(s);
        _finStates.insert(s);

        return s;
    }

    /// Sets a new initial state.
    State setInitState(State init)
    {
        addState(init);
        _init = init;
    }

    // Setters/getters

    /// \return init state.
    State getInitState() const { return _init; }

    /// \return number of states in an automaton.
    size_t getStatesNum() const { return _states.size(); }

    /// \return number of symbols in an automaton.
    size_t getSymbolsNum() const { return _alphabet.size(); }

    /// \return number of transitions in an automaton.
    size_t getTransNum() const { return _transTable.size(); }

    /// \return number of accepting states in an automaton.
    size_t getFinStatesNum() const { return _finStates.size(); }


    /// Tries to get a transition from the state \a s labeled \a a.
    /// \return true is there is a valid transition, so \a d is set to the
    /// destination state; otherwise returns false and \a d is undefined.
    bool getTrans(State s, Alpha a, State& d) const
    {
        auto it = _transTable.find({s, a});
        if (it != _transTable.end())
        {
            d = it->second;
            return true;
        }

        return false;
    }

    /// Checks whether the state \a s belongs to the set of states.
    /// \return true if \a s is a state of the DFA, false otherwise.
    bool hasState(State s) const
    {
        return (_states.find(s) != _states.end());
    }

    /// Checks whether the state \a s belongs to the set of accepting states.
    /// \return true if \a s is an accepting state, false otherwise.
    bool hasFinState(State s) const
    {
        return (_finStates.find(s) != _finStates.end());
    }

protected:
    States _states;             ///< Set of states (Q).
    State _init;                ///< Initial state (q0).
    Alphabet _alphabet;         ///< Alphabet (\Sigma).
    TransFunc _transTable;      ///< Transition table (\delta).
    States _finStates;          ///< Set of accepting states (F).
}; // class Dfa


/*! ****************************************************************************
 *  \brief Player used to replay a given string in a given automaton.
 *
 *  \tparam State is a data type for representing states. Must be compact enough
 *  to maintain multiple copy-by-value operations.
 *  \tparam Alpha represent elements of the alphabet of an automaton. Must be
 *  compact enough to maintain multiple copy-by-value operations.
 ******************************************************************************/
template<typename State, typename Alpha>
class DfaPlayer {
public:
    // aliases for State and Alpha
    typedef State TState;
    typedef Alpha TAlpha;

    /// Specified DFA (with set state and symbol types).
    typedef Dfa<State, Alpha> SpecDfa;

    /// Results of replaying.
    enum class Result {
        Ok,                 ///< Replayed successfully.
        NoTrans,            ///< Broken, no appropriate transition.
        NonFinState,        ///< Ended up in a non-accepting state.
    };

    /// Interface for callbacks.
    class IEventListener {
    public:
        /// Called when the current state is being changed.
        /// \param preS defines previous state.
        /// \param newS defines new state.
        virtual void onStateChanging(State preS, State newS) = 0;

        /// Called when a transition is fired.
        /// \param s defines current state (source).
        /// \param a defines a symbol activating the transition.
        /// \param s defines new state (destination).
        virtual void onTransFired(State s, Alpha a, State d) = 0;
    protected:
        ~IEventListener() {}
    };



public:
    // Constructors and all.

    /// Inititalizes a player with an automaton.
    DfaPlayer(const SpecDfa& dfa, IEventListener* cb = nullptr)
        : _dfa(dfa)
        , _cb(cb)
    {
        _curPos = -1;           // nothing to replay
    }

public:

    /// Plays a sequence provided as a vector.
    /// \return Result::ok if the \a seq was replayed successfully (accepted),
    /// otherwise \a seq is declined. In the latter case see methods getCurState(),
    /// getCurPos() and getLastSymbol() to obain info about last replayed symbol
    /// and corresponding state.
    Result play(const std::vector<Alpha>& seq)
    {
        init();
        for (Alpha a : seq)
        {
            if (!replaySymb(a))
                return Result::NoTrans;
        }

        // check for the accepting state
        if (!_dfa.hasFinState(_curState))
            return Result::NonFinState;

        return Result::Ok;
        //return true;
    }

    /// Returns state being visited.
    State getCurState() const { return _curState; }

    /// Returns current position in the replayed sequence.
    State getCurPos() const { return _curPos; }

    /// Returns the last considered symbol.
    Alpha getLastSymbol() const { return _lastSymb; }

    /// Sets a new event listener.
    void setEventListener(IEventListener* cb)
    {
        _cb = cb;
    }

    /// Returns the set event listener.
    IEventListener* getEventListener() const { return _cb; }


protected:

    /// Initializes the player before the replay.
    void init()
    {
        _curState = _dfa.getInitState();
        _curPos = 0;

        if (_cb)
            _cb->onStateChanging(_curState, _curState);
    }

    /// Tries to replay another given symbol being in the current state.
    /// \return true if the symbol can be replayed, false otherwise.
    bool replaySymb(Alpha a)
    {
        _lastSymb = a;

        State nextSt;
        if(!_dfa.getTrans(_curState, a, nextSt))
            return false;

        // call back if assigned
        if (_cb)
            _cb->onTransFired(_curState, a, nextSt);

        _curState = nextSt;
        ++_curPos;

        return true;
    }



protected:
    const SpecDfa& _dfa;                ///< Ref to the automaton.
    State _curState;                    ///< Current state.
    int _curPos;                        ///< Currently replayed symbol.
    Alpha _lastSymb;                    ///< Stores last replayed symbol.

    IEventListener* _cb;                ///< Callback listener.
}; //



#endif // DFA_HPP_
