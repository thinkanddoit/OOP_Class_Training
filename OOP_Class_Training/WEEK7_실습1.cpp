#include <iostream> 
#include <stdexcept>

class Test {
public:
	Test(char id) : id_(id) {}
	~Test() { std::cout << "Destructor execution: " << id_ << std::endl; }
private:
	char id_;
};
int funcB() {
	Test r('B');
	throw std::runtime_error("Exception from funcB!\n");
	std::cout << "Executed in B" << std::endl;
	return 0;
}
int funcA() {
	Test r('A');
	funcB();
	std::cout << "Executed in A" << std::endl;
	return 0;
}

int main() {
	try {
		funcA();
	}
	catch (std::exception & e) {
		std::cout << "Exception : " << e.what();
	}
}

/* (�ǽ� 1-1) �м���� :
<�ܼ� â ���>
Executed in B
Destructor execution : B
Executed in A
Destructor execution : A

ù��° �־��� �ڵ带 �����ϰ��� ���Ͱ��� ����� ���� ������
�켱 main������ try�����ȿ��� funcA�� �����̵ȴ�.
funcA������ Test��ü�� �����ϰ� funcB�� �����Ѵ�.
funcB������ ���������� Test��ü�� �����ϰ� "Executed in B"��� ù��° ����� ����ȴ�.
���� �ٿ��� 0�� return �ϸ鼭 �Լ��� ����ǰ� ~Test()�Ҹ��ڰ� ����Ǹ鼭
"Destructor execution: B(id��)"��� �ι�° ����� ����ȴ�.
���� �ٽ� funcA�� ���ư��� ���� ��İ� �Ȱ���
"Executed in A", "Destructor execution : A" ��� ����°, �׹�° ����� ����Ǹ� ����ó������ ���α׷��� ����ȴ�.
*/

/* (�ǽ� 1-2) �м���� :
<�ܼ� â ���>
Destructor execution: B
Destructor execution: A
Exception : Exception from funcB!

throw�� �ּ�(//)�� ������ �� ������ ���� ����� ���� ������
Stack unwinding�� �߻��߱� �����̴�.

Stack unwinding�� C++����ó�������� ���ܸ� ������ throw���� try�����ȿ� �־���ϰ�
throw �߻��� ���ܸ� �޾��ִ� catch�������� �����ϰԵȴ�.
�׷��� �Լ������� try, catch �������� throw�� �߻���Ű�� �Լ��� ������ catch������ã������
�Լ� �ܺη� ������. ���ܸ� ���� �Լ����� �ܺη� �����鼭 ���� ���������� �����ϰ� ����������
Stack unwinding�� �̶� �߻��ϰԵȴ�.

���� �ڵ带 �м��غ���
funcA ���� -> funcB ���� �� throw �߻� catch���� ã���鼭 ���� ����
-> funcB�� �Ҹ��ڰ� �ҷ����鼭 "Destructor execution: B" ��¹� �߻�
-> funcA�� ���ƿͼ� ���� catch���� ã���鼭 ���� ����
-> funcA�� �Ҹ��ڰ� �ҷ����鼭 "Destructor execution: A" ��¹� �߻�
-> catch ���� ��ã�Ƽ� main()�Լ����� ���ƿ�
-> catch�� ã�� -> Exception : Exception from funcB! ���� ��¹� �߻�
-> ���α׷� ����
*/