# State design pattern
- A behavioral design pattern
- Allow an object to alter it's behaviour when internal state changes. The object will appear to change it's class.

# problem
- Where there are multiple states for an object to be in, it's difficult to manage those states with conditional statements scattered around different operations and places.
- Client should be able to operate on the same object regardless of it's internal state

# How 
- There is an interface for state
- Each possible state derive from the state interface and override the specific operations that it supports
- There is context object which have all the states just hold the pointer to the state interface
- Context object provide supported operations from state but just delegate it to the state object to decide whether this operation is valid or not for the current state of the object

# Participants

Context (TCPConnection)
- define the interface of interest to client
- maintains a instance of the concrete state subclass that defines the current state

State(TCPState)
- defines an interface to encapculate the behaviour associated with particular state of the context

ConcreteState subclasses (TCPEstablished, TCPListen, TCPClosed)
- each subclass implements the behviour associated with the state of the context

# Examples
- Market order : A market order can have different states i.e., new, working, partial filled, filled, calcelled
- TCP Connection: A TCP connection can have different states i.e., TCPEstablished, TCPListen, TCPClosed etc.


