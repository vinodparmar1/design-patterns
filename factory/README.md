# Factory design pattern

Centrilize and abstract object creation

# Problem

When we need to create objects whose concreate type is decided at runtime (from config, message) and we don't want to put the exact concrete type to create.

# Intent

Provides the interface for creating families of related or independent objects without specifiying their concreate classes.

# How

Define an abstract factory that declares an interface for creating each basic kind sub-object required. There is an interface for each sub kind of sub-object and concrete sub-objects for defining specific behaviours. Abstract factory have an operation to return the new sub-object for each abstract sub-object class. Clients call these oeprations to obtain instances, but clients are not aware of the concrete classes they are using. Thus clients stay independent of the underlying sub-object creation.

# Parts

- Abstract factory: Declare a set of methods for creating each of the abstract products.
- Concrete factories: Implement the creation methods to return specific variants of the products.
- Abstract products: Declare interfaces for a set of distinct but related products.
- Concrete products: Various implementations of abstract products, grouped by variants.
- Client: Works with only interfaces declared by the abstract factory and abstract classes.

                                Abstract Factory
                                    ^
                                    |
                    |--------------------------------|
            concreate factorty1                 concrete factory2
            |               |                   |               |
            |               |                   |               |
productA<---|  productB<----|      productA<----| productB<-----|


# Examples
- Logger creation — createLogger("file") / "console" / "network" returns the right sink.

- Parser/serializer by format — createParser("json") vs "xml" vs "avro"

- Database connection factory — returns a Postgres/MySQL/SQLite connection object from a connection string.
