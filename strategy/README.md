# Strategy design pattern - 

## Problem: When an operation has multiple interchangeable implementations and the choice of which to use should be selectable usually at runtime, without changing the code that invokes it.

### How: It's a behavioural design pattern that defines a family of interchangeable algorithms using a common interface. The choice of algorithm is independent of the code that runs it.

### Modern C++: Runtime hierarchy can be replace by a `std::function<price(const Instrument&)>`: strategy is just a callable and no class hierarchy. Or when strategy is known at the compile time, a template parameter (`template <class Strategy`)can be used to eliminate the virtual dispatch making it compile time and avoiding runtime overheads

### Parts:

- Strategy: The interface stragety defines the operation which is used. Concreate implementation of it define the different implementations.

- Concrete strategy: Different concrete implementations of operation 

- Context: Context take strategy interface and get the concreate object assigned to it at runtime. The Context calls operation from strategy which in turn calls the actual concreate operation implementation.

### Examples:
pricing engine have different implementation of pricing based on the instrument type. For FX spot uses spread model, options uses black-scholes, futures use cost-of-carry. Pricing engine holds the pointer to the interface say priceStrategy. Based on the type of instrument it chooses the actual concrete object of derived priceStrategy and just calls the operation say price()

Code examples:

```cpp
// stragegy
class PriceStrategy {
public:
    virtual void price(const Instrument& ) = 0;
    virtual ~PriceStrategy() = default;
};

// concrete strategy
class FXSpotStrategy : public PriceStrategy{
public:
    void price(const Instrument&) override {
       std::cout << "FX spot specific price calculations\n";
    }
};

// context
class PriceEngine {
    std::unique_ptr<PriceStrategy> strategy_;
public:
    void setStrategy(std::unique_ptr<PriceStrategy> s) {
        strategy_ = move(s);
    }
    void reprice(const Instrument& inst) { strategy_->price(inst);}
};
int main() {

    PricingEngine engine;
    Instrument instrument;
    engine.setStrategy(std::make_unique<FXSpotStrategy>());
    engine.reprice(instrument);

    engine.setStrategy(std::make_unique<OptionStrategy>());
    engine.reprice(instrument);
    return 0;
}
```