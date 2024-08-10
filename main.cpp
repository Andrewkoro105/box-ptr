#include <iostream>
#include <box-ptr/BoxPtr.hpp>

class CreateDetector{
public:
	~CreateDetector(){
		std::cout << "delete A\n";
	}
};

class KeeperNullBoxBrt {
protected:
	bp::BoxPtr<CreateDetector> box_ptr;
	
public:
	explicit KeeperNullBoxBrt(bp::BoxPtr<CreateDetector> box_ptr) : box_ptr(new CreateDetector{*box_ptr.get()}) {
	}
};

int main() {
	KeeperNullBoxBrt b{bp::make_box_ptr<CreateDetector>()};
	return 0;
}
