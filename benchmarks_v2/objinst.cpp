#include <stdlib.h>
#include <iostream>

using namespace std;

class Toggle {
public:
    Toggle(bool start_state) : state(start_state) { }
    virtual bool value() {
		return(state);
    }
    Toggle& activate() {
		state = !state;
		return(*this);
    }
    bool state;
};

class NthToggle : public Toggle {
public:
    NthToggle(bool start_state, int max_counter) :
		Toggle(start_state), count_max(max_counter), counter(0) {
    }
    NthToggle& activate() {
		if (++this->counter >= this->count_max) {
		    state = !state;
		    counter = 0;
		}
		return(*this);
    }
private:
    int count_max;
    int counter;
};

int
main(int argc, char *argv[]) {
    int n = ((argc == 2) ? atoi(argv[1]) : 1);

    Toggle toggle1(true);
    for (int i=0; i<5; i++) {
		cout << ((toggle1.activate().value()) ? "true" : "false") 
			 << " ";
    }
    for (int i=0; i<n; i++) {
		Toggle toggle(true);
    }

    cout << endl;

    NthToggle ntoggle1(true, 3);
    for (int i=0; i<8; i++) {
		cout << ((ntoggle1.activate().value()) ? "true" : "false") 
			 << " ";
    }
    for (int i=0; i<n; i++) {
		NthToggle ntoggle(true, 3);
    }

    cout << endl;

    return 0;
}
