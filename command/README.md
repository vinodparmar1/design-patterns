# Command design pattern
- It's a behavioural design pattern. 

# problem

When we want to represent the action as data so that it can be queued, logged, retry, or send instead of just calling a function immediatly. It is useful when it is not known anything about the operation being requested or the requester of the request.

# participants
- command
declares an interface for executing an operation

- concreteCommand
define the binding between reciever objection and an action
Implements Execute by invoking the corresponding operation(s) on Receiver.

- Client(application)
Asks the command to carry out the request

- Receiver
knows how to perform the operations associated with carrying out a request.