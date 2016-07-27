# SignalsAndSlots
A toy signal/slot implementation for C++, with optional automatic unregistration of observers on their destruction. There are many signal/slot libraries for c++. This one is mine.

No dependencies, just C++11

---

Usage is simple:


```cpp
void print(const std::string &message) {
	std::cout << message << std::endl;
}
...

signals::signal<void(const std::string &)> signal;
signal.connect(print);

signal("Hello world");

// we're done here
signal.disconnect(print);
signal("Silencio!");

```

If the signal receiving object is derived from `signals::reciever` it will automatically unregister itself from any `signal` it is bound to on destruction.

```cpp
class observer : public signals::receiver {

	void method(const std::string &message) {
		std::cout << message << std::endl;
	}

}

...

signals::signal<void(const std::string &)> signal;


observer ob0;
signal.connect(&ob0, &observer::method);

{
	observer ob1;
	signal.connect(&ob1, &observer::method);
	signal("Hello from two observers");
}

signal("Hello from one observer");

```

That's it!