#include <iostream>
#include <cstdint>

namespace orderstate {

class Order;   // forward declaration - states operate on the context

// ---- State interface --------------------------------------------
class OrderState {
public:
    virtual void onAccepted(Order&)            {}   // exchange acked the order
    virtual void onFill(Order&, uint32_t qty)  {}   // partial or full fill
    virtual void onCancel(Order&)              {}   // cancel requested
    virtual void onReject(Order&)              {}   // exchange rejected
    virtual const char* name() const = 0;
    virtual ~OrderState() = default;
};

// Context - holds ALL the data; states hold none
class Order {
    OrderState* state_;          // just a pointer - transitions are pointer writes
    uint64_t id_;
    uint32_t quantity_;
    uint32_t remaining_;
public:
    Order(uint64_t id, uint32_t qty);

    void setState(OrderState* s) {
        std::cout << "  [" << state_->name() << " -> " << s->name() << "]\n";
        state_ = s;              // no copying: one pointer assignment
    }

    // Events delegate to the current state - no if/else on state anywhere
    void accepted()          { state_->onAccepted(*this); }
    void fill(uint32_t qty)  { state_->onFill(*this, qty); }
    void cancel()            { state_->onCancel(*this); }
    void reject()            { state_->onReject(*this); }

    // data accessors used by the states
    uint64_t id()          const { return id_; }
    uint32_t remaining()   const { return remaining_; }
    void reduceRemaining(uint32_t q) { remaining_ -= (q > remaining_ ? remaining_ : q); }
    const char* stateName() const { return state_->name(); }
};

// Concrete states - all stateless, so one shared instance each 
class NewState : public OrderState {
public:
    static NewState& instance() { static NewState s; return s; }
    void onAccepted(Order& o) override;
    void onReject(Order& o) override;
    void onCancel(Order& o) override;
    const char* name() const override { return "New"; }
};

class WorkingState : public OrderState {
public:
    static WorkingState& instance() { static WorkingState s; return s; }
    void onFill(Order& o, uint32_t qty) override;
    void onCancel(Order& o) override;
    const char* name() const override { return "Working"; }
};

class PartiallyFilledState : public OrderState {
public:
    static PartiallyFilledState& instance() { static PartiallyFilledState s; return s; }
    void onFill(Order& o, uint32_t qty) override;
    void onCancel(Order& o) override;
    const char* name() const override { return "PartiallyFilled"; }
};

class FilledState : public OrderState {          // terminal
public:
    static FilledState& instance() { static FilledState s; return s; }
    void onCancel(Order&) override {
        std::cout << "  reject: cannot cancel a filled order\n";
    }
    const char* name() const override { return "Filled"; }
};

class CancelledState : public OrderState {       // terminal
public:
    static CancelledState& instance() { static CancelledState s; return s; }
    const char* name() const override { return "Cancelled"; }
};

class RejectedState : public OrderState {        // terminal
public:
    static RejectedState& instance() { static RejectedState s; return s; }
    const char* name() const override { return "Rejected"; }
};

// Transitions (defined after all states are complete types)
inline Order::Order(uint64_t id, uint32_t qty)
    : state_(&NewState::instance()), id_(id), quantity_(qty), remaining_(qty) {}

inline void NewState::onAccepted(Order& o) { o.setState(&WorkingState::instance()); }
inline void NewState::onReject(Order& o)   { o.setState(&RejectedState::instance()); }
inline void NewState::onCancel(Order& o)   { o.setState(&CancelledState::instance()); }

inline void WorkingState::onFill(Order& o, uint32_t qty) {
    o.reduceRemaining(qty);
    o.setState(o.remaining() == 0 ? static_cast<OrderState*>(&FilledState::instance())
                                  : static_cast<OrderState*>(&PartiallyFilledState::instance()));
}
inline void WorkingState::onCancel(Order& o) { o.setState(&CancelledState::instance()); }

inline void PartiallyFilledState::onFill(Order& o, uint32_t qty) {
    o.reduceRemaining(qty);
    if (o.remaining() == 0) o.setState(&FilledState::instance());
}
inline void PartiallyFilledState::onCancel(Order& o) { o.setState(&CancelledState::instance()); }

} // namespace orderstate

int main() {
    using namespace orderstate;

    std::cout << "--- partial fills then complete ---\n";
    Order o1(1001, 100);
    o1.accepted();        // New -> Working
    o1.fill(40);          // Working -> PartiallyFilled  (60 left)
    o1.fill(60);          // PartiallyFilled -> Filled   (0 left)
    o1.cancel();          // rejected: terminal state
    std::cout << "final: " << o1.stateName() << "\n\n";

    std::cout << "--- cancel while working ---\n";
    Order o2(1002, 50);
    o2.accepted();        // New -> Working
    o2.cancel();          // Working -> Cancelled
    std::cout << "final: " << o2.stateName() << "\n";
}